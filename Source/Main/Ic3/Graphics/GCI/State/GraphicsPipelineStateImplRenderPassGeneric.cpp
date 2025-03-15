
#include "GraphicsPipelineStateImplRenderPassGeneric.h"

namespace Ic3::Graphics::GCI
{

	RenderPassDescriptorGeneric::RenderPassDescriptorGeneric(
			GPUDevice & pGPUDevice,
			const RenderPassConfiguration & pRenderPassConfiguration )
	: GCIPipelineStateDescriptor( pGPUDevice )
	, mPassConfiguration( pRenderPassConfiguration )
	{}

	RenderPassDescriptorGeneric::~RenderPassDescriptorGeneric() = default;

	bool RenderPassDescriptorGeneric::IsAttachmentConfigured( native_uint pAttachmentIndex ) const noexcept
	{
		const auto * attachmentConfig = mPassConfiguration.GetAttachment( pAttachmentIndex );
		return !attachmentConfig->IsEmpty();
	}

	ERenderPassAttachmentLoadAction RenderPassDescriptorGeneric::GetAttachmentLoadAction( native_uint pAttachmentIndex ) const noexcept
	{
		const auto * attachmentConfig = mPassConfiguration.GetAttachment( pAttachmentIndex );
		return attachmentConfig ? attachmentConfig->loadAction : ERenderPassAttachmentLoadAction::Undefined;
	}

	ERenderPassAttachmentStoreAction RenderPassDescriptorGeneric::GetAttachmentStoreAction( native_uint pAttachmentIndex ) const noexcept
	{
		const auto * attachmentConfig = mPassConfiguration.GetAttachment( pAttachmentIndex );
		return attachmentConfig ? attachmentConfig->storeAction : ERenderPassAttachmentStoreAction::Undefined;
	}

	TGfxHandle<RenderPassDescriptorGeneric> RenderPassDescriptorGeneric::CreateInstance(
			GPUDevice & pGPUDevice,
			const RenderPassDescriptorCreateInfo & pCreateInfo,
			const std::initializer_list<ERenderPassAttachmentActionFlags> & pCachedAttachmentsWithFlags )
	{
		if( !Utilities::RTOValidateRenderPassConfiguration( pCreateInfo.passConfiguration ) )
		{
			return nullptr;
		}

		if( pCachedAttachmentsWithFlags.size() > 0 )
		{
			auto renderPassConfigurationCopy = pCreateInfo.passConfiguration;
			for( const auto attachmentActionFlags : pCachedAttachmentsWithFlags )
			{
				renderPassConfigurationCopy.CacheAttachmentsWithActionFlags( attachmentActionFlags );
			}

			return CreateDynamicObject<RenderPassDescriptorGeneric>( pGPUDevice, renderPassConfigurationCopy );
		}
		else
		{
			return CreateDynamicObject<RenderPassDescriptorGeneric>( pGPUDevice, pCreateInfo.passConfiguration );
		}
	}

	TGfxHandle<RenderPassDescriptorGeneric> RenderPassDescriptorGeneric::CreateInstance(
			GPUDevice & pGPUDevice,
			const RenderPassDescriptorCreateInfo & pCreateInfo )
	{
		if( !Utilities::RTOValidateRenderPassConfiguration( pCreateInfo.passConfiguration ) )
		{
			return nullptr;
		}

		return CreateDynamicObject<RenderPassDescriptorGeneric>( pGPUDevice, pCreateInfo.passConfiguration );
	}

} // namespace Ic3::Graphics::GCI
