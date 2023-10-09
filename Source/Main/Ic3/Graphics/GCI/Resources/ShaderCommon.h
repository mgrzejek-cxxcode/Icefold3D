
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHADER_COMMON_H__
#define __IC3_GRAPHICS_GCI_SHADER_COMMON_H__

#include "../Prerequisites.h"
#include <Ic3/Cppx/ByteArray.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( Shader );

	namespace CxDefs
	{

		IC3_ATTR_NO_DISCARD inline constexpr uint32 makeShaderType( native_uint pStageIndex )
		{
			return ( static_cast<uint32>( pStageIndex ) << 16 ) | CxDefs::makeShaderStageBit( static_cast< uint32 >( pStageIndex ) );
		}

	}

	/// @brief
	enum EShaderCreateFlags : uint32
	{
		E_SHADER_CREATE_FLAG_DEBUG_BIT                = 0x0001,
		E_SHADER_CREATE_FLAG_OPTIMIZATION_DISABLE_BIT = 0x0010,
		E_SHADER_CREATE_FLAG_OPTIMIZATION_L0_BIT      = 0x0020,
		E_SHADER_CREATE_FLAG_OPTIMIZATION_L1_BIT      = 0x0040,
		E_SHADER_CREATE_FLAG_OPTIMIZATION_MAX_BIT     = 0x0080,
		E_SHADER_CREATE_FLAGS_DEFAULT = 0
	};

	/// @brief
	enum class EShaderType : uint32
	{
		Unknown    = 0,
		GSVertex   = CxDefs::makeShaderType( E_SHADER_STAGE_INDEX_GRAPHICS_VERTEX        ),
		GSHull     = CxDefs::makeShaderType( E_SHADER_STAGE_INDEX_GRAPHICS_HULL          ),
		GSDomain   = CxDefs::makeShaderType( E_SHADER_STAGE_INDEX_GRAPHICS_DOMAIN        ),
		GSGeometry = CxDefs::makeShaderType( E_SHADER_STAGE_INDEX_GRAPHICS_GEOMETRY      ),
		GSPixel    = CxDefs::makeShaderType( E_SHADER_STAGE_INDEX_GRAPHICS_PIXEL         ),
		CSCompute  = CxDefs::makeShaderType( E_SHADER_STAGE_INDEX_COMPUTE                ),
	};

	struct alignas( 32 ) ShaderBinary
	{
		static constexpr size_t sDataBufferFixedSize = 12;

		uint64 driverSpecificID;
		uint64 driverSpecificType;
		uint32 dataSizeInBytes;
		byte dataBuffer[sDataBufferFixedSize];

		ShaderBinary() = default;

		ShaderBinary( std::nullptr_t )
		: dataSizeInBytes( 0 )
		{}

		IC3_ATTR_NO_DISCARD explicit operator bool() const
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD bool empty() const
		{
			return dataSizeInBytes == 0;
		}

		/// @brief Allocates a ShaderBinary object capable of storing shader binary data of the specified size.
		/// @return An std::unique_ptr holding the created ShaderBinary.
		/// @notes Defined in shader.cpp file.
		IC3_GRAPHICS_GCI_API_NO_DISCARD static std::unique_ptr<ShaderBinary> create( size_t pBinarySize );
	};

	/// @brief
	struct ShaderCreateInfo
	{
		EShaderType shaderType = EShaderType::Unknown;
		UniqueGPUObjectName shaderName;
		Bitmask<EShaderCreateFlags> createFlags = E_SHADER_CREATE_FLAGS_DEFAULT;
		std::unique_ptr<ShaderBinary> shaderBinary;
		DynamicByteArray shaderSource;
		ReadOnlyMemoryView shaderSourceView;
		const char * entryPointName = nullptr;

		explicit operator bool() const noexcept
		{
			return ( shaderType != EShaderType::Unknown ) && ( shaderBinary || !shaderSource.empty() || !shaderSourceView.empty() );
		}
	};

	namespace CxDefs
	{

		IC3_ATTR_NO_DISCARD inline constexpr bool isShaderStageIndexValid( native_uint pStageIndex )
		{
			return ( pStageIndex >= E_SHADER_STAGE_INDEX_BASE ) && ( pStageIndex <= E_SHADER_STAGE_INDEX_MAX );
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isShaderStageIndexValidGraphics( native_uint pStageIndex )
		{
			return ( pStageIndex >= E_SHADER_STAGE_INDEX_BASE ) && ( pStageIndex <= E_SHADER_STAGE_INDEX_MAX_GRAPHICS );
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint32 getShaderStageIndex( EShaderType pShaderType )
		{
			return ( static_cast<uint32>( pShaderType ) >> 16 & 0xFFFF ) - E_SHADER_STAGE_INDEX_BASE;
		}

		IC3_ATTR_NO_DISCARD inline constexpr uint32 getShaderStageBit( EShaderType pShaderType )
		{
			return static_cast<uint32>( pShaderType ) & 0xFFFF;
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isShaderTypeGraphics( EShaderType pShaderType )
		{
			return getShaderStageIndex( pShaderType ) <= E_SHADER_STAGE_INDEX_MAX_GRAPHICS;
		}

		IC3_ATTR_NO_DISCARD inline constexpr bool isShaderTypeValid( EShaderType pShaderType )
		{
			return getShaderStageIndex( pShaderType ) <= E_SHADER_STAGE_INDEX_MAX;
		}

		IC3_ATTR_NO_DISCARD inline constexpr EShaderType getShaderTypeFromStageIndex( native_uint pStageIndex )
		{
			return isShaderStageIndexValid( pStageIndex ) ? static_cast<EShaderType>( CxDefs::makeShaderType( pStageIndex ) ) : EShaderType::Unknown;
		}
		
	}

	namespace rcutil
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD EShaderType getShaderObjectType( Shader & pShader );

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 getShaderObjectStageIndex( Shader & pShader );

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 getShaderObjectStageBit( Shader & pShader );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SHADER_COMMON_H__
