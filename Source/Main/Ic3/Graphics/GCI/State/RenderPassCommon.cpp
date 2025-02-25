
#include "RenderPassCommon.h"
#include <Ic3/Graphics/GCI/Resources/Texture.h>
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>
#include <cppx/stdHelperAlgo.h>

namespace Ic3::Graphics::GCI
{

	cppx::bitmask<ERTAttachmentFlags> RenderPassConfiguration::GetCachedAttachmentsWithFlags(
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		return cppx::get_map_value_ref_or_default( cachedAttachmentsMap, pActionFlags, static_cast<ERTAttachmentFlags>( 0u ) );
	}

	cppx::bitmask<ERTAttachmentFlags> RenderPassConfiguration::GetAttachmentsMaskWithLoadFlags(
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		cppx::bitmask<ERTAttachmentFlags> attachmentsMask = 0;

		if( pActionFlags )
		{
			GCU::ForEachRTColorAttachmentIndex( activeAttachmentsMask,
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

		if( pActionFlags )
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

	void RenderPassConfiguration::SaveCachedAttachmentsInfo( cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags )
	{
		const auto attachmentsWithLoadFlags = GetAttachmentsMaskWithLoadFlags( pActionFlags & eRenderPassAttachmentActionMaskLoadAll );
		const auto attachmentsWithStoreFlags = GetAttachmentsMaskWithStoreFlags( pActionFlags & eRenderPassAttachmentActionMaskStoreAll );

		if( const auto attachmentsMask = ( attachmentsWithLoadFlags & attachmentsWithStoreFlags ) )
		{
			cachedAttachmentsMap[pActionFlags] = attachmentsMask;
		}
	}


	namespace GCU
	{

		bool RTOValidateRenderPassConfiguration( const RenderPassConfiguration & pRenderPassConfiguration )
		{
			return true;
		}

	}

}
