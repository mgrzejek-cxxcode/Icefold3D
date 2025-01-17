
#include "RenderPassCommon.h"

namespace Ic3::Graphics::GCI
{

	void RenderPassConfiguration::ResetAttachmentsFlags() noexcept
	{
		attachmentsAccessRestrictMask = 0;
		attachmentsActionClearMask = 0;
		attachmentsActionResolveMask = 0;

		ForEachRTAttachmentIndex( activeAttachmentsMask,
			[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
			{
				const auto & attachmentConfig = attachments[pIndex];
				if( !attachmentConfig )
				{
					activeAttachmentsMask.unset( pAttachmentBit );
				}
				else
				{
					const auto attachmentActionMask = SMU::GetRenderPassAttachmentActionMask( attachmentConfig );
					if( attachmentActionMask.is_set( E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_ACCESS_RESTRICT_BIT ) )
					{
						attachmentsAccessRestrictMask.set( pAttachmentBit );
					}
					else
					{
						if( attachmentActionMask.is_set( E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_CLEAR_BIT ) )
						{
							attachmentsActionClearMask.set( pAttachmentBit );
						}
						if( attachmentActionMask.is_set( E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_RESOLVE_BIT ) )
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

		cppx::bitmask<ERenderPassAttachmentActionFlags> GetRenderPassAttachmentActionMask(
				const RenderPassAttachmentConfig & pAttachmentConfig )
		{
			cppx::bitmask<ERenderPassAttachmentActionFlags> attachmentActionMask = 0;
			attachmentActionMask.set( static_cast<uint32>( pAttachmentConfig.renderPassLoadAction ) );
			attachmentActionMask.set( static_cast<uint32>( pAttachmentConfig.renderPassStoreAction ) );
			return attachmentActionMask;
		}

		cppx::bitmask<ERTAttachmentFlags> GetRenderPassAttachmentArbitraryActionMask(
				const RenderPassColorAttachmentConfigArray & pColorAttachments,
				const RenderPassAttachmentConfig pDepthStencilAttachment,
				cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask,
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionMask )
		{
			cppx::bitmask<ERTAttachmentFlags> attachmentActionMask = 0;
			ForEachRTAttachmentIndex( pActiveAttachmentsMask,
				[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
				{
					const auto attachmentUsage = GetRenderPassAttachmentActionMask( pColorAttachments[pIndex] );
					if( attachmentUsage.is_set( pActionMask ) )
					{
						attachmentActionMask.set( pAttachmentBit );
					}
					return true;
				} );
			return attachmentActionMask;
		}

		cppx::bitmask<ERTAttachmentFlags> GetRenderPassAttachmentClearMask(
				const RenderPassColorAttachmentConfigArray & pColorAttachments,
				const RenderPassAttachmentConfig pDepthStencilAttachment,
				cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask )
		{
			return GetRenderPassAttachmentArbitraryActionMask(
					pColorAttachments,
					pDepthStencilAttachment,
					pActiveAttachmentsMask,
					E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_CLEAR_BIT );
		}

		cppx::bitmask<ERTAttachmentFlags> GetRenderPassAttachmentResolveMask(
				const RenderPassColorAttachmentConfigArray & pColorAttachments,
				const RenderPassAttachmentConfig pDepthStencilAttachment,
				cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask )
		{
			return GetRenderPassAttachmentArbitraryActionMask(
					pColorAttachments,
					pDepthStencilAttachment,
					pActiveAttachmentsMask,
					E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_RESOLVE_BIT );
		}

	}

}
