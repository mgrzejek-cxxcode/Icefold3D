
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_H__

#include "PipelineStateCommon.h"
#include "PipelineStateDescriptorCacheUnit.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * Adapter for PipelineStateDescriptorFactory used by state cache. Unifies all creation methods into
	 * a single "CreateDescriptor", using overloading to automatically select the right method for a given state.
	 * The selection is based on the type of the input configuration used to create a particular state object.
	 */
	class GraphicsPipelineStateDescriptorFactoryCacheAdapter
	{
	public:
		GraphicsPipelineStateDescriptorFactoryCacheAdapter( PipelineStateDescriptorFactory & pDescriptorFactory );
		~GraphicsPipelineStateDescriptorFactoryCacheAdapter();

		BlendStateDescriptorHandle CreateDescriptor( const BlendStateDescriptorCreateInfo & pCreateInfo );
		DepthStencilStateDescriptorHandle CreateDescriptor( const DepthStencilStateDescriptorCreateInfo & pCreateInfo );
		RasterizerStateDescriptorHandle CreateDescriptor( const RasterizerStateDescriptorCreateInfo & pCreateInfo );
		GraphicsShaderLinkageStateDescriptorHandle CreateDescriptor( const GraphicsShaderLinkageStateDescriptorCreateInfo & pCreateInfo );
		IAVertexAttributeLayoutStateDescriptorHandle CreateDescriptor( const IAVertexAttributeLayoutStateDescriptorCreateInfo & pCreateInfo );
		ShaderRootSignatureStateDescriptorHandle CreateDescriptor( const ShaderRootSignatureStateDescriptorCreateInfo & pCreateInfo );

	private:
		PipelineStateDescriptorFactory * _descriptorFactory;
	};

	Ic3DefinePipelineStateDescriptorTraits( Blend, BlendSettings, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( DepthStencil, DepthStencilSettings, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( Rasterizer, RasterizerSettings, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( GraphicsShaderLinkage, GraphicsShaderBinding, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( IAVertexAttributeLayout, IAVertexAttributeLayoutDefinition, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( ShaderRootSignature, ShaderRootSignatureDesc, GraphicsPipelineStateDescriptorFactoryCacheAdapter );

	/**
	 * 
	 */
	class GraphicsPipelineStateDescriptorCache
	{
	public:
		template <typename TPDescriptorType>
		using DescriptorTraits = PipelineStateDescriptorTraits<TPDescriptorType>;

		PipelineStateDescriptorFactory & mDescriptorFactory;

	public:
		GraphicsPipelineStateDescriptorCache( PipelineStateDescriptorFactory & pDescriptorFactory )
		: mDescriptorFactory( pDescriptorFactory )
		, _descriptorFactoryAdapter( pDescriptorFactory )
		, _stateCacheUnitBlend( _descriptorFactoryAdapter )
		, _stateCacheUnitDepthStencil( _descriptorFactoryAdapter )
		, _stateCacheUnitRasterizer( _descriptorFactoryAdapter )
		, _stateCacheUnitGraphicsShaderLinkage( _descriptorFactoryAdapter )
		, _stateCacheUnitIAVertexAttributeLayout( _descriptorFactoryAdapter )
		, _stateCacheUnitShaderRootSignature( _descriptorFactoryAdapter )
		{}

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD const PipelineStateDescriptorCacheUnit<TPDescriptorType> & GetSubCache() const
		{
			return _GetCacheUnit<TPDescriptorType>();
		}

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetDescriptorByID(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.GetDescriptorByID( pDescriptorID );
		}

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetDescriptorByName(
				const cppx::string_view & pDescriptorName ) const noexcept
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.GetDescriptorByName( pDescriptorName );
		}

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetDescriptorForConfig(
				const typename DescriptorTraits<TPDescriptorType>::InputConfigType & pConfig ) const noexcept
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.GetDescriptorForConfig( pConfig );
		}

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD bool HasDescriptorWithID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.HasDescriptorWithID( pDescriptorID );
		}

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD bool HasDescriptorWithName( const cppx::string_view & pDescriptorName ) const noexcept
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.HasDescriptorWithName( pDescriptorName );
		}

		template <typename TPDescriptorType, typename TPCreateInfo>
		TGfxHandle<TPDescriptorType> CreateDescriptor( const TPCreateInfo & pCreateInfo, cppx::immutable_string pDescriptorName = {} )
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.CreateDescriptor( pCreateInfo, pDescriptorName );
		}

		template <typename TPDescriptorType>
		void ResetSubCache()
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.Reset();
		}

		void Reset( cppx::bitmask<EPipelineStateDescriptorTypeFlags> pResetMask = ePipelineStateDescriptorTypeMaskAll )
		{
			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagBlendBit ) )
			{
				_stateCacheUnitBlend.Reset();
			}

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagDepthStencilBit ) )
			{
				_stateCacheUnitDepthStencil.Reset();
			}

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagRasterizerBit ) )
			{
				_stateCacheUnitRasterizer.Reset();
			}

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagGraphicsShaderLinkageBit ) )
			{
				_stateCacheUnitGraphicsShaderLinkage.Reset();
			}

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagIAVertexAttributeLayoutBit ) )
			{
				_stateCacheUnitIAVertexAttributeLayout.Reset();
			}

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagShaderRootSignatureBit ) )
			{
				_stateCacheUnitShaderRootSignature.Reset();
			}
		}

	private:
		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD PipelineStateDescriptorCacheUnit<TPDescriptorType> & _GetCacheUnit() const;

	private:
		GraphicsPipelineStateDescriptorFactoryCacheAdapter _descriptorFactoryAdapter;

		using BlendStateCacheUnit = PipelineStateDescriptorCacheUnit<BlendStateDescriptor>;
		mutable BlendStateCacheUnit _stateCacheUnitBlend;

		using DepthStencilStateCacheUnit = PipelineStateDescriptorCacheUnit<DepthStencilStateDescriptor>;
		mutable DepthStencilStateCacheUnit _stateCacheUnitDepthStencil;

		using RasterizerStateCacheUnit = PipelineStateDescriptorCacheUnit<RasterizerStateDescriptor>;
		mutable RasterizerStateCacheUnit _stateCacheUnitRasterizer;

		using GraphicsShaderLinkageStateCacheUnit = PipelineStateDescriptorCacheUnit<GraphicsShaderLinkageStateDescriptor>;
		mutable GraphicsShaderLinkageStateCacheUnit _stateCacheUnitGraphicsShaderLinkage;

		using IAVertexAttributeLayoutStateCacheUnit = PipelineStateDescriptorCacheUnit<IAVertexAttributeLayoutStateDescriptor>;
		mutable IAVertexAttributeLayoutStateCacheUnit _stateCacheUnitIAVertexAttributeLayout;

		using ShaderRootSignatureStateCacheUnit = PipelineStateDescriptorCacheUnit<ShaderRootSignatureStateDescriptor>;
		mutable ShaderRootSignatureStateCacheUnit _stateCacheUnitShaderRootSignature;
	};

#define Ic3GCIPipelineStateCacheDefineUnitAccessProxy( pStateName ) \
	template <> \
	inline PipelineStateDescriptorCacheUnit<pStateName##StateDescriptor> & \
	GraphicsPipelineStateDescriptorCache::_GetCacheUnit<pStateName##StateDescriptor>() const \
	{ \
		return _stateCacheUnit##pStateName; \
	}

	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( Blend );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( DepthStencil );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( Rasterizer );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( GraphicsShaderLinkage );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( IAVertexAttributeLayout );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( ShaderRootSignature );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_H__
