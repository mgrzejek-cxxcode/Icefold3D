
#include "GraphicsPipelineStateDescriptorRTODynamic.h"

namespace Ic3::Graphics::GCI
{

	RenderPassDescriptorDynamic::RenderPassDescriptorDynamic( GPUDevice & pGPUDevice )
	: RTArrayDescriptorDynamicProxy<RenderPassDescriptor, RenderPassAttachmentConfig>( pGPUDevice )
	{}

	RenderPassDescriptorDynamic::~RenderPassDescriptorDynamic() = default;

	bool RenderPassDescriptorDynamic::CheckAttachmentLoadActionFlags(
			uint32 pAttachmentIndex,
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		const auto * renderPassAttachmentConfig = GetRenderTargetArrayConfiguration().GetAttachment( pAttachmentIndex );
		return renderPassAttachmentConfig->HasLoadActionFlags( pActionFlags );
	}

	bool RenderPassDescriptorDynamic::CheckAttachmentStoreActionFlags(
			uint32 pAttachmentIndex,
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		const auto * renderPassAttachmentConfig = GetRenderTargetArrayConfiguration().GetAttachment( pAttachmentIndex );
		return renderPassAttachmentConfig->HasStoreActionFlags( pActionFlags );
	}

	TGfxHandle<RenderPassDescriptorDynamic> RenderPassDescriptorDynamic::CreateNew( GPUDevice & pGPUDevice )
	{
		return CreateDynamicObject<RenderPassDescriptorDynamic>( pGPUDevice );
	}


	RenderTargetDescriptorDynamic::RenderTargetDescriptorDynamic( GPUDevice & pGPUDevice )
	: RTArrayDescriptorDynamicProxy<RenderTargetDescriptor, RenderTargetAttachmentBinding>( pGPUDevice )
	{}

	RenderTargetDescriptorDynamic::~RenderTargetDescriptorDynamic() = default;

	TGfxHandle<RenderTargetDescriptorDynamic> RenderTargetDescriptorDynamic::CreateNew( GPUDevice & pGPUDevice )
	{
		return CreateDynamicObject<RenderTargetDescriptorDynamic>( pGPUDevice );
	}

} // namespace Ic3::Graphics::GCI

