
#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/SharedCompiledStateCache.h>

namespace Ic3::Graphics::GCI
{

	class MetalPipelineStateDescriptorFactory : public PipelineStateDescriptorFactory
	{
	public:
		MetalGPUDevice & mMetalGPUDevice;

	public:
		MetalPipelineStateDescriptorFactory( MetalGPUDevice & pMetalGPUDevice );
		virtual ~MetalPipelineStateDescriptorFactory();

		virtual BlendStateDescriptorHandle CreateBlendState( const BlendSettings & pSettings ) override final;
		virtual DepthStencilStateDescriptorHandle CreateDepthStencilState( const DepthStencilSettings & pSettings ) override final;
		virtual GraphicsShaderLinkageCompiledStateHandle CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet ) override final;
		virtual VertexAttributeLayoutDescriptorHandle CreateIAVertexAttributeLayout( const IAVertexAttributeLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary ) override final;
		virtual VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor( const IADataStreamArrayConfiguration & pDefinition ) override final;
		virtual RasterizerStateDescriptorHandle CreateRasterizerState( const RasterizerSettings & pSettings ) override final;
		virtual RenderTargetBindingCompiledStateHandle CreateRenderTargetBindingState( const RenderTargetBindingDefinition & pDefinition ) override final;
		virtual RenderPassConfigurationCompiledStateHandle CreateRenderPassState( const RenderPassConfiguration & pConfiguration ) override final;
	};

}

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
