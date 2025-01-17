
#include "Texture.h"
#include <cppx/utilities.h>

namespace Ic3::Graphics::GCI
{

	TextureInitDataDesc::TextureInitDataDesc( TextureInitDataDesc && pSrcObject )
	{
		if( !pSrcObject.IsEmpty() )
		{
			if( pSrcObject.IsArray() )
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
		TextureInitDataDesc( std::move( pRhs ) ).Swap( *this );
		return *this;
	}

	TextureInitDataDesc::TextureInitDataDesc( const TextureInitDataDesc & pInitData )
	{
		if( !pInitData.IsEmpty() )
		{
			if( pInitData.IsArray() )
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
		TextureInitDataDesc( pRhs ).Swap( *this );
		return *this;
	}

	void TextureInitDataDesc::Initialize( const TextureDimensions & pDimensions )
	{
		ic3DebugAssert( pDimensions.arraySize > 0 );
		ic3DebugAssert( pDimensions.mipLevelsNum > 0 );

		if( pDimensions.arraySize == 1 )
		{
			subTextureInitDataBasePtr = &_subTextureInitData;
		}
		else
		{
			_subTextureInitDataArray.resize( pDimensions.arraySize );
			subTextureInitDataBasePtr = _subTextureInitDataArray.data();
		}

		for( uint32 subTextureIndex = 0; subTextureIndex < pDimensions.arraySize; ++subTextureIndex )
		{
			auto & subTextureInitData = subTextureInitDataBasePtr[subTextureIndex];
			subTextureInitData.subTextureIndex = subTextureIndex;

			uint32 mipLevelWidth = pDimensions.width;
			uint32 mipLevelHeight = pDimensions.height;
			uint32 mipLevelDepth = pDimensions.depth;

			for( uint32 mipLevelIndex = 0; mipLevelIndex < pDimensions.mipLevelsNum; ++mipLevelIndex )
			{
				auto & mipLevelInitData = subTextureInitData.mipLevelInitDataArray[mipLevelIndex];
				mipLevelInitData.mipLevelIndex = mipLevelIndex;
				mipLevelInitData.mipWidth = mipLevelWidth;
				mipLevelInitData.mipHeight = mipLevelHeight;
				mipLevelInitData.mipDepth = mipLevelDepth;

				mipLevelWidth = cppx::get_max_of( mipLevelWidth >> 1, 1u );
				mipLevelHeight = cppx::get_max_of( mipLevelHeight >> 1, 1u );
				mipLevelDepth = cppx::get_max_of( mipLevelDepth >> 1, 1u );
			}

			if( pDimensions.mipLevelsNum > 1 )
			{
				const auto & sMipLevel = subTextureInitData.mipLevelInitDataArray[pDimensions.mipLevelsNum - 2];
				ic3DebugAssert((sMipLevel.mipWidth != 1 ) || (sMipLevel.mipHeight != 1 ) || (sMipLevel.mipDepth != 1 ) );
			}
		}
	}

	void TextureInitDataDesc::Swap( TextureInitDataDesc & pOther )
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

	bool TextureInitDataDesc::IsArray() const
	{
		return !_subTextureInitDataArray.empty();
	}

	bool TextureInitDataDesc::IsEmpty() const
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
			ETextureFormat::Undefined,
			TextureDimensions {
				0, 0, 0, 0, 0
			},
			0,
			0,
			0
		};

		bool CheckTextureFormatAndBindFlagsCompatibility(
				ETextureFormat pTextureFormat,
				cppx::bitmask<resource_flags_value_t> pBindFlags )
		{
			if( pBindFlags.is_set( eGpuResourceUsageMaskRenderTargetDepthStencil ) )
			{
				return ( pTextureFormat == ETextureFormat::D24UNS8U );
			}
			else if( pBindFlags.is_set( eGpuResourceUsageFlagRenderTargetDepthBit ) )
			{
				return
					( pTextureFormat == ETextureFormat::D16UN ) ||
					( pTextureFormat == ETextureFormat::D24UNX8 ) ||
					( pTextureFormat == ETextureFormat::D32F );
			}
			else if( pBindFlags.is_set( eGpuResourceUsageFlagRenderTargetStencilBit ) )
			{
				return ( pTextureFormat == ETextureFormat::X24S8U );
			}

			return false;
		}

		const TextureLayout & QueryTextureLayout( TextureHandle pTexture ) noexcept
		{
			return pTexture ? pTexture->mTextureLayout : sInvalidTextureLayout;
		}

		RenderTargetTextureLayout QueryRenderTargetTextureLayoutForTexture( TextureHandle pTexture ) noexcept
		{
			return pTexture ? QueryRenderTargetTextureLayoutForTexture( pTexture->mTextureLayout ) : RenderTargetTextureLayout{};
		}

		RenderTargetTextureLayout QueryRenderTargetTextureLayoutForTexture( const TextureLayout & pTextureLayout ) noexcept
		{
			RenderTargetTextureLayout rttLayout{};
			rttLayout.internalFormat = pTextureLayout.internalFormat;
			rttLayout.msaaLevel = pTextureLayout.msaaLevel;
			rttLayout.imageRect.width = pTextureLayout.dimensions.width;
			rttLayout.imageRect.height = pTextureLayout.dimensions.height;
			return rttLayout;
		}

	}

} // namespace Ic3::Graphics::GCI
