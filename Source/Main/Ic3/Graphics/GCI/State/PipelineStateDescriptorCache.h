
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
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateDescriptorFactoryCacheAdapter
	{
	public:
		GraphicsPipelineStateDescriptorFactoryCacheAdapter( PipelineStateDescriptorFactory & pDescriptorFactory );
		~GraphicsPipelineStateDescriptorFactoryCacheAdapter();

		BlendStateDescriptorHandle CreateDescriptor( const BlendStateDescriptorCreateInfo & pCreateInfo );

		DepthStencilStateDescriptorHandle CreateDescriptor( const DepthStencilStateDescriptorCreateInfo & pCreateInfo );

		RasterizerStateDescriptorHandle CreateDescriptor( const RasterizerStateDescriptorCreateInfo & pCreateInfo );

		GraphicsShaderLinkageDescriptorHandle CreateDescriptor( const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo );

		VertexAttributeLayoutDescriptorHandle CreateDescriptor( const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo );

		RootSignatureDescriptorHandle CreateDescriptor( const RootSignatureDescriptorCreateInfo & pCreateInfo );

	private:
		PipelineStateDescriptorFactory * _descriptorFactory;
	};

	Ic3DefinePipelineStateDescriptorTraits( BlendState, BlendSettings, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( DepthStencilState, DepthStencilSettings, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( RasterizerState, RasterizerSettings, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( GraphicsShaderLinkage, GraphicsShaderBinding, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( VertexAttributeLayout, IAVertexAttributeLayoutDefinition, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( RootSignature, RootSignatureDesc, GraphicsPipelineStateDescriptorFactoryCacheAdapter );

	/**
	 * 
	 */
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateDescriptorCache
	{
	public:
		template <typename TPDescriptorType>
		using DescriptorTraits = PipelineStateDescriptorTraits<TPDescriptorType>;

		PipelineStateDescriptorFactory & mDescriptorFactory;

	public:
		GraphicsPipelineStateDescriptorCache( PipelineStateDescriptorFactory & pDescriptorFactory );

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
		CPPX_ATTR_NO_DISCARD bool HasDescriptorWithID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.HasDescriptorWithID( pDescriptorID );
		}

		template <typename TPDescriptorType, typename TPCreateInfo>
		TPipelineStateDescriptoCreateResult<TPDescriptorType> CreateDescriptor( const TPCreateInfo & pCreateInfo )
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.CreateDescriptor( pCreateInfo );
		}

		template <typename TPDescriptorType>
		void ResetSubCache()
		{
			PipelineStateDescriptorCacheUnit<TPDescriptorType> & descriptorCacheUnit = _GetCacheUnit<TPDescriptorType>();
			return descriptorCacheUnit.Reset();
		}

		void Reset( cppx::bitmask<EPipelineStateDescriptorTypeFlags> pResetMask = ePipelineStateDescriptorTypeMaskAll );

	private:
		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD PipelineStateDescriptorCacheUnit<TPDescriptorType> & _GetCacheUnit() const noexcept;

	private:
		GraphicsPipelineStateDescriptorFactoryCacheAdapter _descriptorFactoryAdapter;

		using BlendStateCacheUnit = PipelineStateDescriptorCacheUnit<BlendStateDescriptor>;
		mutable BlendStateCacheUnit _cacheUnitBlendState;

		using DepthStencilStateCacheUnit = PipelineStateDescriptorCacheUnit<DepthStencilStateDescriptor>;
		mutable DepthStencilStateCacheUnit _cacheUnitDepthStencilState;

		using RasterizerStateCacheUnit = PipelineStateDescriptorCacheUnit<RasterizerStateDescriptor>;
		mutable RasterizerStateCacheUnit _cacheUnitRasterizerState;

		using GraphicsShaderLinkageStateCacheUnit = PipelineStateDescriptorCacheUnit<GraphicsShaderLinkageDescriptor>;
		mutable GraphicsShaderLinkageStateCacheUnit _cacheUnitGraphicsShaderLinkage;

		using VertexAttributeLayoutCacheUnit = PipelineStateDescriptorCacheUnit<VertexAttributeLayoutDescriptor>;
		mutable VertexAttributeLayoutCacheUnit _cacheUnitVertexAttributeLayout;

		using RootSignatureCacheUnit = PipelineStateDescriptorCacheUnit<RootSignatureDescriptor>;
		mutable RootSignatureCacheUnit _cacheUnitRootSignature;
	};

#define Ic3GCIPipelineStateCacheDefineUnitAccessProxy( pStateName ) \
	template <> \
	inline PipelineStateDescriptorCacheUnit<pStateName##Descriptor> & \
	GraphicsPipelineStateDescriptorCache::_GetCacheUnit<pStateName##Descriptor>() const noexcept \
	{ \
		return _cacheUnit##pStateName; \
	}

	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( BlendState );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( DepthStencilState );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( RasterizerState );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( GraphicsShaderLinkage );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( VertexAttributeLayout );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( RootSignature );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_H__
