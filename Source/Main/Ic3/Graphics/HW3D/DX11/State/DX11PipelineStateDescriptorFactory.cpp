
#include "DX11PipelineStateDescriptorFactory.h"
#include "DX11GraphicsPipelineStateCommon.h"
#include "DX11GraphicsPipelineStateIA.h"
#include "DX11GraphicsPipelineStateRTO.h"
#include "DX11GraphicsPipelineStateShader.h"
#include <Ic3/Graphics/HW3D/DX11/DX11GPUDevice.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11Shader.h>
#include <IC3/Graphics/GCI/State/GraphicsPipelineStateImplRenderPassGeneric.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateImplSeparableState.h>
#include <IC3/Graphics/GCI/State/PipelineStateImplRootSignatureGeneric.h>

namespace Ic3::Graphics::GCI
{

	DX11PipelineStateDescriptorFactory::DX11PipelineStateDescriptorFactory( DX11GPUDevice & pGPUDevice )
	: PipelineStateDescriptorFactory()
	, mDX11GPUDevice( pGPUDevice )
	{}

	DX11PipelineStateDescriptorFactory::~DX11PipelineStateDescriptorFactory() = default;

	GPUDevice & DX11PipelineStateDescriptorFactory::GetGPUDevice() const noexcept
	{
		return mDX11GPUDevice;
	}

	BlendStateDescriptorHandle DX11PipelineStateDescriptorFactory::CreateBlendStateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		return DX11BlendStateDescriptor::CreateInstance( mDX11GPUDevice, pCreateInfo );
	}

	DepthStencilStateDescriptorHandle DX11PipelineStateDescriptorFactory::CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		return DX11DepthStencilStateDescriptor::CreateInstance( mDX11GPUDevice, pCreateInfo );
	}

	RasterizerStateDescriptorHandle DX11PipelineStateDescriptorFactory::CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		return DX11RasterizerStateDescriptor::CreateInstance( mDX11GPUDevice, pCreateInfo );
	}

	GraphicsShaderLinkageDescriptorHandle DX11PipelineStateDescriptorFactory::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return DX11GraphicsShaderLinkageDescriptor::CreateFromBindingConfiguration( mDX11GPUDevice, pCreateInfo.shaderBinding );
	}

	VertexAttributeLayoutDescriptorHandle DX11PipelineStateDescriptorFactory::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		const auto * vertexShaderBinary = pCreateInfo.vertexShaderWithBinary->GetShaderBinary();
		return DX11VertexAttributeLayoutDescriptor::CreateInstance( mDX11GPUDevice, pCreateInfo.layoutDefinition, *vertexShaderBinary );
	}

	RootSignatureDescriptorHandle DX11PipelineStateDescriptorFactory::CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		return DX11RootSignatureDescriptor::CreateInstance( mDX11GPUDevice, pCreateInfo );
	}

	RenderPassDescriptorHandle DX11PipelineStateDescriptorFactory::CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo )
	{
		return RenderPassDescriptorGeneric::CreateInstance(
			mDX11GPUDevice,
			pCreateInfo,
			{
				eRenderPassAttachmentActionFlagLoadClearBit,
				eRenderPassAttachmentActionFlagStoreResolveBit
			} );
	}

	RenderTargetDescriptorHandle DX11PipelineStateDescriptorFactory::CreateRenderTargetDescriptor(
		const RenderTargetDescriptorCreateInfo & pCreateInfo )
	{
		return DX11RenderTargetDescriptor::CreateInstance( mDX11GPUDevice, pCreateInfo.rtArrayBinding );
	}

	VertexSourceBindingDescriptorHandle DX11PipelineStateDescriptorFactory::CreateVertexSourceBindingDescriptor(
		const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		return DX11VertexSourceBindingDescriptor::CreateInstance( mDX11GPUDevice, pCreateInfo.bindingDefinition );
	}
	
}
