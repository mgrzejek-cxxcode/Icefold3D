
#include "PipelineStateDescriptorFactory.h"
#include "../GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	/// #PipelineStateDescriptorFactoryNull

	GPUDevice & PipelineStateDescriptorFactoryNull::GetGPUDevice() const noexcept
	{
		return GPUDevice::GetNullDevice();
	}

	BlendStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateBlendStateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	DepthStencilStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	RasterizerStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	GraphicsShaderLinkageDescriptorHandle PipelineStateDescriptorFactoryNull::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	VertexAttributeLayoutDescriptorHandle PipelineStateDescriptorFactoryNull::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	RootSignatureDescriptorHandle PipelineStateDescriptorFactoryNull::CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	RenderPassDescriptorHandle PipelineStateDescriptorFactoryNull::CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	VertexSourceBindingDescriptorHandle PipelineStateDescriptorFactoryNull::CreateVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
