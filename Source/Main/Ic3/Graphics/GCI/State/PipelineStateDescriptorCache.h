
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_H__

#include "PipelineStateCommon.h"
#include "PipelineStateDescriptorCacheUnit.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * Adapter for PipelineStateDescriptorFactory used by state cache. Unifies all creation methods into
	 * a single "CreateStateDescriptor", using overloading to automatically select the right method for a given state.
	 * The selection is based on the type of the input configuration used to create a particular state object.
	 */
	class GraphicsPipelineStateDescriptorFactoryCacheAdapter
	{
	public:
		GraphicsPipelineStateDescriptorFactoryCacheAdapter( GraphicsPipelineStateDescriptorFactory & pStateFactory );
		~GraphicsPipelineStateDescriptorFactoryCacheAdapter();

		BlendStateDescriptorHandle CreateStateDescriptor( const BlendConfig & pConfig );
		DepthStencilStateDescriptorHandle CreateStateDescriptor( const DepthStencilConfig & pConfig );
		MultiSamplingStateDescriptorHandle CreateStateDescriptor( const MultiSamplingConfig & pConfig );
		RasterizerStateDescriptorHandle CreateStateDescriptor( const RasterizerConfig & pConfig );
		GraphicsShaderLinkageStateDescriptorHandle CreateStateDescriptor( const GraphicsShaderSet & pShaderSet );
		IAInputLayoutStateDescriptorHandle CreateStateDescriptor( const GraphicsShaderBinding & pDefinition, Shader & pVertexShaderWithBinary );
		ShaderRootSignatureStateDescriptorHandle CreateStateDescriptor( const ShaderRootSignatureDesc & pRootSignature );

	private:
		GraphicsPipelineStateDescriptorFactory * _stateFactory;
	};

	Ic3DefinePipelineStateDescriptorTraits( Blend, BlendConfig, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( DepthStencil, DepthStencilConfig, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( GraphicsShaderLinkage, GraphicsShaderSet, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( IAInputLayout, IAInputLayoutDefinition, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( Rasterizer, RasterizerConfig, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( MultiSampling, MultiSamplingConfig, GraphicsPipelineStateDescriptorFactoryCacheAdapter );
	Ic3DefinePipelineStateDescriptorTraits( ShaderRootSignature, ShaderRootSignature, GraphicsPipelineStateDescriptorFactoryCacheAdapter );

	/**
	 * 
	 */
	class GraphicsPipelineStateDescriptorCache
	{
	public:
		template <typename TPDescriptorType>
		using DescriptorTraits = PipelineStateDescriptorTraits<TPDescriptorType>;

		GraphicsPipelineStateDescriptorFactory & mStateFactory;

	public:
		GraphicsPipelineStateDescriptorCache( GraphicsPipelineStateDescriptorFactory & pFactory )
		: mStateFactory( pFactory )
		, _stateFactoryAdapter( pFactory )
		, _stateCacheUnitBlend( _stateFactoryAdapter )
		, _stateCacheUnitDepthStencil( _stateFactoryAdapter )
		, _stateCacheUnitGraphicsShaderLinkage( _stateFactoryAdapter )
		, _stateCacheUnitIAInputLayout( _stateFactoryAdapter )
		, _stateCacheUnitRasterizer( _stateFactoryAdapter )
		, _stateCacheUnitMultiSampling( _stateFactoryAdapter )
		, _stateCacheUnitShaderRootSignature( _stateFactoryAdapter )
		{}

		template <typename TPStateDescriptor>
		CPPX_ATTR_NO_DISCARD typename DescriptorTraits<TPStateDescriptor>::DescriptorHandle GetState( pipeline_state_descriptor_id_t pStateID ) const noexcept
		{
			auto & descriptorCacheUnit = _GetCacheUnit<TPStateDescriptor>();
			return descriptorCacheUnit.GetState( pStateID );
		}

		template <typename TPStateDescriptor>
		CPPX_ATTR_NO_DISCARD typename DescriptorTraits<TPStateDescriptor>::DescriptorHandle GetState( const cppx::string_view & pStateName ) const noexcept
		{
			auto & descriptorCacheUnit = _GetCacheUnit<TPStateDescriptor>();
			return descriptorCacheUnit.GetState( pStateName );
		}

		template <typename TPStateDescriptor>
		CPPX_ATTR_NO_DISCARD typename DescriptorTraits<TPStateDescriptor>::DescriptorHandle GetStateForConfig(
				const typename DescriptorTraits<TPStateDescriptor>::InputConfigType & pConfig ) const noexcept
		{
			auto & descriptorCacheUnit = _GetCacheUnit<TPStateDescriptor>();
			return descriptorCacheUnit.GetState( pConfig );
		}

		template <typename TPStateDescriptor, typename TPInputDesc, typename... TArgs>
		TGfxHandle<TPStateDescriptor> CreateStateDescriptor(
				pipeline_state_descriptor_id_t pStateID,
				const typename TPStateDescriptor::InputConfigType & pConfig,
				TArgs && ...pArgs )
		{
			auto & descriptorCacheUnit = _GetCacheUnit<TPStateDescriptor>();
			return descriptorCacheUnit.CreateStateDescriptor( pStateID, pConfig, std::forward<TArgs>( pArgs )... );
		}

		template <typename TPStateDescriptor, typename TPInputDesc, typename... TArgs>
		TGfxHandle<TPStateDescriptor> CreateNamedStateDescriptor(
				pipeline_state_descriptor_id_t pStateID,
				cppx::immutable_string pName,
				const typename TPStateDescriptor::InputConfigType & pConfig,
				TArgs && ...pArgs )
		{
			auto & descriptorCacheUnit = _GetCacheUnit<TPStateDescriptor>();
			return descriptorCacheUnit.CreateNamedStateDescriptor( pStateID, std::move( pName ), pConfig, std::forward<TArgs>( pArgs )... );
		}

		template <typename TPStateDescriptor>
		void ResetSubCache()
		{
			auto & descriptorCacheUnit = _GetCacheUnit<TPStateDescriptor>();
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

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagGraphicsShaderLinkageBit ) )
			{
				_stateCacheUnitGraphicsShaderLinkage.Reset();
			}

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagIAInputLayoutBit ) )
			{
				_stateCacheUnitIAInputLayout.Reset();
			}

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagRasterizerBit ) )
			{
				_stateCacheUnitRasterizer.Reset();
			}

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagMultiSamplingBit ) )
			{
				_stateCacheUnitMultiSampling.Reset();
			}

			if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagShaderRootSignatureBit ) )
			{
				_stateCacheUnitShaderRootSignature.Reset();
			}
		}

	private:
		template <typename TPStateDescriptor>
		CPPX_ATTR_NO_DISCARD PipelineStateDescriptorCacheUnit<TPStateDescriptor> & _GetCacheUnit() const;

	private:
		GraphicsPipelineStateDescriptorFactoryCacheAdapter _stateFactoryAdapter;

		using BlendStateCacheUnit = PipelineStateDescriptorCacheUnit<BlendStateDescriptor>;
		mutable BlendStateCacheUnit _stateCacheUnitBlend;

		using DepthStencilStateCacheUnit = PipelineStateDescriptorCacheUnit<DepthStencilStateDescriptor>;
		mutable DepthStencilStateCacheUnit _stateCacheUnitDepthStencil;

		using GraphicsShaderLinkageStateCacheUnit = PipelineStateDescriptorCacheUnit<GraphicsShaderLinkageStateDescriptor>;
		mutable GraphicsShaderLinkageStateCacheUnit _stateCacheUnitGraphicsShaderLinkage;

		using IAInputLayoutStateCacheUnit = PipelineStateDescriptorCacheUnit<IAInputLayoutStateDescriptor>;
		mutable IAInputLayoutStateCacheUnit _stateCacheUnitIAInputLayout;

		using RasterizerStateCacheUnit = PipelineStateDescriptorCacheUnit<RasterizerStateDescriptor>;
		mutable RasterizerStateCacheUnit _stateCacheUnitRasterizer;

		using MultiSamplingStateCacheUnit = PipelineStateDescriptorCacheUnit<MultiSamplingStateDescriptor>;
		mutable MultiSamplingStateCacheUnit _stateCacheUnitMultiSampling;

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
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( GraphicsShaderLinkage );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( IAInputLayout );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( Rasterizer );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( MultiSampling );
	Ic3GCIPipelineStateCacheDefineUnitAccessProxy( ShaderRootSignature );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_CACHE_H__
