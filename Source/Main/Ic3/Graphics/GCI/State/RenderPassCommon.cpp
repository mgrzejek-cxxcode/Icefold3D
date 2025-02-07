
#include "RenderPassCommon.h"
#include <Ic3/Graphics/GCI/Resources/Texture.h>
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>

namespace Ic3::Graphics::GCI
{

	cppx::bitmask<ERTAttachmentFlags> RenderPassConfiguration::GetAttachmentsMaskWithLoadFlags(
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		cppx::bitmask<ERTAttachmentFlags> attachmentsMask = 0;

		GCU::ForEachRTColorAttachmentIndex( activeAttachmentsMask,
			[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
			{
				if( const auto & colorAttachmentUsage = colorAttachmentConfigArray[pIndex] )
				{
					if( cppx::make_bitmask( colorAttachmentUsage.loadAction ).is_set( pActionFlags & eRenderPassAttachmentActionMaskLoadAll ) )
					{
						attachmentsMask.set( CXU::RTOMakeColorAttachmentFlag( pIndex ) );
					}
				}
				return true;
			});

		if( depthStencilAttachmentConfig )
		{
			if( cppx::make_bitmask( depthStencilAttachmentConfig.loadAction ).is_set( pActionFlags & eRenderPassAttachmentActionMaskLoadAll ) )
			{
				attachmentsMask.set( CXU::RTOMakeDepthStencilAttachmentFlag() );
			}
		}

		return attachmentsMask;
	}

	cppx::bitmask<ERTAttachmentFlags> RenderPassConfiguration::GetAttachmentsMaskWithStoreFlags(
			cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept
	{
		cppx::bitmask<ERTAttachmentFlags> attachmentsMask = 0;

		GCU::ForEachRTAttachmentIndex( activeAttachmentsMask,
			[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
			{
				if( const auto & colorAttachmentUsage = colorAttachmentConfigArray[pIndex] )
				{
					if( cppx::make_bitmask( colorAttachmentUsage.storeAction ).is_set( pActionFlags & eRenderPassAttachmentActionMaskStoreAll ) )
					{
						attachmentsMask.set( CXU::RTOMakeAttachmentFlag( pIndex ) );
					}
				}
				return true;
			});

		if( depthStencilAttachmentConfig )
		{
			if( cppx::make_bitmask( depthStencilAttachmentConfig.storeAction ).is_set( pActionFlags & eRenderPassAttachmentActionMaskStoreAll ) )
			{
				attachmentsMask.set( CXU::RTOMakeDepthStencilAttachmentFlag() );
			}
		}

		return attachmentsMask;
	}

	namespace GCU
	{

		bool RTOValidateRenderPassConfiguration( const RenderPassConfiguration & pPassConfiguration )
		{
			if( const auto * firstActiveAttachmentRef = pPassConfiguration.FindFirstActiveAttachment() )
			{
				// Image Layout of the first attachment. This will serve as a reference point - all RT attachments
				// are required to have the same layout (dimensions), so we can compare to any of the active ones.
				const auto & commonImageLayout = firstActiveAttachmentRef->attachmentTexture->mRTTextureLayout;

				auto attachmentsValid = ForEachRTAttachmentIndex( pPassConfiguration.activeAttachmentsMask,
					[&]( native_uint pAttachmentIndex, ERTAttachmentFlags pAttachmentBit )
					{
						const auto & attachmentRef = pPassConfiguration.attachmentConfigArray[pAttachmentIndex];
						if( !attachmentRef )
						{
							return false;
						}
						const auto requiredUsageFlags = GCU::RTOGetAttachmentRequiredUsageMask( pAttachmentIndex );
						if( !attachmentRef.attachmentTexture->mResourceFlags.is_set( requiredUsageFlags ) )
						{
							return false;
						}
						const auto & textureLayout = attachmentRef.attachmentTexture->mRTTextureLayout;
						if( textureLayout.imageRect != commonImageLayout.imageRect )
						{
							return false;
						}
						if( textureLayout.msaaLevel != commonImageLayout.msaaLevel )
						{
							return false;
						}
						if( attachmentRef.HasStoreActionFlags( eRenderPassAttachmentActionFlagStoreResolveBit ) && !attachmentRef.resolveTexture )
						{
							return false;
						}
						return true;
					} );

				if( attachmentsValid )
				{
					return true;
				}
			}

			return false;
		}

		RenderTargetArrayLayout RTOGetRenderTargetArrayLayoutForPassConfiguration( const RenderPassConfiguration & pPassConfiguration )
		{
			if( const auto * firstActiveAttachmentRef = pPassConfiguration.FindFirstActiveAttachment() )
			{
				const auto & referenceLayout = firstActiveAttachmentRef->attachmentTexture->mRTTextureLayout;

				RenderTargetArrayLayout renderTargetLayout{};
				renderTargetLayout.activeAttachmentsMask = 0;
				renderTargetLayout.sharedImageSize = referenceLayout.imageRect;
				renderTargetLayout.sharedMSAALevel = referenceLayout.msaaLevel;

				const auto layoutValid = ForEachRTAttachmentIndex( pPassConfiguration.activeAttachmentsMask,
					[&]( native_uint pAttachmentIndex, ERTAttachmentFlags pAttachmentBit )
					{
						const auto & attachmentRef = pPassConfiguration.attachmentConfigArray[pAttachmentIndex];
						if( !attachmentRef )
						{
							return false;
						}

						const auto & attachmentTexture = attachmentRef.attachmentTexture;
						const auto & textureLayout = attachmentTexture->mRTTextureLayout;

						if( ( textureLayout.imageRect != referenceLayout.imageRect ) || ( textureLayout.msaaLevel != referenceLayout.msaaLevel ) )
						{
							return false;
						}

						renderTargetLayout.attachmentConfigArray[pAttachmentIndex].format = textureLayout.internalFormat;
						renderTargetLayout.activeAttachmentsMask.set( pAttachmentBit );

						return true;
					} );

				if( layoutValid )
				{
					return renderTargetLayout;
				}
			}

			return RenderTargetArrayLayout{};
		}

	}

}
