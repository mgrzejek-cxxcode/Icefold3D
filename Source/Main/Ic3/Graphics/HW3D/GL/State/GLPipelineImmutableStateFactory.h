
#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/PipelineImmutableStateCache.h>

namespace Ic3::Graphics::GCI
{

	struct RenderTargetLayout;

	class GLPipelineImmutableStateFactory : public PipelineImmutableStateFactory
	{
	public:
		GLGPUDevice & mGLGPUDevice;

	public:
		GLPipelineImmutableStateFactory( GLGPUDevice & pGPUDevice );
		virtual ~GLPipelineImmutableStateFactory();

		virtual BlendImmutableStateHandle CreateBlendState( const BlendConfig & pConfig ) override final;
		virtual DepthStencilImmutableStateHandle CreateDepthStencilState( const DepthStencilConfig & pConfig ) override final;
		virtual IAVertexStreamImmutableStateHandle CreateIAVertexStreamState( const IAVertexStreamDefinition & pDefinition ) override final;
		virtual RasterizerImmutableStateHandle CreateRasterizerState( const RasterizerConfig & pConfig ) override final;
		virtual RenderTargetBindingImmutableStateHandle CreateRenderTargetBindingState( const RenderTargetBindingDefinition & pDefinition ) override final;
		virtual RenderPassConfigurationImmutableStateHandle CreateRenderPassState( const RenderPassConfiguration & pConfiguration ) override final;
	};

	class GLPipelineImmutableStateFactoryCore : public GLPipelineImmutableStateFactory
	{
	public:
		GLPipelineImmutableStateFactoryCore( GLGPUDevice & pGPUDevice );
		virtual ~GLPipelineImmutableStateFactoryCore();

		virtual GraphicsShaderLinkageImmutableStateHandle CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet ) override final;
		virtual IAInputLayoutImmutableStateHandle CreateIAInputLayoutState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary ) override final;
	};

	class GLPipelineImmutableStateFactoryCompat : public GLPipelineImmutableStateFactory
	{
	public:
		GLPipelineImmutableStateFactoryCompat( GLGPUDevice & pGPUDevice );
		virtual ~GLPipelineImmutableStateFactoryCompat();

		virtual GraphicsShaderLinkageImmutableStateHandle CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet ) override final;
		virtual IAInputLayoutImmutableStateHandle CreateIAInputLayoutState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary ) override final;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
