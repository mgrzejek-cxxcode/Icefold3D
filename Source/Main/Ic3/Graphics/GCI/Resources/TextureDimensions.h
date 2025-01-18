
#pragma once

#ifndef __IC3_GRAPHICS_GCI_TEXTURE_DIMENSIONS_H__
#define __IC3_GRAPHICS_GCI_TEXTURE_DIMENSIONS_H__

#include "CommonGPUResourceDefs.h"

namespace Ic3::Graphics::GCI
{

	/// @brief A special value for texture array index indicating all resources in an array.
	/// For a specific mip level, it references a vertical mip slice for a texture array.
	constexpr auto cxTexSubresourceArrayIndexAllTextures = cppx::meta::limits<uint32>::max_value;

	/// @brief
	constexpr auto cxTexSubresourceCubeMapFaceAllFaces = cppx::meta::limits<uint32>::max_value;

	/// @brief
	constexpr auto cxTexSubresourceDepthAllLayers = cppx::meta::limits<uint32>::max_value;

	/// @brief A special value for mip level index indicating all levels a texture contains.
	/// For a given texture, it references all sub-resources in that texture..
	constexpr auto cxTexSubresourceMipLevelAllMips = cppx::meta::limits<uint32>::max_value;

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

	/// @brief List of valid face identifiers for cube Map textures.
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
		uint32 width;
		/// Height of a single image/layer within the texture.
		uint32 height;
		/// Depth of the texture. Used only for T3D textures. For other classes this is always 1..
		uint32 depth;
		/// Size of the texture array. Used for T2DArray and T2DMSArray textures. For other classes, this is always 1.
		uint32 arraySize;
		/// Number of mip levels within a single texture image/layer. Valid values are from 1 to GCM::cxTextureMaxMipLevelsNum.
		uint32 mipLevelsNum;
	};

	struct TextureOffset2D
	{
		uint32 mipLevel;
		uint32 x;
		uint32 y;

		inline constexpr bool operator==( const TextureOffset2D & pRhs ) const noexcept
		{
			return ( mipLevel == pRhs.mipLevel ) && ( x == pRhs.x ) && ( y == pRhs.y );
		}

		inline constexpr bool operator!=( const TextureOffset2D & pRhs ) const noexcept
		{
			return ( mipLevel != pRhs.mipLevel ) || ( x != pRhs.x ) || ( y != pRhs.y );
		}
	};

	struct TextureOffset2DArray : public TextureOffset2D
	{
		uint32 arrayIndex;

		inline constexpr bool operator==( const TextureOffset2DArray & pRhs ) const noexcept
		{
			return ( mipLevel == pRhs.mipLevel ) && ( x == pRhs.x ) && ( y == pRhs.y ) && ( arrayIndex == pRhs.arrayIndex );
		}

		inline constexpr bool operator!=( const TextureOffset2DArray & pRhs ) const noexcept
		{
			return ( mipLevel != pRhs.mipLevel ) || ( x != pRhs.x ) || ( y != pRhs.y ) || ( arrayIndex != pRhs.arrayIndex );
		}
	};

	struct TextureOffset3D : public TextureOffset2D
	{
		uint32 z;

		inline constexpr bool operator==( const TextureOffset3D & pRhs ) const noexcept
		{
			return ( mipLevel == pRhs.mipLevel ) && ( x == pRhs.x ) && ( y == pRhs.y ) && ( z == pRhs.z );
		}

		inline constexpr bool operator!=( const TextureOffset3D & pRhs ) const noexcept
		{
			return ( mipLevel != pRhs.mipLevel ) || ( x != pRhs.x ) || ( y != pRhs.y ) || ( z != pRhs.z );
		}
	};

	struct TextureOffsetCubeMap : public TextureOffset2D
	{
		uint32 faceIndex;

		inline constexpr bool operator==( const TextureOffsetCubeMap & pRhs ) const noexcept
		{
			return ( faceIndex == pRhs.faceIndex ) && ( mipLevel == pRhs.mipLevel ) && ( x == pRhs.x ) && ( y == pRhs.y );
		}

		inline constexpr bool operator!=( const TextureOffsetCubeMap & pRhs ) const noexcept
		{
			return ( faceIndex != pRhs.faceIndex ) || ( mipLevel != pRhs.mipLevel ) || ( x != pRhs.x ) || ( y != pRhs.y );
		}
	};

	struct TextureOffset
	{
		ETextureClass texClass = ETextureClass::Unknown;

		union
		{
			TextureOffset2D uOff2D;
			TextureOffset2DArray uOff2DArray;
			TextureOffset3D uOff3D;
			TextureOffsetCubeMap uOffCubeMap;
		};

		TextureOffset() = default;

		TextureOffset( const TextureOffset2D & pOffset2D, ETextureClass pTexClass = ETextureClass::T2D )
		: texClass( pTexClass )
		, uOff2D( pOffset2D )
		{}

		TextureOffset( const TextureOffset2DArray & pOffset2DArray, ETextureClass pTexClass = ETextureClass::T2D )
		: texClass( pTexClass )
		, uOff2DArray( pOffset2DArray )
		{}

		TextureOffset( const TextureOffset3D & pOffset3D )
		: texClass( ETextureClass::T3D )
		, uOff3D( pOffset3D )
		{}

		TextureOffset( const TextureOffsetCubeMap & pOffsetCubeMap )
		: texClass( ETextureClass::TCubeMap )
		, uOffCubeMap( pOffsetCubeMap )
		{}

		explicit operator bool() const noexcept
		{
			return texClass != ETextureClass::Unknown;
		}
	};

	struct TextureSize2D
	{
		uint32 width;
		uint32 height;

		inline constexpr bool operator==( const TextureSize2D & pRhs ) const noexcept
		{
			return ( width == pRhs.width ) && ( height == pRhs.height );
		}

		inline constexpr bool operator!=( const TextureSize2D & pRhs ) const noexcept
		{
			return ( width != pRhs.width ) || ( height != pRhs.height );
		}
	};

	struct TextureSize2DArray : public TextureSize2D
	{
		uint32 arraySize;

		inline constexpr bool operator==( const TextureSize2DArray & pRhs ) const noexcept
		{
			return ( width == pRhs.width ) && ( height == pRhs.height ) && ( arraySize == pRhs.arraySize );
		}

		inline constexpr bool operator!=( const TextureSize2DArray & pRhs ) const noexcept
		{
			return ( width != pRhs.width ) || ( height != pRhs.height ) || ( arraySize != pRhs.arraySize );
		}
	};

	struct TextureSize3D : public TextureSize2D
	{
		uint32 depth;

		inline constexpr bool operator==( const TextureSize3D & pRhs ) const noexcept
		{
			return ( width == pRhs.width ) && ( height == pRhs.height ) && (depth == pRhs.depth );
		}

		inline constexpr bool operator!=( const TextureSize3D & pRhs ) const noexcept
		{
			return ( width != pRhs.width ) || ( height != pRhs.height ) || (depth != pRhs.depth );
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
		TextureOffset2D offset;
		TextureSize2D size;
	};

	struct TextureSubRegion2DArray
	{
		TextureOffset2DArray offset;
		TextureSize2DArray size;
	};

	struct TextureSubRegion3D
	{
		TextureOffset3D offset;
		TextureSize3D size;
	};

	struct TextureSubRegionCubeMap
	{
		TextureOffsetCubeMap offset;
		TextureSizeCubeMap size;
	};

	struct TextureSubRegion
	{
		ETextureClass texClass = ETextureClass::Unknown;

		union
		{
			TextureSubRegion2D uSubReg2D;
			TextureSubRegion2DArray uSubReg2DArray;
			TextureSubRegion3D uSubReg3D;
			TextureSubRegionCubeMap uSubRegCubeMap;
		};

		TextureSubRegion() = default;

		TextureSubRegion( const TextureSubRegion2D & pSubReg2D, ETextureClass pTexClass = ETextureClass::T2D )
		: texClass( pTexClass )
		, uSubReg2D( pSubReg2D )
		{}

		TextureSubRegion( const TextureSubRegion2DArray & pSubReg2DArray, ETextureClass pTexClass = ETextureClass::T2D )
		: texClass( pTexClass )
		, uSubReg2DArray( pSubReg2DArray )
		{}

		TextureSubRegion( const TextureSubRegion3D & pSubReg3D )
		: texClass( ETextureClass::T3D )
		, uSubReg3D( pSubReg3D )
		{}

		TextureSubRegion( const TextureSubRegionCubeMap & pSubRegCubeMap )
		: texClass( ETextureClass::TCubeMap )
		, uSubRegCubeMap( pSubRegCubeMap )
		{}

		explicit operator bool() const noexcept
		{
			return texClass != ETextureClass::Unknown;
		}
	};

	struct TextureSubResource2D
	{
		/// A mip level index.
		uint32 mipLevel;
	};

	struct TextureSubResource2DArray : public TextureSubResource2D
	{
		/// An index within a texture array. Use CxDef::TEX_SUBRESOURCE_INDEX_MIP_VERTICAL_SLICE for a vertical mip slice.
		uint32 arrayIndex;
	};

	struct TextureSubResource3D : public TextureSubResource2D
	{
		///
		uint32 depthLayerIndex;
	};

	struct TextureSubResourceCubeMap : public TextureSubResource2D
	{
		/// An index of the cube Map face. Use CxDef::TEX_SUBRESOURCE_INDEX_MIP_VERTICAL_SLICE for a vertical mip slice.
		uint32 faceIndex;
	};

	struct TextureSubResource
	{
		ETextureClass texClass = ETextureClass::Unknown;

		union
		{
			TextureSubResource2D uSubRes2D;
			TextureSubResource2DArray uSubRes2DArray;
			TextureSubResource3D uSubRes3D;
			TextureSubResourceCubeMap uSubResCubeMap;
		};

		TextureSubResource() = default;

		TextureSubResource( const TextureSubResource2D & pSubRes2D, ETextureClass pTexClass = ETextureClass::T2D )
		: texClass( pTexClass )
		, uSubRes2D( pSubRes2D )
		{}

		TextureSubResource( const TextureSubResource2DArray & pSubRes2DArray, ETextureClass pTexClass = ETextureClass::T2DArray )
		: texClass( pTexClass )
		, uSubRes2DArray( pSubRes2DArray )
		{}

		TextureSubResource( const TextureSubResource3D & pSubRes3D )
		: texClass( ETextureClass::T3D )
		, uSubRes3D( pSubRes3D )
		{}

		TextureSubResource( const TextureSubResourceCubeMap & pSubResCubeMap )
		: texClass( ETextureClass::TCubeMap )
		, uSubResCubeMap( pSubResCubeMap )
		{}

		explicit operator bool() const noexcept
		{
			return texClass != ETextureClass::Unknown;
		}

		void reset()
		{
			texClass = ETextureClass::Unknown;
		}
	};

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD TextureDimensions QueryMipLevelDimensions(
				const TextureDimensions & pDimensions,
				uint32 pMipLevel ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD TextureSubResource GetTextureSubResourceWholeTexture( TextureHandle pTexture ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubRegion(
				TextureHandle pTexture,
				const TextureSubRegion & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubRegion(
				const TextureDimensions & pDimensions,
				const TextureSubRegion & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubRegion2D(
				const TextureDimensions & pDimensions,
				const TextureSubRegion2D & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubRegion2DArray(
				const TextureDimensions & pDimensions,
				const TextureSubRegion2DArray & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubRegion2DMS(
				const TextureDimensions & pDimensions,
				const TextureSubRegion2D & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubRegion3D(
				const TextureDimensions & pDimensions,
				const TextureSubRegion3D & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubRegionCubeMap(
				const TextureDimensions & pDimensions,
				const TextureSubRegionCubeMap & pSubRegion ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubResource(
				TextureHandle pTexture,
				const TextureSubResource & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubResource(
				const TextureDimensions & pDimensions,
				const TextureSubResource & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubResource2D(
				const TextureDimensions & pDimensions,
				const TextureSubResource2D & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubResource2DArray(
				const TextureDimensions & pDimensions,
				const TextureSubResource2DArray & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubResource2DMS(
				const TextureDimensions & pDimensions,
				const TextureSubResource2D & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubResource3D(
				const TextureDimensions & pDimensions,
				const TextureSubResource3D & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureSubResourceCubeMap(
				const TextureDimensions & pDimensions,
				const TextureSubResourceCubeMap & pSubResource ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubRegion(
				const TextureSubRegion & pSubRegion1,
				const TextureSubRegion & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubRegion2D(
				const TextureSubRegion2D & pSubRegion1,
				const TextureSubRegion2D & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubRegion2DArray(
				const TextureSubRegion2DArray & pSubRegion1,
				const TextureSubRegion2DArray & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubRegion2DMS(
				const TextureSubRegion2D & pSubRegion1,
				const TextureSubRegion2D & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubRegion3D(
				const TextureSubRegion3D & pSubRegion1,
				const TextureSubRegion3D & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubRegionCubeMap(
				const TextureSubRegionCubeMap & pSubRegion1,
				const TextureSubRegionCubeMap & pSubRegion2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubResource(
				const TextureSubResource & pSubResource1,
				const TextureSubResource & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubResource2D(
				const TextureSubResource2D & pSubResource1,
				const TextureSubResource2D & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubResource2DArray(
				const TextureSubResource2DArray & pSubResource1,
				const TextureSubResource2DArray & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubResource2DMS(
				const TextureSubResource2D & pSubResource1,
				const TextureSubResource2D & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubResource3D(
				const TextureSubResource3D & pSubResource1,
				const TextureSubResource3D & pSubResource2 ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CmpEqTextureSubResourceCubeMap(
				const TextureSubResourceCubeMap & pSubResource1,
				const TextureSubResourceCubeMap & pSubResource2 ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_TEXTURE_DIMENSIONS_H__
