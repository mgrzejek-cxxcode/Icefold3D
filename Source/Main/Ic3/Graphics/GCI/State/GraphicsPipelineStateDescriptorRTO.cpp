
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
	: RenderTargetArrayConfigStateDescriptor( pGPUDevice )
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

	RenderTargetDescriptor::RenderTargetDescriptor( GPUDevice & pGPUDevice )
	: RenderTargetArrayConfigStateDescriptor( pGPUDevice )
	{}

	RenderTargetDescriptor::~RenderTargetDescriptor() = default;

	EPipelineStateDescriptorType RenderTargetDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRenderTarget;
	}


	namespace PIM
	{

		//!! RenderPassDescriptorNative

		RenderPassDescriptorNative::RenderPassDescriptorNative( GPUDevice & pGPUDevice )
		: RenderPassDescriptor( pGPUDevice )
		{}

		RenderPassDescriptorNative::~RenderPassDescriptorNative() = default;


		//!! RenderTargetDescriptorGeneric

		RenderTargetDescriptorGeneric::RenderTargetDescriptorGeneric( GPUDevice & pGPUDevice, const RenderTargetBinding & pTargetBinding )
		: RenderTargetDescriptor( pGPUDevice )
		{}

		RenderTargetDescriptorGeneric::~RenderTargetDescriptorGeneric() = default;

		bool RenderTargetDescriptorGeneric::IsAttachmentActive( native_uint pAttachmentIndex ) const noexcept
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

		RenderTargetDescriptorNative::RenderTargetDescriptorNative( GPUDevice & pGPUDevice )
		: RenderTargetDescriptor( pGPUDevice )
		{}

		RenderTargetDescriptorNative::~RenderTargetDescriptorNative() = default;

	}

}
