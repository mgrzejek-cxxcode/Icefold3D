
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_IMMUTABLE_STATE_CACHE_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_IMMUTABLE_STATE_CACHE_H__

#include "GraphicsPipelineImmutableState.h"
#include <unordered_map>

#define Ic3GCIStateSubCacheDeclare( pStateName ) \
	using pStateName##StateSubCache = PipelineImmutableStateSubCache<pStateName##ImmutableState>; \
	mutable pStateName##StateSubCache _stateSubCache##pStateName;

#define Ic3GCIStateSubCacheInit( pStateName, pFactory ) \
	_stateSubCache##pStateName( pFactory )

#define Ic3GCIStateSubCacheDefineProxy( pStateName ) \
	template <> \
	inline PipelineImmutableStateSubCache<pStateName##ImmutableState> & PipelineImmutableStateCache::_SubCacheProxy<pStateName##ImmutableState>() const \
	{ \
		return _stateSubCache##pStateName; \
	}

namespace Ic3::Graphics::GCI
{

	/// @brief An extra wrapper for data required to Create an immutable state object.
	/// Used to pack the data together and avoid changing function signatures when a change is made.
	template <typename TPInputDesc>
	struct PipelineImmutableStateCreateInfo
	{
		/// A unique ID given to an immutable state object.
		GfxObjectID uniqueID = cxGpuObjectIDInvalid;

		/// A unique name given to an immutable state object.
		cppx::ref<const GfxObjectName> uniqueName;

		/// Data needed to Initialize the state. Different for every state type.
		cppx::ref<const TPInputDesc> inputDesc;
	};

#define Ic3GCIMakeStateCreateInfo( pUniqueID, pInputDesc ) \
	PipelineImmutableStateCreateInfo<typename std::decay<decltype( pInputDesc )>::type>{ pUniqueID, #pUniqueID, pInputDesc }

	/// @brief A "sub-cache" used by the actual cache. Manages single state type.
	template <typename TPState>
	class PipelineImmutableStateSubCache
	{
	public:
		PipelineImmutableStateSubCache( PipelineImmutableStateFactory & pFactory )
		: _stateFactoryAdapter( pFactory )
		{}

		~PipelineImmutableStateSubCache() = default;

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPState> GetState( GfxObjectID pStateObjectID ) const noexcept
		{
			const auto existingStateRef = _cachedStates.find( pStateObjectID.AsValue() );

			if( existingStateRef != _cachedStates.end() )
			{
				const auto & cachedStateData = existingStateRef->second;
				return cachedStateData.immutableStateObject;
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD TGfxHandle<TPState> GetState( const GfxObjectName & pStateObjectName ) const noexcept
		{
			const auto uniqueID = GenerateGfxObjectID( pStateObjectName );
			return GetState( uniqueID );
		}

		template <typename TCreateData, typename... TArgs>
		TGfxHandle<TPState> CreateState( PipelineImmutableStateCreateInfo<TCreateData> pCreateInfo, TArgs && ...pArgs )
		{
			const auto controlInputHash = cppx::hash_compute<cppx::hash_algo::fnv1a64>( pCreateInfo.inputDesc.get() );

			if(( pCreateInfo.uniqueID == cxGpuObjectIDAuto ) && !pCreateInfo.uniqueName->empty() )
			{
				pCreateInfo.uniqueID = GenerateGfxObjectID( pCreateInfo.uniqueName );
			}

			if( CxDef::IsGfxObjectIDValid( pCreateInfo.uniqueID ) )
			{
				const auto existingStateRef = _cachedStates.find( pCreateInfo.uniqueID.asValue() );
				if( existingStateRef != _cachedStates.end() )
				{
					const auto & cachedStateData = existingStateRef->second;

					if( cachedStateData.controlInputHash != controlInputHash )
					{
						// The same ID has been generated for another immutable state of the same type. Fix it.
						Ic3Throw( 0 );
					}

					const auto & stateHandle = cachedStateData.immutableStateObject;
					// Return the pointer to the existing state.
					return stateHandle;
				}
			}

			auto newImmutableState = _stateFactoryAdapter.CreateState( pCreateInfo.inputDesc, std::forward<TArgs>( pArgs )... );

			if( !newImmutableState )
			{
				return nullptr;
			}

			if( !CxDef::IsGfxObjectIDValid( pCreateInfo.uniqueID ) )
			{
				pCreateInfo.uniqueID.idValue = reinterpret_cast<uint64>( newImmutableState.get() );
			}

			CachedStateData cachedStateData{};
			cachedStateData.controlInputHash = controlInputHash;
			cachedStateData.immutableStateObject = newImmutableState;

			_cachedStates[pCreateInfo.uniqueID] = std::move( cachedStateData );

			return newImmutableState;
		}

		void Reset()
		{
			_cachedStates.clear();
		}

	private:
		/// The data stored internally for every cached state object.
		struct CachedStateData
		{
			using ControlInputHash = hash_object<cppx::hash_algo::fnv1a64>;
			/// Control hash which is a hash of the inputDesc (passed inside the createInfo struct).
			ControlInputHash controlInputHash;
			/// The actual immutable state.
			TGfxHandle<TPState> immutableStateObject;
		};

		using cache_map_key_t = GfxObjectID::ValueType;
		using CachedStateMap = std::unordered_map<cache_map_key_t, CachedStateData>;

	private:
		/// Adapter for the state factory. Allows the cache to simply call CreateState() for every state type.
		PipelineImmutableStateFactoryAdapter _stateFactoryAdapter;
		/// The actual Map which serves as a storage for cached objects.
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
		, Ic3GCIStateSubCacheInit( Blend, mStateFactory )
		, Ic3GCIStateSubCacheInit( DepthStencil, mStateFactory )
		, Ic3GCIStateSubCacheInit( GraphicsShaderLinkage, mStateFactory )
		, Ic3GCIStateSubCacheInit( IAInputLayout, mStateFactory )
		, Ic3GCIStateSubCacheInit( IAVertexStream, mStateFactory )
		, Ic3GCIStateSubCacheInit( Rasterizer, mStateFactory )
		, Ic3GCIStateSubCacheInit( RenderTargetBinding, mStateFactory )
		, Ic3GCIStateSubCacheInit( RenderPassConfiguration, mStateFactory )
		{}

		template <typename TPState>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPState> GetState( GfxObjectID pStateObjectID ) const noexcept
		{
			auto & subCache = _SubCacheProxy<TPState>();
			return subCache.GetState( pStateObjectID );
		}

		template <typename TPState>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPState> GetState( const GfxObjectName & pStateObjectName ) const noexcept
		{
			auto & subCache = _SubCacheProxy<TPState>();
			return subCache.GetState( pStateObjectName );
		}

		template <typename TPState, typename TPInputDesc, typename... TArgs>
		TGfxHandle<TPState> CreateState( GfxObjectID pUniqueID, const TPInputDesc & pInputDesc, TArgs && ...pArgs )
		{
			PipelineImmutableStateCreateInfo<TPInputDesc> createInfo{};
			createInfo.uniqueID = pUniqueID;
			createInfo.inputDesc = std::ref( pInputDesc );
			auto & subCache = _SubCacheProxy<TPState>();
			return subCache.CreateState( std::move( createInfo ), std::forward<TArgs>( pArgs )... );
		}

		template <typename TPState, typename TPInputDesc, typename... TArgs>
		TGfxHandle<TPState> CreateState( const GfxObjectName & pUniqueName, const TPInputDesc & pInputDesc, TArgs && ...pArgs )
		{
			PipelineImmutableStateCreateInfo<TPInputDesc> createInfo{};
			createInfo.uniqueName = std::ref( pUniqueName );
			createInfo.inputDesc = std::ref( pInputDesc );
			auto & subCache = _SubCacheProxy<TPState>();
			return subCache.CreateState( std::move( createInfo ), std::forward<TArgs>( pArgs )... );
		}

		template <typename TPState>
		void ResetSubCache()
		{
			auto & subCache = _SubCacheProxy<TPState>();
			return subCache.reset();
		}

		void Reset( cppx::bitmask<EPipelineImmutableStateTypeFlags> pResetMask = ePipelineImmutableStateTypeMaskAll )
		{
			if( pResetMask.is_set( ePipelineImmutableStateTypeFlagBlendBit ) )
				_stateSubCacheBlend.Reset();

			if( pResetMask.is_set( ePipelineImmutableStateTypeFlagDepthStencilBit ) )
				_stateSubCacheDepthStencil.Reset();

			if( pResetMask.is_set( ePipelineImmutableStateTypeFlagGraphicsShaderLinkageBit ) )
				_stateSubCacheGraphicsShaderLinkage.Reset();

			if( pResetMask.is_set( ePipelineImmutableStateTypeFlagIAInputLayoutBit ) )
				_stateSubCacheIAInputLayout.Reset();

			if( pResetMask.is_set( ePipelineImmutableStateTypeFlagIAVertexStreamBit ) )
				_stateSubCacheIAVertexStream.Reset();

			if( pResetMask.is_set( ePipelineImmutableStateTypeFlagRasterizerBit ) )
				_stateSubCacheRasterizer.Reset();

			if( pResetMask.is_set( ePipelineImmutableStateTypeFlagRenderTargetBindingBit ) )
				_stateSubCacheRenderTargetBinding.Reset();

			if( pResetMask.is_set( ePipelineImmutableStateTypeFlagRenderPassBit ) )
				_stateSubCacheRenderPassConfiguration.Reset();
		}

	private:
		template <typename TPState>
		CPPX_ATTR_NO_DISCARD PipelineImmutableStateSubCache<TPState> & _SubCacheProxy() const;

	private:
		Ic3GCIStateSubCacheDeclare( Blend );
		Ic3GCIStateSubCacheDeclare( DepthStencil );
		Ic3GCIStateSubCacheDeclare( GraphicsShaderLinkage );
		Ic3GCIStateSubCacheDeclare( IAInputLayout );
		Ic3GCIStateSubCacheDeclare( IAVertexStream );
		Ic3GCIStateSubCacheDeclare( Rasterizer );
		Ic3GCIStateSubCacheDeclare( RenderTargetBinding );
		Ic3GCIStateSubCacheDeclare( RenderPassConfiguration );
	};

	Ic3GCIStateSubCacheDefineProxy( Blend );
	Ic3GCIStateSubCacheDefineProxy( DepthStencil );
	Ic3GCIStateSubCacheDefineProxy( GraphicsShaderLinkage );
	Ic3GCIStateSubCacheDefineProxy( IAInputLayout );
	Ic3GCIStateSubCacheDefineProxy( IAVertexStream );
	Ic3GCIStateSubCacheDefineProxy( Rasterizer );
	Ic3GCIStateSubCacheDefineProxy( RenderTargetBinding );
	Ic3GCIStateSubCacheDefineProxy( RenderPassConfiguration );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_IMMUTABLE_STATE_CACHE_H__
