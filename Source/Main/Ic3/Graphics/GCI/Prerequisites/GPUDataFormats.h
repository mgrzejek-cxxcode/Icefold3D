
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
		eGPUDataFormatFlagNormalizedBit   = 0x01,
		eGPUDataFormatFlagTypeSignedBit   = 0x02,
		eGPUDataFormatFlagTypeUnsignedBit = 0x04,
		eGPUDataFormatFlagSRGBBit         = 0x08,
		eGPUDataFormatFlagDepthBit        = 0x10,
		eGPUDataFormatFlagStencilBit      = 0x20,
		eGPUDataFormatFlagDepthStencilBit = eGPUDataFormatFlagDepthBit | eGPUDataFormatFlagStencilBit,
		eGPUDataFormatFlagCompressedBit   = 0x80,
		eGPUDataFormatFlagSNorm           = eGPUDataFormatFlagNormalizedBit | eGPUDataFormatFlagTypeSignedBit,
		eGPUDataFormatFlagUNorm           = eGPUDataFormatFlagNormalizedBit | eGPUDataFormatFlagTypeUnsignedBit,
		eGPUDataFormatFlagCompressedSRGB  = eGPUDataFormatFlagCompressedBit | eGPUDataFormatFlagSRGBBit,
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
		Undefined  = 0,
		Unknown    = QLimits<gpu_pixel_format_value_t>::sMaxValue,
		R32F       = CxDef::declareTextureFormat( 0, EPixelDataLayout::Red, EBaseDataType::Float32, 4, eGPUDataFormatFlagTypeSignedBit ),
		R32I       = CxDef::declareTextureFormat( 1, EPixelDataLayout::Red, EBaseDataType::Int32, 4, eGPUDataFormatFlagTypeSignedBit ),
		R32U       = CxDef::declareTextureFormat( 2, EPixelDataLayout::Red, EBaseDataType::Uint32, 4, eGPUDataFormatFlagTypeUnsignedBit ),
		RG32F      = CxDef::declareTextureFormat( 3, EPixelDataLayout::RG, EBaseDataType::Float32, 8, eGPUDataFormatFlagTypeSignedBit ),
		RG32I      = CxDef::declareTextureFormat( 4, EPixelDataLayout::RG, EBaseDataType::Int32, 8, eGPUDataFormatFlagTypeSignedBit ),
		RG32U      = CxDef::declareTextureFormat( 5, EPixelDataLayout::RG, EBaseDataType::Uint32, 8, eGPUDataFormatFlagTypeUnsignedBit ),
		RGB32F     = CxDef::declareTextureFormat( 6, EPixelDataLayout::RGB, EBaseDataType::Float32, 12, eGPUDataFormatFlagTypeSignedBit ),
		RGB32I     = CxDef::declareTextureFormat( 7, EPixelDataLayout::RGB, EBaseDataType::Int32, 12, eGPUDataFormatFlagTypeSignedBit ),
		RGB32U     = CxDef::declareTextureFormat( 8, EPixelDataLayout::RGB, EBaseDataType::Uint32, 12, eGPUDataFormatFlagTypeUnsignedBit ),
		RGBA32F    = CxDef::declareTextureFormat( 9, EPixelDataLayout::RGBA, EBaseDataType::Float32, 16, eGPUDataFormatFlagTypeSignedBit ),
		RGBA32I    = CxDef::declareTextureFormat( 10, EPixelDataLayout::RGBA, EBaseDataType::Int32, 16, eGPUDataFormatFlagTypeSignedBit ),
		RGBA32U    = CxDef::declareTextureFormat( 11, EPixelDataLayout::RGBA, EBaseDataType::Uint32, 16, eGPUDataFormatFlagTypeUnsignedBit ),
		R16F       = CxDef::declareTextureFormat( 12, EPixelDataLayout::RG, EBaseDataType::Float16, 2, eGPUDataFormatFlagTypeSignedBit ),
		R16I       = CxDef::declareTextureFormat( 13, EPixelDataLayout::RG, EBaseDataType::Int16, 2, eGPUDataFormatFlagTypeSignedBit ),
		R16U       = CxDef::declareTextureFormat( 14, EPixelDataLayout::RG, EBaseDataType::Uint16, 2, eGPUDataFormatFlagTypeUnsignedBit ),
		RG16F      = CxDef::declareTextureFormat( 15, EPixelDataLayout::RG, EBaseDataType::Float16, 4, eGPUDataFormatFlagTypeSignedBit ),
		RG16I      = CxDef::declareTextureFormat( 16, EPixelDataLayout::RG, EBaseDataType::Int16, 4, eGPUDataFormatFlagTypeSignedBit ),
		RG16U      = CxDef::declareTextureFormat( 17, EPixelDataLayout::RG, EBaseDataType::Uint16, 4, eGPUDataFormatFlagTypeUnsignedBit ),
		RGBA16F    = CxDef::declareTextureFormat( 18, EPixelDataLayout::RGBA, EBaseDataType::Float16, 8, eGPUDataFormatFlagTypeSignedBit ),
		RGBA16I    = CxDef::declareTextureFormat( 19, EPixelDataLayout::RGBA, EBaseDataType::Int16, 8, eGPUDataFormatFlagTypeSignedBit ),
		RGBA16U    = CxDef::declareTextureFormat( 20, EPixelDataLayout::RGBA, EBaseDataType::Uint16, 8, eGPUDataFormatFlagTypeUnsignedBit ),
		R8I        = CxDef::declareTextureFormat( 21, EPixelDataLayout::Red, EBaseDataType::Byte, 1, eGPUDataFormatFlagTypeSignedBit ),
		R8U        = CxDef::declareTextureFormat( 22, EPixelDataLayout::Red, EBaseDataType::Ubyte, 1, eGPUDataFormatFlagTypeUnsignedBit ),
		R8IN       = CxDef::declareTextureFormat( 23, EPixelDataLayout::Red, EBaseDataType::Byte, 1, eGPUDataFormatFlagSNorm ),
		R8UN       = CxDef::declareTextureFormat( 24, EPixelDataLayout::Red, EBaseDataType::Ubyte, 1, eGPUDataFormatFlagUNorm ),
		RG8I       = CxDef::declareTextureFormat( 25, EPixelDataLayout::RG, EBaseDataType::Byte, 2, eGPUDataFormatFlagTypeSignedBit ),
		RG8U       = CxDef::declareTextureFormat( 26, EPixelDataLayout::RG, EBaseDataType::Ubyte, 2, eGPUDataFormatFlagTypeUnsignedBit ),
		RG8IN      = CxDef::declareTextureFormat( 27, EPixelDataLayout::RG, EBaseDataType::Byte, 2, eGPUDataFormatFlagSNorm ),
		RG8UN      = CxDef::declareTextureFormat( 28, EPixelDataLayout::RG, EBaseDataType::Ubyte, 2, eGPUDataFormatFlagUNorm ),
		BGRX8UN    = CxDef::declareTextureFormat( 29, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagUNorm ),
		BGRX8SRGB  = CxDef::declareTextureFormat( 30, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagSRGBBit ),
		BGRA8UN    = CxDef::declareTextureFormat( 31, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagUNorm ),
		BGRA8SRGB  = CxDef::declareTextureFormat( 32, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagSRGBBit ),
		RGBA8I     = CxDef::declareTextureFormat( 33, EPixelDataLayout::RGBA, EBaseDataType::Byte, 4, eGPUDataFormatFlagTypeSignedBit ),
		RGBA8U     = CxDef::declareTextureFormat( 34, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagTypeUnsignedBit ),
		RGBA8IN    = CxDef::declareTextureFormat( 35, EPixelDataLayout::RGBA, EBaseDataType::Byte, 4, eGPUDataFormatFlagSNorm ),
		RGBA8UN    = CxDef::declareTextureFormat( 36, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagUNorm ),
		RGBA8SRGB  = CxDef::declareTextureFormat( 37, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagSRGBBit ),
		R5G5B5A1   = CxDef::declareTextureFormat( 38, EPixelDataLayout::RGBA,  EBaseDataType::Ubyte,     2,  0 ),
		R5G6B5     = CxDef::declareTextureFormat( 39, EPixelDataLayout::RGB,   EBaseDataType::Ubyte,     2,  0 ),
		R9G9B9E5   = CxDef::declareTextureFormat( 40, EPixelDataLayout::RGBA,  EBaseDataType::Uint32,    4,  0 ),
		RGB10A2U   = CxDef::declareTextureFormat( 41, EPixelDataLayout::RGBA, EBaseDataType::Uint32, 4, eGPUDataFormatFlagTypeUnsignedBit ),
		RGB10A2UN  = CxDef::declareTextureFormat( 42, EPixelDataLayout::RGBA, EBaseDataType::Float32, 4, eGPUDataFormatFlagUNorm ),
		R11G11B10F = CxDef::declareTextureFormat( 43, EPixelDataLayout::RGB, EBaseDataType::Float32, 4, eGPUDataFormatFlagTypeUnsignedBit ),
		D16UN      = CxDef::declareTextureFormat( 44, EPixelDataLayout::Depth, EBaseDataType::Uint16, 2, eGPUDataFormatFlagDepthBit | eGPUDataFormatFlagUNorm ),
		D24UNS8U   = CxDef::declareTextureFormat( 45, EPixelDataLayout::DS, EBaseDataType::Uint24S8, 4, eGPUDataFormatFlagDepthStencilBit ),
		D24UNX8    = CxDef::declareTextureFormat( 46, EPixelDataLayout::DS, EBaseDataType::Uint24S8, 4, eGPUDataFormatFlagDepthBit ),
		X24S8U     = CxDef::declareTextureFormat( 47, EPixelDataLayout::DS, EBaseDataType::Uint24S8, 4, eGPUDataFormatFlagStencilBit ),
		D32F       = CxDef::declareTextureFormat( 48, EPixelDataLayout::DS, EBaseDataType::Float32, 4, eGPUDataFormatFlagDepthBit ),
		BC1        = CxDef::declareTextureFormat( 49, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC1SRGB    = CxDef::declareTextureFormat( 50, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedSRGB ),
		BC2        = CxDef::declareTextureFormat( 51, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC2SRGB    = CxDef::declareTextureFormat( 52, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedSRGB ),
		BC3        = CxDef::declareTextureFormat( 53, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC3SRGB    = CxDef::declareTextureFormat( 54, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedSRGB ),
		BC4IN      = CxDef::declareTextureFormat( 55, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC4UN      = CxDef::declareTextureFormat( 56, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC5IN      = CxDef::declareTextureFormat( 57, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC5UN      = CxDef::declareTextureFormat( 58, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC6HSF     = CxDef::declareTextureFormat( 59, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC6HUF     = CxDef::declareTextureFormat( 60, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC7        = CxDef::declareTextureFormat( 61, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC7SRGB    = CxDef::declareTextureFormat( 62, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedSRGB ),
	};

	enum class EVertexAttribFormat : vertex_attrib_format_value_t
	{
		Undefined = 0,
		Unknown   = QLimits<vertex_attrib_format_value_t>::sMaxValue,
		F16       = CxDef::declareVertexAttribFormat( EBaseDataType::Float16, 1, eGPUDataFormatFlagTypeSignedBit ),
		F32       = CxDef::declareVertexAttribFormat( EBaseDataType::Float32, 1, eGPUDataFormatFlagTypeSignedBit ),
		I8        = CxDef::declareVertexAttribFormat( EBaseDataType::Byte, 1, eGPUDataFormatFlagTypeSignedBit ),
		I16       = CxDef::declareVertexAttribFormat( EBaseDataType::Int16, 1, eGPUDataFormatFlagTypeSignedBit ),
		I32       = CxDef::declareVertexAttribFormat( EBaseDataType::Int32, 1, eGPUDataFormatFlagTypeSignedBit ),
		U8        = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte, 1, eGPUDataFormatFlagTypeUnsignedBit ),
		U16       = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16, 1, eGPUDataFormatFlagTypeUnsignedBit ),
		U32       = CxDef::declareVertexAttribFormat( EBaseDataType::Uint32, 1, eGPUDataFormatFlagTypeUnsignedBit ),
		I8N       = CxDef::declareVertexAttribFormat( EBaseDataType::Byte, 1, eGPUDataFormatFlagSNorm ),
		I16N      = CxDef::declareVertexAttribFormat( EBaseDataType::Int16, 1, eGPUDataFormatFlagSNorm ),
		U8N       = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte, 1, eGPUDataFormatFlagUNorm ),
		U16N      = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16, 1, eGPUDataFormatFlagUNorm ),
		Vec2F16   = CxDef::declareVertexAttribFormat( EBaseDataType::Float16, 2, eGPUDataFormatFlagTypeSignedBit ),
		Vec2F32   = CxDef::declareVertexAttribFormat( EBaseDataType::Float32, 2, eGPUDataFormatFlagTypeSignedBit ),
		Vec2I8    = CxDef::declareVertexAttribFormat( EBaseDataType::Byte, 2, eGPUDataFormatFlagTypeSignedBit ),
		Vec2I16   = CxDef::declareVertexAttribFormat( EBaseDataType::Int16, 2, eGPUDataFormatFlagTypeSignedBit ),
		Vec2I32   = CxDef::declareVertexAttribFormat( EBaseDataType::Int32, 2, eGPUDataFormatFlagTypeSignedBit ),
		Vec2U8    = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte, 2, eGPUDataFormatFlagTypeUnsignedBit ),
		Vec2U16   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16, 2, eGPUDataFormatFlagTypeUnsignedBit ),
		Vec2U32   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint32, 2, eGPUDataFormatFlagTypeUnsignedBit ),
		Vec2I8N   = CxDef::declareVertexAttribFormat( EBaseDataType::Byte, 2, eGPUDataFormatFlagSNorm ),
		Vec2I16N  = CxDef::declareVertexAttribFormat( EBaseDataType::Int16, 2, eGPUDataFormatFlagSNorm ),
		Vec2U8N   = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte, 2, eGPUDataFormatFlagUNorm ),
		Vec2U16N  = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16, 2, eGPUDataFormatFlagUNorm ),
		Vec3F32   = CxDef::declareVertexAttribFormat( EBaseDataType::Float32, 3, eGPUDataFormatFlagTypeSignedBit ),
		Vec3I32   = CxDef::declareVertexAttribFormat( EBaseDataType::Int32, 3, eGPUDataFormatFlagTypeSignedBit ),
		Vec3U32   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint32, 3, eGPUDataFormatFlagTypeUnsignedBit ),
		Vec4F16   = CxDef::declareVertexAttribFormat( EBaseDataType::Float16, 4, eGPUDataFormatFlagTypeSignedBit ),
		Vec4F32   = CxDef::declareVertexAttribFormat( EBaseDataType::Float32, 4, eGPUDataFormatFlagTypeSignedBit ),
		Vec4I8    = CxDef::declareVertexAttribFormat( EBaseDataType::Byte, 4, eGPUDataFormatFlagTypeSignedBit ),
		Vec4I16   = CxDef::declareVertexAttribFormat( EBaseDataType::Int16, 4, eGPUDataFormatFlagTypeSignedBit ),
		Vec4I32   = CxDef::declareVertexAttribFormat( EBaseDataType::Int32, 4, eGPUDataFormatFlagTypeSignedBit ),
		Vec4U8    = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte, 4, eGPUDataFormatFlagTypeUnsignedBit ),
		Vec4U16   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16, 4, eGPUDataFormatFlagTypeUnsignedBit ),
		Vec4U32   = CxDef::declareVertexAttribFormat( EBaseDataType::Uint32, 4, eGPUDataFormatFlagTypeUnsignedBit ),
		Vec4I8N   = CxDef::declareVertexAttribFormat( EBaseDataType::Byte, 4, eGPUDataFormatFlagSNorm ),
		Vec4I16N  = CxDef::declareVertexAttribFormat( EBaseDataType::Int16, 4, eGPUDataFormatFlagSNorm ),
		Vec4U8N   = CxDef::declareVertexAttribFormat( EBaseDataType::Ubyte, 4, eGPUDataFormatFlagUNorm ),
		Vec4U16N  = CxDef::declareVertexAttribFormat( EBaseDataType::Uint16, 4, eGPUDataFormatFlagUNorm ),
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

		IC3_ATTR_NO_DISCARD inline constexpr TBitmask<uint8> getVertexAttribFormatFlags( EVertexAttribFormat pFormat )
		{
			return ( uint8 )( ( ( vertex_attrib_format_value_t )pFormat >> 8 ) & 0xFF );
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isTextureFormatSRGB( ETextureFormat pFormat ) noexcept
		{
			const TBitmask<uint8> formatFlags = getTextureFormatFlags( pFormat );
			return formatFlags.isSet( eGPUDataFormatFlagSRGBBit );
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isVertexAttribFormatNormalized( EVertexAttribFormat pFormat ) noexcept
		{
			const TBitmask<uint8> formatFlags = getVertexAttribFormatFlags( pFormat );
			return formatFlags.isSet( eGPUDataFormatFlagNormalizedBit );
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint32 getVertexAttribFormatByteSize( EVertexAttribFormat pFormat ) noexcept
		{
			return getBaseDataTypeByteSize( getVertexAttribFormatBaseDataType( pFormat ) ) * getVertexAttribFormatComponentsNum( pFormat );
		}

	}

	namespace SMU
	{

		inline ETextureFormat getTextureFormatForGraphicsPixelLayout( EPixelDataLayout pPixelLayout )
		{
			switch( pPixelLayout )
			{
				ic3CaseReturn( EPixelDataLayout::Undefined , ETextureFormat::Unknown );
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

			return ETextureFormat::Unknown;
		}

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DATA_FORMATS_H__
