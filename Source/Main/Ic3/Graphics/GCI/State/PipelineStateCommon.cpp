
#include "PipelineStateCommon.h"

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

	BlendStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateBlendStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const BlendConfig & pBlendConfig )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pBlendConfig );
		return nullptr;
	}

	DepthStencilStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateDepthStencilStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const DepthStencilConfig & pDepthStencilConfig )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pDepthStencilConfig );
		return nullptr;
	}

	MultiSamplingStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateMultiSamplingStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const MultiSamplingConfig & pMultiSamplingConfig )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pMultiSamplingConfig );
		return nullptr;
	}

	RasterizerStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateRasterizerStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RasterizerConfig & pRasterizerConfig )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pRasterizerConfig );
		return nullptr;
	}

	GraphicsShaderLinkageStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateGraphicsShaderLinkageStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const GraphicsShaderBinding & pShaderBinding )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pShaderBinding );
		return nullptr;
	}

	IAInputLayoutStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateIAInputLayoutStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const IAInputLayoutDefinition & pInputLayoutDefinition,
			Shader & pVertexShaderWithBinary )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pInputLayoutDefinition );
		Ic3UnusedParam( pVertexShaderWithBinary );
		return nullptr;
	}

	IAVertexStreamStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateIAVertexStreamStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const IAVertexStreamConfiguration & pVertexStreamDefinition )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pVertexStreamDefinition );
		return nullptr;
	}

	RenderPassConfigStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateRenderPassConfigStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RenderPassConfiguration & pPassConfiguration )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pPassConfiguration );
		return nullptr;
	}

	ShaderRootSignatureStateDescriptorHandle PipelineStateDescriptorFactoryNull::CreateShaderRootSignatureStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const ShaderRootSignatureDesc & pRootSignatureDesc )
	{
		Ic3UnusedParam( pDescriptorID );
		Ic3UnusedParam( pBlendConfig );
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
