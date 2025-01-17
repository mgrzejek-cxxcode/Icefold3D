
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DATA_FORMATS_H__
#define __IC3_GRAPHICS_GCI_GPU_DATA_FORMATS_H__

#include <Ic3/CoreLib/PixelCommon.h>

namespace Ic3::Graphics::GCI
{

	using base_data_type_value_t = uint16;
	using texture_format_value_t = uint64;
	using vertex_attrib_format_value_t = uint32;

	enum class EBaseDataType : base_data_type_value_t;

	enum EGpuDataFormatFlags : uint8
	{
		eGpuDataFormatFlagNormalizedBit   = 0x01,
		eGpuDataFormatFlagTypeSignedBit   = 0x02,
		eGpuDataFormatFlagTypeUnsignedBit = 0x04,
		eGpuDataFormatFlagSRGBBit         = 0x08,
		eGpuDataFormatFlagDepthBit        = 0x10,
		eGpuDataFormatFlagStencilBit      = 0x20,
		eGpuDataFormatFlagDepthStencilBit = eGpuDataFormatFlagDepthBit | eGpuDataFormatFlagStencilBit,
		eGpuDataFormatFlagCompressedBit   = 0x80,
		eGpuDataFormatMaskDepthNorm       = eGpuDataFormatFlagDepthBit | eGpuDataFormatFlagNormalizedBit,
		eGpuDataFormatMaskSNorm           = eGpuDataFormatFlagNormalizedBit | eGpuDataFormatFlagTypeSignedBit,
		eGpuDataFormatMaskUNorm           = eGpuDataFormatFlagNormalizedBit | eGpuDataFormatFlagTypeUnsignedBit,
		eGpuDataFormatMaskCompressedSRGB  = eGpuDataFormatFlagCompressedBit | eGpuDataFormatFlagSRGBBit,
		eGpuDataFormatMaskAll             = 0xFF
	};

	namespace CxDef
	{

		///
		constexpr uint8 VBM_ATTRIB_FMT_CONTROL_KEY = 0xAF;

		/**
		 *
		 * @param pIdx
		 * @param pBsz
		 * @param pFlg
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr base_data_type_value_t makeBaseDataTypeEnumValue(
				uint8 pIdx, uint8 pBsz, uint8 pFlg = 0 ) noexcept
		{
			// Note: EVertexAttribFormat (and related functionality) relies on the fact that last 8 bits are flags (this
			// is shared with other enums to fit into certain size limits). Plan carefully before changing the bit layout.
			return ( ( ( uint16 )pBsz & 0xF ) << 12 ) | ( ( ( uint16 )pIdx & 0xF ) << 8 ) | ( pFlg & eGpuDataFormatMaskAll );
		}

		/**
		 *
		 * @param pIdx
		 * @param pPix
		 * @param pBdt
		 * @param pBsz
		 * @param pFlg
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr texture_format_value_t makeTextureDataFormatEnumValue(
				uint8 pIdx, EPixelDataLayout pPix, EBaseDataType pBdt, uint8 pBsz, uint8 pFlg = 0 ) noexcept
		{
			return ( ( ( uint64 )pPix ) << 32 ) | ( ( ( uint64 )pBsz ) << 24 ) | ( ( ( uint64 )pBdt ) << 8 ) | ( ( ( uint64 )pFlg  & eGpuDataFormatMaskAll ) << 8 ) | ( pIdx );
		}

		/**
		 *
		 * @param pBcn
		 * @param pBdt
		 * @param pFlg
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr vertex_attrib_format_value_t makeVertexAttribFormatEnumValue(
				uint8 pBcn, EBaseDataType pBdt, uint8 pFlg = 0 ) noexcept
		{
			return ( ( uint32 )VBM_ATTRIB_FMT_CONTROL_KEY << 24 ) | ( ( ( uint32 )pBcn ) << 16 ) | ( ( uint32 )pBdt ) | ( ( uint32 )pFlg & eGpuDataFormatMaskAll );
		}

	}

	enum class EBaseDataType : base_data_type_value_t
	{
		Undefined    = CxDef::makeBaseDataTypeEnumValue( 0 , 0 , 0 ),
		Byte         = CxDef::makeBaseDataTypeEnumValue( 1 , 1 , eGpuDataFormatFlagTypeSignedBit   ),
		Ubyte        = CxDef::makeBaseDataTypeEnumValue( 2 , 1 , eGpuDataFormatFlagTypeUnsignedBit ),
		Int16        = CxDef::makeBaseDataTypeEnumValue( 3 , 2 , eGpuDataFormatFlagTypeSignedBit   ),
		Uint16       = CxDef::makeBaseDataTypeEnumValue( 4 , 2 , eGpuDataFormatFlagTypeUnsignedBit ),
		Int32        = CxDef::makeBaseDataTypeEnumValue( 5 , 4 , eGpuDataFormatFlagTypeSignedBit   ),
		Uint32       = CxDef::makeBaseDataTypeEnumValue( 6 , 4 , eGpuDataFormatFlagTypeUnsignedBit ),
		Float16      = CxDef::makeBaseDataTypeEnumValue( 7 , 2 , eGpuDataFormatFlagTypeSignedBit   ),
		Float32      = CxDef::makeBaseDataTypeEnumValue( 8 , 4 , eGpuDataFormatFlagTypeSignedBit   ),
		Uint24S8     = CxDef::makeBaseDataTypeEnumValue( 9 , 4 , eGpuDataFormatFlagTypeUnsignedBit ),
	};

	enum class EIndexDataFormat : base_data_type_value_t
	{
		Undefined = 0,
		Uint16 = static_cast<base_data_type_value_t>( EBaseDataType::Uint16 ),
		Uint32 = static_cast<base_data_type_value_t>( EBaseDataType::Uint32 ),
	};

	enum class ETextureFormat : texture_format_value_t
	{
		Undefined  = 0,

		R32F       = CxDef::makeTextureDataFormatEnumValue(  0 , EPixelDataLayout::Red  , EBaseDataType::Float32 , 4  ),
		R32I       = CxDef::makeTextureDataFormatEnumValue(  1 , EPixelDataLayout::Red  , EBaseDataType::Int32   , 4  ),
		R32U       = CxDef::makeTextureDataFormatEnumValue(  2 , EPixelDataLayout::Red  , EBaseDataType::Uint32  , 4  ),
		RG32F      = CxDef::makeTextureDataFormatEnumValue(  3 , EPixelDataLayout::RG   , EBaseDataType::Float32 , 8  ),
		RG32I      = CxDef::makeTextureDataFormatEnumValue(  4 , EPixelDataLayout::RG   , EBaseDataType::Int32   , 8  ),
		RG32U      = CxDef::makeTextureDataFormatEnumValue(  5 , EPixelDataLayout::RG   , EBaseDataType::Uint32  , 8  ),
		RGB32F     = CxDef::makeTextureDataFormatEnumValue(  6 , EPixelDataLayout::RGB  , EBaseDataType::Float32 , 12 ),
		RGB32I     = CxDef::makeTextureDataFormatEnumValue(  7 , EPixelDataLayout::RGB  , EBaseDataType::Int32   , 12 ),
		RGB32U     = CxDef::makeTextureDataFormatEnumValue(  8 , EPixelDataLayout::RGB  , EBaseDataType::Uint32  , 12 ),
		RGBA32F    = CxDef::makeTextureDataFormatEnumValue(  9 , EPixelDataLayout::RGBA , EBaseDataType::Float32 , 16 ),
		RGBA32I    = CxDef::makeTextureDataFormatEnumValue( 10 , EPixelDataLayout::RGBA , EBaseDataType::Int32   , 16 ),
		RGBA32U    = CxDef::makeTextureDataFormatEnumValue( 11 , EPixelDataLayout::RGBA , EBaseDataType::Uint32  , 16 ),
		R16F       = CxDef::makeTextureDataFormatEnumValue( 12 , EPixelDataLayout::RG   , EBaseDataType::Float16 , 2  ),
		R16I       = CxDef::makeTextureDataFormatEnumValue( 13 , EPixelDataLayout::RG   , EBaseDataType::Int16   , 2  ),
		R16U       = CxDef::makeTextureDataFormatEnumValue( 14 , EPixelDataLayout::RG   , EBaseDataType::Uint16  , 2  ),
		RG16F      = CxDef::makeTextureDataFormatEnumValue( 15 , EPixelDataLayout::RG   , EBaseDataType::Float16 , 4  ),
		RG16I      = CxDef::makeTextureDataFormatEnumValue( 16 , EPixelDataLayout::RG   , EBaseDataType::Int16   , 4  ),
		RG16U      = CxDef::makeTextureDataFormatEnumValue( 17 , EPixelDataLayout::RG   , EBaseDataType::Uint16  , 4  ),
		RGBA16F    = CxDef::makeTextureDataFormatEnumValue( 18 , EPixelDataLayout::RGBA , EBaseDataType::Float16 , 8  ),
		RGBA16I    = CxDef::makeTextureDataFormatEnumValue( 19 , EPixelDataLayout::RGBA , EBaseDataType::Int16   , 8  ),
		RGBA16U    = CxDef::makeTextureDataFormatEnumValue( 20 , EPixelDataLayout::RGBA , EBaseDataType::Uint16  , 8  ),

		R8I        = CxDef::makeTextureDataFormatEnumValue( 21 , EPixelDataLayout::Red  , EBaseDataType::Byte  , 1 ),
		R8U        = CxDef::makeTextureDataFormatEnumValue( 22 , EPixelDataLayout::Red  , EBaseDataType::Ubyte , 1 ),
		R8IN       = CxDef::makeTextureDataFormatEnumValue( 23 , EPixelDataLayout::Red  , EBaseDataType::Byte  , 1 , eGpuDataFormatFlagNormalizedBit ),
		R8UN       = CxDef::makeTextureDataFormatEnumValue( 24 , EPixelDataLayout::Red  , EBaseDataType::Ubyte , 1 , eGpuDataFormatFlagNormalizedBit ),
		RG8I       = CxDef::makeTextureDataFormatEnumValue( 25 , EPixelDataLayout::RG   , EBaseDataType::Byte  , 2 ),
		RG8U       = CxDef::makeTextureDataFormatEnumValue( 26 , EPixelDataLayout::RG   , EBaseDataType::Ubyte , 2 ),
		RG8IN      = CxDef::makeTextureDataFormatEnumValue( 27 , EPixelDataLayout::RG   , EBaseDataType::Byte  , 2 , eGpuDataFormatFlagNormalizedBit ),
		RG8UN      = CxDef::makeTextureDataFormatEnumValue( 28 , EPixelDataLayout::RG   , EBaseDataType::Ubyte , 2 , eGpuDataFormatFlagNormalizedBit ),
		BGRX8UN    = CxDef::makeTextureDataFormatEnumValue( 29 , EPixelDataLayout::RGBA , EBaseDataType::Ubyte , 4 , eGpuDataFormatFlagNormalizedBit ),
		BGRX8SRGB  = CxDef::makeTextureDataFormatEnumValue( 30 , EPixelDataLayout::RGBA , EBaseDataType::Ubyte , 4 , eGpuDataFormatFlagSRGBBit       ),
		BGRA8UN    = CxDef::makeTextureDataFormatEnumValue( 31 , EPixelDataLayout::RGBA , EBaseDataType::Ubyte , 4 , eGpuDataFormatFlagNormalizedBit ),
		BGRA8SRGB  = CxDef::makeTextureDataFormatEnumValue( 32 , EPixelDataLayout::RGBA , EBaseDataType::Ubyte , 4 , eGpuDataFormatFlagSRGBBit       ),
		RGBA8I     = CxDef::makeTextureDataFormatEnumValue( 33 , EPixelDataLayout::RGBA , EBaseDataType::Byte  , 4 ),
		RGBA8U     = CxDef::makeTextureDataFormatEnumValue( 34 , EPixelDataLayout::RGBA , EBaseDataType::Ubyte , 4 ),
		RGBA8IN    = CxDef::makeTextureDataFormatEnumValue( 35 , EPixelDataLayout::RGBA , EBaseDataType::Byte  , 4 , eGpuDataFormatFlagNormalizedBit ),
		RGBA8UN    = CxDef::makeTextureDataFormatEnumValue( 36 , EPixelDataLayout::RGBA , EBaseDataType::Ubyte , 4 , eGpuDataFormatFlagNormalizedBit ),
		RGBA8SRGB  = CxDef::makeTextureDataFormatEnumValue( 37 , EPixelDataLayout::RGBA , EBaseDataType::Ubyte , 4 , eGpuDataFormatFlagSRGBBit ),

		R5G5B5A1   = CxDef::makeTextureDataFormatEnumValue( 38 , EPixelDataLayout::RGBA , EBaseDataType::Ubyte   , 2 ),
		R5G6B5     = CxDef::makeTextureDataFormatEnumValue( 39 , EPixelDataLayout::RGB  , EBaseDataType::Ubyte   , 2 ),
		R9G9B9E5   = CxDef::makeTextureDataFormatEnumValue( 40 , EPixelDataLayout::RGBA , EBaseDataType::Uint32  , 4 ),
		RGB10A2U   = CxDef::makeTextureDataFormatEnumValue( 41 , EPixelDataLayout::RGBA , EBaseDataType::Uint32  , 4 ),
		RGB10A2UN  = CxDef::makeTextureDataFormatEnumValue( 42 , EPixelDataLayout::RGBA , EBaseDataType::Float32 , 4 , eGpuDataFormatFlagNormalizedBit ),
		R11G11B10F = CxDef::makeTextureDataFormatEnumValue( 43 , EPixelDataLayout::RGB  , EBaseDataType::Float32 , 4 ),

		D16UN      = CxDef::makeTextureDataFormatEnumValue( 44 , EPixelDataLayout::Depth , EBaseDataType::Uint16   , 2 , eGpuDataFormatMaskDepthNorm       ),
		D24UNS8U   = CxDef::makeTextureDataFormatEnumValue( 45 , EPixelDataLayout::DS    , EBaseDataType::Uint24S8 , 4 , eGpuDataFormatFlagDepthStencilBit ),
		D24UNX8    = CxDef::makeTextureDataFormatEnumValue( 46 , EPixelDataLayout::DS    , EBaseDataType::Uint24S8 , 4 , eGpuDataFormatFlagDepthBit        ),
		X24S8U     = CxDef::makeTextureDataFormatEnumValue( 47 , EPixelDataLayout::DS    , EBaseDataType::Uint24S8 , 4 , eGpuDataFormatFlagStencilBit      ),
		D32F       = CxDef::makeTextureDataFormatEnumValue( 48 , EPixelDataLayout::DS    , EBaseDataType::Float32  , 4 , eGpuDataFormatFlagDepthBit        ),

		BC1        = CxDef::makeTextureDataFormatEnumValue( 49 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC1SRGB    = CxDef::makeTextureDataFormatEnumValue( 50 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatMaskCompressedSRGB ),
		BC2        = CxDef::makeTextureDataFormatEnumValue( 51 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC2SRGB    = CxDef::makeTextureDataFormatEnumValue( 52 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatMaskCompressedSRGB ),
		BC3        = CxDef::makeTextureDataFormatEnumValue( 53 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC3SRGB    = CxDef::makeTextureDataFormatEnumValue( 54 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatMaskCompressedSRGB ),
		BC4IN      = CxDef::makeTextureDataFormatEnumValue( 55 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC4UN      = CxDef::makeTextureDataFormatEnumValue( 56 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC5IN      = CxDef::makeTextureDataFormatEnumValue( 57 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC5UN      = CxDef::makeTextureDataFormatEnumValue( 58 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC6HSF     = CxDef::makeTextureDataFormatEnumValue( 59 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC6HUF     = CxDef::makeTextureDataFormatEnumValue( 60 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC7        = CxDef::makeTextureDataFormatEnumValue( 61 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatFlagCompressedBit  ),
		BC7SRGB    = CxDef::makeTextureDataFormatEnumValue( 62 , EPixelDataLayout::S3TC ,  EBaseDataType::Undefined , 0 , eGpuDataFormatMaskCompressedSRGB ),
	};

	enum class EVertexAttribFormat : vertex_attrib_format_value_t
	{
		Undefined = 0,

		F16       = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Float16 ),
		F32       = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Float32 ),
		I8        = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Byte    ),
		I16       = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Int16   ),
		I32       = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Int32   ),
		U8        = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Ubyte   ),
		U16       = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Uint16  ),
		U32       = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Uint32  ),
		I8N       = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Byte    , eGpuDataFormatFlagNormalizedBit ),
		I16N      = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Int16   , eGpuDataFormatFlagNormalizedBit ),
		U8N       = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Ubyte   , eGpuDataFormatFlagNormalizedBit ),
		U16N      = CxDef::makeVertexAttribFormatEnumValue( 1 , EBaseDataType::Uint16  , eGpuDataFormatFlagNormalizedBit ),
		Vec2F16   = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Float16 ),
		Vec2F32   = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Float32 ),
		Vec2I8    = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Byte    ),
		Vec2I16   = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Int16   ),
		Vec2I32   = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Int32   ),
		Vec2U8    = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Ubyte   ),
		Vec2U16   = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Uint16  ),
		Vec2U32   = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Uint32  ),
		Vec2I8N   = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Byte    , eGpuDataFormatFlagNormalizedBit ),
		Vec2I16N  = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Int16   , eGpuDataFormatFlagNormalizedBit ),
		Vec2U8N   = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Ubyte   , eGpuDataFormatFlagNormalizedBit ),
		Vec2U16N  = CxDef::makeVertexAttribFormatEnumValue( 2 , EBaseDataType::Uint16  , eGpuDataFormatFlagNormalizedBit ),
		Vec3F32   = CxDef::makeVertexAttribFormatEnumValue( 3 , EBaseDataType::Float32 ),
		Vec3I32   = CxDef::makeVertexAttribFormatEnumValue( 3 , EBaseDataType::Int32   ),
		Vec3U32   = CxDef::makeVertexAttribFormatEnumValue( 3 , EBaseDataType::Uint32  ),
		Vec4F16   = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Float16 ),
		Vec4F32   = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Float32 ),
		Vec4I8    = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Byte    ),
		Vec4I16   = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Int16   ),
		Vec4I32   = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Int32   ),
		Vec4U8    = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Ubyte   ),
		Vec4U16   = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Uint16  ),
		Vec4U32   = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Uint32  ),
		Vec4I8N   = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Byte    , eGpuDataFormatFlagNormalizedBit ),
		Vec4I16N  = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Int16   , eGpuDataFormatFlagNormalizedBit ),
		Vec4U8N   = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Ubyte   , eGpuDataFormatFlagNormalizedBit ),
		Vec4U16N  = CxDef::makeVertexAttribFormatEnumValue( 4 , EBaseDataType::Uint16  , eGpuDataFormatFlagNormalizedBit ),
	};

	using EShaderInputConstantFormat = EVertexAttribFormat;

	namespace CxDef
	{

		CPPX_ATTR_NO_DISCARD inline constexpr uint16 GetBaseDataTypeByteSize( EBaseDataType pBaseType ) noexcept
		{
			return ( uint16 )( ( ( base_data_type_value_t )pBaseType >> 12 ) & 0xF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr cppx::bitmask<EGpuDataFormatFlags> GetBaseDataTypeFlags( EBaseDataType pBaseType ) noexcept
		{
			return cppx::make_bitmask<uint8>( ( base_data_type_value_t )pBaseType & 0xF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr uint16 GetIndexDataFormatByteSize( EIndexDataFormat pFormat ) noexcept
		{
			return GetBaseDataTypeByteSize( static_cast<EBaseDataType>( pFormat ) );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr EPixelDataLayout GetTextureFormatPixelDataLayout( ETextureFormat pFormat ) noexcept
		{
			return ( EPixelDataLayout )( ( ( texture_format_value_t )pFormat >> 32 ) & 0xFFFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr uint8 GetTextureFormatByteSize( ETextureFormat pFormat ) noexcept
		{
			return ( uint8 )( ( ( texture_format_value_t )pFormat >> 24 ) & 0xFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr EBaseDataType GetTextureFormatBaseDataType( ETextureFormat pFormat ) noexcept
		{
			return ( EBaseDataType )( ( ( texture_format_value_t )pFormat >> 8 ) & 0xFFFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr cppx::bitmask<EGpuDataFormatFlags> GetTextureFormatFlags( ETextureFormat pFormat ) noexcept
		{
			return cppx::make_bitmask<uint8>( ( ( texture_format_value_t )pFormat >> 8 ) & 0xFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr uint8 GetVertexAttribFormatComponentsNum( EVertexAttribFormat pFormat ) noexcept
		{
			return ( uint8 )( ( ( vertex_attrib_format_value_t )pFormat >> 16 ) & 0xFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr EBaseDataType GetVertexAttribFormatBaseDataType( EVertexAttribFormat pFormat ) noexcept
		{
			return ( EBaseDataType )( ( vertex_attrib_format_value_t )pFormat & 0xFFFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr cppx::bitmask<EGpuDataFormatFlags> GetVertexAttribFormatFlags( EVertexAttribFormat pFormat )
		{
			return cppx::make_bitmask<uint8>( ( vertex_attrib_format_value_t )pFormat & 0xFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsTextureFormatSRGB( ETextureFormat pFormat ) noexcept
		{
			const auto formatFlags = GetTextureFormatFlags( pFormat );
			return formatFlags.is_set( eGpuDataFormatFlagSRGBBit );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsVertexAttribFormatNormalized( EVertexAttribFormat pFormat ) noexcept
		{
			const auto formatFlags = GetVertexAttribFormatFlags( pFormat );
			return formatFlags.is_set( eGpuDataFormatFlagNormalizedBit );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsVertexAttribFormatValid( EVertexAttribFormat pFormat ) noexcept
		{
			return ( pFormat != EVertexAttribFormat::Undefined ) && ( pFormat != EVertexAttribFormat::Undefined );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr uint32 GetVertexAttribFormatByteSize( EVertexAttribFormat pFormat ) noexcept
		{
			ic3DebugAssert( pFormat != EVertexAttribFormat::Undefined );
			return GetBaseDataTypeByteSize( GetVertexAttribFormatBaseDataType( pFormat ) ) * GetVertexAttribFormatComponentsNum( pFormat );
		}

	}

	namespace SMU
	{

		inline ETextureFormat GetTextureFormatForGraphicsPixelLayout( EPixelDataLayout pPixelLayout )
		{
			switch( pPixelLayout )
			{
				ic3CaseReturn( EPixelDataLayout::Undefined , ETextureFormat::Undefined );
				ic3CaseReturn( EPixelDataLayout::Alpha     , ETextureFormat::R8UN );
				ic3CaseReturn( EPixelDataLayout::Depth     , ETextureFormat::R8UN );
				ic3CaseReturn( EPixelDataLayout::DS        , ETextureFormat::RG8UN );
				ic3CaseReturn( EPixelDataLayout::BGR       , ETextureFormat::BGRX8UN );
				ic3CaseReturn( EPixelDataLayout::BGRA      , ETextureFormat::BGRA8UN );
				ic3CaseReturn( EPixelDataLayout::Red       , ETextureFormat::R8UN );
				ic3CaseReturn( EPixelDataLayout::RG        , ETextureFormat::RG8UN );
				ic3CaseReturn( EPixelDataLayout::RGB       , ETextureFormat::BGRX8UN );
				ic3CaseReturn( EPixelDataLayout::RGBA      , ETextureFormat::RGBA8UN );
				ic3CaseReturn( EPixelDataLayout::S3TC      , ETextureFormat::BC3 );
			}

			return ETextureFormat::Undefined;
		}

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DATA_FORMATS_H__
