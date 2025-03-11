
#include "GraphicsPipelineStateImplGenericRenderPass.h"

namespace Ic3::Graphics::GCI
{

	RenderPassDescriptorGeneric::RenderPassDescriptorGeneric(
			GPUDevice & pGPUDevice,
			const RenderPassConfiguration & pRenderPassConfiguration )
	: RenderPassDescriptor( pGPUDevice )
	, mPassConfiguration( pRenderPassConfiguration )
	{}

	RenderPassDescriptorGeneric::~RenderPassDescriptorGeneric() = default;

	bool RenderPassDescriptorGeneric::IsAttachmentActive( native_uint pAttachmentIndex ) const noexcept
	{
		const auto * attachmentConfig = mPassConfiguration.GetAttachment( pAttachmentIndex );
		return !attachmentConfig->IsEmpty();
	}

	bool RenderPassDescriptorGeneric::CheckAttachmentLoadActionFlags(
			uint32 pAttachmentIndex,
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		const auto * attachmentConfig = mPassConfiguration.GetAttachment( pAttachmentIndex );
		return !attachmentConfig->HasLoadActionFlags( pActionFlags & eRenderPassAttachmentActionMaskLoadAll );
	}

	bool RenderPassDescriptorGeneric::CheckAttachmentStoreActionFlags(
			uint32 pAttachmentIndex,
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		const auto * attachmentConfig = mPassConfiguration.GetAttachment( pAttachmentIndex );
		return !attachmentConfig->HasStoreActionFlags( pActionFlags & eRenderPassAttachmentActionMaskStoreAll );
	}

	TGfxHandle<RenderPassDescriptorGeneric> RenderPassDescriptorGeneric::CreateInstance(
			GPUDevice & pGPUDevice,
			const RenderPassDescriptorCreateInfo & pCreateInfo,
			const std::initializer_list<ERenderPassAttachmentActionFlags> & pCachedAttachmentsWithFlags )
	{
		if( !GCU::RTOValidateRenderPassConfiguration( pCreateInfo.passConfiguration ) )
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
		if( !GCU::RTOValidateRenderPassConfiguration( pCreateInfo.passConfiguration ) )
		{
			return nullptr;
		}

		return CreateDynamicObject<RenderPassDescriptorGeneric>( pGPUDevice, pCreateInfo.passConfiguration );
	}

} // namespace Ic3::Graphics::GCI
