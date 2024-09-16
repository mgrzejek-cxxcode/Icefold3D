
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_IMMUTABLE_STATE_CACHE_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_IMMUTABLE_STATE_CACHE_H__

#include "GraphicsPipelineImmutableState.h"
#include <unordered_map>

#define ic3GpaStateSubCacheDeclare( pStateName ) \
	using pStateName##StateSubCache = PipelineImmutableStateSubCache<pStateName##ImmutableState>; \
	mutable pStateName##StateSubCache _stateSubCache##pStateName;

#define ic3GpaStateSubCacheInit( pStateName, pFactory ) \
	_stateSubCache##pStateName( pFactory )

#define ic3GpaStateSubCacheDefineProxy( pStateName ) \
	template <> \
	inline PipelineImmutableStateSubCache<pStateName##ImmutableState> & PipelineImmutableStateCache::_subCacheProxy<pStateName##ImmutableState>() const \
	{ \
		return _stateSubCache##pStateName; \
	}

namespace Ic3::Graphics::GCI
{

	/// @brief An extra wrapper for data required to create an immutable state object.
	/// Used to pack the data together and avoid changing function signatures when a change is made.
	template <typename TPInputDesc>
	struct PipelineImmutableStateCreateInfo
	{
		/// A unique name given to an immutable state object.
		UniqueGPUObjectID uniqueID = CxDef::GPU_OBJECT_ID_INVALID;

		/// A unique name given to an immutable state object.
		RefWrapper<const UniqueGPUObjectName> uniqueName;

		/// Data needed to initialize the state. Different for every state type.
		RefWrapper<const TPInputDesc> inputDesc;
	};

	/// @brief A "sub-cache" used by the actual cache. Manages single state type.
	template <typename TState>
	class PipelineImmutableStateSubCache
	{
	public:
		PipelineImmutableStateSubCache( PipelineImmutableStateFactory & pFactory )
		: _stateFactoryAdapter( pFactory )
		{}

		~PipelineImmutableStateSubCache() = default;

		IC3_ATTR_NO_DISCARD TGPAHandle<TState> getState( UniqueGPUObjectID pStateObjectID ) const noexcept
		{
			const auto existingStateRef = _cachedStates.find( pStateObjectID.asValue() );

			if( existingStateRef != _cachedStates.end() )
			{
				const auto & cachedStateData = existingStateRef->second;
				return cachedStateData.immutableStateObject;
			}

			return nullptr;
		}

		IC3_ATTR_NO_DISCARD TGPAHandle<TState> getState( const UniqueGPUObjectName & pStateObjectName ) const noexcept
		{
			const auto uniqueID = generateUniqueGPUObjectID( pStateObjectName );
			return getState( uniqueID );
		}

		template <typename TCreateData, typename... TArgs>
		TGPAHandle<TState> createState( PipelineImmutableStateCreateInfo<TCreateData> pCreateInfo, TArgs && ...pArgs )
		{
			const auto controlInputHash = hashCompute<EHashAlgo::FNV1A64>( pCreateInfo.inputDesc.get() );

			if( ( pCreateInfo.uniqueID == CxDef::GPU_OBJECT_ID_AUTO ) && !pCreateInfo.uniqueName->empty() )
			{
				pCreateInfo.uniqueID = generateUniqueGPUObjectID( pCreateInfo.uniqueName );
			}

			if( CxDef::isUniqueGPUObjectIDValid( pCreateInfo.uniqueID ) )
			{
				const auto existingStateRef = _cachedStates.find( pCreateInfo.uniqueID.asValue() );
				if( existingStateRef != _cachedStates.end() )
				{
					const auto & cachedStateData = existingStateRef->second;

					if( cachedStateData.controlInputHash != controlInputHash )
					{
						// The same ID has been generated for another immutable state of the same type. Fix it.
						ic3Throw( 0 );
					}

					const auto & stateHandle = cachedStateData.immutableStateObject;
					// Return the pointer to the existing state.
					return stateHandle;
				}
			}

			auto newImmutableState = _stateFactoryAdapter.createState( pCreateInfo.inputDesc, std::forward<TArgs>( pArgs )... );

			if( !newImmutableState )
			{
				return nullptr;
			}

			if( !CxDef::isUniqueGPUObjectIDValid( pCreateInfo.uniqueID ) )
			{
				pCreateInfo.uniqueID.idValue = reinterpret_cast<uint64>( newImmutableState.get() );
			}

			CachedStateData cachedStateData{};
			cachedStateData.controlInputHash = controlInputHash;
			cachedStateData.immutableStateObject = newImmutableState;

			_cachedStates[pCreateInfo.uniqueID] = std::move( cachedStateData );

			return newImmutableState;
		}

		void reset()
		{
			_cachedStates.clear();
		}

	private:
		/// The data stored internally for every cached state object.
		struct CachedStateData
		{
			using ControlInputHash = THashObject<EHashAlgo::FNV1A64>;
			/// Control hash which is a hash of the inputDesc (passed inside the createInfo struct).
			ControlInputHash controlInputHash;
			/// The actual immutable state.
			TGPAHandle<TState> immutableStateObject;
		};

		using cache_map_key_t = UniqueGPUObjectID::ValueType;
		using CachedStateMap = std::unordered_map<cache_map_key_t, CachedStateData>;

	private:
		/// Adapter for the state factory. Allows the cache to simply call createState() for every state type.
		PipelineImmutableStateFactoryAdapter _stateFactoryAdapter;
		/// The actual map which serves as a storage for cached objects.
		CachedStateMap _cachedStates;
	};

	/// @brief
	class PipelineImmutableStateCache
	{
	public:
		PipelineImmutableStateFactory & mStateFactory;

	public:
		PipelineImmutableStateCache( PipelineImmutableStateFactory & pFactory )
		: mStateFactory( pFactory )
		, ic3GpaStateSubCacheInit( Blend, mStateFactory )
		, ic3GpaStateSubCacheInit( DepthStencil, mStateFactory )
		, ic3GpaStateSubCacheInit( GraphicsShaderLinkage, mStateFactory )
		, ic3GpaStateSubCacheInit( IAInputLayout, mStateFactory )
		, ic3GpaStateSubCacheInit( IAVertexStream, mStateFactory )
		, ic3GpaStateSubCacheInit( Rasterizer, mStateFactory )
		, ic3GpaStateSubCacheInit( RenderTargetBinding, mStateFactory )
		, ic3GpaStateSubCacheInit( RenderPassConfiguration, mStateFactory )
		{}

		template <typename TState>
		IC3_ATTR_NO_DISCARD TGPAHandle<TState> getState( UniqueGPUObjectID pStateObjectID ) const noexcept
		{
			auto & subCache = _subCacheProxy<TState>();
			return subCache.getState( pStateObjectID );
		}

		template <typename TState>
		IC3_ATTR_NO_DISCARD TGPAHandle<TState> getState( const UniqueGPUObjectName & pStateObjectName ) const noexcept
		{
			auto & subCache = _subCacheProxy<TState>();
			return subCache.getState( pStateObjectName );
		}

		template <typename TState, typename TPInputDesc, typename... TArgs>
		TGPAHandle<TState> createState( UniqueGPUObjectID pUniqueID, const TPInputDesc & pInputDesc, TArgs && ...pArgs )
		{
			PipelineImmutableStateCreateInfo<TPInputDesc> createInfo{};
			createInfo.uniqueID = pUniqueID;
			createInfo.inputDesc = std::ref( pInputDesc );
			auto & subCache = _subCacheProxy<TState>();
			return subCache.createState( std::move( createInfo ), std::forward<TArgs>( pArgs )... );
		}

		template <typename TState, typename TPInputDesc, typename... TArgs>
		TGPAHandle<TState> createState( const UniqueGPUObjectName & pUniqueName, const TPInputDesc & pInputDesc, TArgs && ...pArgs )
		{
			PipelineImmutableStateCreateInfo<TPInputDesc> createInfo{};
			createInfo.uniqueName = std::ref( pUniqueName );
			createInfo.inputDesc = std::ref( pInputDesc );
			auto & subCache = _subCacheProxy<TState>();
			return subCache.createState( std::move( createInfo ), std::forward<TArgs>( pArgs )... );
		}

		template <typename TState>
		void resetSubCache()
		{
			auto & subCache = _subCacheProxy<TState>();
			return subCache.reset();
		}

		void reset( TBitmask<EPipelineImmutableStateTypeFlags> pResetMask = ePipelineImmutableStateTypeMaskAll )
		{
			if( pResetMask.isSet( ePipelineImmutableStateTypeFlagBlendBit ) )
				_stateSubCacheBlend.reset();

			if( pResetMask.isSet( ePipelineImmutableStateTypeFlagDepthStencilBit ) )
				_stateSubCacheDepthStencil.reset();

			if( pResetMask.isSet( ePipelineImmutableStateTypeFlagGraphicsShaderLinkageBit ) )
				_stateSubCacheGraphicsShaderLinkage.reset();

			if( pResetMask.isSet( ePipelineImmutableStateTypeFlagIAInputLayoutBit ) )
				_stateSubCacheIAInputLayout.reset();

			if( pResetMask.isSet( ePipelineImmutableStateTypeFlagIAVertexStreamBit ) )
				_stateSubCacheIAVertexStream.reset();

			if( pResetMask.isSet( ePipelineImmutableStateTypeFlagRasterizerBit ) )
				_stateSubCacheRasterizer.reset();

			if( pResetMask.isSet( ePipelineImmutableStateTypeFlagRenderTargetBindingBit ) )
				_stateSubCacheRenderTargetBinding.reset();

			if( pResetMask.isSet( ePipelineImmutableStateTypeFlagRenderPassBit ) )
				_stateSubCacheRenderPassConfiguration.reset();
		}

	private:
		template <typename TState>
		IC3_ATTR_NO_DISCARD PipelineImmutableStateSubCache<TState> & _subCacheProxy() const;

	private:
		ic3GpaStateSubCacheDeclare( Blend );
		ic3GpaStateSubCacheDeclare( DepthStencil );
		ic3GpaStateSubCacheDeclare( GraphicsShaderLinkage );
		ic3GpaStateSubCacheDeclare( IAInputLayout );
		ic3GpaStateSubCacheDeclare( IAVertexStream );
		ic3GpaStateSubCacheDeclare( Rasterizer );
		ic3GpaStateSubCacheDeclare( RenderTargetBinding );
		ic3GpaStateSubCacheDeclare( RenderPassConfiguration );
	};

	ic3GpaStateSubCacheDefineProxy( Blend );
	ic3GpaStateSubCacheDefineProxy( DepthStencil );
	ic3GpaStateSubCacheDefineProxy( GraphicsShaderLinkage );
	ic3GpaStateSubCacheDefineProxy( IAInputLayout );
	ic3GpaStateSubCacheDefineProxy( IAVertexStream );
	ic3GpaStateSubCacheDefineProxy( Rasterizer );
	ic3GpaStateSubCacheDefineProxy( RenderTargetBinding );
	ic3GpaStateSubCacheDefineProxy( RenderPassConfiguration );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_IMMUTABLE_STATE_CACHE_H__
