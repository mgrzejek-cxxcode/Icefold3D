
#include "GraphicsPipelineStateDescriptorCommon.h"

namespace Ic3::Graphics::GCI
{

	BlendStateDescriptor::BlendStateDescriptor(
			GPUDevice & pGPUDevice,
			cppx::bitmask<EBlendConfigFlags> pBlendFlags )
	: CachedPipelineStateDescriptor( pGPUDevice )
	, mBlendFlags( pBlendFlags & eBlendConfigMaskAll )
	{}

	BlendStateDescriptor::~BlendStateDescriptor() = default;

	EPipelineStateDescriptorType BlendStateDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTBlendState;
	}


	DepthStencilStateDescriptor::DepthStencilStateDescriptor(
			GPUDevice & pGPUDevice,
			cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags )
	: CachedPipelineStateDescriptor( pGPUDevice )
	, mDepthStencilFlags( pDepthStencilFlags & eDepthStencilConfigMaskAll )
	{}

	DepthStencilStateDescriptor::~DepthStencilStateDescriptor() = default;

	EPipelineStateDescriptorType DepthStencilStateDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTDepthStencilState;
	}


	RasterizerStateDescriptor::RasterizerStateDescriptor(
			GPUDevice & pGPUDevice,
			cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags )
	: CachedPipelineStateDescriptor( pGPUDevice )
	, mRasterizerFlags( pRasterizerFlags & eRasterizerConfigMaskAll )
	{}

	RasterizerStateDescriptor::~RasterizerStateDescriptor() = default;

	EPipelineStateDescriptorType RasterizerStateDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRasterizerState;
	}

} // namespace Ic3::Graphics::GCI
