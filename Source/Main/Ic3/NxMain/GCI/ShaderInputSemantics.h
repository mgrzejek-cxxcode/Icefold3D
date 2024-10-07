
#ifndef __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
#define __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__

#include "CommonGCIDefs.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>

namespace Ic3
{

	using VertexAttributeKey = uint64;

	enum EShaderInputSemanticFlags : uint32
	{
		eShaderInputSemanticFlagPositionBit         = 0x00001,
		eShaderInputSemanticFlagNormalBit           = 0x00002,
		eShaderInputSemanticFlagTangentBit          = 0x00004,
		eShaderInputSemanticFlagBiTangentBit        = 0x00008,

		eShaderInputSemanticFlagTexCoord0Bit        = 0x00010,
		eShaderInputSemanticFlagTexCoord1Bit        = 0x00020,
		eShaderInputSemanticFlagTexCoord2Bit        = 0x00040,
		eShaderInputSemanticFlagTexCoord3Bit        = 0x00080,
		eShaderInputSemanticFlagTexCoord4Bit        = 0x00100,
		eShaderInputSemanticFlagTexCoord5Bit        = 0x00200,
		eShaderInputSemanticFlagTexCoord6Bit        = 0x00400,
		eShaderInputSemanticFlagTexCoord7Bit        = 0x00800,
		eShaderInputSemanticMaskTexCoordAll         = 0x00FF0,

		eShaderInputSemanticFlagFixedColorBit       = 0x01000,

		eShaderInputSemanticFlagBlendIndicesBit     = 0x02000,
		eShaderInputSemanticFlagBlendWeightsBit     = 0x04000,
		eShaderInputSemanticMaskSkinDataAll         = 0x06000,

		eShaderInputSemanticFlagInstanceMatrixBit   = 0x10000,
		eShaderInputSemanticFlagInstanceUserDataBit = 0x20000,
		eShaderInputSemanticMaskInstanceDataAll     = 0x30000,

		eShaderInputSemanticMaskStandardAttribsAll  = 0x37FFF,

		eShaderInputSemanticFlagCustomAttributeBit  = 0x40000,

		eShaderInputSemanticMaskAll                 = 0x7FFFF
	};

	namespace CxDef
	{

		IC3_ATTR_NO_DISCARD inline constexpr uint32 declareShaderInputSemanticID( EShaderInputSemanticFlags pSemanticFlags )
		{
			return static_cast<uint32>( pSemanticFlags );
		}

	}

	enum class EShaderInputSemanticID : uint32
	{
		Undefined        = 0,
		Position         = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagPositionBit   ),
		Normal           = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagNormalBit     ),
		Tangent          = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagTangentBit    ),
		BiTangent        = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagBiTangentBit  ),
		TexCoord0        = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagTexCoord0Bit  ),
		TexCoord1        = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagTexCoord1Bit  ),
		TexCoord2        = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagTexCoord2Bit  ),
		TexCoord3        = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagTexCoord3Bit  ),
		TexCoord4        = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagTexCoord4Bit  ),
		TexCoord5        = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagTexCoord5Bit  ),
		TexCoord6        = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagTexCoord6Bit  ),
		TexCoord7        = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagTexCoord7Bit  ),
		FixedColor       = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagFixedColorBit ),
		BlendIndices     = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagBlendIndicesBit ),
		BlendWeights     = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagBlendWeightsBit ),
		InstanceMatrix   = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagInstanceMatrixBit   ),
		InstanceUserData = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagInstanceUserDataBit ),
		Custom           = CxDef::declareShaderInputSemanticID( eShaderInputSemanticFlagCustomAttributeBit  ),
	};

	namespace CxDef
	{

		inline constexpr EShaderInputSemanticFlags getShaderInputSemanticIDFlags( EShaderInputSemanticID pSemanticID )
		{
			return static_cast<EShaderInputSemanticFlags>( pSemanticID );
		}

	}

	namespace GCU
	{

		IC3_ATTR_NO_DISCARD EShaderInputSemanticID getShaderInputSemanticIDFromName( const std::string_view & pSemanticName );

		IC3_ATTR_NO_DISCARD std::string_view getShaderInputSemanticNameFromID( EShaderInputSemanticID pSemanticID );

	}

	struct ShaderSemantics
	{
		EShaderInputSemanticID smtID = EShaderInputSemanticID::Undefined;

		uint32 smtIndex = 0u;

		std::string smtName {};

		ShaderSemantics() = default;

		ShaderSemantics( EShaderInputSemanticID pSemanticID )
		: smtID( pSemanticID )
		, smtName( GCU::getShaderInputSemanticNameFromID( smtID ) )
		{}

		ShaderSemantics( std::string pSemanticName, uint32 pSmtIndex = 0u )
		: smtID( GCU::getShaderInputSemanticIDFromName( pSemanticName ) )
		, smtName( std::move( pSemanticName ) )
		{}

		ShaderSemantics( StringView pSemanticName, uint32 pSmtIndex = 0u )
		: smtID( GCU::getShaderInputSemanticIDFromName( pSemanticName ) )
		, smtName( pSemanticName.str(), pSemanticName.length() )
		{}

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD bool operator==( const ShaderSemantics & pRhs ) const noexcept
		{
			return ( smtName == pRhs.smtName ) && ( smtIndex == pRhs.smtIndex );
		}

		IC3_ATTR_NO_DISCARD bool operator<( const ShaderSemantics & pRhs ) const noexcept
		{
			return ( smtName < pRhs.smtName ) || ( ( smtName == pRhs.smtName ) && ( smtIndex < pRhs.smtIndex ) );
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return ( smtID == EShaderInputSemanticID::Undefined ) || smtName.empty();
		}

		IC3_ATTR_NO_DISCARD bool valid() const noexcept
		{
			return !empty();
		}

		void clear()
		{
			smtID = EShaderInputSemanticID::Undefined;
			smtName.clear();
		}

		IC3_NXMAIN_API bool resolve() noexcept;

		IC3_NXMAIN_API_NO_DISCARD static ShaderSemantics resolveSemantics( const ShaderSemantics & pSemantics );

		IC3_NXMAIN_API_NO_DISCARD static ShaderSemantics fromVertexAttributeKey( VertexAttributeKey pAttributeKey, uint32 pSmtIndex = 0u );
	};

} // namespace Ic3

namespace std
{

	template <>
	struct hash<Ic3::ShaderSemantics>
	{
		size_t operator()( const Ic3::ShaderSemantics & pInput ) const noexcept
		{
			return hash<std::string>()( pInput.smtName );
		}
	};

}

#endif // __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
