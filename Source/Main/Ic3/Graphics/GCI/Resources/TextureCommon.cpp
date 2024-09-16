
#include "Texture.h"
#include <Ic3/Cppx/Utilities.h>

namespace Ic3::Graphics::GCI
{

	TextureInitDataDesc::TextureInitDataDesc( TextureInitDataDesc && pSrcObject )
	{
		if( !pSrcObject.empty() )
		{
			if( pSrcObject.isArray() )
			{
				_subTextureInitDataArray = std::move( pSrcObject._subTextureInitDataArray );
				subTextureInitDataBasePtr = _subTextureInitDataArray.data();
			}
			else
			{
				_subTextureInitData = pSrcObject._subTextureInitData;
				subTextureInitDataBasePtr = &_subTextureInitData;
			}
			pSrcObject.subTextureInitDataBasePtr = nullptr;
		}
	}

	TextureInitDataDesc & TextureInitDataDesc::operator=( TextureInitDataDesc && pRhs )
	{
		TextureInitDataDesc( std::move( pRhs ) ).swap( *this );
		return *this;
	}

	TextureInitDataDesc::TextureInitDataDesc( const TextureInitDataDesc & pInitData )
	{
		if( !pInitData.empty() )
		{
			if( pInitData.isArray() )
			{
				_subTextureInitDataArray = pInitData._subTextureInitDataArray;
				subTextureInitDataBasePtr = _subTextureInitDataArray.data();
			}
			else
			{
				_subTextureInitData = pInitData._subTextureInitData;
				subTextureInitDataBasePtr = &_subTextureInitData;
			}
		}
	}

	TextureInitDataDesc & TextureInitDataDesc::operator=( const TextureInitDataDesc & pRhs )
	{
		TextureInitDataDesc( pRhs ).swap( *this );
		return *this;
	}

	void TextureInitDataDesc::initialize( const TextureDimensions & pDimensions )
	{
		ic3DebugAssert( pDimensions.mArraySize > 0 );
		ic3DebugAssert( pDimensions.mMipLevelsNum > 0 );

		if( pDimensions.mArraySize == 1 )
		{
			subTextureInitDataBasePtr = &_subTextureInitData;
		}
		else
		{
			_subTextureInitDataArray.resize( pDimensions.mArraySize );
			subTextureInitDataBasePtr = _subTextureInitDataArray.data();
		}

		for( uint32 subTextureIndex = 0; subTextureIndex < pDimensions.mArraySize; ++subTextureIndex )
		{
			auto & subTextureInitData = subTextureInitDataBasePtr[subTextureIndex];
			subTextureInitData.mSubTextureIndex = subTextureIndex;

			uint32 mipLevelWidth = pDimensions.mWidth;
			uint32 mipLevelHeight = pDimensions.mHeight;
			uint32 mipLevelDepth = pDimensions.mDepth;

			for( uint32 mipLevelIndex = 0; mipLevelIndex < pDimensions.mMipLevelsNum; ++mipLevelIndex )
			{
				auto & mipLevelInitData = subTextureInitData.mMipLevelInitDataArray[mipLevelIndex];
				mipLevelInitData.mMipLevelIndex = mipLevelIndex;
				mipLevelInitData.mMipWidth = mipLevelWidth;
				mipLevelInitData.mMipHeight = mipLevelHeight;
				mipLevelInitData.mMipDepth = mipLevelDepth;

				mipLevelWidth = getMaxOf( mipLevelWidth >> 1, 1u );
				mipLevelHeight = getMaxOf( mipLevelHeight >> 1, 1u );
				mipLevelDepth = getMaxOf( mipLevelDepth >> 1, 1u );
			}

			if( pDimensions.mMipLevelsNum > 1 )
			{
				const auto & sMipLevel = subTextureInitData.mMipLevelInitDataArray[pDimensions.mMipLevelsNum - 2];
				ic3DebugAssert((sMipLevel.mMipWidth != 1 ) || (sMipLevel.mMipHeight != 1 ) || (sMipLevel.mMipDepth != 1 ) );
			}
		}
	}

	void TextureInitDataDesc::swap( TextureInitDataDesc & pOther )
	{
		if( _subTextureInitDataArray.empty() )
		{
			std::swap( _subTextureInitData, pOther._subTextureInitData );
		}
		else
		{
			std::swap( _subTextureInitDataArray, pOther._subTextureInitDataArray );
			std::swap( subTextureInitDataBasePtr, pOther.subTextureInitDataBasePtr );
		}
	}

	bool TextureInitDataDesc::isArray() const
	{
		return !_subTextureInitDataArray.empty();
	}

	bool TextureInitDataDesc::empty() const
	{
		return subTextureInitDataBasePtr == nullptr;
	}

	TextureInitDataDesc TextureInitDataDesc::copy() const
	{
		TextureInitDataDesc copyResult;
		if( _subTextureInitDataArray.empty() )
		{
			copyResult._subTextureInitData = _subTextureInitData;
			copyResult.subTextureInitDataBasePtr = &( copyResult._subTextureInitData );
		}
		else
		{
			copyResult._subTextureInitDataArray = _subTextureInitDataArray;
			copyResult.subTextureInitDataBasePtr = copyResult._subTextureInitDataArray.data();
		}
		return copyResult;
	}


	namespace RCU
	{

		static const TextureLayout sInvalidTextureLayout {
			ETextureClass::Unknown,
			ETextureFormat::Unknown,
			TextureDimensions {
				0, 0, 0, 0, 0
			},
			0,
			0,
			0
		};

		bool checkTextureFormatAndBindFlagsCompatibility(
				ETextureFormat pTextureFormat,
				TBitmask<resource_flags_value_t> pBindFlags )
		{
			if( pBindFlags.isSet( eGPUResourceUsageMaskRenderTargetDepthStencil ) )
			{
				return ( pTextureFormat == ETextureFormat::D24UNS8U );
			}
			else if( pBindFlags.isSet( eGPUResourceUsageFlagRenderTargetDepthBit ) )
			{
				return
					( pTextureFormat == ETextureFormat::D16UN ) ||
					( pTextureFormat == ETextureFormat::D24UNX8 ) ||
					( pTextureFormat == ETextureFormat::D32F );
			}
			else if( pBindFlags.isSet( eGPUResourceUsageFlagRenderTargetStencilBit ) )
			{
				return ( pTextureFormat == ETextureFormat::X24S8U );
			}

			return false;
		}

		const TextureLayout & queryTextureLayout( TextureHandle pTexture ) noexcept
		{
			return pTexture ? pTexture->mTextureLayout : sInvalidTextureLayout;
		}

		RenderTargetTextureLayout queryRenderTargetTextureLayoutForTexture( TextureHandle pTexture ) noexcept
		{
			return pTexture ? queryRenderTargetTextureLayoutForTexture( pTexture->mTextureLayout ) : RenderTargetTextureLayout{};
		}

		RenderTargetTextureLayout queryRenderTargetTextureLayoutForTexture( const TextureLayout & pTextureLayout ) noexcept
		{
			RenderTargetTextureLayout rttLayout{};
			rttLayout.mInternalFormat = pTextureLayout.mInternalFormat;
			rttLayout.mMSAALevel = pTextureLayout.mMSAALevel;
			rttLayout.mImageRect.mWidth = pTextureLayout.mDimensions.mWidth;
			rttLayout.mImageRect.mHeight = pTextureLayout.mDimensions.mHeight;
			return rttLayout;
		}

	}

} // namespace Ic3::Graphics::GCI
