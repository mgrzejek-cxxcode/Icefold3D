
#ifndef __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
#define __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__

#include "CommonGCIDefs.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>

namespace Ic3
{

	enum EShaderInputSemanticFlags : uint32
	{
		E_SHADER_INPUT_SEMANTIC_FLAG_POSITION_BIT    =    0x01,
		E_SHADER_INPUT_SEMANTIC_FLAG_NORMAL_BIT      =    0x02,
		E_SHADER_INPUT_SEMANTIC_FLAG_TANGENT_BIT     =    0x04,
		E_SHADER_INPUT_SEMANTIC_FLAG_FIXED_COLOR_BIT =    0x08,
		E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_0_BIT =   0x010,
		E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_1_BIT =   0x020,
		E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_2_BIT =   0x040,
		E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_3_BIT =   0x080,
		E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_4_BIT =   0x100,
		E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_5_BIT =   0x200,
		E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_6_BIT =   0x400,
		E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_7_BIT =   0x800,
		E_SHADER_INPUT_SEMANTIC_MASK_TEX_COORD_ALL   =   0xFF0,
		E_SHADER_INPUT_SEMANTIC_FLAG_RESERVED_13_BIT =  0x1000,
		E_SHADER_INPUT_SEMANTIC_FLAG_RESERVED_14_BIT =  0x2000,
		E_SHADER_INPUT_SEMANTIC_FLAG_RESERVED_15_BIT =  0x4000,
		E_SHADER_INPUT_SEMANTIC_MASK_RESERVED_ALL    =  0x7000,
		E_SHADER_INPUT_SEMANTIC_FLAG_INSTANCE_0_BIT  = 0x10000,
		E_SHADER_INPUT_SEMANTIC_FLAG_INSTANCE_1_BIT  = 0x20000,
		E_SHADER_INPUT_SEMANTIC_MASK_INSTANCE_ALL    = 0x30000,
		E_SHADER_INPUT_SEMANTIC_MASK_ALL             = 0x37FFF,

		E_SHADER_INPUT_SEMANTIC_MASK_INSTANCE_0_OVERLAP =
				E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_3_BIT |
				E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_4_BIT |
				E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_5_BIT |
				E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_6_BIT,

		E_SHADER_INPUT_SEMANTIC_MASK_INSTANCE_1_OVERLAP =
				E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_7_BIT |
				E_SHADER_INPUT_SEMANTIC_FLAG_RESERVED_13_BIT |
				E_SHADER_INPUT_SEMANTIC_FLAG_RESERVED_14_BIT |
				E_SHADER_INPUT_SEMANTIC_FLAG_RESERVED_15_BIT,
	};

	namespace CxDef
	{

		inline constexpr uint32 declareShaderInputSemanticID( EShaderInputSemanticFlags pSemanticFlags )
		{
			return static_cast<uint32>( pSemanticFlags );
		}

		inline constexpr uint64 declareStandardVertexAttributeID(
				uint16 pBaseIndex,
				uint16 pComponentsNum,
				GCI::EVertexAttribFormat pBaseFormat,
				Bitmask<EShaderInputSemanticFlags> pSemanticFlags )
		{
			return static_cast<uint64>(
					( ( ( uint64 )pBaseIndex & 0xF ) << 60 ) |
					( ( ( uint64 )pComponentsNum & 0xF ) << 56 ) |
					( ( ( uint64 )pBaseFormat & QLimits<uint32>::maxValue ) << 24 ) |
					( ( uint64 )pSemanticFlags & E_SHADER_INPUT_SEMANTIC_MASK_ALL ) );
		}

	}

	enum class EShaderInputSemanticID : uint32
	{
		Undefined  = 0,
		Custom     = static_cast<uint32>( E_SHADER_INPUT_SEMANTIC_MASK_ALL ) ^ QLimits<uint32>::maxValue,
		Position   = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_POSITION_BIT    ),
		Normal     = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_NORMAL_BIT      ),
		Tangent    = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_TANGENT_BIT     ),
		FixedColor = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_FIXED_COLOR_BIT ),
		TexCoord0  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_0_BIT ),
		TexCoord1  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_1_BIT ),
		TexCoord2  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_2_BIT ),
		TexCoord3  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_3_BIT ),
		TexCoord4  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_4_BIT ),
		TexCoord5  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_5_BIT ),
		TexCoord6  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_6_BIT ),
		TexCoord7  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_7_BIT ),
		Instance0  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_INSTANCE_0_BIT  ),
		Instance1  = CxDef::declareShaderInputSemanticID( E_SHADER_INPUT_SEMANTIC_FLAG_INSTANCE_1_BIT  ),
	};

	enum class EStandardVertexAttributeID : uint64
	{
		Undefined  = 0,
		Position   = CxDef::declareStandardVertexAttributeID(  0, 1, GCI::EVertexAttribFormat::Vec3F32, E_SHADER_INPUT_SEMANTIC_FLAG_POSITION_BIT ),
		Normal     = CxDef::declareStandardVertexAttributeID(  1, 1, GCI::EVertexAttribFormat::Vec3F32, E_SHADER_INPUT_SEMANTIC_FLAG_NORMAL_BIT ),
		Tangent    = CxDef::declareStandardVertexAttributeID(  2, 1, GCI::EVertexAttribFormat::Vec3F32, E_SHADER_INPUT_SEMANTIC_FLAG_TANGENT_BIT ),
		FixedColor = CxDef::declareStandardVertexAttributeID(  4, 1, GCI::EVertexAttribFormat::Vec4F32, E_SHADER_INPUT_SEMANTIC_FLAG_FIXED_COLOR_BIT ),
		TexCoord0  = CxDef::declareStandardVertexAttributeID(  5, 1, GCI::EVertexAttribFormat::Vec2F32, E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_0_BIT ),
		TexCoord1  = CxDef::declareStandardVertexAttributeID(  6, 1, GCI::EVertexAttribFormat::Vec2F32, E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_1_BIT ),
		TexCoord2  = CxDef::declareStandardVertexAttributeID(  7, 1, GCI::EVertexAttribFormat::Vec2F32, E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_2_BIT ),
		TexCoord3  = CxDef::declareStandardVertexAttributeID(  8, 1, GCI::EVertexAttribFormat::Vec2F32, E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_3_BIT ),
		TexCoord4  = CxDef::declareStandardVertexAttributeID(  9, 1, GCI::EVertexAttribFormat::Vec2F32, E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_4_BIT ),
		TexCoord5  = CxDef::declareStandardVertexAttributeID( 10, 1, GCI::EVertexAttribFormat::Vec2F32, E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_5_BIT ),
		TexCoord6  = CxDef::declareStandardVertexAttributeID( 11, 1, GCI::EVertexAttribFormat::Vec2F32, E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_6_BIT ),
		TexCoord7  = CxDef::declareStandardVertexAttributeID( 12, 1, GCI::EVertexAttribFormat::Vec2F32, E_SHADER_INPUT_SEMANTIC_FLAG_TEX_COORD_7_BIT ),
		Reserved13 = CxDef::declareStandardVertexAttributeID( 13, 1, GCI::EVertexAttribFormat::Vec4F32, E_SHADER_INPUT_SEMANTIC_FLAG_RESERVED_13_BIT ),
		Reserved14 = CxDef::declareStandardVertexAttributeID( 14, 1, GCI::EVertexAttribFormat::Vec4F32, E_SHADER_INPUT_SEMANTIC_FLAG_RESERVED_14_BIT ),
		Reserved15 = CxDef::declareStandardVertexAttributeID( 15, 1, GCI::EVertexAttribFormat::Vec4F32, E_SHADER_INPUT_SEMANTIC_FLAG_RESERVED_15_BIT ),
		Instance0  = CxDef::declareStandardVertexAttributeID(  8, 4, GCI::EVertexAttribFormat::Vec4F32, E_SHADER_INPUT_SEMANTIC_FLAG_INSTANCE_0_BIT ),
		Instance1  = CxDef::declareStandardVertexAttributeID( 12, 4, GCI::EVertexAttribFormat::Vec4F32, E_SHADER_INPUT_SEMANTIC_FLAG_INSTANCE_1_BIT ),
	};

	namespace CxDef
	{

		inline uint16 getStandardVertexAttributeBaseIndex( EStandardVertexAttributeID pStandardAttributeID )
		{
			return static_cast<uint16>( ( ( uint64 )pStandardAttributeID >> 60 ) & 0xF );
		}

		inline uint16 getStandardVertexAttributeComponentsNum( EStandardVertexAttributeID pStandardAttributeID )
		{
			return static_cast<uint16>( ( ( uint64 )pStandardAttributeID >> 56 ) & 0xF );
		}

		inline GCI::EVertexAttribFormat getStandardVertexAttributeBaseFormat( EStandardVertexAttributeID pStandardAttributeID )
		{
			return static_cast<GCI::EVertexAttribFormat>( ( ( uint64 )pStandardAttributeID >> 24 ) & QLimits<uint32>::maxValue );
		}

		inline uint32 getStandardVertexAttributeComponentByteSize( EStandardVertexAttributeID pStandardAttributeID )
		{
			return GCI::CxDef::getVertexAttribFormatByteSize( getStandardVertexAttributeBaseFormat( pStandardAttributeID ) );
		}

		inline uint32 getStandardVertexAttributeByteSize( EStandardVertexAttributeID pStandardAttributeID )
		{
			return getStandardVertexAttributeComponentsNum( pStandardAttributeID ) * getStandardVertexAttributeComponentByteSize( pStandardAttributeID );
		}

		inline Bitmask<EShaderInputSemanticFlags> getStandardShaderInputSemanticFlags( EStandardVertexAttributeID pStandardAttributeID )
		{
			return static_cast<uint32>( ( uint64 )pStandardAttributeID & 0xFFFFFF );
		}

		inline EShaderInputSemanticID getStandardVertexAttributeSemanticID( EStandardVertexAttributeID pStandardAttributeID )
		{
			return static_cast<EShaderInputSemanticID>( declareShaderInputSemanticID( getStandardShaderInputSemanticFlags( pStandardAttributeID ) ) );
		}

		inline bool isStandardShaderInputSemanticID( EShaderInputSemanticID pSemanticID )
		{
			const auto semanticsMask = makeBitmask( pSemanticID );
			return semanticsMask.isSetAnyOf( E_SHADER_INPUT_SEMANTIC_MASK_ALL ) && ( semanticsMask.countBits() == 1 );
		}

	}

	namespace GCIUtils
	{

		IC3_ATTR_NO_DISCARD EShaderInputSemanticID getShaderInputSemanticIDFromName( const std::string_view & pSemanticName );

		IC3_ATTR_NO_DISCARD const char * getShaderInputSemanticNameFromID( EShaderInputSemanticID pSemanticID );

	}

	struct SShaderSemantics
	{
		EShaderInputSemanticID smtID = EShaderInputSemanticID::Undefined;

		std::string smtName {};

		SShaderSemantics() = default;

		SShaderSemantics( EShaderInputSemanticID pSemanticID )
		: smtID( pSemanticID )
		, smtName( GCIUtils::getShaderInputSemanticNameFromID( smtID ) )
		{}

		SShaderSemantics( EStandardVertexAttributeID pStandardAttributeID )
		: smtID( CxDef::getStandardVertexAttributeSemanticID( pStandardAttributeID ) )
		, smtName( GCIUtils::getShaderInputSemanticNameFromID( smtID ) )
		{}

		explicit SShaderSemantics( std::string pSemanticName )
		: smtID( GCIUtils::getShaderInputSemanticIDFromName( pSemanticName ) )
		, smtName( std::move( pSemanticName ) )
		{}

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
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

		IC3_NXMAIN_API static bool resolveSemantics( SShaderSemantics & pSemantics );

		IC3_NXMAIN_API_NO_DISCARD static SShaderSemantics resolveSemantics( const SShaderSemantics & pSemantics );
	};

	inline bool operator==( const SShaderSemantics & pLhs, const SShaderSemantics & pRhs ) noexcept
	{
		return pLhs.smtName == pRhs.smtName;
	}

	inline bool operator<( const SShaderSemantics & pLhs, const SShaderSemantics & pRhs ) noexcept
	{
		return pLhs.smtName < pRhs.smtName;
	}

} // namespace Ic3

namespace std
{

	template <>
	struct hash<Ic3::SShaderSemantics>
	{
		size_t operator()( const Ic3::SShaderSemantics & pInput ) const noexcept
		{
			return hash<std::string>()( pInput.smtName );
		}
	};

}

#endif // __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
