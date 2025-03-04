
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

	enum EGPUDataFormatFlags : uint8
	{
		eGPUDataFormatFlagNormalizedBit        = 0x01,
		eGPUDataFormatFlagTypeSignedBit        = 0x02,
		eGPUDataFormatFlagTypeIntegralBit      = 0x04,
		eGPUDataFormatFlagTypeFloatingPointBit = 0x08 | eGPUDataFormatFlagTypeSignedBit,
		eGPUDataFormatFlagSRGBBit              = 0x10,
		eGPUDataFormatFlagDepthBit             = 0x20,
		eGPUDataFormatFlagStencilBit           = 0x40,
		eGPUDataFormatFlagCompressedBit        = 0x80,
		eGPUDataFormatMaskDepthStencilBit      = eGPUDataFormatFlagDepthBit | eGPUDataFormatFlagStencilBit,
		eGPUDataFormatMaskDepthNorm            = eGPUDataFormatFlagDepthBit | eGPUDataFormatFlagNormalizedBit,
		eGPUDataFormatMaskSInt                 = eGPUDataFormatFlagTypeIntegralBit | eGPUDataFormatFlagTypeSignedBit,
		eGPUDataFormatMaskUInt                 = eGPUDataFormatFlagTypeIntegralBit,
		eGPUDataFormatMaskSNorm                = eGPUDataFormatFlagNormalizedBit | eGPUDataFormatFlagTypeSignedBit,
		eGPUDataFormatMaskUNorm                = eGPUDataFormatFlagNormalizedBit,
		eGPUDataFormatMaskCompressedSRGB       = eGPUDataFormatFlagCompressedBit | eGPUDataFormatFlagSRGBBit,
		eGPUDataFormatMaskAll                  = 0xFF
	};

	namespace CXU
	{

		///
		constexpr uint8 VBM_ATTRIB_FMT_CONTROL_KEY = 0xAF;

		///
		constexpr uint8 VBM_TEXTURE_FMT_CONTROL_KEY = 0xEF;

		/**
		 *
		 * @param pIdx
		 * @param pBsz
		 * @param pFlg
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr base_data_type_value_t makeBaseDataTypeEnumValue( uint8 pBsz, uint8 pFlg = 0 ) noexcept
		{
			// Note: EVertexAttribFormat (and related functionality) relies on the fact that last 8 bits are flags (this
			// is shared with other enums to fit into certain size limits). Plan carefully before changing the bit layout.
			return ( ( ( uint16 )pBsz & 0xF ) << 8 ) | ( pFlg & eGPUDataFormatMaskAll );
		}

		/**
		 * @param pIdx
		 * @param pPix
		 * @param pBdt
		 * @param pBsz
		 * @param pFlg
		 * @return
		 */
	CPPX_ATTR_NO_DISCARD inline constexpr texture_format_value_t MakeTextureDataFormatEnumValue(
				uint8 pIdx, EPixelDataLayout pPix, EBaseDataType pBdt, uint8 pBsz, uint8 pFlg = 0 ) noexcept
		{
			return ( ( uint64 )VBM_TEXTURE_FMT_CONTROL_KEY << 56 ) | ( ( uint64 )pIdx << 48 ) | ( ( uint64 )pPix << 32 ) | ( ( uint64 )pBsz << 24 ) | ( ( ( uint64 )pBdt & 0xFFF ) << 12 ) | ( ( ( uint64 )pFlg  & eGPUDataFormatMaskAll ) << 4 );
		}

		/**
		 *
		 * @param pBcn
		 * @param pBdt
		 * @param pFlg
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr vertex_attrib_format_value_t MakeVertexAttribFormatEnumValue(
				uint8 pBcn, EBaseDataType pBdt, uint8 pFlg = 0 ) noexcept
		{
			return ( ( uint32 )VBM_ATTRIB_FMT_CONTROL_KEY << 24 ) | ( ( ( uint32 )pBcn & 0xF ) << 20 ) | ( ( ( uint32 ) pBdt & 0xFFF ) << 8 ) | ( ( uint32 )pFlg & eGPUDataFormatMaskAll );
		}

	}

	enum class EBaseDataType : base_data_type_value_t
	{
		Undefined    = CXU::makeBaseDataTypeEnumValue( 0 , 0 ),
		Bool         = CXU::makeBaseDataTypeEnumValue( 1 , 0 ),
		Byte         = CXU::makeBaseDataTypeEnumValue( 1 , eGPUDataFormatMaskSInt ),
		Ubyte        = CXU::makeBaseDataTypeEnumValue( 1 , eGPUDataFormatMaskUInt ),
		Int16        = CXU::makeBaseDataTypeEnumValue( 2 , eGPUDataFormatMaskSInt ),
		Uint16       = CXU::makeBaseDataTypeEnumValue( 2 , eGPUDataFormatMaskUInt ),
		Int32        = CXU::makeBaseDataTypeEnumValue( 4 , eGPUDataFormatMaskSInt ),
		Uint32       = CXU::makeBaseDataTypeEnumValue( 4 , eGPUDataFormatMaskUInt ),
		Float16      = CXU::makeBaseDataTypeEnumValue( 2 , eGPUDataFormatFlagTypeFloatingPointBit ),
		Float32      = CXU::makeBaseDataTypeEnumValue( 4 , eGPUDataFormatFlagTypeFloatingPointBit ),
		Double       = CXU::makeBaseDataTypeEnumValue( 8 , eGPUDataFormatFlagTypeFloatingPointBit ),
		Uint24S8     = CXU::makeBaseDataTypeEnumValue( 4 , 0 ),
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

		R32F       = CXU::MakeTextureDataFormatEnumValue( 0x01, EPixelDataLayout::Red,  EBaseDataType::Float32, 4 ),
		R32I       = CXU::MakeTextureDataFormatEnumValue( 0x02, EPixelDataLayout::Red,  EBaseDataType::Int32,   4 ),
		R32U       = CXU::MakeTextureDataFormatEnumValue( 0x03, EPixelDataLayout::Red,  EBaseDataType::Uint32,  4 ),
		RG32F      = CXU::MakeTextureDataFormatEnumValue( 0x04, EPixelDataLayout::RG,   EBaseDataType::Float32, 8 ),
		RG32I      = CXU::MakeTextureDataFormatEnumValue( 0x05, EPixelDataLayout::RG,   EBaseDataType::Int32,   8 ),
		RG32U      = CXU::MakeTextureDataFormatEnumValue( 0x06, EPixelDataLayout::RG,   EBaseDataType::Uint32,  8 ),
		RGB32F     = CXU::MakeTextureDataFormatEnumValue( 0x07, EPixelDataLayout::RGB,  EBaseDataType::Float32, 12 ),
		RGB32I     = CXU::MakeTextureDataFormatEnumValue( 0x08, EPixelDataLayout::RGB,  EBaseDataType::Int32,   12 ),
		RGB32U     = CXU::MakeTextureDataFormatEnumValue( 0x09, EPixelDataLayout::RGB,  EBaseDataType::Uint32,  12 ),
		RGBA32F    = CXU::MakeTextureDataFormatEnumValue( 0x0A, EPixelDataLayout::RGBA, EBaseDataType::Float32, 16 ),
		RGBA32I    = CXU::MakeTextureDataFormatEnumValue( 0x0B, EPixelDataLayout::RGBA, EBaseDataType::Int32,   16 ),
		RGBA32U    = CXU::MakeTextureDataFormatEnumValue( 0x0C, EPixelDataLayout::RGBA, EBaseDataType::Uint32,  16 ),
		R16F       = CXU::MakeTextureDataFormatEnumValue( 0x0D, EPixelDataLayout::RG,   EBaseDataType::Float16, 2 ),
		R16I       = CXU::MakeTextureDataFormatEnumValue( 0x0E, EPixelDataLayout::RG,   EBaseDataType::Int16,   2 ),
		R16U       = CXU::MakeTextureDataFormatEnumValue( 0x0F, EPixelDataLayout::RG,   EBaseDataType::Uint16,  2 ),
		RG16F      = CXU::MakeTextureDataFormatEnumValue( 0x11, EPixelDataLayout::RG,   EBaseDataType::Float16, 4 ),
		RG16I      = CXU::MakeTextureDataFormatEnumValue( 0x12, EPixelDataLayout::RG,   EBaseDataType::Int16,   4 ),
		RG16U      = CXU::MakeTextureDataFormatEnumValue( 0x13, EPixelDataLayout::RG,   EBaseDataType::Uint16,  4 ),
		RGBA16F    = CXU::MakeTextureDataFormatEnumValue( 0x14, EPixelDataLayout::RGBA, EBaseDataType::Float16, 8 ),
		RGBA16I    = CXU::MakeTextureDataFormatEnumValue( 0x15, EPixelDataLayout::RGBA, EBaseDataType::Int16,   8 ),
		RGBA16U    = CXU::MakeTextureDataFormatEnumValue( 0x16, EPixelDataLayout::RGBA, EBaseDataType::Uint16,  8 ),

		R8I        = CXU::MakeTextureDataFormatEnumValue( 0x21, EPixelDataLayout::Red,  EBaseDataType::Byte,  1 ),
		R8U        = CXU::MakeTextureDataFormatEnumValue( 0x22, EPixelDataLayout::Red,  EBaseDataType::Ubyte, 1 ),
		R8IN       = CXU::MakeTextureDataFormatEnumValue( 0x23, EPixelDataLayout::Red,  EBaseDataType::Byte,  1, eGPUDataFormatFlagNormalizedBit ),
		R8UN       = CXU::MakeTextureDataFormatEnumValue( 0x24, EPixelDataLayout::Red,  EBaseDataType::Ubyte, 1, eGPUDataFormatFlagNormalizedBit ),
		RG8I       = CXU::MakeTextureDataFormatEnumValue( 0x25, EPixelDataLayout::RG,   EBaseDataType::Byte,  2 ),
		RG8U       = CXU::MakeTextureDataFormatEnumValue( 0x26, EPixelDataLayout::RG,   EBaseDataType::Ubyte, 2 ),
		RG8IN      = CXU::MakeTextureDataFormatEnumValue( 0x27, EPixelDataLayout::RG,   EBaseDataType::Byte,  2, eGPUDataFormatFlagNormalizedBit ),
		RG8UN      = CXU::MakeTextureDataFormatEnumValue( 0x28, EPixelDataLayout::RG,   EBaseDataType::Ubyte, 2, eGPUDataFormatFlagNormalizedBit ),
		BGRX8UN    = CXU::MakeTextureDataFormatEnumValue( 0x29, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagNormalizedBit ),
		BGRX8SRGB  = CXU::MakeTextureDataFormatEnumValue( 0x2A, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagSRGBBit ),
		BGRA8UN    = CXU::MakeTextureDataFormatEnumValue( 0x2B, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagNormalizedBit ),
		BGRA8SRGB  = CXU::MakeTextureDataFormatEnumValue( 0x2C, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagSRGBBit ),
		RGBA8I     = CXU::MakeTextureDataFormatEnumValue( 0x2D, EPixelDataLayout::RGBA, EBaseDataType::Byte,  4 ),
		RGBA8U     = CXU::MakeTextureDataFormatEnumValue( 0x2E, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4 ),
		RGBA8IN    = CXU::MakeTextureDataFormatEnumValue( 0x2F, EPixelDataLayout::RGBA, EBaseDataType::Byte,  4, eGPUDataFormatFlagNormalizedBit ),
		RGBA8UN    = CXU::MakeTextureDataFormatEnumValue( 0x31, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagNormalizedBit ),
		RGBA8SRGB  = CXU::MakeTextureDataFormatEnumValue( 0x32, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagSRGBBit ),
		RGBX8UN    = CXU::MakeTextureDataFormatEnumValue( 0x33, EPixelDataLayout::RGBA, EBaseDataType::Ubyte, 4, eGPUDataFormatFlagNormalizedBit ),

		R5G5B5A1   = CXU::MakeTextureDataFormatEnumValue( 0x41, EPixelDataLayout::RGBA, EBaseDataType::Ubyte,   2 ),
		R5G6B5     = CXU::MakeTextureDataFormatEnumValue( 0x42, EPixelDataLayout::RGB,  EBaseDataType::Ubyte,   2 ),
		R9G9B9E5   = CXU::MakeTextureDataFormatEnumValue( 0x43, EPixelDataLayout::RGBA, EBaseDataType::Uint32,  4 ),
		RGB10A2U   = CXU::MakeTextureDataFormatEnumValue( 0x44, EPixelDataLayout::RGBA, EBaseDataType::Uint32,  4 ),
		RGB10A2UN  = CXU::MakeTextureDataFormatEnumValue( 0x45, EPixelDataLayout::RGBA, EBaseDataType::Float32, 4, eGPUDataFormatFlagNormalizedBit ),
		R11G11B10F = CXU::MakeTextureDataFormatEnumValue( 0x46, EPixelDataLayout::RGB,  EBaseDataType::Float32, 4 ),

		D16UN      = CXU::MakeTextureDataFormatEnumValue( 0x47, EPixelDataLayout::Depth, EBaseDataType::Uint16,   2, eGPUDataFormatMaskDepthNorm ),
		D24UNS8U   = CXU::MakeTextureDataFormatEnumValue( 0x48, EPixelDataLayout::DS,    EBaseDataType::Uint24S8, 4, eGPUDataFormatMaskDepthStencilBit ),
		D24UNX8    = CXU::MakeTextureDataFormatEnumValue( 0x49, EPixelDataLayout::DS,    EBaseDataType::Uint24S8, 4, eGPUDataFormatFlagDepthBit ),
		X24S8U     = CXU::MakeTextureDataFormatEnumValue( 0x4A, EPixelDataLayout::DS,    EBaseDataType::Uint24S8, 4, eGPUDataFormatFlagStencilBit ),
		D32F       = CXU::MakeTextureDataFormatEnumValue( 0x4B, EPixelDataLayout::DS,    EBaseDataType::Float32,  4, eGPUDataFormatFlagDepthBit ),
		D32FS8     = CXU::MakeTextureDataFormatEnumValue( 0x4C, EPixelDataLayout::DS,    EBaseDataType::Float32,  8, eGPUDataFormatMaskDepthStencilBit ),

		BC1        = CXU::MakeTextureDataFormatEnumValue( 0xC0, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC1SRGB    = CXU::MakeTextureDataFormatEnumValue( 0xC1, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatMaskCompressedSRGB ),
		BC2        = CXU::MakeTextureDataFormatEnumValue( 0xC2, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC2SRGB    = CXU::MakeTextureDataFormatEnumValue( 0xC3, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatMaskCompressedSRGB ),
		BC3        = CXU::MakeTextureDataFormatEnumValue( 0xC4, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC3SRGB    = CXU::MakeTextureDataFormatEnumValue( 0xC5, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatMaskCompressedSRGB ),
		BC4IN      = CXU::MakeTextureDataFormatEnumValue( 0xC6, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC4UN      = CXU::MakeTextureDataFormatEnumValue( 0xC7, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC5IN      = CXU::MakeTextureDataFormatEnumValue( 0xC8, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC5UN      = CXU::MakeTextureDataFormatEnumValue( 0xC9, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC6HSF     = CXU::MakeTextureDataFormatEnumValue( 0xCA, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC6HUF     = CXU::MakeTextureDataFormatEnumValue( 0xCB, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC7        = CXU::MakeTextureDataFormatEnumValue( 0xCC, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatFlagCompressedBit ),
		BC7SRGB    = CXU::MakeTextureDataFormatEnumValue( 0xCD, EPixelDataLayout::S3TC, EBaseDataType::Undefined, 0, eGPUDataFormatMaskCompressedSRGB ),
	};

	enum class EVertexAttribFormat : vertex_attrib_format_value_t
	{
		Undefined = 0,

		F16       = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Float16 ),
		F32       = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Float32 ),
		I8        = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Byte    ),
		I16       = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Int16   ),
		I32       = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Int32   ),
		U8        = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Ubyte   ),
		U16       = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Uint16  ),
		U32       = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Uint32  ),
		I8N       = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Byte    , eGPUDataFormatFlagNormalizedBit ),
		I16N      = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Int16   , eGPUDataFormatFlagNormalizedBit ),
		U8N       = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Ubyte   , eGPUDataFormatFlagNormalizedBit ),
		U16N      = CXU::MakeVertexAttribFormatEnumValue( 1 , EBaseDataType::Uint16  , eGPUDataFormatFlagNormalizedBit ),
		Vec2F16   = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Float16 ),
		Vec2F32   = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Float32 ),
		Vec2I8    = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Byte    ),
		Vec2I16   = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Int16   ),
		Vec2I32   = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Int32   ),
		Vec2U8    = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Ubyte   ),
		Vec2U16   = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Uint16  ),
		Vec2U32   = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Uint32  ),
		Vec2I8N   = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Byte    , eGPUDataFormatFlagNormalizedBit ),
		Vec2I16N  = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Int16   , eGPUDataFormatFlagNormalizedBit ),
		Vec2U8N   = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Ubyte   , eGPUDataFormatFlagNormalizedBit ),
		Vec2U16N  = CXU::MakeVertexAttribFormatEnumValue( 2 , EBaseDataType::Uint16  , eGPUDataFormatFlagNormalizedBit ),
		Vec3F32   = CXU::MakeVertexAttribFormatEnumValue( 3 , EBaseDataType::Float32 ),
		Vec3I32   = CXU::MakeVertexAttribFormatEnumValue( 3 , EBaseDataType::Int32   ),
		Vec3U32   = CXU::MakeVertexAttribFormatEnumValue( 3 , EBaseDataType::Uint32  ),
		Vec4F16   = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Float16 ),
		Vec4F32   = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Float32 ),
		Vec4I8    = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Byte    ),
		Vec4I16   = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Int16   ),
		Vec4I32   = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Int32   ),
		Vec4U8    = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Ubyte   ),
		Vec4U16   = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Uint16  ),
		Vec4U32   = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Uint32  ),
		Vec4I8N   = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Byte    , eGPUDataFormatFlagNormalizedBit ),
		Vec4I16N  = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Int16   , eGPUDataFormatFlagNormalizedBit ),
		Vec4U8N   = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Ubyte   , eGPUDataFormatFlagNormalizedBit ),
		Vec4U16N  = CXU::MakeVertexAttribFormatEnumValue( 4 , EBaseDataType::Uint16  , eGPUDataFormatFlagNormalizedBit ),
	};

	using EShaderInputConstantFormat = EVertexAttribFormat;

	namespace CXU
	{

		CPPX_ATTR_NO_DISCARD inline constexpr uint16 GetBaseDataTypeByteSize( EBaseDataType pBaseType ) noexcept
		{
			return ( uint16 )( ( ( base_data_type_value_t )pBaseType >> 8 ) & 0xF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr cppx::bitmask<EGPUDataFormatFlags> GetBaseDataTypeFlags( EBaseDataType pBaseType ) noexcept
		{
			return cppx::make_bitmask<uint8>( ( base_data_type_value_t )pBaseType & 0xFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsBaseDataTypeFloatingPoint( EBaseDataType pBaseType ) noexcept
		{
			return GetBaseDataTypeFlags( pBaseType ).is_set( eGPUDataFormatFlagTypeFloatingPointBit );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsBaseDataTypeIntegral( EBaseDataType pBaseType ) noexcept
		{
			return GetBaseDataTypeFlags( pBaseType ).is_set( eGPUDataFormatFlagTypeIntegralBit );
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
			return ( EBaseDataType )( ( ( texture_format_value_t )pFormat >> 12 ) & 0xFFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr cppx::bitmask<EGPUDataFormatFlags> GetTextureFormatFlags( ETextureFormat pFormat ) noexcept
		{
			return cppx::make_bitmask<uint8>( ( ( texture_format_value_t )pFormat >> 4 ) & 0xFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr uint8 GetVertexAttribFormatComponentsNum( EVertexAttribFormat pFormat ) noexcept
		{
			return ( uint8 )( ( ( vertex_attrib_format_value_t )pFormat >> 20 ) & 0xF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr EBaseDataType GetVertexAttribFormatBaseDataType( EVertexAttribFormat pFormat ) noexcept
		{
			return ( EBaseDataType )( ( ( vertex_attrib_format_value_t )pFormat >> 8 ) & 0xFFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr cppx::bitmask<EGPUDataFormatFlags> GetVertexAttribFormatFlags( EVertexAttribFormat pFormat )
		{
			return cppx::make_bitmask<uint8>( ( vertex_attrib_format_value_t )pFormat & 0xFF );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsTextureFormatSRGB( ETextureFormat pFormat ) noexcept
		{
			const auto formatFlags = GetTextureFormatFlags( pFormat );
			return formatFlags.is_set( eGPUDataFormatFlagSRGBBit );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsVertexAttribFormatNormalized( EVertexAttribFormat pFormat ) noexcept
		{
			const auto formatFlags = GetVertexAttribFormatFlags( pFormat );
			return formatFlags.is_set( eGPUDataFormatFlagNormalizedBit );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsVertexAttribFormatValid( EVertexAttribFormat pFormat ) noexcept
		{
			return ( pFormat != EVertexAttribFormat::Undefined ) && ( pFormat != EVertexAttribFormat::Undefined );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr uint32 GetVertexAttribFormatByteSize( EVertexAttribFormat pFormat ) noexcept
		{
			Ic3DebugAssert( pFormat != EVertexAttribFormat::Undefined );
			return GetBaseDataTypeByteSize( GetVertexAttribFormatBaseDataType( pFormat ) ) * GetVertexAttribFormatComponentsNum( pFormat );
		}

	}

	namespace GCU
	{

		inline ETextureFormat GetTextureFormatForGraphicsPixelLayout( EPixelDataLayout pPixelLayout )
		{
			switch( pPixelLayout )
			{
				Ic3CaseReturn( EPixelDataLayout::Undefined , ETextureFormat::Undefined );
				Ic3CaseReturn( EPixelDataLayout::Alpha     , ETextureFormat::R8UN );
				Ic3CaseReturn( EPixelDataLayout::Depth     , ETextureFormat::R8UN );
				Ic3CaseReturn( EPixelDataLayout::DS        , ETextureFormat::RG8UN );
				Ic3CaseReturn( EPixelDataLayout::BGR       , ETextureFormat::BGRX8UN );
				Ic3CaseReturn( EPixelDataLayout::BGRA      , ETextureFormat::BGRA8UN );
				Ic3CaseReturn( EPixelDataLayout::Red       , ETextureFormat::R8UN );
				Ic3CaseReturn( EPixelDataLayout::RG        , ETextureFormat::RG8UN );
				Ic3CaseReturn( EPixelDataLayout::RGB       , ETextureFormat::BGRX8UN );
				Ic3CaseReturn( EPixelDataLayout::RGBA      , ETextureFormat::RGBA8UN );
				Ic3CaseReturn( EPixelDataLayout::S3TC      , ETextureFormat::BC3 );
			}

			return ETextureFormat::Undefined;
		}

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DATA_FORMATS_H__
