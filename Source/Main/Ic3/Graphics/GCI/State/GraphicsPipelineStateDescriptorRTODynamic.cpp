
#include "GraphicsPipelineStateDescriptorRTODynamic.h"

namespace Ic3::Graphics::GCI
{

	RenderPassDescriptorDynamic::RenderPassDescriptorDynamic(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	: RTArrayDescriptorDynamicProxy<RenderPassDescriptor, RenderPassAttachmentConfig>( pGPUDevice, pDescriptorID )
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

	TGfxHandle<RenderPassDescriptorDynamic> RenderPassDescriptorDynamic::CreateNew(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	{
		return CreateDynamicObject<RenderPassDescriptorDynamic>( pGPUDevice, pDescriptorID );
	}


	RenderTargetDescriptorDynamic::RenderTargetDescriptorDynamic(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	: RTArrayDescriptorDynamicProxy<RenderTargetDescriptor, RenderTargetAttachmentBinding>( pGPUDevice, pDescriptorID )
	{}

	RenderTargetDescriptorDynamic::~RenderTargetDescriptorDynamic() = default;

	TGfxHandle<RenderTargetDescriptorDynamic> RenderTargetDescriptorDynamic::CreateNew(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	{
		return CreateDynamicObject<RenderTargetDescriptorDynamic>( pGPUDevice, pDescriptorID );
	}

} // namespace Ic3::Graphics::GCI

