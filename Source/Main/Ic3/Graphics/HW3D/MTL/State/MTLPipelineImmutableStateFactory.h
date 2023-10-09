
#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/PipelineImmutableStateCache.h>

namespace Ic3::Graphics::GCI
{

	class MetalPipelineImmutableStateFactory : public PipelineImmutableStateFactory
	{
	public:
		MetalGPUDevice & mMetalGPUDevice;

	public:
		MetalPipelineImmutableStateFactory( MetalGPUDevice & pMetalGPUDevice );
		virtual ~MetalPipelineImmutableStateFactory();

		virtual BlendImmutableStateHandle createBlendState( const BlendConfig & pConfig ) override final;
		virtual DepthStencilImmutableStateHandle createDepthStencilState( const DepthStencilConfig & pConfig ) override final;
		virtual GraphicsShaderLinkageImmutableStateHandle createGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet ) override final;
		virtual IAInputLayoutImmutableStateHandle createIAInputLayoutState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary ) override final;
		virtual IAVertexStreamImmutableStateHandle createIAVertexStreamState( const IAVertexStreamDefinition & pDefinition ) override final;
		virtual RasterizerImmutableStateHandle createRasterizerState( const RasterizerConfig & pConfig ) override final;
		virtual RenderTargetBindingImmutableStateHandle createRenderTargetBindingState( const RenderTargetBindingDefinition & pDefinition ) override final;
		virtual RenderPassConfigurationImmutableStateHandle createRenderPassState( const RenderPassConfiguration & pConfiguration ) override final;
	};

}

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
