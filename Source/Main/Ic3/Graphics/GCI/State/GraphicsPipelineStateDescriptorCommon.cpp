
#include "GraphicsPipelineStateDescriptorCommon.h"

namespace Ic3::Graphics::GCI
{

	BlendStateDescriptor::BlendStateDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			cppx::bitmask<EBlendConfigFlags> pBlendFlags )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	, mBlendFlags( pBlendFlags & eBlendConfigMaskAll )
	{}

	BlendStateDescriptor::~BlendStateDescriptor() = default;


	DepthStencilStateDescriptor::DepthStencilStateDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	, mDepthStencilFlags( pDepthStencilFlags & eDepthStencilConfigMaskAll )
	{}

	DepthStencilStateDescriptor::~DepthStencilStateDescriptor() = default;


	RasterizerStateDescriptor::RasterizerStateDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	, mRasterizerFlags( pRasterizerFlags & eRasterizerConfigMaskAll )
	{}

	RasterizerStateDescriptor::~RasterizerStateDescriptor() = default;

} // namespace Ic3::Graphics::GCI
