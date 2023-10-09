
#include "DX11pipelineImmutableStateFactory.h"
#include "DX11CommonGraphicsConfig.h"
#include "DX11graphicsShaderState.h"
#include "DX11inputAssembler.h"
#include "DX11RenderTarget.h"
#include "../DX11gpuDevice.h"
#include "../Resources/DX11Shader.h"

namespace Ic3::Graphics::GCI
{
	
	DX11PipelineImmutableStateFactory::DX11PipelineImmutableStateFactory( DX11GPUDevice & pGPUDevice )
	: PipelineImmutableStateFactorySeparableShader( pGPUDevice  )
	, mDX11GPUDevice( pGPUDevice )
	{}

	DX11PipelineImmutableStateFactory::~DX11PipelineImmutableStateFactory() = default;

	BlendImmutableStateHandle DX11PipelineImmutableStateFactory::createBlendState(
			const BlendConfig & pConfig )
	{
		return DX11BlendImmutableState::createInstance( mDX11GPUDevice, pConfig );
	}

	DepthStencilImmutableStateHandle DX11PipelineImmutableStateFactory::createDepthStencilState(
			const DepthStencilConfig & pConfig )
	{
		return DX11DepthStencilImmutableState::createInstance( mDX11GPUDevice, pConfig );
	}

	IAInputLayoutImmutableStateHandle DX11PipelineImmutableStateFactory::createIAInputLayoutState(
			const IAInputLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return DX11IAInputLayoutImmutableState::createInstance( mDX11GPUDevice, pDefinition, *pVertexShaderWithBinary.mShaderBinary );
	}

	IAVertexStreamImmutableStateHandle DX11PipelineImmutableStateFactory::createIAVertexStreamState(
			const IAVertexStreamDefinition & pDefinition )
	{
		return DX11IAVertexStreamImmutableState::createInstance( mDX11GPUDevice, pDefinition );
	}

	RasterizerImmutableStateHandle DX11PipelineImmutableStateFactory::createRasterizerState(
			const RasterizerConfig & pConfig )
	{
		return DX11RasterizerImmutableState::createInstance( mDX11GPUDevice, pConfig );
	}

	RenderTargetBindingImmutableStateHandle DX11PipelineImmutableStateFactory::createRenderTargetBindingState(
			const RenderTargetBindingDefinition & pDefinition )
	{
		return DX11RenderTargetBindingImmutableState::createInstance( mDX11GPUDevice, pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle DX11PipelineImmutableStateFactory::createRenderPassState(
			const RenderPassConfiguration & pConfiguration )
	{
		return DX11RenderPassConfigurationImmutableState::createInstance( mDX11GPUDevice, pConfiguration );
	}
	
}
