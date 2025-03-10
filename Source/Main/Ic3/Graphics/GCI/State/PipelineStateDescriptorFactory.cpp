
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
			pipeline_state_descriptor_id_t pDescriptorID,
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	DepthStencilStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateDepthStencilStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	RasterizerStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateRasterizerStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	GraphicsShaderLinkageDescriptorHandle PipelineStateDescriptorFactoryNull::CreateGraphicsShaderLinkageDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	VertexAttributeLayoutDescriptorHandle PipelineStateDescriptorFactoryNull::CreateVertexAttributeLayoutDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	RootSignatureDescriptorHandle PipelineStateDescriptorFactoryNull::CreateRootSignatureDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	RenderPassDescriptorHandle PipelineStateDescriptorFactoryNull::CreateRenderPassDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RenderPassDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	VertexSourceBindingDescriptorHandle PipelineStateDescriptorFactoryNull::CreateVertexSourceBindingDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
