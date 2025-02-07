
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_UNIT_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_UNIT_H__

#include "PipelineStateIDDefs.h"

#include <cppx/hash.h>
#include <cppx/immutableString.h>
#include <unordered_map>

namespace Ic3::Graphics::GCI
{

	/**
	 *
	 * @tparam TPDescriptor
	 */
	template <typename TPStateDescriptor>
	struct PipelineStateDescriptorTraits
	{
		using DescriptorType = TPStateDescriptor;
		using DescriptorHandle = TGfxHandle<TPStateDescriptor>;
		using InputConfigType = void;
		using FactoryInterface = void;
	};

	/**
	 *
	 */
	#define Ic3DefinePipelineStateDescriptorTraits( pDescriptorType, pInputConfig, pFactoryInterface ) \
		template <> struct PipelineStateDescriptorTraits<pDescriptorType##StateDescriptor> { \
			using DescriptorType = pDescriptorType##StateDescriptor; \
			using DescriptorHandle = TGfxHandle<pDescriptorType##StateDescriptor>; \
			using InputConfigType = pInputConfig; \
			using FactoryInterface = pFactoryInterface; \
		};

	/**
	 * A cache unit ("sub-cache") used by the actual compute/graphics cache. Manages state descriptors of a single type.
	 * @tparam TPStateDescriptor
	 */
	template <typename TPStateDescriptor>
	class PipelineStateDescriptorCacheUnit
	{
	public:
		using DescriptorTraits = PipelineStateDescriptorTraits<TPStateDescriptor>;
		using InputConfigType = typename DescriptorTraits::InputConfigType;
		using FactoryInterface = typename DescriptorTraits::FactoryInterface;
		using InputConfigHashType = cppx::hash_object<cppx::hash_algo::fnv1a64>;
		using InputConfigHashValue = InputConfigHashType::value_type;

	public:
		PipelineStateDescriptorCacheUnit( FactoryInterface & pFactoryInterface )
		: _stateFactoryInterface( pFactoryInterface )
		{}

		~PipelineStateDescriptorCacheUnit() = default;

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPStateDescriptor> GetStateDescriptor( pipeline_state_descriptor_id_t pStateID ) const noexcept
		{
			const auto stateObjectIter = _stateIDToObjectMap.find( pStateID );
			if( stateObjectIter != _stateIDToObjectMap.end() )
			{
				return stateObjectIter->second.compiledStateObject;
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPStateDescriptor> GetStateDescriptor( const cppx::string_view & pStateName ) const noexcept
		{
			const auto stateIDIter = _nameToStateIDMap.find( pStateName );
			if( stateIDIter != _nameToStateIDMap.end() )
			{
				const auto stateObjectIter = _stateIDToObjectMap.find( stateIDIter->second );
				if( stateObjectIter != _stateIDToObjectMap.end() )
				{
					return stateObjectIter->second.compiledStateObject;
				}
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPStateDescriptor> GetStateDescriptorForConfig( const InputConfigType & pConfig ) const noexcept
		{
			const auto configHash = cppx::hash_compute<cppx::hash_algo::fnv1a64>( pConfig );

			const auto stateIDIter = _configHashToStateIDMap.find( configHash );
			if( stateIDIter != _configHashToStateIDMap.end() )
			{
				const auto stateObjectIter = _stateIDToObjectMap.find( stateIDIter->second );
				if( stateObjectIter != _stateIDToObjectMap.end() )
				{
					return stateObjectIter->second.compiledStateObject;
				}
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD bool HasStateDescriptorWithID( pipeline_state_descriptor_id_t pStateID ) const noexcept
		{
			const auto stateObjectIter = _stateIDToObjectMap.find( pStateID );
			return stateObjectIter != _stateIDToObjectMap.end();
		}

		CPPX_ATTR_NO_DISCARD bool HasStateDescriptorWithName( const cppx::string_view & pStateName ) const noexcept
		{
			const auto stateIDIter = _nameToStateIDMap.find( pStateName );
			return stateIDIter != _nameToStateIDMap.end();
		}

		template <typename... TArgs>
		TGfxHandle<TPStateDescriptor> CreateStateDescriptor(
				pipeline_state_descriptor_id_t pStateID,
				const InputConfigType & pConfig,
				TArgs && ...pArgs )
		{
			if( HasStateDescriptorWithID( pStateID ) )
			{
				return nullptr;
			}

			//
			auto newStateDescriptor = _stateFactoryInterface.CreateState( pConfig, std::forward<TArgs>( pArgs )... );

			if( newStateDescriptor )
			{
				//
				const auto configHash = cppx::hash_compute<cppx::hash_algo::fnv1a64>( pConfig );

				_stateIDToObjectMap[pStateID] = newStateDescriptor;
				_configHashToStateIDMap[configHash] = pStateID;
			}

			return newStateDescriptor;
		}

		template <typename... TArgs>
		TGfxHandle<TPStateDescriptor> CreateNamedStateDescriptor(
				pipeline_state_descriptor_id_t pStateID,
				cppx::immutable_string pName,
				const InputConfigType & pConfig,
				TArgs && ...pArgs )
		{
			if( HasStateDescriptorWithID( pStateID ) )
			{
				return nullptr;
			}

			if( HasStateDescriptorWithName( pName ) )
			{
				return nullptr;
			}

			//
			auto newStateDescriptor = _stateFactoryInterface.CreateState( pConfig, std::forward<TArgs>( pArgs )... );

			if( newStateDescriptor )
			{
				//
				const auto configHash = cppx::hash_compute<cppx::hash_algo::fnv1a64>( pConfig );

				CachedStateData stateData{};
				stateData.inputConfigHash = configHash;
				stateData.name = pName;
				stateData.compiledStateObject = newStateDescriptor;

				_stateIDToObjectMap[pStateID] = std::move( stateData );
				_configHashToStateIDMap[configHash] = pStateID;
				_nameToStateIDMap[pName] = pStateID;
			}

			return newStateDescriptor;
		}

		void Reset()
		{
			_stateIDToObjectMap.clear();
			_configHashToStateIDMap.clear();
			_nameToStateIDMap.clear();
		}

	private:
		// The data stored internally for every cached state object.
		struct CachedStateData
		{
			// Control hash which is a hash of the inputDesc (passed inside the createInfo struct).
			InputConfigHashValue inputConfigHash;

			//
			cppx::immutable_string name;

			// The actual compiled state.
			TGfxHandle<TPStateDescriptor> compiledStateObject;
		};

		using StateIDToObjectMap = std::unordered_map<pipeline_state_descriptor_id_t, CachedStateData>;
		using ConfigHashToStateIDMap = std::unordered_map<InputConfigHashValue, pipeline_state_descriptor_id_t>;
		using NameToStateIDMap = std::unordered_map<cppx::immutable_string , pipeline_state_descriptor_id_t>;

	private:
		// Adapter for the state factory. Allows the cache to simply call CreateState() for every state type.
		FactoryInterface & _stateFactoryInterface;

		// The actual Map which serves as a storage for cached objects.
		StateIDToObjectMap _stateIDToObjectMap;

		//
		ConfigHashToStateIDMap _configHashToStateIDMap;

		//
		NameToStateIDMap _nameToStateIDMap;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_UNIT_H__
