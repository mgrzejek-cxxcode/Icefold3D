
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_UNIT_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_UNIT_H__

#include "PipelineStateCommon.h"

#include <cppx/hash.h>
#include <cppx/immutableString.h>
#include <unordered_map>

namespace Ic3::Graphics::GCI
{

	class GraphicsPipelineStateDescriptorCache;

	/**
	 *
	 * @tparam TPDescriptor
	 */
	template <typename TPDescriptorType>
	struct PipelineStateDescriptorTraits
	{
		using DescriptorType = TPDescriptorType;
		using DescriptorHandle = TGfxHandle<TPDescriptorType>;
		using InputConfigType = void;
		using FactoryInterface = void;
	};

	/**
	 *
	 */
	#define Ic3DefinePipelineStateDescriptorTraits( pDescriptorType, pInputConfig, pFactoryInterface ) \
		template <> struct PipelineStateDescriptorTraits<pDescriptorType##Descriptor> { \
			using DescriptorType = pDescriptorType##Descriptor; \
			using CreateInfoType = pDescriptorType##DescriptorCreateInfo; \
			using InputConfigType = pInputConfig; \
			using FactoryInterface = pFactoryInterface; \
			static inline constexpr auto sDescriptorType = EPipelineStateDescriptorType::DT##pDescriptorType; \
		};

	/**
	 * A cache unit ("sub-cache") used by the actual compute/graphics cache. Manages state descriptors of a single type.
	 * @tparam TPDescriptorType
	 */
	template <typename TPDescriptorType>
	class PipelineStateDescriptorCacheUnit
	{
	public:
		using DescriptorTraits = PipelineStateDescriptorTraits<TPDescriptorType>;
		using CreateInfoType = typename DescriptorTraits::CreateInfoType;
		using InputConfigType = typename DescriptorTraits::InputConfigType;
		using FactoryInterface = typename DescriptorTraits::FactoryInterface;

		static inline constexpr auto sDescriptorType = DescriptorTraits::sDescriptorType;

	public:
		PipelineStateDescriptorCacheUnit( FactoryInterface & pFactoryInterface )
		: _descriptorFactoryInterface( pFactoryInterface )
		, _descriptorInstanceCounter( 0 )
		{}

		~PipelineStateDescriptorCacheUnit() = default;

		CPPX_ATTR_NO_DISCARD uint32 GetDescriptorInstanceCounter() const noexcept
		{
			return _descriptorInstanceCounter.load( std::memory_order_relaxed );
		}

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetDescriptorByID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
		{
			const auto stateObjectIter = _descriptorIDToObjectMap.find( pDescriptorID );
			if( stateObjectIter != _descriptorIDToObjectMap.end() )
			{
				return stateObjectIter->second.cachedStateDescriptor;
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD bool HasDescriptorWithID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
		{
			const auto stateObjectIter = _descriptorIDToObjectMap.find( pDescriptorID );
			return stateObjectIter != _descriptorIDToObjectMap.end();
		}

		TPipelineStateDescriptoCreateResult<TPDescriptorType> CreateDescriptor( const CreateInfoType & pCreateInfo )
		{
			return _CreateNewCachedDescriptor( pCreateInfo );
		}

		void Reset()
		{
			_descriptorInstanceCounter.store( 0, std::memory_order_relaxed );
			_descriptorIDToObjectMap.clear();
			_configHashToDescriptorIDMap.clear();
		}

	private:
		// The data stored internally for every cached state object.
		struct CachedDescriptorData
		{
			// Control hash which is a hash of the inputDesc (passed inside the createInfo struct).
			pipeline_config_hash_t inputConfigHash;

			// The actual compiled state.
			TGfxHandle<TPDescriptorType> cachedStateDescriptor;
		};

		using DescriptorIDToObjectMap = std::unordered_map<pipeline_state_descriptor_id_t, CachedDescriptorData>;
		using ConfigHashToDescriptorIDMap = std::unordered_map<pipeline_config_hash_value_t, pipeline_state_descriptor_id_t>;

	private:
		static bool _ValidateDescriptorID( pipeline_state_descriptor_id_t pDescriptorID ) noexcept
		{
			if( CXU::IsPipelineStateDescriptorIDValid( pDescriptorID ) )
			{
				const auto typeComponent = CXU::GetPipelineStateDescriptorIDTypeComponent( pDescriptorID );
				if( ( typeComponent != EPipelineStateDescriptorType::Unknown ) && ( typeComponent == sDescriptorType ) )
				{
					return true;
				}
			}

			return false;
		}

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> _GetDescriptorWithConfigHash( pipeline_config_hash_t pConfigHash ) const noexcept
		{
			const auto descriptorIDIter = _configHashToDescriptorIDMap.find( pConfigHash );
			if( descriptorIDIter != _configHashToDescriptorIDMap.end() )
			{
				const auto stateObjectIter = _descriptorIDToObjectMap.find( descriptorIDIter->second );
				if( stateObjectIter != _descriptorIDToObjectMap.end() )
				{
					return stateObjectIter->second.cachedStateDescriptor;
				}
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD uint32 _GenerateDescriptorAutoIDUserComponent()
		{
			const auto descriptorIDUserComponent = _descriptorInstanceCounter.fetch_add( 1, std::memory_order_acq_rel ) + 1;

			if( descriptorIDUserComponent > kPipelineStateDescriptorSingleTypeLimit )
			{
				Ic3DebugOutputFmt(
						"Error: maximum number of descriptors (%u) of type %s has been exceeded.",
						kPipelineStateDescriptorSingleTypeLimit,
						QueryEnumTypeInfo<EPipelineStateDescriptorType>().GetConstantName( sDescriptorType ).c_str() );

				Ic3ThrowDesc( 0, "PipelineStateDescriptorCacheUnit: maximum number of descriptors exceeded" );
			}

			if( descriptorIDUserComponent == kPipelineStateDescriptorSingleTypeLimit )
			{
				Ic3DebugOutputFmt(
						"Warning: maximum number of descriptors (%u) of type %s reached.",
						kPipelineStateDescriptorSingleTypeLimit,
						QueryEnumTypeInfo<EPipelineStateDescriptorType>().GetConstantName( sDescriptorType ).c_str() );
			}

			return descriptorIDUserComponent;
		}

		TPipelineStateDescriptoCreateResult<TPDescriptorType> _CreateNewCachedDescriptor( const CreateInfoType & pCreateInfo ) noexcept
		{
			// Cast the createInfo object to the required base type - compile-time check in case of future refactoring.
			const auto & baseCreateInfo = static_cast<const PipelineStateDescriptorCreateInfoBase &>( pCreateInfo );

			// All XxxDescriptorCreateInfo types have GetConfigHash() which returns a hash value for its config
			// (each config is different so it's up to the CreateInfo to decide how to compute it). We can now
			// look at this hash and see if a descriptor for the specified config has been already created.
			const auto configHash = pCreateInfo.GetConfigHash();

			// Fetch the descriptor from the internal cache map. If it is not empty,
			// return it immediately and avoid creating another one with the same config.
			if( const auto existingDescriptorWithSameHash = _GetDescriptorWithConfigHash( configHash ) )
			{
				return { existingDescriptorWithSameHash };
			}

			const auto descriptorIDUserComponent = _GenerateDescriptorAutoIDUserComponent();
			const auto cachedDescriptorAutoID = CXU::MakePipelineStateDescriptorID( sDescriptorType, descriptorIDUserComponent );
			Ic3DebugAssert( CXU::IsPipelineStateDescriptorIDValid( cachedDescriptorAutoID ) );

			auto newStateDescriptor = _descriptorFactoryInterface.CreateDescriptor( pCreateInfo );
			if( !cachedDescriptorAutoID )
			{
				return { nullptr };
			}

			CachedDescriptorData descriptorData{};
			descriptorData.inputConfigHash = configHash;
			descriptorData.cachedStateDescriptor = newStateDescriptor;

			// Save the descriptor in the cache, referenced by its ID. This action may overwrite the existing
			// descriptor, as determined by _DetermineCacheStoreAction() function. See the documentation of
			// ePipelineStateDescriptorCreateFlagCachePolicyOverwriteConfig for details.
			_descriptorIDToObjectMap[cachedDescriptorAutoID] = std::move( descriptorData );

			// Associate the computed hash of the descriptor configuration, so it can be
			// retrieved later on, if another descriptor with the same config is requested.
			_configHashToDescriptorIDMap[configHash.value] = cachedDescriptorAutoID;

			return { newStateDescriptor, cachedDescriptorAutoID };
		}

	private:
		// Adapter for the state factory. Allows the cache to simply call Create() for every state type.
		FactoryInterface & _descriptorFactoryInterface;

		//
		std::atomic<uint32> _descriptorInstanceCounter = 0;

		// The actual Map which serves as a storage for cached objects.
		DescriptorIDToObjectMap _descriptorIDToObjectMap;

		//
		ConfigHashToDescriptorIDMap _configHashToDescriptorIDMap;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_UNIT_H__
