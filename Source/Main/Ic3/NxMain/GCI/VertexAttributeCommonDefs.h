
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
#define __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__

#include "ShaderInputSemantics.h"

namespace Ic3
{

	/**
	 * Additional attribute flags.
	 */
	enum EVertexAttributeKeyFlags : uint32
	{
		/// Specifies that the attribute is a per-instance attribute instead of per-vertex.
		eVertexAttributeKeyFlagPerInstanceBit = 0x80000
	};

	namespace CxDef
	{

		/**
		 * Builds a VertexAttributeKey value from the specified attribute properties.
		 * @param pBaseSlot       Base IS slot of the attribute.
		 * @param pComponentsNum  Number of attribute components (i.e. number of occupied IA slots).
		 * @param pBaseFormat     Format of an individual component of the attribute.
		 * @param pSemanticFlags  Semantic flags of the attribute.
		 * @param pAttributeFlags Additional attribute flags. @see EVertexAttributeKeyFlags
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		IC3_ATTR_NO_DISCARD inline constexpr VertexAttributeKey makeVertexAttributeKey(
				uint16 pBaseSlot,
				uint16 pComponentsNum,
				GCI::EVertexAttribFormat pBaseFormat,
				TBitmask<EShaderInputSemanticFlags> pSemanticFlags,
				TBitmask<EVertexAttributeKeyFlags> pAttributeFlags = 0 )
		{
			return static_cast<uint64>(
					( ( ( uint64 )pBaseSlot & 0xF ) << 60 ) |
					( ( ( uint64 )pComponentsNum & 0xF ) << 56 ) |
					( ( ( uint64 )pBaseFormat & QLimits<uint32>::sMaxValue ) << 24 ) |
					( ( uint64 )pSemanticFlags & eShaderInputSemanticMaskAll ) | pAttributeFlags );
		}

		/**
		 * Builds a per-vertex VertexAttributeKey value from the specified attribute properties. This function is a more
		 * specialized version of makeVertexAttributeKey, but assumes single-component attributes with no extra flags.
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		IC3_ATTR_NO_DISCARD inline constexpr VertexAttributeKey makeVertexAttributeKeyV(
				uint16 pBaseSlot,
				GCI::EVertexAttribFormat pBaseFormat,
				TBitmask<EShaderInputSemanticFlags> pSemanticFlags )
		{
			return makeVertexAttributeKey( pBaseSlot, 1, pBaseFormat, pSemanticFlags, 0u );
		}

		/**
		 * Builds a per-instance VertexAttributeKey value from the specified attribute properties. This function is a more
		 * specialized version of makeVertexAttributeKey, but assumes per-instance attribute flag.
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		IC3_ATTR_NO_DISCARD inline constexpr VertexAttributeKey makeVertexAttributeKeyI(
				uint16 pBaseSlot,
				uint16 pComponentsNum,
				GCI::EVertexAttribFormat pBaseFormat,
				TBitmask<EShaderInputSemanticFlags> pSemanticFlags )
		{
			return makeVertexAttributeKey( pBaseSlot, pComponentsNum, pBaseFormat, pSemanticFlags, eVertexAttributeKeyFlagPerInstanceBit );
		}

	}

	enum EStandardVertexAttributeKeys : VertexAttributeKey
	{
		/// Pre-defined vertex position: three 32-bit floats, located at slot 0.
		eStandardVertexAttributeKeyPosition =
				CxDef::makeVertexAttributeKeyV( 0, GCI::EVertexAttribFormat::Vec3F32, eShaderInputSemanticFlagPositionBit ),

		/// Pre-defined vertex normal: three 32-bit floats, located at slot 1.
		eStandardVertexAttributeKeyNormal =
				CxDef::makeVertexAttributeKeyV( 1, GCI::EVertexAttribFormat::Vec3F32, eShaderInputSemanticFlagNormalBit ),

		/// Pre-defined vertex tangent: three 32-bit floats, located at slot 2.
		eStandardVertexAttributeKeyTangent =
				CxDef::makeVertexAttributeKeyV( 2, GCI::EVertexAttribFormat::Vec3F32, eShaderInputSemanticFlagTangentBit ),

		/// Pre-defined vertex bi-tangent: three 32-bit floats, located at slot 3.
		eStandardVertexAttributeKeyBiTangent =
				CxDef::makeVertexAttributeKeyV( 3, GCI::EVertexAttribFormat::Vec3F32, eShaderInputSemanticFlagBiTangentBit ),

		/// Pre-defined vertex fixed color: four 8-bit uint values (normalized), located at slot 4.
		eStandardVertexAttributeKeyFixedColor =
				CxDef::makeVertexAttributeKeyV( 4, GCI::EVertexAttribFormat::Vec4U8N, eShaderInputSemanticFlagFixedColorBit ),

		/// Pre-defined vertex tex coord 0: two 32-bit floats, located at slot 5.
		eStandardVertexAttributeKeyTexCoord0 =
				CxDef::makeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec2F32, eShaderInputSemanticFlagTexCoord0Bit ),

		/// Pre-defined vertex tex coord 1: two 32-bit floats, located at slot 6.
		eStandardVertexAttributeKeyTexCoord1 =
				CxDef::makeVertexAttributeKeyV( 6, GCI::EVertexAttribFormat::Vec2F32, eShaderInputSemanticFlagTexCoord1Bit ),

		/// Pre-defined vertex tex coord 0&1 combined: four 32-bit floats, located at slot 5.
		eStandardVertexAttributeKeyTexCoord01Combined =
		CxDef::makeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec4F32, eShaderInputSemanticFlagTexCoord0Bit | eShaderInputSemanticFlagTexCoord1Bit ),

		/// Pre-defined vertex tex coord 2: two 32-bit floats, located at slot 7.
		eStandardVertexAttributeKeyTexCoord2 =
				CxDef::makeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec2F32, eShaderInputSemanticFlagTexCoord2Bit ),

		/// Pre-defined vertex tex coord 2&3 combined: four 32-bit floats, located at slot 7.
		eStandardVertexAttributeKeyTexCoord23Combined =
				CxDef::makeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec4F32, eShaderInputSemanticFlagTexCoord2Bit | eShaderInputSemanticFlagTexCoord3Bit ),

		/// Pre-defined vertex blend indices
		eStandardVertexAttributeKeyBlendIndices =
				CxDef::makeVertexAttributeKeyV( 8, GCI::EVertexAttribFormat::Vec4U32, eShaderInputSemanticFlagBlendIndicesBit ),

		/// Pre-defined vertex blend weights
		eStandardVertexAttributeKeyBlendWeights =
				CxDef::makeVertexAttributeKeyV( 9, GCI::EVertexAttribFormat::Vec4F32, eShaderInputSemanticFlagBlendWeightsBit ),

		/// Pre-defined instance matrix
		eStandardVertexAttributeKeyInstanceMatrix =
				CxDef::makeVertexAttributeKeyI( 10, 4, GCI::EVertexAttribFormat::Vec4F32, eShaderInputSemanticFlagInstanceMatrixBit ),

		/// Pre-defined instance user data
		eStandardVertexAttributeKeyInstanceUserData =
				CxDef::makeVertexAttributeKeyI( 14, 2, GCI::EVertexAttribFormat::Vec2F32, eShaderInputSemanticFlagInstanceUserDataBit ),
	};

	namespace CxDef
	{

		inline constexpr GCI::input_assembler_index_t getVertexAttributeKeyBaseIndex( VertexAttributeKey pAttributeKey )
		{
			return ( GCI::input_assembler_index_t )( ( ( uint64 )pAttributeKey >> 60 ) & 0xF );
		}

		inline constexpr uint16 getVertexAttributeKeyComponentsNum( VertexAttributeKey pAttributeKey )
		{
			return ( uint16 )( ( ( uint64 )pAttributeKey >> 56 ) & 0xF );
		}

		inline GCI::EVertexAttribFormat getVertexAttributeKeyBaseFormat( VertexAttributeKey pAttributeKey )
		{
			return ( GCI::EVertexAttribFormat )( ( ( uint64 )pAttributeKey >> 24 ) & QLimits<uint32>::sMaxValue );
		}

		inline uint32 getVertexAttributeKeyComponentByteSize( VertexAttributeKey pAttributeKey )
		{
			const auto baseFormat = getVertexAttributeKeyBaseFormat( pAttributeKey );
			return GCI::CxDef::getVertexAttribFormatByteSize( baseFormat );
		}

		inline uint32 getVertexAttributeKeyTotalByteSize( VertexAttributeKey pAttributeKey )
		{
			const auto componentByteSize = getVertexAttributeKeyComponentByteSize( pAttributeKey );
			const auto componentsNum = getVertexAttributeKeyComponentsNum( pAttributeKey );
			return componentByteSize * componentsNum;
		}

		inline TBitmask<EShaderInputSemanticFlags> getVertexAttributeKeySemanticFlags( VertexAttributeKey pAttributeKey )
		{
			return ( uint32 )( ( uint64 )pAttributeKey & eShaderInputSemanticMaskAll );
		}

		inline EShaderInputSemanticID getVertexAttributeKeySemanticID( VertexAttributeKey pAttributeKey )
		{
			return static_cast<EShaderInputSemanticID>( declareShaderInputSemanticID( getVertexAttributeKeySemanticFlags( pAttributeKey ) ) );
		}

		inline bool isVertexAttributeKeyPerInstance( VertexAttributeKey pAttributeKey )
		{
			const auto attributeFlagsAll = Cppx::makeBitmask( pAttributeKey );
			return attributeFlagsAll.isSet( eVertexAttributeKeyFlagPerInstanceBit );
		}

		inline bool isStandardShaderInputSemanticID( EShaderInputSemanticID pSemanticID )
		{
			const auto semanticsMask = Cppx::makeBitmask( pSemanticID );
			return semanticsMask.isSetAnyOf( eShaderInputSemanticMaskStandardAttribsAll ) && ( semanticsMask.countBits() == 1 );
		}

		inline constexpr TBitmask<GCI::EIAVertexAttributeFlags> getVertexAttributeKeyGCIAttributeMask( VertexAttributeKey pAttributeKey )
		{
			const auto baseIndex = getVertexAttributeKeyBaseIndex( pAttributeKey );
			const auto componentsNum = getVertexAttributeKeyComponentsNum( pAttributeKey );

			return GCI::CxDef::makeIAVertexAttributeFlag( baseIndex ) |
			       ( ( componentsNum > 1 ) ? GCI::CxDef::makeIAVertexAttributeFlag( baseIndex + 1 ) : 0u ) |
			       ( ( componentsNum > 2 ) ? GCI::CxDef::makeIAVertexAttributeFlag( baseIndex + 2 ) : 0u ) |
			       ( ( componentsNum > 3 ) ? GCI::CxDef::makeIAVertexAttributeFlag( baseIndex + 3 ) : 0u );
		}

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
