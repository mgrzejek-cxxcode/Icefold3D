
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
#define __IC3_GRAPHICS_HW3D_DX11_PIPELINE_IMMUTABLE_STATE_FACTORY_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/State/SharedCompiledStateCache.h>

namespace Ic3::Graphics::GCI
{

	struct RenderTargetLayout;

	class DX11PipelineStateDescriptorFactory : public PipelineStateDescriptorFactorySeparableShader
	{
	public:
		DX11GPUDevice & mDX11GPUDevice;

	public:
		DX11PipelineStateDescriptorFactory( DX11GPUDevice & pGPUDevice );
		virtual ~DX11PipelineStateDescriptorFactory();

		virtual BlendStateDescriptorHandle CreateBlendState( const BlendSettings & pSettings ) override final;
		virtual DepthStencilStateDescriptorHandle CreateDepthStencilState( const DepthStencilSettings & pSettings ) override final;
		virtual VertexAttributeLayoutDescriptorHandle CreateIAVertexAttributeLayout( const IAVertexAttributeLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary ) override final;
		virtual VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor( const IADataStreamArrayConfiguration & pDefinition ) override final;
		virtual RasterizerStateDescriptorHandle CreateRasterizerState( const RasterizerSettings & pSettings ) override final;
		virtual RenderTargetBindingCompiledStateHandle CreateRenderTargetBindingState( const RenderTargetBindingDefinition & pDefinition ) override final;
		virtual RenderPassConfigurationCompiledStateHandle CreateRenderPassState( const RenderPassConfiguration & pConfiguration ) override final;
	};
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
