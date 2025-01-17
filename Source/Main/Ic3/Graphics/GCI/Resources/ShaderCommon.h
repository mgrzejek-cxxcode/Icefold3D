
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHADER_COMMON_H__
#define __IC3_GRAPHICS_GCI_SHADER_COMMON_H__

#include "../Prerequisites.h"
#include <cppx/byteArray.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( Shader );

	namespace CxDef
	{

		CPPX_ATTR_NO_DISCARD inline constexpr uint32 makeShaderType( native_uint pStageIndex )
		{
			return ( static_cast<uint32>( pStageIndex ) << 16 ) | CxDef::makeShaderStageBit( static_cast< uint32 >( pStageIndex ) );
		}

	}

	/// @brief
	enum EShaderCreateFlags : uint32
	{
		eShaderCreateFlagDebugBit               = 0x0001,
		eShaderCreateFlagOptimizationDisableBit = 0x0010,
		eShaderCreateFlagOptimizationL0Bit      = 0x0020,
		eShaderCreateFlagOptimizationL1Bit      = 0x0040,
		eShaderCreateFlagOptimizationMaxBit     = 0x0080,

		eShaderCreateFlagsDefault = 0
	};

	/// @brief
	enum class EShaderType : uint32
	{
		Unknown    = 0,
		GSVertex   = CxDef::makeShaderType( eShaderStageIndexGraphicsVertex   ),
		GSHull     = CxDef::makeShaderType( eShaderStageIndexGraphicsHull     ),
		GSDomain   = CxDef::makeShaderType( eShaderStageIndexGraphicsDomain   ),
		GSGeometry = CxDef::makeShaderType( eShaderStageIndexGraphicsGeometry ),
		GSPixel    = CxDef::makeShaderType( eShaderStageIndexGraphicsPixel    ),
		CSCompute  = CxDef::makeShaderType( eShaderStageIndexCompute          ),
	};

	struct alignas( 32 ) ShaderBinary
	{
		static constexpr size_t dataBufferFixedSize = 12;

		uint64 driverSpecificID;

		uint64 driverSpecificType;

		uint32 dataSizeInBytes;

		byte dataBuffer[dataBufferFixedSize];

		ShaderBinary() = default;

		ShaderBinary( std::nullptr_t )
		: dataSizeInBytes( 0 )
		{}

		CPPX_ATTR_NO_DISCARD explicit operator bool() const
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const
		{
			return dataSizeInBytes == 0;
		}

		/// @brief Allocates a ShaderBinary object capable of storing shader binary data of the specified size.
		/// @return An std::unique_ptr holding the created ShaderBinary.
		/// @notes Defined in shader.cpp file.
		IC3_GRAPHICS_GCI_API_NO_DISCARD static std::unique_ptr<ShaderBinary> Create( size_t pBinarySize );
	};

	/// @brief
	struct ShaderCreateInfo
	{
		EShaderType shaderType = EShaderType::Unknown;

		GfxObjectName shaderName;

		cppx::bitmask<EShaderCreateFlags> createFlags = eShaderCreateFlagsDefault;

		std::unique_ptr<ShaderBinary> shaderBinary;

		cppx::dynamic_byte_array shaderSource;

		cppx::read_only_memory_view shaderSourceView;

		cppx::string_view entryPointName;

		explicit operator bool() const noexcept
		{
			return ( shaderType != EShaderType::Unknown ) && ( shaderBinary || !shaderSource.empty() || !shaderSourceView.empty() );
		}
	};

	namespace CxDef
	{

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsShaderStageIndexValid( native_uint pStageIndex )
		{
			return ( pStageIndex >= eShaderStageIndexBase ) && ( pStageIndex <= eShaderStageIndexMax );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsShaderStageIndexValidGraphics( native_uint pStageIndex )
		{
			return ( pStageIndex >= eShaderStageIndexBase ) && ( pStageIndex <= eShaderStageIndexMaxGraphics );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr uint32 GetShaderStageIndex( EShaderType pShaderType )
		{
			return ( static_cast<uint32>( pShaderType ) >> 16 & 0xFFFF ) - eShaderStageIndexBase;
		}

		CPPX_ATTR_NO_DISCARD inline constexpr uint32 GetShaderStageBit( EShaderType pShaderType )
		{
			return static_cast<uint32>( pShaderType ) & 0xFFFF;
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsShaderTypeGraphics( EShaderType pShaderType )
		{
			return GetShaderStageIndex( pShaderType ) <= eShaderStageIndexMaxGraphics;
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool IsShaderTypeValid( EShaderType pShaderType )
		{
			return GetShaderStageIndex( pShaderType ) <= eShaderStageIndexMax;
		}

		CPPX_ATTR_NO_DISCARD inline constexpr EShaderType GetShaderTypeFromStageIndex( native_uint pStageIndex )
		{
			return IsShaderStageIndexValid( pStageIndex ) ? static_cast<EShaderType>( CxDef::makeShaderType( pStageIndex ) ) : EShaderType::Unknown;
		}
		
	}

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD EShaderType GetShaderObjectType( Shader & pShader );

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 GetShaderObjectStageIndex( Shader & pShader );

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 GetShaderObjectStageBit( Shader & pShader );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SHADER_COMMON_H__
