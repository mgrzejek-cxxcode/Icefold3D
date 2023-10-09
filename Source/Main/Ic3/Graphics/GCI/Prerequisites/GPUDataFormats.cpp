
#include "../Prerequisites.h"

namespace Ic3::Graphics::GCI
{

//	IC3_EnumDefineTypeInfoGx( EBaseDataType )
//	{
//		IC3_EnumTypeInfoBegin( EBaseDataType );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Byte );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Ubyte );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Int16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Uint16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Int32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Uint32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Int64 );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Uint64 );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Float );
//		IC3_EnumTypeInfoRegisterClassConstant( EBaseDataType::Double );
//	}
//
//	IC3_EnumDefineTypeInfoGx( EIndexDataFormat )
//	{
//		IC3_EnumTypeInfoBegin( EIndexDataFormat );
//		IC3_EnumTypeInfoRegisterClassConstant( EIndexDataFormat::Uint16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EIndexDataFormat::Uint32 );
//	}
//
//	IC3_EnumDefineTypeInfoGx( ETextureFormat )
//	{
//		IC3_EnumTypeInfoBegin( ETextureFormat );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32G32_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32G32_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32G32_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32G32B32_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32G32B32_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32G32B32_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32G32B32A32_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32G32B32A32_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R32G32B32A32_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16_SNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16_SNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16B16A16_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16B16A16_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16B16A16_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16B16A16_SNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R16G16B16A16_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8_SNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8_SRGB );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8_SNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8_SRGB );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8_SNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8_SRGB );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8_SNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8_SRGB );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8A8_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8A8_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8A8_SNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8A8_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R8G8B8A8_SRGB );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8A8_SINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8A8_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8A8_SNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8A8_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::B8G8R8A8_SRGB );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R5G5B5A1 );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R5G6B5 );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R9G9B9E5 );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R10G10B10A2_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R10G10B10A2_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::R11G11B10_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::D16_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::D16_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::D16_UNORM_S8_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::D24_UNORM_S8_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::D24_UNORM_X8 );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::D32_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::D32_SFLOAT_S8_UINT );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::A8_UNORM );
//		IC3_EnumTypeInfoRegisterClassConstant( ETextureFormat::UNKNOWN );
//	}
//
//	IC3_EnumDefineTypeInfoGx( EVertexAttribFormat )
//	{
//		IC3_EnumTypeInfoBegin( EVertexAttribFormat );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::SBYTE );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::SINT16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::SINT32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::UBYTE );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::UINT16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::UINT32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::SBYTE_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::SINT16_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::UBYTE_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::UINT16_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_SBYTE );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_SINT16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_SINT32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_UBYTE );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_UINT16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_UINT32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_SBYTE_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_SINT16_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_UBYTE_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC2_UINT16_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_SBYTE );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_SINT16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_SINT32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_UBYTE );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_UINT16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_UINT32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_SBYTE_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_SINT16_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_UBYTE_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC3_UINT16_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_SFLOAT );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_SBYTE );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_SINT16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_SINT32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_UBYTE );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_UINT16 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_UINT32 );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_SBYTE_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_SINT16_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_UBYTE_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::VEC4_UINT16_NORM );
//		IC3_EnumTypeInfoRegisterClassConstant( EVertexAttribFormat::UNKNOWN );
//	}

} // namespace Ic3::Graphics::GCI
