
#include "GraphicsPipelineStateDescriptorRTO.h"

namespace Ic3::Graphics::GCI
{

	//!! RenderTargetArrayConfigStateDescriptor

	RenderTargetArrayConfigStateDescriptor::RenderTargetArrayConfigStateDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	{}

	RenderTargetArrayConfigStateDescriptor::~RenderTargetArrayConfigStateDescriptor() = default;


	//!! RenderPassDescriptor

	RenderPassDescriptor::RenderPassDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	: RenderTargetArrayConfigStateDescriptor( pGPUDevice, pDescriptorID )
	{}

	RenderPassDescriptor::~RenderPassDescriptor() = default;

	EPipelineStateDescriptorType RenderPassDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRenderPass;
	}

	bool RenderPassDescriptor::CheckAttachmentLoadAction(
				uint32 pAttachmentIndex,
				ERenderPassAttachmentLoadAction pAction ) const noexcept
	{
		return CheckAttachmentLoadActionFlags( pAttachmentIndex, pAction );
	}

	bool RenderPassDescriptor::CheckAttachmentStoreAction(
			uint32 pAttachmentIndex,
			ERenderPassAttachmentLoadAction pAction ) const noexcept
	{
		return CheckAttachmentStoreActionFlags( pAttachmentIndex, pAction );
	}


	//!! RenderTargetDescriptor

	RenderTargetDescriptor::RenderTargetDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	: RenderTargetArrayConfigStateDescriptor( pGPUDevice, pDescriptorID )
	{}

	RenderTargetDescriptor::~RenderTargetDescriptor() = default;

	EPipelineStateDescriptorType RenderTargetDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRenderTarget;
	}


	namespace PIM
	{

		//!! RenderPassDescriptorNative

		RenderPassDescriptorNative::RenderPassDescriptorNative(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID )
		: RenderPassDescriptor( pGPUDevice, pDescriptorID )
		{}

		RenderPassDescriptorNative::~RenderPassDescriptorNative() = default;


		//!! RenderTargetDescriptorGeneric

		RenderTargetDescriptorGeneric::RenderTargetDescriptorGeneric(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const RenderTargetBinding & pTargetBinding )
		: RenderTargetDescriptor( pGPUDevice, pDescriptorID )
		{}

		RenderTargetDescriptorGeneric::~RenderTargetDescriptorGeneric() = default;

		bool RenderTargetDescriptorGeneric::IsAttachmentActive( uint32 pAttachmentIndex ) const noexcept
		{
			const auto * attachmentBinding = mRenderTargetBinding.GetAttachment( pAttachmentIndex );
			return attachmentBinding->IsActive();
		}

		TGfxHandle<RenderTargetDescriptorGeneric> RenderTargetDescriptorGeneric::CreateFromRenderTargetBinding(
				GPUDevice & pGPUDevice,
				const RenderTargetBinding & pRenderTargetBinding,
				pipeline_state_descriptor_id_t pDescriptorID )
		{
			return nullptr;
		}


		//!! RenderPassDescriptorNative

		RenderTargetDescriptorNative::RenderTargetDescriptorNative(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID )
		: RenderTargetDescriptor( pGPUDevice, pDescriptorID )
		{}

		RenderTargetDescriptorNative::~RenderTargetDescriptorNative() = default;

	}

}
