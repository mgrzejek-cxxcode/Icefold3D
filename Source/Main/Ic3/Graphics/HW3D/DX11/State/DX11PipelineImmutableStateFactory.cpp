
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

	BlendImmutableStateHandle DX11PipelineImmutableStateFactory::CreateBlendState(
			const BlendConfig & pConfig )
	{
		return DX11BlendImmutableState::CreateInstance( mDX11GPUDevice, pConfig );
	}

	DepthStencilImmutableStateHandle DX11PipelineImmutableStateFactory::CreateDepthStencilState(
			const DepthStencilConfig & pConfig )
	{
		return DX11DepthStencilImmutableState::CreateInstance( mDX11GPUDevice, pConfig );
	}

	IAInputLayoutImmutableStateHandle DX11PipelineImmutableStateFactory::CreateIAInputLayoutState(
			const IAInputLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return DX11IAInputLayoutImmutableState::CreateInstance( mDX11GPUDevice, pDefinition, *pVertexShaderWithBinary.mShaderBinary );
	}

	IAVertexStreamImmutableStateHandle DX11PipelineImmutableStateFactory::CreateIAVertexStreamState(
			const IAVertexStreamDefinition & pDefinition )
	{
		return DX11IAVertexStreamImmutableState::CreateInstance( mDX11GPUDevice, pDefinition );
	}

	RasterizerImmutableStateHandle DX11PipelineImmutableStateFactory::CreateRasterizerState(
			const RasterizerConfig & pConfig )
	{
		return DX11RasterizerImmutableState::CreateInstance( mDX11GPUDevice, pConfig );
	}

	RenderTargetBindingImmutableStateHandle DX11PipelineImmutableStateFactory::CreateRenderTargetBindingState(
			const RenderTargetBindingDefinition & pDefinition )
	{
		return DX11RenderTargetBindingImmutableState::CreateInstance( mDX11GPUDevice, pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle DX11PipelineImmutableStateFactory::CreateRenderPassState(
			const RenderPassConfiguration & pConfiguration )
	{
		return DX11RenderPassConfigurationImmutableState::CreateInstance( mDX11GPUDevice, pConfiguration );
	}
	
}
