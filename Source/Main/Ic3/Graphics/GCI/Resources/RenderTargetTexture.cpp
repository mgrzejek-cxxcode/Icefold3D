
#include "RenderTargetTexture.h"
#include "Texture.h"

namespace Ic3::Graphics::GCI
{

	RenderTargetTexture::RenderTargetTexture(
			GpuDevice & pGpuDevice,
			ERenderTargetTextureType pRTTextureType,
			const RenderTargetTextureLayout & pRTTextureLayout,
			TextureReference pTargetTexture )
	: GpuResourceView( pGpuDevice, EGpuResourceBaseType::Texture, pTargetTexture->mTextureProperties.resourceFlags )
	, mRTTextureType( pRTTextureType )
	, mRTBufferMask( CxDef::GetRTBufferMaskForRenderTargetTextureType( pRTTextureType ) )
	, mRTTextureLayout( pRTTextureLayout )
	, mTargetTexture( pTargetTexture )
	{}

	RenderTargetTexture::RenderTargetTexture(
			GpuDevice & pGpuDevice,
			ERenderTargetTextureType pRTTextureType,
			const RenderTargetTextureLayout & pRTTextureLayout,
			TGfxHandle<GpuDeviceChildObject> pInternalRenderBuffer,
			cppx::bitmask<resource_flags_value_t> pRenderBufferFlags )
	: GpuResourceView( pGpuDevice, EGpuResourceBaseType::Texture, pRenderBufferFlags )
	, mRTTextureType( pRTTextureType )
	, mRTBufferMask( CxDef::GetRTBufferMaskForRenderTargetTextureType( pRTTextureType ) )
	, mRTTextureLayout( pRTTextureLayout )
	, mTargetTexture()
	, _internalRenderBuffer( pInternalRenderBuffer )
	{}

	RenderTargetTexture::~RenderTargetTexture() = default;

	bool RenderTargetTexture::IsEmpty() const noexcept
	{
		return !mTargetTexture && !_internalRenderBuffer;
	}

	bool RenderTargetTexture::IsDepthStencilTexture() const noexcept
	{
		return mRTBufferMask.is_set_any_of( E_RENDER_TARGET_BUFFER_MASK_DEPTH_STENCIL );
	}

	bool RenderTargetTexture::IsDepthStencilRenderBuffer() const noexcept
	{
		return _internalRenderBuffer && IsDepthStencilTexture();
	}

	namespace RCU
	{

		ERenderTargetTextureType QueryRenderTargetTextureType( ETextureFormat pFormat )
		{
			const cppx::bitmask<uint8> pixelFormatFlags = CxDef::GetTextureFormatFlags( pFormat );

			if( pixelFormatFlags.is_set( eGpuDataFormatFlagCompressedBit ) )
			{
				return ERenderTargetTextureType::Unknown;
			}
			else if( pixelFormatFlags.is_set( eGpuDataFormatFlagDepthStencilBit ) )
			{
				return ERenderTargetTextureType::RTDepthStencil;
			}
			else if( pixelFormatFlags.is_set( eGpuDataFormatFlagDepthBit ) )
			{
				return ERenderTargetTextureType::RTDepthOnly;
			}
			else if( pixelFormatFlags.is_set( eGpuDataFormatFlagStencilBit ) )
			{
				return ERenderTargetTextureType::RTStencilOnly;
			}
			else /* Not a depth/stencil, not a compressed, should be a color-compatible one. */
			{
				return ERenderTargetTextureType::RTColor;
			}
		}

		RenderTargetTextureLayout QueryRenderTargetTextureLayout( const TextureLayout & pTextureLayout )
		{
			RenderTargetTextureLayout rtTextureLayout{};
			rtTextureLayout.imageRect.width = pTextureLayout.dimensions.width;
			rtTextureLayout.imageRect.height = pTextureLayout.dimensions.height;
			rtTextureLayout.internalFormat = pTextureLayout.internalFormat;
			rtTextureLayout.msaaLevel = pTextureLayout.msaaLevel;

			return rtTextureLayout;
		}

		bool ValidateRenderTargetTextureLayout( TextureHandle pTargetTexture, const RenderTargetTextureLayout & pRTTextureLayout )
		{
			if( !pTargetTexture )
			{
				return false;
			}

			const auto & targetTextureLayout = pTargetTexture->mTextureLayout;

			const auto layoutMatch =
					(pRTTextureLayout.imageRect.width == targetTextureLayout.dimensions.width ) &&
					(pRTTextureLayout.imageRect.height == targetTextureLayout.dimensions.height ) &&
					(pRTTextureLayout.internalFormat == targetTextureLayout.internalFormat ) &&
					(pRTTextureLayout.msaaLevel == targetTextureLayout.msaaLevel );

			return layoutMatch;
		}

	}

}
