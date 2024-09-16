
#include "RenderTargetTexture.h"
#include "Texture.h"

namespace Ic3::Graphics::GCI
{

	RenderTargetTexture::RenderTargetTexture(
			GPUDevice & pGPUDevice,
			ERenderTargetTextureType pRTTextureType,
			const RenderTargetTextureLayout & pRTTextureLayout,
			TextureReference pTargetTexture )
	: GPUResourceView( pGPUDevice, EGPUResourceBaseType::Texture, pTargetTexture->mTextureProperties.mResourceFlags )
	, mRTTextureType( pRTTextureType )
	, mRTBufferMask( CxDef::getRTBufferMaskForRenderTargetTextureType( pRTTextureType ) )
	, mRTTextureLayout( pRTTextureLayout )
	, mTargetTexture( pTargetTexture )
	{}

	RenderTargetTexture::RenderTargetTexture(
			GPUDevice & pGPUDevice,
			ERenderTargetTextureType pRTTextureType,
			const RenderTargetTextureLayout & pRTTextureLayout,
			TGPAHandle<GPUDeviceChildObject> pInternalRenderBuffer,
			TBitmask<resource_flags_value_t> pRenderBufferFlags )
	: GPUResourceView( pGPUDevice, EGPUResourceBaseType::Texture, pRenderBufferFlags )
	, mRTTextureType( pRTTextureType )
	, mRTBufferMask( CxDef::getRTBufferMaskForRenderTargetTextureType( pRTTextureType ) )
	, mRTTextureLayout( pRTTextureLayout )
	, mTargetTexture()
	, _internalRenderBuffer( pInternalRenderBuffer )
	{}

	RenderTargetTexture::~RenderTargetTexture() = default;

	bool RenderTargetTexture::empty() const noexcept
	{
		return !mTargetTexture && !_internalRenderBuffer;
	}

	bool RenderTargetTexture::isDepthStencilTexture() const noexcept
	{
		return mRTBufferMask.isSetAnyOf( E_RENDER_TARGET_BUFFER_MASK_DEPTH_STENCIL );
	}

	bool RenderTargetTexture::isDepthStencilRenderBuffer() const noexcept
	{
		return _internalRenderBuffer && isDepthStencilTexture();
	}

	namespace RCU
	{

		ERenderTargetTextureType queryRenderTargetTextureType( ETextureFormat pFormat )
		{
			const TBitmask<uint8> pixelFormatFlags = CxDef::getTextureFormatFlags( pFormat );

			if( pixelFormatFlags.isSet( eGPUDataFormatFlagCompressedBit ) )
			{
				return ERenderTargetTextureType::Unknown;
			}
			else if( pixelFormatFlags.isSet( eGPUDataFormatFlagDepthStencilBit ) )
			{
				return ERenderTargetTextureType::RTDepthStencil;
			}
			else if( pixelFormatFlags.isSet( eGPUDataFormatFlagDepthBit ) )
			{
				return ERenderTargetTextureType::RTDepthOnly;
			}
			else if( pixelFormatFlags.isSet( eGPUDataFormatFlagStencilBit ) )
			{
				return ERenderTargetTextureType::RTStencilOnly;
			}
			else /* Not a depth/stencil, not a compressed, should be a color-compatible one. */
			{
				return ERenderTargetTextureType::RTColor;
			}
		}

		RenderTargetTextureLayout queryRenderTargetTextureLayout( const TextureLayout & pTextureLayout )
		{
			RenderTargetTextureLayout rtTextureLayout{};
			rtTextureLayout.mImageRect.mWidth = pTextureLayout.mDimensions.mWidth;
			rtTextureLayout.mImageRect.mHeight = pTextureLayout.mDimensions.mHeight;
			rtTextureLayout.mInternalFormat = pTextureLayout.mInternalFormat;
			rtTextureLayout.mMSAALevel = pTextureLayout.mMSAALevel;

			return rtTextureLayout;
		}

		bool validateRenderTargetTextureLayout( TextureHandle pTargetTexture, const RenderTargetTextureLayout & pRTTextureLayout )
		{
			if( !pTargetTexture )
			{
				return false;
			}

			const auto & targetTextureLayout = pTargetTexture->mTextureLayout;

			const auto layoutMatch =
					(pRTTextureLayout.mImageRect.mWidth == targetTextureLayout.mDimensions.mWidth ) &&
					(pRTTextureLayout.mImageRect.mHeight == targetTextureLayout.mDimensions.mHeight ) &&
					(pRTTextureLayout.mInternalFormat == targetTextureLayout.mInternalFormat ) &&
					(pRTTextureLayout.mMSAALevel == targetTextureLayout.mMSAALevel );

			return layoutMatch;
		}

	}

}
