
#include "PipelineStateCommon.h"
#include "../GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	/// #PipelineStateDescriptor

	PipelineStateDescriptor::PipelineStateDescriptor( GPUDevice & pGPUDevice, pipeline_state_descriptor_id_t pDescriptorID )
	: GPUDeviceChildObject( pGPUDevice )
	, mDescriptorID( pDescriptorID )
	{}

	PipelineStateDescriptor::~PipelineStateDescriptor() = default;

	bool PipelineStateDescriptor::IsDynamicDescriptor() const noexcept
	{
		return false;
	};


	/// #PipelineStateObject

	PipelineStateObject::PipelineStateObject( GPUDevice & pGPUDevice, pipeline_state_object_id_t pStateObjectID )
	: GPUDeviceChildObject( pGPUDevice )
	, mStateObjectID( pStateObjectID )
	{}

	PipelineStateObject::~PipelineStateObject() = default;


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

	GraphicsShaderLinkageStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateGraphicsShaderLinkageStateDescriptor(
			const GraphicsShaderLinkageStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	IAVertexAttributeLayoutStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateIAVertexAttributeLayoutStateDescriptor(
			const IAVertexAttributeLayoutStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	ShaderRootSignatureStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateShaderRootSignatureStateDescriptor(
			const ShaderRootSignatureStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	IAVertexStreamBindingStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateIAVertexStreamBindingStateDescriptor(
			const IAVertexStreamBindingStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

	RenderPassConfigurationStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateRenderPassConfigurationStateDescriptor(
			const RenderPassConfigurationStateDescriptorCreateInfo & pCreateInfo )
	{
		Ic3UnusedParam( pCreateInfo );
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
