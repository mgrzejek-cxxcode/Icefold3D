
#include "RenderPassCommon.h"

namespace Ic3::Graphics::GCI
{

	void RenderPassConfiguration::resetAttachmentsFlags() noexcept
	{
		attachmentsAccessRestrictMask = 0;
		attachmentsActionClearMask = 0;
		attachmentsActionResolveMask = 0;

		foreachRTAttachmentIndex( activeAttachmentsMask,
			[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
			{
				const auto & attachmentConfig = attachments[pIndex];
				if( !attachmentConfig )
				{
					activeAttachmentsMask.unset( pAttachmentBit );
				}
				else
				{
					const auto attachmentActionMask = SMU::getRenderPassAttachmentActionMask( attachmentConfig );
					if( attachmentActionMask.isSet( E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_ACCESS_RESTRICTBit ) )
					{
						attachmentsAccessRestrictMask.set( pAttachmentBit );
					}
					else
					{
						if( attachmentActionMask.isSet( E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_CLEARBit ) )
						{
							attachmentsActionClearMask.set( pAttachmentBit );
						}
						if( attachmentActionMask.isSet( E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_RESOLVEBit ) )
						{
							attachmentsActionResolveMask.set( pAttachmentBit );
						}
					}
				}
				return true;
			} );
	}

	namespace SMU
	{

		TBitmask<ERenderPassAttachmentActionFlags> getRenderPassAttachmentActionMask(
				const RenderPassAttachmentConfig & pAttachmentConfig )
		{
			TBitmask<ERenderPassAttachmentActionFlags> attachmentActionMask = 0;
			attachmentActionMask.set( static_cast<uint32>( pAttachmentConfig.renderPassLoadAction ) );
			attachmentActionMask.set( static_cast<uint32>( pAttachmentConfig.renderPassStoreAction ) );
			return attachmentActionMask;
		}

		TBitmask<ERTAttachmentFlags> getRenderPassAttachmentArbitraryActionMask(
				const RenderPassColorAttachmentConfigArray & pColorAttachments,
				const RenderPassAttachmentConfig pDepthStencilAttachment,
				TBitmask<ERTAttachmentFlags> pActiveAttachmentsMask,
				TBitmask<ERenderPassAttachmentActionFlags> pActionMask )
		{
			TBitmask<ERTAttachmentFlags> attachmentActionMask = 0;
			foreachRTAttachmentIndex( pActiveAttachmentsMask,
				[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
				{
					const auto attachmentUsage = getRenderPassAttachmentActionMask( pColorAttachments[pIndex] );
					if( attachmentUsage.isSet( pActionMask ) )
					{
						attachmentActionMask.set( pAttachmentBit );
					}
					return true;
				} );
			return attachmentActionMask;
		}

		TBitmask<ERTAttachmentFlags> getRenderPassAttachmentClearMask(
				const RenderPassColorAttachmentConfigArray & pColorAttachments,
				const RenderPassAttachmentConfig pDepthStencilAttachment,
				TBitmask<ERTAttachmentFlags> pActiveAttachmentsMask )
		{
			return getRenderPassAttachmentArbitraryActionMask(
					pColorAttachments,
					pDepthStencilAttachment,
					pActiveAttachmentsMask,
					E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_CLEARBit );
		}

		TBitmask<ERTAttachmentFlags> getRenderPassAttachmentResolveMask(
				const RenderPassColorAttachmentConfigArray & pColorAttachments,
				const RenderPassAttachmentConfig pDepthStencilAttachment,
				TBitmask<ERTAttachmentFlags> pActiveAttachmentsMask )
		{
			return getRenderPassAttachmentArbitraryActionMask(
					pColorAttachments,
					pDepthStencilAttachment,
					pActiveAttachmentsMask,
					E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_RESOLVEBit );
		}

	}

}
