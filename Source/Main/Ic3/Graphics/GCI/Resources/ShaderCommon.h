
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHADER_COMMON_H__
#define __IC3_GRAPHICS_GCI_SHADER_COMMON_H__

#include "../Prerequisites.h"
#include <Ic3/Cppx/ByteArray.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( Shader );

	namespace CxDef
	{

		IC3_ATTR_NO_DISCARD inline constexpr uint32 makeShaderType( native_uint pStageIndex )
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
		static constexpr size_t sDataBufferFixedSize = 12;

		uint64 mDriverSpecificID;

		uint64 mDriverSpecificType;

		uint32 mDataSizeInBytes;

		byte mDataBuffer[sDataBufferFixedSize];

		ShaderBinary() = default;

		ShaderBinary( std::nullptr_t )
		: mDataSizeInBytes( 0 )
		{}

		IC3_ATTR_NO_DISCARD explicit operator bool() const
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD bool empty() const
		{
			return mDataSizeInBytes == 0;
		}

		/// @brief Allocates a ShaderBinary object capable of storing shader binary data of the specified size.
		/// @return An std::unique_ptr holding the created ShaderBinary.
		/// @notes Defined in shader.cpp file.
		IC3_GRAPHICS_GCI_API_NO_DISCARD static std::unique_ptr<ShaderBinary> create( size_t pBinarySize );
	};

	/// @brief
	struct ShaderCreateInfo
	{
		EShaderType mShaderType = EShaderType::Unknown;
		UniqueGPUObjectName mShaderName;
		TBitmask<EShaderCreateFlags> mCreateFlags = eShaderCreateFlagsDefault;
		std::unique_ptr<ShaderBinary> mShaderBinary;
		Cppx::DynamicByteArray mShaderSource;
		Cppx::ReadOnlyMemoryView mShaderSourceView;
		const char * mEntryPointName = nullptr;

		explicit operator bool() const noexcept
		{
			return ( mShaderType != EShaderType::Unknown ) && (mShaderBinary || !mShaderSource.empty() || !mShaderSourceView.empty() );
		}
	};

	namespace CxDef
	{

		IC3_ATTR_NO_DISCARD inline constexpr bool isShaderStageIndexValid( native_uint pStageIndex )
		{
			return ( pStageIndex >= eShaderStageIndexBase ) && ( pStageIndex <= eShaderStageIndexMax );
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isShaderStageIndexValidGraphics( native_uint pStageIndex )
		{
			return ( pStageIndex >= eShaderStageIndexBase ) && ( pStageIndex <= eShaderStageIndexMaxGraphics );
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint32 getShaderStageIndex( EShaderType pShaderType )
		{
			return ( static_cast<uint32>( pShaderType ) >> 16 & 0xFFFF ) - eShaderStageIndexBase;
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint32 getShaderStageBit( EShaderType pShaderType )
		{
			return static_cast<uint32>( pShaderType ) & 0xFFFF;
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isShaderTypeGraphics( EShaderType pShaderType )
		{
			return getShaderStageIndex( pShaderType ) <= eShaderStageIndexMaxGraphics;
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isShaderTypeValid( EShaderType pShaderType )
		{
			return getShaderStageIndex( pShaderType ) <= eShaderStageIndexMax;
		}

		IC3_ATTR_NO_DISCARD inline constexpr EShaderType getShaderTypeFromStageIndex( native_uint pStageIndex )
		{
			return isShaderStageIndexValid( pStageIndex ) ? static_cast<EShaderType>( CxDef::makeShaderType( pStageIndex ) ) : EShaderType::Unknown;
		}
		
	}

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD EShaderType getShaderObjectType( Shader & pShader );

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 getShaderObjectStageIndex( Shader & pShader );

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 getShaderObjectStageBit( Shader & pShader );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SHADER_COMMON_H__
