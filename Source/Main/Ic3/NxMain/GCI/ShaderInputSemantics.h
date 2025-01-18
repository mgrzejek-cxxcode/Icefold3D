
#ifndef __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
#define __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__

#include "CommonGCIDefs.h"
#include <Ic3/Cppx/ImmutableString.h>
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

		IC3_ATTR_NO_DISCARD TBitmask<ESystemAttributeSemanticFlags> getAttributeSystemSemanticFlagsFromName(
				const StringView & pSemanticName );

		IC3_ATTR_NO_DISCARD StringView getStandardSemanticNameFromSystemFlags(
				TBitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags );

		IC3_ATTR_NO_DISCARD inline bool isStandardShaderInputAttribute( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags )
		{
			return // Standard attributes, by default are single-slot attributes with unique semantics.
					( pSysSmtFlags.isSetAnyOf( eSystemAttributeSemanticMaskStandardAttribsAll ) && ( pSysSmtFlags.countBits() == 1 ) ) ||
					// Exception: packed texture coordinates, where a single attribute holds two set of texture coords.
					( ( pSysSmtFlags & eSystemAttributeSemanticMaskTexCoordAll ).countBits() == 2 );
		}

	}

	struct ShaderSemantics
	{
		Cppx::ImmutableString semanticName {};

		uint32 semanticIndex = 0u;

		TBitmask<ESystemAttributeSemanticFlags> systemSemanticFlags = 0;

		ShaderSemantics() = default;

		ShaderSemantics( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags )
		: semanticName( GCU::getStandardSemanticNameFromSystemFlags( pSysSmtFlags ) )
		, systemSemanticFlags( pSysSmtFlags )
		{}

		ShaderSemantics(
			Cppx::ImmutableString pSemanticName,
			uint32 pSemanticGroupIndex = 0u,
			TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags = 0 )
		: semanticName( std::move( pSemanticName ) )
		, semanticIndex( pSemanticGroupIndex )
		, systemSemanticFlags( pSysSmtFlags ? pSysSmtFlags : GCU::getAttributeSystemSemanticFlagsFromName( pSemanticName.strView() ) )
		{}

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD bool operator==( const ShaderSemantics & pRhs ) const noexcept
		{
			return ( semanticName == pRhs.semanticName ) && ( semanticIndex == pRhs.semanticIndex );
		}

		IC3_ATTR_NO_DISCARD bool operator<( const ShaderSemantics & pRhs ) const noexcept
		{
			return ( semanticName < pRhs.semanticName ) || ( ( semanticName == pRhs.semanticName ) && ( semanticIndex < pRhs.semanticIndex ) );
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return systemSemanticFlags.empty() || semanticName.empty();
		}

		IC3_ATTR_NO_DISCARD bool valid() const noexcept
		{
			return !empty();
		}

		void clear()
		{
			semanticName.clear();
			semanticIndex = 0;
			systemSemanticFlags = 0;
		}

		IC3_NXMAIN_API bool resolve() noexcept;

		IC3_NXMAIN_API_NO_DISCARD static ShaderSemantics resolveSemantics( const ShaderSemantics & pSemantics );

		// IC3_NXMAIN_API_NO_DISCARD static ShaderSemantics fromVertexAttributeKey( VertexAttributeKey pAttributeKey, uint32 pSmtIndex = 0u );
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
