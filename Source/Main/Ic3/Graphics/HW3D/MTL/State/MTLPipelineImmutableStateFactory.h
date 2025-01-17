
#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/PipelineImmutableStateCache.h>

namespace Ic3::Graphics::GCI
{

	class MetalPipelineImmutableStateFactory : public PipelineImmutableStateFactory
	{
	public:
		MetalGpuDevice & mMetalGpuDevice;

	public:
		MetalPipelineImmutableStateFactory( MetalGpuDevice & pMetalGpuDevice );
		virtual ~MetalPipelineImmutableStateFactory();

		virtual BlendImmutableStateHandle CreateBlendState( const BlendConfig & pConfig ) override final;
		virtual DepthStencilImmutableStateHandle CreateDepthStencilState( const DepthStencilConfig & pConfig ) override final;
		virtual GraphicsShaderLinkageImmutableStateHandle CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet ) override final;
		virtual IAInputLayoutImmutableStateHandle CreateIAInputLayoutState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary ) override final;
		virtual IAVertexStreamImmutableStateHandle CreateIAVertexStreamState( const IAVertexStreamDefinition & pDefinition ) override final;
		virtual RasterizerImmutableStateHandle CreateRasterizerState( const RasterizerConfig & pConfig ) override final;
		virtual RenderTargetBindingImmutableStateHandle CreateRenderTargetBindingState( const RenderTargetBindingDefinition & pDefinition ) override final;
		virtual RenderPassConfigurationImmutableStateHandle CreateRenderPassState( const RenderPassConfiguration & pConfiguration ) override final;
	};

}

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
