
#include "GLPipelineImmutableStateFactory.h"
#include "GLCommonGraphicsConfig.h"
#include "GLGraphicsShaderState.h"
#include "GLInputAssembler.h"
#include "GLRenderTarget.h"
#include "../GLGPUDevice.h"

namespace Ic3::Graphics::GCI
{

	GLPipelineImmutableStateFactory::GLPipelineImmutableStateFactory( GLGPUDevice & pGPUDevice )
	: PipelineImmutableStateFactory()
	, mGLGPUDevice( pGPUDevice )
	{}

	GLPipelineImmutableStateFactory::~GLPipelineImmutableStateFactory() = default;

	BlendImmutableStateHandle GLPipelineImmutableStateFactory::CreateBlendState(
			const BlendConfig & pConfig )
	{
		return GLBlendImmutableState::CreateInstance( mGLGPUDevice, pConfig );
	}

	DepthStencilImmutableStateHandle GLPipelineImmutableStateFactory::CreateDepthStencilState(
			const DepthStencilConfig & pConfig )
	{
		return GLDepthStencilImmutableState::CreateInstance( mGLGPUDevice, pConfig );
	}

	IAVertexStreamImmutableStateHandle GLPipelineImmutableStateFactory::CreateIAVertexStreamState(
			const IAVertexStreamDefinition & pDefinition )
	{
		return GLIAVertexStreamImmutableState::CreateInstance( mGLGPUDevice, pDefinition );
	}

	RasterizerImmutableStateHandle GLPipelineImmutableStateFactory::CreateRasterizerState(
			const RasterizerConfig & pConfig )
	{
		return GLRasterizerImmutableState::CreateInstance( mGLGPUDevice, pConfig );
	}

	RenderTargetBindingImmutableStateHandle GLPipelineImmutableStateFactory::CreateRenderTargetBindingState(
			const RenderTargetBindingDefinition & pDefinition )
	{
		return GLRenderTargetBindingImmutableState::CreateInstance( mGLGPUDevice, pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle GLPipelineImmutableStateFactory::CreateRenderPassState(
			const RenderPassConfiguration & pConfiguration )
	{
		return CreateRenderPassStateDefault( mGLGPUDevice, pConfiguration );
	}


	GLPipelineImmutableStateFactoryCore::GLPipelineImmutableStateFactoryCore( GLGPUDevice & pGPUDevice )
	: GLPipelineImmutableStateFactory( pGPUDevice )
	{}

	GLPipelineImmutableStateFactoryCore::~GLPipelineImmutableStateFactoryCore() = default;

	GraphicsShaderLinkageImmutableStateHandle GLPipelineImmutableStateFactoryCore::CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet )
	{
		return GLGraphicsShaderLinkageImmutableStateCore::CreateInstance( mGLGPUDevice, pShaderSet );
	}

	IAInputLayoutImmutableStateHandle GLPipelineImmutableStateFactoryCore::CreateIAInputLayoutState(
			const IAInputLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return GLIAInputLayoutImmutableStateCore::CreateInstance( mGLGPUDevice, pDefinition );
	}


	GLPipelineImmutableStateFactoryCompat::GLPipelineImmutableStateFactoryCompat( GLGPUDevice & pGPUDevice )
	: GLPipelineImmutableStateFactory( pGPUDevice )
	{}

	GLPipelineImmutableStateFactoryCompat::~GLPipelineImmutableStateFactoryCompat() = default;

	GraphicsShaderLinkageImmutableStateHandle GLPipelineImmutableStateFactoryCompat::CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet )
	{
		return GLGraphicsShaderLinkageImmutableStateCompat::CreateInstance( mGLGPUDevice, pShaderSet );
	}

	IAInputLayoutImmutableStateHandle GLPipelineImmutableStateFactoryCompat::CreateIAInputLayoutState(
			const IAInputLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return GLIAInputLayoutImmutableStateCompat::CreateInstance( mGLGPUDevice, pDefinition );
	}

}
