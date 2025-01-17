
#include "GLPipelineImmutableStateFactory.h"
#include "GLCommonGraphicsConfig.h"
#include "GLGraphicsShaderState.h"
#include "GLInputAssembler.h"
#include "GLRenderTarget.h"
#include "../GLGpuDevice.h"

namespace Ic3::Graphics::GCI
{

	GLPipelineImmutableStateFactory::GLPipelineImmutableStateFactory( GLGpuDevice & pGpuDevice )
	: PipelineImmutableStateFactory()
	, mGLGpuDevice( pGpuDevice )
	{}

	GLPipelineImmutableStateFactory::~GLPipelineImmutableStateFactory() = default;

	BlendImmutableStateHandle GLPipelineImmutableStateFactory::CreateBlendState(
			const BlendConfig & pConfig )
	{
		return GLBlendImmutableState::CreateInstance( mGLGpuDevice, pConfig );
	}

	DepthStencilImmutableStateHandle GLPipelineImmutableStateFactory::CreateDepthStencilState(
			const DepthStencilConfig & pConfig )
	{
		return GLDepthStencilImmutableState::CreateInstance( mGLGpuDevice, pConfig );
	}

	IAVertexStreamImmutableStateHandle GLPipelineImmutableStateFactory::CreateIAVertexStreamState(
			const IAVertexStreamDefinition & pDefinition )
	{
		return GLIAVertexStreamImmutableState::CreateInstance( mGLGpuDevice, pDefinition );
	}

	RasterizerImmutableStateHandle GLPipelineImmutableStateFactory::CreateRasterizerState(
			const RasterizerConfig & pConfig )
	{
		return GLRasterizerImmutableState::CreateInstance( mGLGpuDevice, pConfig );
	}

	RenderTargetBindingImmutableStateHandle GLPipelineImmutableStateFactory::CreateRenderTargetBindingState(
			const RenderTargetBindingDefinition & pDefinition )
	{
		return GLRenderTargetBindingImmutableState::CreateInstance( mGLGpuDevice, pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle GLPipelineImmutableStateFactory::CreateRenderPassState(
			const RenderPassConfiguration & pConfiguration )
	{
		return CreateRenderPassStateDefault( mGLGpuDevice, pConfiguration );
	}


	GLPipelineImmutableStateFactoryCore::GLPipelineImmutableStateFactoryCore( GLGpuDevice & pGpuDevice )
	: GLPipelineImmutableStateFactory( pGpuDevice )
	{}

	GLPipelineImmutableStateFactoryCore::~GLPipelineImmutableStateFactoryCore() = default;

	GraphicsShaderLinkageImmutableStateHandle GLPipelineImmutableStateFactoryCore::CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet )
	{
		return GLGraphicsShaderLinkageImmutableStateCore::CreateInstance( mGLGpuDevice, pShaderSet );
	}

	IAInputLayoutImmutableStateHandle GLPipelineImmutableStateFactoryCore::CreateIAInputLayoutState(
			const IAInputLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return GLIAInputLayoutImmutableStateCore::CreateInstance( mGLGpuDevice, pDefinition );
	}


	GLPipelineImmutableStateFactoryCompat::GLPipelineImmutableStateFactoryCompat( GLGpuDevice & pGpuDevice )
	: GLPipelineImmutableStateFactory( pGpuDevice )
	{}

	GLPipelineImmutableStateFactoryCompat::~GLPipelineImmutableStateFactoryCompat() = default;

	GraphicsShaderLinkageImmutableStateHandle GLPipelineImmutableStateFactoryCompat::CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet )
	{
		return GLGraphicsShaderLinkageImmutableStateCompat::CreateInstance( mGLGpuDevice, pShaderSet );
	}

	IAInputLayoutImmutableStateHandle GLPipelineImmutableStateFactoryCompat::CreateIAInputLayoutState(
			const IAInputLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return GLIAInputLayoutImmutableStateCompat::CreateInstance( mGLGpuDevice, pDefinition );
	}

}
