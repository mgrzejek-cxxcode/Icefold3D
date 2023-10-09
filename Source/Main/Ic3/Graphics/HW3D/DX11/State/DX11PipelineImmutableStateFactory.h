
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
#define __IC3_GRAPHICS_HW3D_DX11_PIPELINE_IMMUTABLE_STATE_FACTORY_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/State/PipelineImmutableStateCache.h>

namespace Ic3::Graphics::GCI
{

	struct RenderTargetLayout;

	class DX11PipelineImmutableStateFactory : public PipelineImmutableStateFactorySeparableShader
	{
	public:
		DX11GPUDevice & mDX11GPUDevice;

	public:
		DX11PipelineImmutableStateFactory( DX11GPUDevice & pGPUDevice );
		virtual ~DX11PipelineImmutableStateFactory();

		virtual BlendImmutableStateHandle createBlendState( const BlendConfig & pConfig ) override final;
		virtual DepthStencilImmutableStateHandle createDepthStencilState( const DepthStencilConfig & pConfig ) override final;
		virtual IAInputLayoutImmutableStateHandle createIAInputLayoutState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary ) override final;
		virtual IAVertexStreamImmutableStateHandle createIAVertexStreamState( const IAVertexStreamDefinition & pDefinition ) override final;
		virtual RasterizerImmutableStateHandle createRasterizerState( const RasterizerConfig & pConfig ) override final;
		virtual RenderTargetBindingImmutableStateHandle createRenderTargetBindingState( const RenderTargetBindingDefinition & pDefinition ) override final;
		virtual RenderPassConfigurationImmutableStateHandle createRenderPassState( const RenderPassConfiguration & pConfiguration ) override final;
	};
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
