
#include "DX11pipelineImmutableStateFactory.h"
#include "DX11CommonGraphicsConfig.h"
#include "DX11graphicsShaderState.h"
#include "DX11inputAssembler.h"
#include "DX11RenderTarget.h"
#include "../DX11gpuDevice.h"
#include "../Resources/DX11Shader.h"

namespace Ic3::Graphics::GCI
{
	
	DX11PipelineImmutableStateFactory::DX11PipelineImmutableStateFactory( DX11GpuDevice & pGpuDevice )
	: PipelineImmutableStateFactorySeparableShader( pGpuDevice  )
	, mDX11GpuDevice( pGpuDevice )
	{}

	DX11PipelineImmutableStateFactory::~DX11PipelineImmutableStateFactory() = default;

	BlendImmutableStateHandle DX11PipelineImmutableStateFactory::CreateBlendState(
			const BlendConfig & pConfig )
	{
		return DX11BlendImmutableState::CreateInstance( mDX11GpuDevice, pConfig );
	}

	DepthStencilImmutableStateHandle DX11PipelineImmutableStateFactory::CreateDepthStencilState(
			const DepthStencilConfig & pConfig )
	{
		return DX11DepthStencilImmutableState::CreateInstance( mDX11GpuDevice, pConfig );
	}

	IAInputLayoutImmutableStateHandle DX11PipelineImmutableStateFactory::CreateIAInputLayoutState(
			const IAInputLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return DX11IAInputLayoutImmutableState::CreateInstance( mDX11GpuDevice, pDefinition, *pVertexShaderWithBinary.mShaderBinary );
	}

	IAVertexStreamImmutableStateHandle DX11PipelineImmutableStateFactory::CreateIAVertexStreamState(
			const IAVertexStreamDefinition & pDefinition )
	{
		return DX11IAVertexStreamImmutableState::CreateInstance( mDX11GpuDevice, pDefinition );
	}

	RasterizerImmutableStateHandle DX11PipelineImmutableStateFactory::CreateRasterizerState(
			const RasterizerConfig & pConfig )
	{
		return DX11RasterizerImmutableState::CreateInstance( mDX11GpuDevice, pConfig );
	}

	RenderTargetBindingImmutableStateHandle DX11PipelineImmutableStateFactory::CreateRenderTargetBindingState(
			const RenderTargetBindingDefinition & pDefinition )
	{
		return DX11RenderTargetBindingImmutableState::CreateInstance( mDX11GpuDevice, pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle DX11PipelineImmutableStateFactory::CreateRenderPassState(
			const RenderPassConfiguration & pConfiguration )
	{
		return DX11RenderPassConfigurationImmutableState::CreateInstance( mDX11GpuDevice, pConfiguration );
	}
	
}
