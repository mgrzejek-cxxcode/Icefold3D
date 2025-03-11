
#include "GraphicsPipelineStateDescriptorRTODynamic.h"

namespace Ic3::Graphics::GCI
{

	RenderPassDescriptorDynamic::RenderPassDescriptorDynamic( GPUDevice & pGPUDevice )
	: RTArrayDescriptorDynamicProxy<RenderPassDescriptor, RenderPassAttachmentConfig>( pGPUDevice )
	{}

	RenderPassDescriptorDynamic::~RenderPassDescriptorDynamic() = default;

	ERenderPassAttachmentLoadAction RenderPassDescriptorDynamic::GetAttachmentLoadAction( native_uint pAttachmentIndex ) const noexcept
	{
		const auto * renderPassAttachmentConfig = GetAttachmentConfig( pAttachmentIndex );
		return renderPassAttachmentConfig ? renderPassAttachmentConfig->loadAction : ERenderPassAttachmentLoadAction::Undefined;
	}

	ERenderPassAttachmentStoreAction RenderPassDescriptorDynamic::GetAttachmentStoreAction( native_uint pAttachmentIndex ) const noexcept
	{
		const auto * renderPassAttachmentConfig = GetAttachmentConfig( pAttachmentIndex );
		return renderPassAttachmentConfig ? renderPassAttachmentConfig->storeAction : ERenderPassAttachmentStoreAction::Undefined;
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

