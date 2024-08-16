
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DATA_FORMATS_H__
#define __IC3_GRAPHICS_GCI_GPU_DATA_FORMATS_H__

#include <Ic3/CoreLib/PixelCommon.h>

namespace Ic3::Graphics::GCI
{

	using base_data_type_value_t = uint8;
	using gpu_pixel_format_value_t = uint64;
	using vertex_attrib_format_value_t = uint32;

	enum class EBaseDataType : base_data_type_value_t;

	namespace CxDef
	{

		IC3_ATTR_NO_DISCARD inline constexpr base_data_type_value_t declareBaseDataType( uint8 pIndex, uint8 pByteSize ) noexcept
		{
			return ( ( pByteSize & 0xF ) << 4 ) | ( pIndex & 0xF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr gpu_pixel_format_value_t declareTextureFormat(
				uint8 pIndex, EPixelDataLayout pPixelLayout, EBaseDataType pBaseType, uint8 pByteSize, uint8 pFlags ) noexcept
		{
			return ( ( ( uint64 )pPixelLayout ) << 32 ) | ( ( ( uint64 )pBaseType ) << 24 ) | ( ( ( uint64 )pByteSize ) << 16 ) | ( ( ( uint64 )pFlags ) << 8 ) | ( uint64 )pIndex;
		}

		IC3_ATTR_NO_DISCARD inline constexpr vertex_attrib_format_value_t declareVertexAttribFormat(
				EBaseDataType pBaseType, uint8 pCompNum, uint8 pFlags ) noexcept
		{
			return ( ( ( uint32 )pBaseType ) << 24 ) | ( ( ( uint32 )pCompNum ) << 16 ) | ( ( ( uint32 )pFlags ) << 8 );
		}

	}

	enum EGPUDataFormatFlags : uint8
	{
		E_GPU_DATA_FORMAT_FLAG_NORMALIZED_BIT = 0x01,
		E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT = 0x02,
		E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT = 0x04,
		E_GPU_DATA_FORMAT_FLAG_SRGB_BIT = 0x08,
		E_GPU_DATA_FORMAT_FLAG_DEPTH_BIT = 0x10,
		E_GPU_DATA_FORMAT_FLAG_STENCIL_BIT = 0x20,
		E_GPU_DATA_FORMAT_FLAG_DEPTH_STENCIL_BIT = E_GPU_DATA_FORMAT_FLAG_DEPTH_BIT | E_GPU_DATA_FORMAT_FLAG_STENCIL_BIT,
		E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT = 0x80,
		E_GPU_DATA_FORMAT_FLAG_SNORM = E_GPU_DATA_FORMAT_FLAG_NORMALIZED_BIT | E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT,
		E_GPU_DATA_FORMAT_FLAG_UNORM = E_GPU_DATA_FORMAT_FLAG_NORMALIZED_BIT | E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT,
		E_GPU_DATA_FORMAT_FLAG_COMPRESSED_SRGB = E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT | E_GPU_DATA_FORMAT_FLAG_SRGB_BIT,
	};

	enum class EBaseDataType : base_data_type_value_t
	{
		Undefined    = CxDef::declareBaseDataType( 0, 0 ),
		Byte         = CxDef::declareBaseDataType( 1, 1 ),
		Ubyte        = CxDef::declareBaseDataType( 2, 1 ),
		Int16        = CxDef::declareBaseDataType( 3, 2 ),
		Uint16       = CxDef::declareBaseDataType( 4, 2 ),
		Int32        = CxDef::declareBaseDataType( 5, 4 ),
		Uint32       = CxDef::declareBaseDataType( 6, 4 ),
		Float16      = CxDef::declareBaseDataType( 7, 2 ),
		Float32      = CxDef::declareBaseDataType( 8, 4 ),
		Uint24S8     = CxDef::declareBaseDataType( 9, 4 ),
	};

	enum class EIndexDataFormat : base_data_type_value_t
	{
		Undefined = 0,
		Uint16 = static_cast<base_data_type_value_t>( EBaseDataType::Uint16 ),
		Uint32 = static_cast<base_data_type_value_t>( EBaseDataType::Uint32 ),
	};

	enum class ETextureFormat : gpu_pixel_format_value_t
	{
		R32F       = CxDef::declareTextureFormat( 0,  EPixelDataLayout::Red,   EBaseDataType::Float32,   4,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		R32I       = CxDef::declareTextureFormat( 1,  EPixelDataLayout::Red,   EBaseDataType::Int32,     4,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		R32U       = CxDef::declareTextureFormat( 2,  EPixelDataLayout::Red,   EBaseDataType::Uint32,    4,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		RG32F      = CxDef::declareTextureFormat( 3,  EPixelDataLayout::RG,    EBaseDataType::Float32,   8,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RG32I      = CxDef::declareTextureFormat( 4,  EPixelDataLayout::RG,    EBaseDataType::Int32,     8,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RG32U      = CxDef::declareTextureFormat( 5,  EPixelDataLayout::RG,    EBaseDataType::Uint32,    8,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		RGB32F     = CxDef::declareTextureFormat( 6,  EPixelDataLayout::RGB,   EBaseDataType::Float32,   12, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RGB32I     = CxDef::declareTextureFormat( 7,  EPixelDataLayout::RGB,   EBaseDataType::Int32,     12, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RGB32U     = CxDef::declareTextureFormat( 8,  EPixelDataLayout::RGB,   EBaseDataType::Uint32,    12, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		RGBA32F    = CxDef::declareTextureFormat( 9,  EPixelDataLayout::RGBA,  EBaseDataType::Float32,   16, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RGBA32I    = CxDef::declareTextureFormat( 10, EPixelDataLayout::RGBA,  EBaseDataType::Int32,     16, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RGBA32U    = CxDef::declareTextureFormat( 11, EPixelDataLayout::RGBA,  EBaseDataType::Uint32,    16, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		R16F       = CxDef::declareTextureFormat( 12, EPixelDataLayout::RG,    EBaseDataType::Float16,   2,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		R16I       = CxDef::declareTextureFormat( 13, EPixelDataLayout::RG,    EBaseDataType::Int16,     2,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		R16U       = CxDef::declareTextureFormat( 14, EPixelDataLayout::RG,    EBaseDataType::Uint16,    2,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		RG16F      = CxDef::declareTextureFormat( 15, EPixelDataLayout::RG,    EBaseDataType::Float16,   4,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RG16I      = CxDef::declareTextureFormat( 16, EPixelDataLayout::RG,    EBaseDataType::Int16,     4,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RG16U      = CxDef::declareTextureFormat( 17, EPixelDataLayout::RG,    EBaseDataType::Uint16,    4,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		RGBA16F    = CxDef::declareTextureFormat( 18, EPixelDataLayout::RGBA,  EBaseDataType::Float16,   8,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RGBA16I    = CxDef::declareTextureFormat( 19, EPixelDataLayout::RGBA,  EBaseDataType::Int16,     8,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RGBA16U    = CxDef::declareTextureFormat( 20, EPixelDataLayout::RGBA,  EBaseDataType::Uint16,    8,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		R8I        = CxDef::declareTextureFormat( 21, EPixelDataLayout::Red,   EBaseDataType::Byte,      1,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		R8U        = CxDef::declareTextureFormat( 22, EPixelDataLayout::Red,   EBaseDataType::Ubyte,     1,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		R8IN       = CxDef::declareTextureFormat( 23, EPixelDataLayout::Red,   EBaseDataType::Byte,      1,  E_GPU_DATA_FORMAT_FLAG_SNORM ),
		R8UN       = CxDef::declareTextureFormat( 24, EPixelDataLayout::Red,   EBaseDataType::Ubyte,     1,  E_GPU_DATA_FORMAT_FLAG_UNORM ),
		RG8I       = CxDef::declareTextureFormat( 25, EPixelDataLayout::RG,    EBaseDataType::Byte,      2,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RG8U       = CxDef::declareTextureFormat( 26, EPixelDataLayout::RG,    EBaseDataType::Ubyte,     2,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		RG8IN      = CxDef::declareTextureFormat( 27, EPixelDataLayout::RG,    EBaseDataType::Byte,      2,  E_GPU_DATA_FORMAT_FLAG_SNORM ),
		RG8UN      = CxDef::declareTextureFormat( 28, EPixelDataLayout::RG,    EBaseDataType::Ubyte,     2,  E_GPU_DATA_FORMAT_FLAG_UNORM ),
		BGRX8UN    = CxDef::declareTextureFormat( 29, EPixelDataLayout::RGBA,  EBaseDataType::Ubyte,     4,  E_GPU_DATA_FORMAT_FLAG_UNORM ),
		BGRX8SRGB  = CxDef::declareTextureFormat( 30, EPixelDataLayout::RGBA,  EBaseDataType::Ubyte,     4,  E_GPU_DATA_FORMAT_FLAG_SRGB_BIT ),
		BGRA8UN    = CxDef::declareTextureFormat( 31, EPixelDataLayout::RGBA,  EBaseDataType::Ubyte,     4,  E_GPU_DATA_FORMAT_FLAG_UNORM ),
		BGRA8SRGB  = CxDef::declareTextureFormat( 32, EPixelDataLayout::RGBA,  EBaseDataType::Ubyte,     4,  E_GPU_DATA_FORMAT_FLAG_SRGB_BIT ),
		RGBA8I     = CxDef::declareTextureFormat( 33, EPixelDataLayout::RGBA,  EBaseDataType::Byte,      4,  E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		RGBA8U     = CxDef::declareTextureFormat( 34, EPixelDataLayout::RGBA,  EBaseDataType::Ubyte,     4,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		RGBA8IN    = CxDef::declareTextureFormat( 35, EPixelDataLayout::RGBA,  EBaseDataType::Byte,      4,  E_GPU_DATA_FORMAT_FLAG_SNORM ),
		RGBA8UN    = CxDef::declareTextureFormat( 36, EPixelDataLayout::RGBA,  EBaseDataType::Ubyte,     4,  E_GPU_DATA_FORMAT_FLAG_UNORM ),
		RGBA8SRGB  = CxDef::declareTextureFormat( 37, EPixelDataLayout::RGBA,  EBaseDataType::Ubyte,     4,  E_GPU_DATA_FORMAT_FLAG_SRGB_BIT ),
		R5G5B5A1   = CxDef::declareTextureFormat( 38, EPixelDataLayout::RGBA,  EBaseDataType::Ubyte,     2,  0 ),
		R5G6B5     = CxDef::declareTextureFormat( 39, EPixelDataLayout::RGB,   EBaseDataType::Ubyte,     2,  0 ),
		R9G9B9E5   = CxDef::declareTextureFormat( 40, EPixelDataLayout::RGBA,  EBaseDataType::Uint32,    4,  0 ),
		RGB10A2U   = CxDef::declareTextureFormat( 41, EPixelDataLayout::RGBA,  EBaseDataType::Uint32,    4,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		RGB10A2UN  = CxDef::declareTextureFormat( 42, EPixelDataLayout::RGBA,  EBaseDataType::Float32,   4,  E_GPU_DATA_FORMAT_FLAG_UNORM ),
		R11G11B10F = CxDef::declareTextureFormat( 43, EPixelDataLayout::RGB,   EBaseDataType::Float32,   4,  E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		D16UN      = CxDef::declareTextureFormat( 44, EPixelDataLayout::Depth, EBaseDataType::Uint16,    2,  E_GPU_DATA_FORMAT_FLAG_DEPTH_BIT | E_GPU_DATA_FORMAT_FLAG_UNORM ),
		D24UNS8U   = CxDef::declareTextureFormat( 45, EPixelDataLayout::DS,    EBaseDataType::Uint24S8,  4,  E_GPU_DATA_FORMAT_FLAG_DEPTH_STENCIL_BIT ),
		D24UNX8    = CxDef::declareTextureFormat( 46, EPixelDataLayout::DS,    EBaseDataType::Uint24S8,  4,  E_GPU_DATA_FORMAT_FLAG_DEPTH_BIT ),
		X24S8U     = CxDef::declareTextureFormat( 47, EPixelDataLayout::DS,    EBaseDataType::Uint24S8,  4,  E_GPU_DATA_FORMAT_FLAG_STENCIL_BIT ),
		D32F       = CxDef::declareTextureFormat( 48, EPixelDataLayout::DS,    EBaseDataType::Float32,   4,  E_GPU_DATA_FORMAT_FLAG_DEPTH_BIT ),
		BC1        = CxDef::declareTextureFormat( 49, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC1SRGB    = CxDef::declareTextureFormat( 50, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_SRGB ),
		BC2        = CxDef::declareTextureFormat( 51, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC2SRGB    = CxDef::declareTextureFormat( 52, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_SRGB ),
		BC3        = CxDef::declareTextureFormat( 53, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC3SRGB    = CxDef::declareTextureFormat( 54, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_SRGB ),
		BC4IN      = CxDef::declareTextureFormat( 55, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC4UN      = CxDef::declareTextureFormat( 56, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC5IN      = CxDef::declareTextureFormat( 57, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC5UN      = CxDef::declareTextureFormat( 58, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC6HSF     = CxDef::declareTextureFormat( 59, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC6HUF     = CxDef::declareTextureFormat( 60, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC7        = CxDef::declareTextureFormat( 61, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_BIT ),
		BC7SRGB    = CxDef::declareTextureFormat( 62, EPixelDataLayout::S3TC,  EBaseDataType::Undefined, 0,  E_GPU_DATA_FORMAT_FLAG_COMPRESSED_SRGB ),
		UNKNOWN    = 0
	};

	enum class EVertexAttribFormat : vertex_attrib_format_value_t
	{
		F16       = CxDef::declareVertexAttribFormat( EBaseDataType::Float16, 1, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		F32       = CxDef::declareVertexAttribFormat( EBaseDataType::Float32, 1, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		I8        = CxDef::declareVertexAttribFormat( EBaseDataType::Byte,    1, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		I16       = CxDef::declareVertexAttribFormat( EBaseDataType::Int16,   1, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		I32       = CxDef::declareVertexAttribFormat( EBaseDataType::Int32,   1, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		U8        = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte,   1, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		U16       = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16,  1, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		U32       = CxDef::declareVertexAttribFormat( EBaseDataType::Uint32,  1, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		I8N       = CxDef::declareVertexAttribFormat( EBaseDataType::Byte,    1, E_GPU_DATA_FORMAT_FLAG_SNORM ),
		I16N      = CxDef::declareVertexAttribFormat( EBaseDataType::Int16,   1, E_GPU_DATA_FORMAT_FLAG_SNORM ),
		U8N       = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte,   1, E_GPU_DATA_FORMAT_FLAG_UNORM ),
		U16N      = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16,  1, E_GPU_DATA_FORMAT_FLAG_UNORM ),
		Vec2F16   = CxDef::declareVertexAttribFormat( EBaseDataType::Float16, 2, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec2F32   = CxDef::declareVertexAttribFormat( EBaseDataType::Float32, 2, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec2I8    = CxDef::declareVertexAttribFormat( EBaseDataType::Byte,    2, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec2I16   = CxDef::declareVertexAttribFormat( EBaseDataType::Int16,   2, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec2I32   = CxDef::declareVertexAttribFormat( EBaseDataType::Int32,   2, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec2U8    = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte,   2, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		Vec2U16   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16,  2, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		Vec2U32   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint32,  2, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		Vec2I8N   = CxDef::declareVertexAttribFormat( EBaseDataType::Byte,    2, E_GPU_DATA_FORMAT_FLAG_SNORM ),
		Vec2I16N  = CxDef::declareVertexAttribFormat( EBaseDataType::Int16,   2, E_GPU_DATA_FORMAT_FLAG_SNORM ),
		Vec2U8N   = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte,   2, E_GPU_DATA_FORMAT_FLAG_UNORM ),
		Vec2U16N  = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16,  2, E_GPU_DATA_FORMAT_FLAG_UNORM ),
		Vec3F32   = CxDef::declareVertexAttribFormat( EBaseDataType::Float32, 3, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec3I32   = CxDef::declareVertexAttribFormat( EBaseDataType::Int32,   3, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec3U32   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint32,  3, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		Vec4F16   = CxDef::declareVertexAttribFormat( EBaseDataType::Float16, 4, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec4F32   = CxDef::declareVertexAttribFormat( EBaseDataType::Float32, 4, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec4I8    = CxDef::declareVertexAttribFormat( EBaseDataType::Byte,    4, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec4I16   = CxDef::declareVertexAttribFormat( EBaseDataType::Int16,   4, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec4I32   = CxDef::declareVertexAttribFormat( EBaseDataType::Int32,   4, E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT ),
		Vec4U8    = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte,   4, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		Vec4U16   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16,  4, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		Vec4U32   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint32,  4, E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT ),
		Vec4I8N   = CxDef::declareVertexAttribFormat( EBaseDataType::Byte,    4, E_GPU_DATA_FORMAT_FLAG_SNORM ),
		Vec4I16N  = CxDef::declareVertexAttribFormat( EBaseDataType::Int16,   4, E_GPU_DATA_FORMAT_FLAG_SNORM ),
		Vec4U8N   = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte,   4, E_GPU_DATA_FORMAT_FLAG_UNORM ),
		Vec4U16N  = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16,  4, E_GPU_DATA_FORMAT_FLAG_UNORM ),
		Undefined = 0
	};

	using EShaderInputConstantFormat = EVertexAttribFormat;

	namespace CxDef
	{

		IC3_ATTR_NO_DISCARD inline constexpr uint16 getBaseDataTypeByteSize( EBaseDataType pBaseType ) noexcept
		{
			return ( uint16 )( ( ( base_data_type_value_t )pBaseType >> 4 ) & 0xF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint16 getIndexDataFormatByteSize( EIndexDataFormat pFormat ) noexcept
		{
			return getBaseDataTypeByteSize( static_cast<EBaseDataType>( pFormat ) );
		}

		IC3_ATTR_NO_DISCARD inline constexpr EPixelDataLayout getTextureFormatPixelDataLayout( ETextureFormat pFormat ) noexcept
		{
			return ( EPixelDataLayout )( ( ( gpu_pixel_format_value_t )pFormat >> 32 ) & 0xFF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr EBaseDataType getTextureFormatBaseDataType( ETextureFormat pFormat ) noexcept
		{
			return ( EBaseDataType )( ( ( gpu_pixel_format_value_t )pFormat >> 24 ) & 0xFF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint8 getTextureFormatByteSize( ETextureFormat pFormat ) noexcept
		{
			return ( uint8 )( ( ( gpu_pixel_format_value_t )pFormat >> 16 ) & 0xFF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint8 getTextureFormatFlags( ETextureFormat pFormat ) noexcept
		{
			return ( uint8 )( ( ( gpu_pixel_format_value_t )pFormat >> 8 ) & 0xFF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr EBaseDataType getVertexAttribFormatBaseDataType( EVertexAttribFormat pFormat ) noexcept
		{
			return ( EBaseDataType )( ( ( vertex_attrib_format_value_t )pFormat >> 24 ) & 0xFF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint8 getVertexAttribFormatComponentsNum( EVertexAttribFormat pFormat ) noexcept
		{
			return ( uint8 )( ( ( vertex_attrib_format_value_t )pFormat >> 16 ) & 0xFF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr Cppx::Bitmask<uint8> getVertexAttribFormatFlags( EVertexAttribFormat pFormat )
		{
			return ( uint8 )( ( ( vertex_attrib_format_value_t )pFormat >> 8 ) & 0xFF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isTextureFormatSRGB( ETextureFormat pFormat ) noexcept
		{
			const Cppx::Bitmask<uint8> formatFlags = getTextureFormatFlags( pFormat );
			return formatFlags.isSet( E_GPU_DATA_FORMAT_FLAG_SRGB_BIT );
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isVertexAttribFormatNormalized( EVertexAttribFormat pFormat ) noexcept
		{
			const Cppx::Bitmask<uint8> formatFlags = getVertexAttribFormatFlags( pFormat );
			return formatFlags.isSet( E_GPU_DATA_FORMAT_FLAG_NORMALIZED_BIT );
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint32 getVertexAttribFormatByteSize( EVertexAttribFormat pFormat ) noexcept
		{
			return getBaseDataTypeByteSize( getVertexAttribFormatBaseDataType( pFormat ) ) * getVertexAttribFormatComponentsNum( pFormat );
		}

	}

	namespace smutil
	{

		inline ETextureFormat getTextureFormatForGraphicsPixelLayout( EPixelDataLayout pPixelLayout )
		{
			switch( pPixelLayout )
			{
				ic3CaseReturn( EPixelDataLayout::Undefined , ETextureFormat::UNKNOWN );
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

			return ETextureFormat::UNKNOWN;
		}

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DATA_FORMATS_H__
