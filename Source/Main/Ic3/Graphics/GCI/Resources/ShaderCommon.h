
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHADER_COMMON_H__
#define __IC3_GRAPHICS_GCI_SHADER_COMMON_H__

#include "../Prerequisites.h"
#include <cppx/byteArray.h>

namespace Ic3::Graphics::GCI
{

	Ic3GCIDeclareClassHandle( Shader );

	namespace CXU
	{

		CPPX_ATTR_NO_DISCARD inline constexpr uint32 SHMakeShaderType( native_uint pStageIndex )
		{
			return ( static_cast<uint32>( pStageIndex ) << 16 ) | SHMakeShaderStageBit( static_cast< uint32 >( pStageIndex ) );
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
		Unknown      = 0,
		GSVertex     = CXU::SHMakeShaderType( kShaderStageIndexGraphicsVertex     ),
		GSTessHull   = CXU::SHMakeShaderType( kShaderStageIndexGraphicsTessHull   ),
		GSTessDomain = CXU::SHMakeShaderType( kShaderStageIndexGraphicsTessDomain ),
		GSGeometry   = CXU::SHMakeShaderType( kShaderStageIndexGraphicsGeometry   ),
		GSPixel      = CXU::SHMakeShaderType( kShaderStageIndexGraphicsPixel      ),
		CSCompute    = CXU::SHMakeShaderType( kShaderStageIndexCompute            ),
	};

	struct alignas( 32 ) ShaderBinary
	{
		static constexpr size_t dataBufferFixedSize = 12;

		uint64 driverSpecificID;

		uint64 driverSpecificFormatTag;

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
	struct ShaderCreateInfo : public GfxObjectCreateInfo
	{
		EShaderType shaderType = EShaderType::Unknown;

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

	namespace CXU
	{

		CPPX_ATTR_NO_DISCARD inline constexpr uint32 SHGetShaderStageIndex( EShaderType pShaderType )
		{
			return ( static_cast<uint32>( pShaderType ) >> 16 & 0xFFFF ) - kShaderStageIndexBase;
		}

		CPPX_ATTR_NO_DISCARD inline constexpr uint32 SHGetShaderStageBit( EShaderType pShaderType )
		{
			return static_cast<uint32>( pShaderType ) & 0xFFFF;
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool SHIsShaderTypeGraphics( EShaderType pShaderType )
		{
			return SHGetShaderStageIndex( pShaderType ) <= kShaderStageIndexMaxGraphics;
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool SHIsShaderTypeValid( EShaderType pShaderType )
		{
			return SHGetShaderStageIndex( pShaderType ) <= kShaderStageIndexMax;
		}

		CPPX_ATTR_NO_DISCARD inline constexpr EShaderType SHGetShaderTypeFromStageIndex( native_uint pStageIndex )
		{
			return SHIsShaderStageIndexValid( pStageIndex ) ? static_cast<EShaderType>( SHMakeShaderType( pStageIndex ) ) : EShaderType::Unknown;
		}
		
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SHADER_COMMON_H__
