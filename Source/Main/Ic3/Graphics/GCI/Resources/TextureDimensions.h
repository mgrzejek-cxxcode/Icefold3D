
#pragma once

#ifndef __IC3_GRAPHICS_GCI_TEXTURE_DIMENSIONS_H__
#define __IC3_GRAPHICS_GCI_TEXTURE_DIMENSIONS_H__

#include "CommonGPUResourceDefs.h"

namespace Ic3::Graphics::GCI
{

	/// @brief A special value for texture array index indicating all resources in an array.
	/// For a specific mip level, it references a vertical mip slice for a texture array.
	constexpr auto cxTexSubresourceArrayIndexAllTextures = Cppx::QLimits<uint32>::sMaxValue;

	/// @brief
	constexpr auto cxTexSubresourceCubeMapFaceAllFaces = Cppx::QLimits<uint32>::sMaxValue;

	/// @brief
	constexpr auto cxTexSubresourceDepthAllLayers = Cppx::QLimits<uint32>::sMaxValue;

	/// @brief A special value for mip level index indicating all levels a texture contains.
	/// For a given texture, it references all sub-resources in that texture..
	constexpr auto cxTexSubresourceMipLevelAllMips = Cppx::QLimits<uint32>::sMaxValue;

	/// @brief List of valid texture classes (types).
	enum class ETextureClass : enum_default_value_t
	{
		/// Unknown class.
		Unknown,
		/// 2D Texture: a single, 2-dimensional image.
		T2D,
		/// 2D Texture Array: an array of one or more 2-dimensional images.
		T2DArray,
		/// 2D MS Texture: a single, 2-dimensional image with multisampling.
		T2DMS,
		/// 2D MS Texture Array: an array of one or more 2-dimensional images with multisampling.
		T2DMSArray,
		/// 3D Texture: a single, 3-dimensional image.
		T3D,
		/// Cube Map Texture: a collection of six, 2-dimensional images (faces).
		TCubeMap,
	};

	/// @brief List of valid face identifiers for cube map textures.
	enum ETextureCubeMapFace : uint32
	{
		/// Face located on the positive side of the X axis (right).
		eTextureCubeMapFacePositiveX,
		/// Face located on the negative side of the X axis (left).
		eTextureCubeMapFaceNegativeX,
		/// Face located on the positive side of the Y axis (top).
		eTextureCubeMapFacePositiveY,
		/// Face located on the negative side of the Y axis (bottom).
		eTextureCubeMapFaceNegativeY,
		/// Face located on the positive side of the Z axis (front).
		eTextureCubeMapFacePositiveZ,
		/// Face located on the negative side of the Z axis (back).
		eTextureCubeMapFaceNegativeZ,
		/// Alias for the first face index (X+).
		eTextureCubeMapFaceFirst = eTextureCubeMapFacePositiveX,
		/// Alias for the last face index (Z-).
		eTextureCubeMapFaceLast = eTextureCubeMapFaceNegativeZ,
	};

	/// @brief Used to represent dimensions of a texture, regardless of the texture class.
	struct TextureDimensions
	{
		/// Width of a single image/layer within the texture.
		uint32 mWidth;
		/// Height of a single image/layer within the texture.
		uint32 mHeight;
		/// Depth of the texture. Used only for T3D textures. For other classes this is always 1..
		uint32 mDepth;
		/// Size of the texture array. Used for T2DArray and T2DMSArray textures. For other classes, this is always 1.
		uint32 mArraySize;
		/// Number of mip levels within a single texture image/layer. Valid values are from 1 to GCM::cxTextureMaxMipLevelsNum.
		uint32 mMipLevelsNum;
	};

	struct TextureOffset2D
	{
		uint32 mMipLevel;
		uint32 mPX;
		uint32 mPY;

		inline constexpr bool operator==( const TextureOffset2D & pRhs ) const noexcept
		{
			return ( mMipLevel == pRhs.mMipLevel ) && ( mPX == pRhs.mPX ) && ( mPY == pRhs.mPY );
		}

		inline constexpr bool operator!=( const TextureOffset2D & pRhs ) const noexcept
		{
			return ( mMipLevel != pRhs.mMipLevel ) || ( mPX != pRhs.mPX ) || ( mPY != pRhs.mPY );
		}
	};

	struct TextureOffset2DArray : public TextureOffset2D
	{
		uint32 mArrayIndex;

		inline constexpr bool operator==( const TextureOffset2DArray & pRhs ) const noexcept
		{
			return ( mMipLevel == pRhs.mMipLevel ) && ( mPX == pRhs.mPX ) && ( mPY == pRhs.mPY ) && ( mArrayIndex == pRhs.mArrayIndex );
		}

		inline constexpr bool operator!=( const TextureOffset2DArray & pRhs ) const noexcept
		{
			return ( mMipLevel != pRhs.mMipLevel ) || ( mPX != pRhs.mPX ) || ( mPY != pRhs.mPY ) || ( mArrayIndex != pRhs.mArrayIndex );
		}
	};

	struct TextureOffset3D : public TextureOffset2D
	{
		uint32 mPZ;

		inline constexpr bool operator==( const TextureOffset3D & pRhs ) const noexcept
		{
			return ( mMipLevel == pRhs.mMipLevel ) && ( mPX == pRhs.mPX ) && ( mPY == pRhs.mPY ) && ( mPZ == pRhs.mPZ );
		}

		inline constexpr bool operator!=( const TextureOffset3D & pRhs ) const noexcept
		{
			return ( mMipLevel != pRhs.mMipLevel ) || ( mPX != pRhs.mPX ) || ( mPY != pRhs.mPY ) || ( mPZ != pRhs.mPZ );
		}
	};

	struct TextureOffsetCubeMap : public TextureOffset2D
	{
		uint32 mFaceIndex;

		inline constexpr bool operator==( const TextureOffsetCubeMap & pRhs ) const noexcept
		{
			return ( mFaceIndex == pRhs.mFaceIndex ) && ( mMipLevel == pRhs.mMipLevel ) && ( mPX == pRhs.mPX ) && ( mPY == pRhs.mPY );
		}

		inline constexpr bool operator!=( const TextureOffsetCubeMap & pRhs ) const noexcept
		{
			return ( mFaceIndex != pRhs.mFaceIndex ) || ( mMipLevel != pRhs.mMipLevel ) || ( mPX != pRhs.mPX ) || ( mPY != pRhs.mPY );
		}
	};

	struct TextureOffset
	{
		ETextureClass mTexClass = ETextureClass::Unknown;

		union
		{
			TextureOffset2D uOff2D;
			TextureOffset2DArray uOff2DArray;
			TextureOffset3D uOff3D;
			TextureOffsetCubeMap uOffCubeMap;
		};

		TextureOffset() = default;

		TextureOffset( const TextureOffset2D & pOffset2D, ETextureClass pTexClass = ETextureClass::T2D )
		: mTexClass( pTexClass )
		, uOff2D( pOffset2D )
		{}

		TextureOffset( const TextureOffset2DArray & pOffset2DArray, ETextureClass pTexClass = ETextureClass::T2D )
		: mTexClass( pTexClass )
		, uOff2DArray( pOffset2DArray )
		{}

		TextureOffset( const TextureOffset3D & pOffset3D )
		: mTexClass( ETextureClass::T3D )
		, uOff3D( pOffset3D )
		{}

		TextureOffset( const TextureOffsetCubeMap & pOffsetCubeMap )
		: mTexClass( ETextureClass::TCubeMap )
		, uOffCubeMap( pOffsetCubeMap )
		{}

		explicit operator bool() const noexcept
		{
			return mTexClass != ETextureClass::Unknown;
		}
	};

	struct TextureSize2D
	{
		uint32 mWidth;
		uint32 mHeight;

		inline constexpr bool operator==( const TextureSize2D & pRhs ) const noexcept
		{
			return ( mWidth == pRhs.mWidth ) && ( mHeight == pRhs.mHeight );
		}

		inline constexpr bool operator!=( const TextureSize2D & pRhs ) const noexcept
		{
			return ( mWidth != pRhs.mWidth ) || ( mHeight != pRhs.mHeight );
		}
	};

	struct TextureSize2DArray : public TextureSize2D
	{
		uint32 mArraySize;

		inline constexpr bool operator==( const TextureSize2DArray & pRhs ) const noexcept
		{
			return ( mWidth == pRhs.mWidth ) && ( mHeight == pRhs.mHeight ) && ( mArraySize == pRhs.mArraySize );
		}

		inline constexpr bool operator!=( const TextureSize2DArray & pRhs ) const noexcept
		{
			return ( mWidth != pRhs.mWidth ) || ( mHeight != pRhs.mHeight ) || ( mArraySize != pRhs.mArraySize );
		}
	};

	struct TextureSize3D : public TextureSize2D
	{
		uint32 mDepth;

		inline constexpr bool operator==( const TextureSize3D & pRhs ) const noexcept
		{
			return ( mWidth == pRhs.mWidth ) && ( mHeight == pRhs.mHeight ) && (mDepth == pRhs.mDepth );
		}

		inline constexpr bool operator!=( const TextureSize3D & pRhs ) const noexcept
		{
			return ( mWidth != pRhs.mWidth ) || ( mHeight != pRhs.mHeight ) || (mDepth != pRhs.mDepth );
		}
	};

	struct TextureSizeCubeMap : public TextureSize2D
	{
		static constexpr uint32 facesNum = 6;
	};

	struct TextureSize
	{
		ETextureClass texClass = ETextureClass::Unknown;

		union
		{
			TextureSize2D uSz2D;
			TextureSize2DArray uSz2DArray;
			TextureSize3D uSz3D;
			TextureSizeCubeMap uSzCubeMap;
		};

		TextureSize() = default;

		TextureSize( const TextureSize2D & pSize2D, ETextureClass pTexClass = ETextureClass::T2D )
		: texClass( pTexClass )
		, uSz2D( pSize2D )
		{}

		TextureSize( const TextureSize2DArray & pSize2DArray, ETextureClass pTexClass = ETextureClass::T2DArray )
		: texClass( pTexClass )
		, uSz2DArray( pSize2DArray )
		{}

		TextureSize( const TextureSize3D & pSize3D )
		: texClass( ETextureClass::T3D )
		, uSz3D( pSize3D )
		{}

		TextureSize( const TextureSizeCubeMap & pSizeCubeMap )
		: texClass( ETextureClass::TCubeMap )
		, uSzCubeMap( pSizeCubeMap )
		{}

		explicit operator bool() const noexcept
		{
			return texClass != ETextureClass::Unknown;
		}
	};

	struct TextureSubRegion2D
	{
		TextureOffset2D mOffset;
		TextureSize2D mSize;
	};

	struct TextureSubRegion2DArray
	{
		TextureOffset2DArray mOffset;
		TextureSize2DArray mSize;
	};

	struct TextureSubRegion3D
	{
		TextureOffset3D mOffset;
		TextureSize3D mSize;
	};

	struct TextureSubRegionCubeMap
	{
		TextureOffsetCubeMap mOffset;
		TextureSizeCubeMap mSize;
	};

	struct TextureSubRegion
	{
		ETextureClass mTexClass = ETextureClass::Unknown;

		union
		{
			TextureSubRegion2D uSubReg2D;
			TextureSubRegion2DArray uSubReg2DArray;
			TextureSubRegion3D uSubReg3D;
			TextureSubRegionCubeMap uSubRegCubeMap;
		};

		TextureSubRegion() = default;

		TextureSubRegion( const TextureSubRegion2D & pSubReg2D, ETextureClass pTexClass = ETextureClass::T2D )
		: mTexClass( pTexClass )
		, uSubReg2D( pSubReg2D )
		{}

		TextureSubRegion( const TextureSubRegion2DArray & pSubReg2DArray, ETextureClass pTexClass = ETextureClass::T2D )
		: mTexClass( pTexClass )
		, uSubReg2DArray( pSubReg2DArray )
		{}

		TextureSubRegion( const TextureSubRegion3D & pSubReg3D )
		: mTexClass( ETextureClass::T3D )
		, uSubReg3D( pSubReg3D )
		{}

		TextureSubRegion( const TextureSubRegionCubeMap & pSubRegCubeMap )
		: mTexClass( ETextureClass::TCubeMap )
		, uSubRegCubeMap( pSubRegCubeMap )
		{}

		explicit operator bool() const noexcept
		{
			return mTexClass != ETextureClass::Unknown;
		}
	};

	struct TextureSubResource2D
	{
		/// A mip level index.
		uint32 mMipLevel;
	};

	struct TextureSubResource2DArray : public TextureSubResource2D
	{
		/// An index within a texture array. Use CxDef::TEX_SUBRESOURCE_INDEX_MIP_VERTICAL_SLICE for a vertical mip slice.
		uint32 mArrayIndex;
	};

	struct TextureSubResource3D : public TextureSubResource2D
	{
		///
		uint32 mDepthLayerIndex;
	};

	struct TextureSubResourceCubeMap : public TextureSubResource2D
	{
		/// An index of the cube map face. Use CxDef::TEX_SUBRESOURCE_INDEX_MIP_VERTICAL_SLICE for a vertical mip slice.
		uint32 mFaceIndex;
	};

	struct TextureSubResource
	{
		ETextureClass mTexClass = ETextureClass::Unknown;

		union
		{
			TextureSubResource2D uSubRes2D;
			TextureSubResource2DArray uSubRes2DArray;
			TextureSubResource3D uSubRes3D;
			TextureSubResourceCubeMap uSubResCubeMap;
		};

		TextureSubResource() = default;

		TextureSubResource( const TextureSubResource2D & pSubRes2D, ETextureClass pTexClass = ETextureClass::T2D )
		: mTexClass( pTexClass )
		, uSubRes2D( pSubRes2D )
		{}

		TextureSubResource( const TextureSubResource2DArray & pSubRes2DArray, ETextureClass pTexClass = ETextureClass::T2DArray )
		: mTexClass( pTexClass )
		, uSubRes2DArray( pSubRes2DArray )
		{}

		TextureSubResource( const TextureSubResource3D & pSubRes3D )
		: mTexClass( ETextureClass::T3D )
		, uSubRes3D( pSubRes3D )
		{}

		TextureSubResource( const TextureSubResourceCubeMap & pSubResCubeMap )
		: mTexClass( ETextureClass::TCubeMap )
		, uSubResCubeMap( pSubResCubeMap )
		{}

		explicit operator bool() const noexcept
		{
			return mTexClass != ETextureClass::Unknown;
		}

		void reset()
		{
			mTexClass = ETextureClass::Unknown;
		}
	};

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD TextureDimensions queryMipLevelDimensions(
				const TextureDimensions & pDimensions,
				uint32 pMipLevel ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD TextureSubResource getTextureSubResourceWholeTexture( TextureHandle pTexture ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubRegion(
				TextureHandle pTexture,
				const TextureSubRegion & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubRegion(
				const TextureDimensions & pDimensions,
				const TextureSubRegion & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubRegion2D(
				const TextureDimensions & pDimensions,
				const TextureSubRegion2D & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubRegion2DArray(
				const TextureDimensions & pDimensions,
				const TextureSubRegion2DArray & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubRegion2DMS(
				const TextureDimensions & pDimensions,
				const TextureSubRegion2D & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubRegion3D(
				const TextureDimensions & pDimensions,
				const TextureSubRegion3D & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubRegionCubeMap(
				const TextureDimensions & pDimensions,
				const TextureSubRegionCubeMap & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubResource(
				TextureHandle pTexture,
				const TextureSubResource & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubResource(
				const TextureDimensions & pDimensions,
				const TextureSubResource & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubResource2D(
				const TextureDimensions & pDimensions,
				const TextureSubResource2D & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubResource2DArray(
				const TextureDimensions & pDimensions,
				const TextureSubResource2DArray & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubResource2DMS(
				const TextureDimensions & pDimensions,
				const TextureSubResource2D & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubResource3D(
				const TextureDimensions & pDimensions,
				const TextureSubResource3D & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureSubResourceCubeMap(
				const TextureDimensions & pDimensions,
				const TextureSubResourceCubeMap & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubRegion(
				const TextureSubRegion & pSubRegion1,
				const TextureSubRegion & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubRegion2D(
				const TextureSubRegion2D & pSubRegion1,
				const TextureSubRegion2D & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubRegion2DArray(
				const TextureSubRegion2DArray & pSubRegion1,
				const TextureSubRegion2DArray & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubRegion2DMS(
				const TextureSubRegion2D & pSubRegion1,
				const TextureSubRegion2D & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubRegion3D(
				const TextureSubRegion3D & pSubRegion1,
				const TextureSubRegion3D & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubRegionCubeMap(
				const TextureSubRegionCubeMap & pSubRegion1,
				const TextureSubRegionCubeMap & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubResource(
				const TextureSubResource & pSubResource1,
				const TextureSubResource & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubResource2D(
				const TextureSubResource2D & pSubResource1,
				const TextureSubResource2D & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubResource2DArray(
				const TextureSubResource2DArray & pSubResource1,
				const TextureSubResource2DArray & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubResource2DMS(
				const TextureSubResource2D & pSubResource1,
				const TextureSubResource2D & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubResource3D(
				const TextureSubResource3D & pSubResource1,
				const TextureSubResource3D & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool cmpEqTextureSubResourceCubeMap(
				const TextureSubResourceCubeMap & pSubResource1,
				const TextureSubResourceCubeMap & pSubResource2 ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_TEXTURE_DIMENSIONS_H__
