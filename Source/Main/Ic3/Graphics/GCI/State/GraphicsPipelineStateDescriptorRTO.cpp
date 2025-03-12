
#include "GraphicsPipelineStateDescriptorRTO.h"

namespace Ic3::Graphics::GCI
{

	//!! RenderTargetArrayConfigStateDescriptor

	RenderTargetArrayConfigStateDescriptor::RenderTargetArrayConfigStateDescriptor( GPUDevice & pGPUDevice )
	: PipelineStateDescriptor( pGPUDevice )
	{}

	RenderTargetArrayConfigStateDescriptor::~RenderTargetArrayConfigStateDescriptor() = default;


	//!! RenderPassDescriptor

	RenderPassDescriptor::RenderPassDescriptor( GPUDevice & pGPUDevice )
	: NonCachedPipelineStateDescriptor( pGPUDevice )
	{}

	RenderPassDescriptor::~RenderPassDescriptor() = default;

	EPipelineStateDescriptorType RenderPassDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRenderPass;
	}

	bool RenderPassDescriptor::CheckAttachmentLoadActionFlags(
			native_uint pAttachmentIndex,
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		const auto attachmentLoadAction = GetAttachmentLoadAction( pAttachmentIndex );
		return cppx::make_bitmask( attachmentLoadAction ).is_set( pActionFlags );
	}

	bool RenderPassDescriptor::CheckAttachmentStoreActionFlags(
			native_uint pAttachmentIndex,
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		const auto attachmentStoreAction = GetAttachmentStoreAction( pAttachmentIndex );
		return cppx::make_bitmask( attachmentStoreAction ).is_set( pActionFlags );
	}

	bool RenderPassDescriptor::CheckAttachmentLoadAction(
			native_uint pAttachmentIndex,
			ERenderPassAttachmentLoadAction pLoadAction ) const noexcept
	{
		const auto attachmentLoadAction = GetAttachmentLoadAction( pAttachmentIndex );
		return attachmentLoadAction == pLoadAction;
	}

	bool RenderPassDescriptor::CheckAttachmentStoreAction(
			native_uint pAttachmentIndex,
			ERenderPassAttachmentStoreAction pStoreAction ) const noexcept
	{
		const auto attachmentStoreAction = GetAttachmentStoreAction( pAttachmentIndex );
		return attachmentStoreAction == pStoreAction;
	}


	//!! RenderTargetDescriptor

	RenderTargetDescriptor::RenderTargetDescriptor( GPUDevice & pGPUDevice )
	: NonCachedPipelineStateDescriptor( pGPUDevice )
	{}

	RenderTargetDescriptor::~RenderTargetDescriptor() = default;

	EPipelineStateDescriptorType RenderTargetDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRenderTarget;
	}

}
