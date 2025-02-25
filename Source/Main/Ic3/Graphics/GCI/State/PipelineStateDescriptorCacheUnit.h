
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_UNIT_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_UNIT_H__

#include "CommonGPUStateDefs.h"

#include <cppx/hash.h>
#include <cppx/immutableString.h>
#include <unordered_map>

namespace Ic3::Graphics::GCI
{

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
		{}

		~PipelineStateDescriptorCacheUnit() = default;

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetDescriptorByID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
		{
			const auto stateObjectIter = _descriptorIDToObjectMap.find( pDescriptorID );
			if( stateObjectIter != _descriptorIDToObjectMap.end() )
			{
				return stateObjectIter->second.cachedStateDescriptor;
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetDescriptorByName( const cppx::string_view & pDescriptorName ) const noexcept
		{
			const auto descriptorIDIter = _nameToDescriptorIDMap.find( pDescriptorName );
			if( descriptorIDIter != _nameToDescriptorIDMap.end() )
			{
				const auto stateObjectIter = _descriptorIDToObjectMap.find( descriptorIDIter->second );
				if( stateObjectIter != _descriptorIDToObjectMap.end() )
				{
					return stateObjectIter->second.cachedStateDescriptor;
				}
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetDescriptorForConfig( const InputConfigType & pConfig ) const noexcept
		{
			const auto configHash = cppx::hash_compute<cppx::hash_algo::fnv1a64>( pConfig );

			const auto descriptorIDIter = _configHashToDescriptorIDMap.find( configHash );
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

		CPPX_ATTR_NO_DISCARD bool HasDescriptorWithConfigHash( pipeline_config_hash_value_t pConfigHash ) const noexcept
		{
			const auto stateObjectIter = _configHashToDescriptorIDMap.find( pConfigHash );
			return stateObjectIter != _configHashToDescriptorIDMap.end();
		}

		CPPX_ATTR_NO_DISCARD bool HasDescriptorWithID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
		{
			const auto stateObjectIter = _descriptorIDToObjectMap.find( pDescriptorID );
			return stateObjectIter != _descriptorIDToObjectMap.end();
		}

		CPPX_ATTR_NO_DISCARD bool HasDescriptorWithName( const cppx::string_view & pDescriptorName ) const noexcept
		{
			const auto descriptorIDIter = _nameToDescriptorIDMap.find( pDescriptorName );
			return descriptorIDIter != _nameToDescriptorIDMap.end();
		}

		TGfxHandle<TPDescriptorType> CreateDescriptor( const CreateInfoType & pCreateInfo, cppx::immutable_string pDescriptorName )
		{
			return _CreateNewCachedDescriptor( pCreateInfo, pDescriptorName );
		}

		void Reset()
		{
			_descriptorIDToObjectMap.clear();
			_configHashToDescriptorIDMap.clear();
			_nameToDescriptorIDMap.clear();
		}

	private:
		// The data stored internally for every cached state object.
		struct CachedDescriptorData
		{
			// Control hash which is a hash of the inputDesc (passed inside the createInfo struct).
			pipeline_config_hash_t inputConfigHash;

			//
			cppx::immutable_string name;

			// The actual compiled state.
			TGfxHandle<TPDescriptorType> cachedStateDescriptor;
		};

		using DescriptorIDToObjectMap = std::unordered_map<pipeline_state_descriptor_id_t, CachedDescriptorData>;
		using NameToDescriptorIDMap = std::unordered_map<cppx::immutable_string , pipeline_state_descriptor_id_t>;
		using ConfigHashToDescriptorIDMap = std::unordered_map<pipeline_config_hash_value_t, pipeline_state_descriptor_id_t>;

	private:
		static bool _ValidateDescriptorID( pipeline_state_descriptor_id_t pDescriptorID ) noexcept
		{
			if( CXU::IsPipelineStateDescriptorIDValid( pDescriptorID ) )
			{
				const auto typeComponent = CXU::GetPipelineStateDescriptorIDTypeComponent( pDescriptorID );
				if( ( typeComponent == EPipelineStateDescriptorType::Unknown ) && ( typeComponent == sDescriptorType ) )
				{
					return true;
				}
			}

			return false;
		}

		bool _CheckDescriptorCreateRequest(
				const PipelineStateDescriptorCreateInfoBase & pCreateInfo,
				const cppx::immutable_string & pDescriptorName,
				pipeline_config_hash_value_t pConfigHash ) const noexcept
		{
			if( !_ValidateDescriptorID( pCreateInfo.descriptorID ) )
			{
				Ic3DebugOutputFmt(
					"Specified descriptor ID (%#x) is either invalid or does not match requested type.",
					pCreateInfo.descriptorID );

				return false;
			}

			if( HasDescriptorWithID( pCreateInfo.descriptorID ) )
			{
				Ic3DebugOutputFmt(
					"PSD with the specified ID (%#x) is already registered in the cache.",
					pCreateInfo.descriptorID );

				return false;

			}

			if( pDescriptorName && HasDescriptorWithName( pDescriptorName ) )
			{
				Ic3DebugOutputFmt(
					"PSD with the specified name (%s) is already registered in the cache.",
					pDescriptorName.data() );

				return false;

			}

			const auto configHashFound = HasDescriptorWithConfigHash( pConfigHash );
			if( configHashFound )
			{
				if( !pCreateInfo.flags.is_set( ePipelineStateDescriptorCreateFlagCachePolicyOverwriteConfig ) )
				{
					Ic3DebugOutputFmt(
						"PSD with ConfigHash = %#x (specified ID: %#x) has been already created and cached. Possible collision?",
						pConfigHash,
						pCreateInfo.descriptorID );

					return false;
				}
				else
				{
					Ic3DebugOutputFmt(
						"PSD with ID %#x will overwrite %#x for config hash = %#x",
						pCreateInfo.descriptorID,
						_configHashToDescriptorIDMap.at( pConfigHash ),
						pConfigHash );
				}
			}

			return true;
		}

		TGfxHandle<TPDescriptorType> _CreateNewCachedDescriptor( const CreateInfoType & pCreateInfo, cppx::immutable_string pDescriptorName ) noexcept
		{
			// Cast the createInfo object to the required base type - compile-time check in case of future refactoring.
			const auto & baseCreateInfo = static_cast<const PipelineStateDescriptorCreateInfoBase &>( pCreateInfo );

			const auto configHash = pCreateInfo.GetConfigHash();

			if( _CheckDescriptorCreateRequest( baseCreateInfo, pDescriptorName, configHash.value ) )
			{
				//
				auto newStateDescriptor = _descriptorFactoryInterface.CreateDescriptor( pCreateInfo );

				CachedDescriptorData descriptorData{};
				descriptorData.inputConfigHash = configHash;
				descriptorData.name = pDescriptorName;
				descriptorData.cachedStateDescriptor = newStateDescriptor;

				// Save the descriptor in the cache, referenced by its ID. This action may overwrite the existing
				// descriptor, as determined by _DetermineCacheStoreAction() function. See the documentation of
				// ePipelineStateDescriptorCreateFlagCachePolicyOverwriteConfig for details.
				_descriptorIDToObjectMap[baseCreateInfo.descriptorID] = std::move( descriptorData );

				_configHashToDescriptorIDMap[configHash.value] = baseCreateInfo.descriptorID;

				if( pDescriptorName )
				{
					_nameToDescriptorIDMap[pDescriptorName] = baseCreateInfo.descriptorID;
				}

				return newStateDescriptor;
			}

			return nullptr;
		}

	private:
		// Adapter for the state factory. Allows the cache to simply call Create() for every state type.
		FactoryInterface & _descriptorFactoryInterface;

		// The actual Map which serves as a storage for cached objects.
		DescriptorIDToObjectMap _descriptorIDToObjectMap;

		//
		ConfigHashToDescriptorIDMap _configHashToDescriptorIDMap;

		//
		NameToDescriptorIDMap _nameToDescriptorIDMap;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_UNIT_H__
