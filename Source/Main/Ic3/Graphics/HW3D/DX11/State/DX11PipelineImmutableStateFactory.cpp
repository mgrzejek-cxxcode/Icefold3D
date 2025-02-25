
#include "DX11pipelineCompiledDescriptorFactory.h"
#include "DX11CommonGraphicsConfig.h"
#include "DX11graphicsShaderState.h"
#include "DX11inputAssembler.h"
#include "DX11RenderTarget.h"
#include "../DX11gpuDevice.h"
#include "../Resources/DX11Shader.h"

namespace Ic3::Graphics::GCI
{
	
	DX11PipelineStateDescriptorFactory::DX11PipelineStateDescriptorFactory( DX11GPUDevice & pGPUDevice )
	: PipelineStateDescriptorFactorySeparableShader( pGPUDevice  )
	, mDX11GPUDevice( pGPUDevice )
	{}

	DX11PipelineStateDescriptorFactory::~DX11PipelineStateDescriptorFactory() = default;

	BlendStateDescriptorHandle DX11PipelineStateDescriptorFactory::CreateBlendState(
			const BlendSettings & pSettings )
	{
		return DX11BlendStateDescriptor::CreateInstance( mDX11GPUDevice, pConfig );
	}

	DepthStencilStateDescriptorHandle DX11PipelineStateDescriptorFactory::CreateDepthStencilState(
			const DepthStencilSettings & pSettings )
	{
		return DX11DepthStencilStateDescriptor::CreateInstance( mDX11GPUDevice, pConfig );
	}

	VertexAttributeLayoutDescriptorHandle DX11PipelineStateDescriptorFactory::CreateIAVertexAttributeLayout(
			const IAVertexAttributeLayoutDefinition & pDefinition,
			Shader & pVertexShaderWithBinary )
	{
		return DX11VertexAttributeLayoutDescriptor::CreateInstance( mDX11GPUDevice, pDefinition, *pVertexShaderWithBinary.mShaderBinary );
	}

	VertexSourceBindingDescriptorHandle DX11PipelineStateDescriptorFactory::CreateVertexSourceBindingDescriptor(
			const IADataStreamArrayConfiguration & pDefinition )
	{
		return DX11VertexSourceBindingDescriptor::CreateInstance( mDX11GPUDevice, pDefinition );
	}

	RasterizerStateDescriptorHandle DX11PipelineStateDescriptorFactory::CreateRasterizerState(
			const RasterizerSettings & pSettings )
	{
		return DX11RasterizerStateDescriptor::CreateInstance( mDX11GPUDevice, pConfig );
	}

	RenderTargetBindingCompiledStateHandle DX11PipelineStateDescriptorFactory::CreateRenderTargetBindingState(
			const RenderTargetBindingDefinition & pDefinition )
	{
		return DX11RenderTargetBindingCompiledState::CreateInstance( mDX11GPUDevice, pDefinition );
	}

	RenderPassConfigurationCompiledStateHandle DX11PipelineStateDescriptorFactory::CreateRenderPassState(
			const RenderPassConfiguration & pConfiguration )
	{
		return DX11RenderPassConfigurationCompiledState::CreateInstance( mDX11GPUDevice, pConfiguration );
	}
	
}
