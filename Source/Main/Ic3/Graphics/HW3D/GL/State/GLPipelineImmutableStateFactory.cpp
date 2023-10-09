
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

	BlendImmutableStateHandle GLPipelineImmutableStateFactory::createBlendState(
			const BlendConfig & pConfig )
	{
		return GLBlendImmutableState::createInstance( mGLGPUDevice, pConfig );
	}

	DepthStencilImmutableStateHandle GLPipelineImmutableStateFactory::createDepthStencilState(
			const DepthStencilConfig & pConfig )
	{
		return GLDepthStencilImmutableState::createInstance( mGLGPUDevice, pConfig );
	}

	IAVertexStreamImmutableStateHandle GLPipelineImmutableStateFactory::createIAVertexStreamState(
			const IAVertexStreamDefinition & pDefinition )
	{
		return GLIAVertexStreamImmutableState::createInstance( mGLGPUDevice, pDefinition );
	}

	RasterizerImmutableStateHandle GLPipelineImmutableStateFactory::createRasterizerState(
			const RasterizerConfig & pConfig )
	{
		return GLRasterizerImmutableState::createInstance( mGLGPUDevice, pConfig );
	}

	RenderTargetBindingImmutableStateHandle GLPipelineImmutableStateFactory::createRenderTargetBindingState(
			const RenderTargetBindingDefinition & pDefinition )
	{
		return GLRenderTargetBindingImmutableState::createInstance( mGLGPUDevice, pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle GLPipelineImmutableStateFactory::createRenderPassState(
			const RenderPassConfiguration & pConfiguration )
	{
		return createRenderPassStateDefault( mGLGPUDevice, pConfiguration );
	}


	GLPipelineImmutableStateFactoryCore::GLPipelineImmutableStateFactoryCore( GLGPUDevice & pGPUDevice )
	: GLPipelineImmutableStateFactory( pGPUDevice )
	{}

	GLPipelineImmutableStateFactoryCore::~GLPipelineImmutableStateFactoryCore() = default;

	GraphicsShaderLinkageImmutableStateHandle GLPipelineImmutableStateFactoryCore::createGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet )
	{
		return GLGraphicsShaderLinkageImmutableStateCore::createInstance( mGLGPUDevice, pShaderSet );
	}

	IAInputLayoutImmutableStateHandle GLPipelineImmutableStateFactoryCore::createIAInputLayoutState(
			const IAInputLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return GLIAInputLayoutImmutableStateCore::createInstance( mGLGPUDevice, pDefinition );
	}


	GLPipelineImmutableStateFactoryCompat::GLPipelineImmutableStateFactoryCompat( GLGPUDevice & pGPUDevice )
	: GLPipelineImmutableStateFactory( pGPUDevice )
	{}

	GLPipelineImmutableStateFactoryCompat::~GLPipelineImmutableStateFactoryCompat() = default;

	GraphicsShaderLinkageImmutableStateHandle GLPipelineImmutableStateFactoryCompat::createGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet )
	{
		return GLGraphicsShaderLinkageImmutableStateCompat::createInstance( mGLGPUDevice, pShaderSet );
	}

	IAInputLayoutImmutableStateHandle GLPipelineImmutableStateFactoryCompat::createIAInputLayoutState(
			const IAInputLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return GLIAInputLayoutImmutableStateCompat::createInstance( mGLGPUDevice, pDefinition );
	}

}
