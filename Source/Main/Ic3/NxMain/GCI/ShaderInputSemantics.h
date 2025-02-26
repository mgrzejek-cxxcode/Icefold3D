
#ifndef __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
#define __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__

#include "CommonGCIDefs.h"
#include <cppx/immutableString.h>
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>

namespace Ic3
{

	enum ESystemAttributeSemanticFlags : uint32
	{
		eSystemAttributeSemanticFlagPositionBit         = 0x00001,
		eSystemAttributeSemanticFlagNormalBit           = 0x00002,
		eSystemAttributeSemanticFlagTangentBit          = 0x00004,
		eSystemAttributeSemanticFlagBiTangentBit        = 0x00008,
		eSystemAttributeSemanticFlagTexCoord0Bit        = 0x00010,
		eSystemAttributeSemanticFlagTexCoord1Bit        = 0x00020,
		eSystemAttributeSemanticFlagTexCoord2Bit        = 0x00040,
		eSystemAttributeSemanticFlagTexCoord3Bit        = 0x00080,
		eSystemAttributeSemanticFlagTexCoord4Bit        = 0x00100,
		eSystemAttributeSemanticFlagTexCoord5Bit        = 0x00200,
		eSystemAttributeSemanticFlagTexCoord6Bit        = 0x00400,
		eSystemAttributeSemanticFlagTexCoord7Bit        = 0x00800,
		eSystemAttributeSemanticMaskTexCoordAll         = 0x00FF0,
		eSystemAttributeSemanticFlagFixedColorBit       = 0x01000,
		eSystemAttributeSemanticFlagBlendIndicesBit     = 0x02000,
		eSystemAttributeSemanticFlagBlendWeightsBit     = 0x04000,
		eSystemAttributeSemanticMaskSkinDataAll         = 0x06000,
		eSystemAttributeSemanticFlagInstanceMatrixBit   = 0x10000,
		eSystemAttributeSemanticFlagInstanceUserDataBit = 0x20000,
		eSystemAttributeSemanticMaskInstanceDataAll     = 0x30000,
		eSystemAttributeSemanticMaskStandardAttribsAll  = 0x37FFF,
		eSystemAttributeSemanticFlagCustomAttributeBit  = 0x40000,
		eSystemAttributeSemanticMaskAll                 = 0x7FFFF,

		eSystemAttributeSemanticMaskTexCoord01Packed =
			eSystemAttributeSemanticFlagTexCoord0Bit | eSystemAttributeSemanticFlagTexCoord1Bit,

		eSystemAttributeSemanticMaskTexCoord23Packed =
			eSystemAttributeSemanticFlagTexCoord2Bit | eSystemAttributeSemanticFlagTexCoord3Bit,
	};

	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD cppx::bitmask<ESystemAttributeSemanticFlags> GetAttributeSystemSemanticFlagsFromName(
				const cppx::string_view & pSemanticName );

		CPPX_ATTR_NO_DISCARD cppx::string_view GetStandardSemanticNameFromSystemFlags(
				cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags );

		CPPX_ATTR_NO_DISCARD inline bool IsStandardShaderInputAttribute( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags )
		{
			return // Standard attributes, by default are single-slot attributes with unique semantics.
					( pSystemSemanticFlags.is_set_any_of( eSystemAttributeSemanticMaskStandardAttribsAll ) && ( pSystemSemanticFlags.count_bits() == 1 ) ) ||
					// Exception: packed texture coordinates, where a single attribute holds two set of texture coords.
					( ( pSystemSemanticFlags & eSystemAttributeSemanticMaskTexCoordAll ).count_bits() == 2 );
		}

	}

	struct ShaderSemantics
	{
		cppx::immutable_string semanticName {};

		uint32 semanticIndex = 0u;

		cppx::bitmask<ESystemAttributeSemanticFlags> systemSemanticFlags = 0;

		ShaderSemantics() = default;

		ShaderSemantics( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags )
		: semanticName( GCU::GetStandardSemanticNameFromSystemFlags( pSystemSemanticFlags ) )
		, systemSemanticFlags( pSystemSemanticFlags )
		{}

		ShaderSemantics(
				cppx::immutable_string pSemanticName,
				uint32 pSemanticGroupIndex = 0u,
				cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags = 0 )
		: semanticName( std::move( pSemanticName ) )
		, semanticIndex( pSemanticGroupIndex )
		, systemSemanticFlags( pSystemSemanticFlags ? pSystemSemanticFlags : GCU::GetAttributeSystemSemanticFlagsFromName( semanticName.strView() ) )
		{}

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool operator==( const ShaderSemantics & pRhs ) const noexcept
		{
			return ( semanticName == pRhs.semanticName ) && ( semanticIndex == pRhs.semanticIndex );
		}

		CPPX_ATTR_NO_DISCARD bool operator<( const ShaderSemantics & pRhs ) const noexcept
		{
			return ( semanticName < pRhs.semanticName ) || ( ( semanticName == pRhs.semanticName ) && ( semanticIndex < pRhs.semanticIndex ) );
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return systemSemanticFlags.empty() || semanticName.empty();
		}

		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept
		{
			return !IsEmpty();
		}

		void Clear()
		{
			semanticName.clear();
			semanticIndex = 0;
			systemSemanticFlags = 0;
		}

		IC3_NXMAIN_API bool Resolve() noexcept;

		IC3_NXMAIN_API_NO_DISCARD static ShaderSemantics ResolveSemantics( const ShaderSemantics & pSemantics );

		IC3_NXMAIN_API_NO_DISCARD static ShaderSemantics FromVertexAttributeKey(
				vertex_attribute_key_value_t pAttributeKeyValue,
				uint32 pSemanticIndex = 0u );
	};

} // namespace Ic3

namespace std
{

	template <>
	struct hash<Ic3::ShaderSemantics>
	{
		size_t operator()( const Ic3::ShaderSemantics & pInput ) const noexcept
		{
			return hash<std::string>()( pInput.semanticName.str() );
		}
	};

}

#endif // __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
