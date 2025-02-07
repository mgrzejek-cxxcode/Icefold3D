
#include "GraphicsPipelineStateDescriptorCommon.h"

namespace Ic3::Graphics::GCI
{

	BlendStateDescriptor::BlendStateDescriptor( GPUDevice & pGPUDevice, cppx::bitmask<EBlendConfigFlags> pBlendFlags )
	: GraphicsPipelineStateDescriptor( pGPUDevice )
	, mBlendFlags( pBlendFlags & eBlendConfigMaskAll )
	{}

	BlendStateDescriptor::~BlendStateDescriptor() = default;


	DepthStencilStateDescriptor::DepthStencilStateDescriptor( GPUDevice & pGPUDevice, cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags )
	: GraphicsPipelineStateDescriptor( pGPUDevice )
	, mDepthStencilFlags( pDepthStencilFlags & eDepthStencilConfigMaskAll )
	{}

	DepthStencilStateDescriptor::~DepthStencilStateDescriptor() = default;


	RasterizerStateDescriptor::RasterizerStateDescriptor( GPUDevice & pGPUDevice, cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags )
	: GraphicsPipelineStateDescriptor( pGPUDevice )
	, mRasterizerFlags( pRasterizerFlags & eRasterizerConfigMaskAll )
	{}

	RasterizerStateDescriptor::~RasterizerStateDescriptor() = default;

} // namespace Ic3::Graphics::GCI
