
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
				_subTextureInitDataBasePtr = _subTextureInitDataArray.data();
			}
			else
			{
				_subTextureInitData = pSrcObject._subTextureInitData;
				_subTextureInitDataBasePtr = &_subTextureInitData;
			}
			pSrcObject._subTextureInitDataBasePtr = nullptr;
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
				_subTextureInitDataBasePtr = _subTextureInitDataArray.data();
			}
			else
			{
				_subTextureInitData = pInitData._subTextureInitData;
				_subTextureInitDataBasePtr = &_subTextureInitData;
			}
		}
	}

	TextureInitDataDesc & TextureInitDataDesc::operator=( const TextureInitDataDesc & pRhs )
	{
		TextureInitDataDesc( pRhs ).Swap( *this );
		return *this;
	}

	bool TextureInitDataDesc::IsArray() const noexcept
	{
		return !_subTextureInitDataArray.empty();
	}

	bool TextureInitDataDesc::IsEmpty() const noexcept
	{
		return _subTextureInitDataBasePtr == nullptr;
	}

	TextureInitDataDesc TextureInitDataDesc::Copy() const noexcept
	{
		TextureInitDataDesc copyResult;
		if( _subTextureInitDataArray.empty() )
		{
			copyResult._subTextureInitData = _subTextureInitData;
			copyResult._subTextureInitDataBasePtr = &( copyResult._subTextureInitData );
		}
		else
		{
			copyResult._subTextureInitDataArray = _subTextureInitDataArray;
			copyResult._subTextureInitDataBasePtr = copyResult._subTextureInitDataArray.data();
		}
		return copyResult;
	}

	cppx::bitmask<ETextureInitFlags> TextureInitDataDesc::GetInitFlags() const noexcept
	{
		return _textureInitFlags;
	}

	const TextureSubTextureInitDataDesc * TextureInitDataDesc::GetSubTextureInitDataArrayPtr() const noexcept
	{
		return !_subTextureInitDataArray.empty() ? _subTextureInitDataArray.data() : nullptr;
	}

	TextureSubTextureInitDataDesc & TextureInitDataDesc::GetSubTextureInitDesc( native_uint pSubTextureIndex ) noexcept
	{
		return _subTextureInitDataArray[pSubTextureIndex];
	}

	const TextureSubTextureInitDataDesc & TextureInitDataDesc::GetSubTextureInitDesc( native_uint pSubTextureIndex ) const noexcept
	{
		return _subTextureInitDataArray[pSubTextureIndex];
	}

	void TextureInitDataDesc::Initialize( const TextureDimensions & pDimensions, cppx::bitmask<ETextureInitFlags> pInitFlags )
	{
		Ic3DebugAssert( pDimensions.arraySize > 0 );
		Ic3DebugAssert( pDimensions.mipLevelsNum > 0 );

		if( pDimensions.arraySize == 1 )
		{
			_subTextureInitDataBasePtr = &_subTextureInitData;
		}
		else
		{
			_subTextureInitDataArray.resize( pDimensions.arraySize );
			_subTextureInitDataBasePtr = _subTextureInitDataArray.data();
		}

		for( uint32 subTextureIndex = 0; subTextureIndex < pDimensions.arraySize; ++subTextureIndex )
		{
			auto & subTextureInitData = _subTextureInitDataBasePtr[subTextureIndex];
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
				Ic3DebugAssert( ( sMipLevel.mipWidth != 1 ) || ( sMipLevel.mipHeight != 1 ) || ( sMipLevel.mipDepth != 1 ) );
			}
		}

		_textureInitFlags = pInitFlags;
	}

	void TextureInitDataDesc::SetInitFlags( cppx::bitmask<ETextureInitFlags> pInitFlags )
	{
		_textureInitFlags = pInitFlags;
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
			std::swap( _subTextureInitDataBasePtr, pOther._subTextureInitDataBasePtr );
		}
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
			if( pBindFlags.is_set( eGPUResourceUsageMaskRenderTargetDepthStencil ) )
			{
				return ( pTextureFormat == ETextureFormat::D24UNS8U );
			}
			else if( pBindFlags.is_set( eGPUResourceUsageFlagRenderTargetDepthBit ) )
			{
				return
					( pTextureFormat == ETextureFormat::D16UN ) ||
					( pTextureFormat == ETextureFormat::D24UNX8 ) ||
					( pTextureFormat == ETextureFormat::D32F );
			}
			else if( pBindFlags.is_set( eGPUResourceUsageFlagRenderTargetStencilBit ) )
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
