
#include "RenderPassCommon.h"
#include <Ic3/Graphics/GCI/Resources/Texture.h>
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>
#include <cppx/stdHelperAlgo.h>

namespace Ic3::Graphics::GCI
{

	cppx::bitmask<ERTAttachmentFlags> RenderPassConfiguration::GetAttachmentsMaskWithFlags(
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags,
			bool pBypassCache ) const noexcept
	{
		if( !pBypassCache )
		{
			// If pBypassCache is not set, first try to find the descriptors mask in the internal map.
			// We don't cache empty masks (0), so we use this as the default to indicate "not found".
			auto cachedAttachmentsMask = cppx::get_map_value_ref_or_default(
					cachedAttachmentsMap,
					pActionFlags,
					static_cast<ERTAttachmentFlags>( 0u ) );

			if( cachedAttachmentsMask )
			{
				return cachedAttachmentsMask;
			}
		}

		// pActionFlags can have both load and store bits. Since we treat those differently, we extract them into
		// separate variables and check for attachments independently.

		const auto loadActionBits = pActionFlags & eRenderPassAttachmentActionMaskLoadAll;
		// Get a mask with attachments that have load flags set for their render pass action.
		const auto attachmentsWithLoadFlags = GetAttachmentsMaskWithLoadFlags( loadActionBits );

		const auto storeActionBits = pActionFlags & eRenderPassAttachmentActionMaskStoreAll;
		// Get a mask with attachments that have store flags set for their render pass action.
		const auto attachmentsWithStoreFlags = GetAttachmentsMaskWithStoreFlags( storeActionBits );

		// We need to return attachments that have *all* requested bits set (i.e. their load and store actions include
		// the requested action flags), so in order to do this, we combine the results and then execute bit-wise AND.
		// If either load or store bits have not been specified, we skip that part, otherwise it would resolve to M & 0,
		// always producing an empty attachment mask.

		auto attachmentsMask = ( attachmentsWithLoadFlags | attachmentsWithStoreFlags );

		if( attachmentsMask )
		{
			if( loadActionBits )
			{
				attachmentsMask &= attachmentsWithLoadFlags;
			}
			if( storeActionBits )
			{
				attachmentsMask &= attachmentsWithStoreFlags;
			}
		}

		return attachmentsMask;
	}

	cppx::bitmask<ERTAttachmentFlags> RenderPassConfiguration::GetAttachmentsMaskWithLoadFlags(
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		cppx::bitmask<ERTAttachmentFlags> attachmentsMask = 0;

		if( pActionFlags & eRenderPassAttachmentActionMaskLoadAll )
		{
			GCU::ForEachRTAttachmentIndex( activeAttachmentsMask,
				[&]( native_uint pAttachmentIndex, ERTAttachmentFlags pAttachmentBit )
				{
					if( const auto * attachmentConfig = GetAttachment( pAttachmentIndex ) )
					{
						if( cppx::make_bitmask( attachmentConfig->loadAction ).is_set( pActionFlags & eRenderPassAttachmentActionMaskLoadAll ) )
						{
							attachmentsMask.set( pAttachmentBit );
						}
					}
					return true;
				});
		}

		return attachmentsMask;
	}

	cppx::bitmask<ERTAttachmentFlags> RenderPassConfiguration::GetAttachmentsMaskWithStoreFlags(
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		cppx::bitmask<ERTAttachmentFlags> attachmentsMask = 0;

		if( pActionFlags & eRenderPassAttachmentActionMaskStoreAll )
		{
			GCU::ForEachRTAttachmentIndex(
				activeAttachmentsMask,
				[&]( native_uint pAttachmentIndex, ERTAttachmentFlags pAttachmentBit ) -> bool
				{
					if( const auto * attachmentConfig = GetAttachment( pAttachmentIndex ) )
					{
						if( cppx::make_bitmask( attachmentConfig->storeAction ).is_set( pActionFlags & eRenderPassAttachmentActionMaskStoreAll ) )
						{
							attachmentsMask.set( pAttachmentBit );
						}
					}
					return true;
				});
		}

		return attachmentsMask;
	}

	void RenderPassConfiguration::CacheAttachmentsWithActionFlags( cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags )
	{
		if( const auto attachmentsMask = GetAttachmentsMaskWithFlags( pActionFlags ) )
		{
			cachedAttachmentsMap[pActionFlags] = attachmentsMask;
		}
	}

	void RenderPassConfiguration::ResetCachedAttachments()
	{
		cachedAttachmentsMap.clear();
	}


	namespace GCU
	{

		bool RTOValidateRenderPassConfiguration( const RenderPassConfiguration & pRenderPassConfiguration )
		{
			return true;
		}

	}

}
