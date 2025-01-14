
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
#define __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__

#include "IACommonDefs.h"
#include "ShaderInputSemantics.h"

namespace Ic3
{

	using vertex_attribute_key_value_t = uint64;

	/**
	 * Additional attribute flags.
	 */
	enum EVertexAttributeKeyFlags : uint32
	{
		/// Specifies that the attribute is a per-instance attribute instead of per-vertex.
		eVertexAttributeKeyFlagPerInstanceBit = 0x080000,
		eVertexAttributeKeyMaskAll            = 0xF80000,
	};

	union VertexAttributeKey
	{
		struct
		{
			uint8 uBaseSlot : 4;
			uint8 uSemanticGroupSize : 4;
			uint32 uCombinedSemanticKeyFlags : 24;
			GCI::EVertexAttribFormat uBaseFormat;
		};

		vertex_attribute_key_value_t value;

		constexpr explicit VertexAttributeKey( uint64 pValue = 0 )
		: value( pValue )
		{}

		constexpr VertexAttributeKey(
			native_uint pBaseSlot,
			native_uint pSemanticGroupSize,
			GCI::EVertexAttribFormat pBaseFormat,
			TBitmask<uint32> pCombinedSemanticKeyFlags )
		: uBaseSlot( static_cast<uint8>( pBaseSlot ) & 0xF )
		, uSemanticGroupSize( static_cast<uint8>( pSemanticGroupSize ) & 0xF )
		, uCombinedSemanticKeyFlags( pCombinedSemanticKeyFlags & ( eSystemAttributeSemanticMaskAll | eVertexAttributeKeyMaskAll ) )
		, uBaseFormat( pBaseFormat )
		{}

		IC3_ATTR_NO_DISCARD uint32 getBaseByteSize() const noexcept
		{
			return GCI::CxDef::getVertexAttribFormatByteSize( uBaseFormat );
		}

		IC3_ATTR_NO_DISCARD EVertexDataRate getVertexDataRate() const noexcept
		{
			return isAttributePerInstance() ? EVertexDataRate::PerInstance : EVertexDataRate::PerVertex;
		}

		IC3_ATTR_NO_DISCARD TBitmask<uint32> getFlags() const noexcept
		{
			return uCombinedSemanticKeyFlags;
		}

		IC3_ATTR_NO_DISCARD TBitmask<ESystemAttributeSemanticFlags> getSystemSemanticFlags() const noexcept
		{
			return ( uCombinedSemanticKeyFlags & eSystemAttributeSemanticMaskAll );
		}

		IC3_ATTR_NO_DISCARD TBitmask<EVertexAttributeKeyFlags> getVertexAttribKeyFlags() const noexcept
		{
			return ( uCombinedSemanticKeyFlags & eVertexAttributeKeyMaskAll );
		}

		IC3_ATTR_NO_DISCARD TBitmask<GCI::EIAVertexAttributeFlags> getVertexAttributeKeyGCIAttributeMask() const noexcept
		{
			return GCI::CxDef::makeIAVertexAttributeFlag( uBaseSlot ) |
			       ( ( uSemanticGroupSize > 1 ) ? GCI::CxDef::makeIAVertexAttributeFlag( uBaseSlot + 1 ) : 0u ) |
			       ( ( uSemanticGroupSize > 2 ) ? GCI::CxDef::makeIAVertexAttributeFlag( uBaseSlot + 2 ) : 0u ) |
			       ( ( uSemanticGroupSize > 3 ) ? GCI::CxDef::makeIAVertexAttributeFlag( uBaseSlot + 3 ) : 0u );
		}

		IC3_ATTR_NO_DISCARD bool isAttributePerInstance() const noexcept
		{
			return Cppx::makeBitmask( uCombinedSemanticKeyFlags ).isSet( eVertexAttributeKeyFlagPerInstanceBit );
		}
	};

	namespace GCU
	{

		/**
		 * Builds a VertexAttributeKey value from the specified attribute properties.
		 * @param pBaseSlot           Base IA slot of the attribute (4 bits).
		 * @param pSemanticGroupSize  Number of semantic components (i.e. number of occupied IA slots) (4 bits).
		 * @param pBaseFormat         Format of an individual component of the attribute (32 bits).
		 * @param pSemanticFlags      Semantic flags of the attribute (24 bits).
		 * @param pAttributeFlags     Additional attribute flags (bits shared with pSemanticFlags). @see EVertexAttributeKeyFlags
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		IC3_ATTR_NO_DISCARD inline constexpr VertexAttributeKey makeVertexAttributeKey(
				native_uint pBaseSlot,
				native_uint pSemanticGroupSize,
				GCI::EVertexAttribFormat pBaseFormat,
				TBitmask<ESystemAttributeSemanticFlags> pSemanticFlags,
				TBitmask<EVertexAttributeKeyFlags> pAttributeFlags = 0 )
		{
			return VertexAttributeKey{ pBaseSlot, pSemanticGroupSize, pBaseFormat, pSemanticFlags | pAttributeFlags };
		}

		/**
		 * Builds a per-vertex VertexAttributeKey value from the specified attribute properties. This function is a more
		 * specialized version of makeVertexAttributeKey, but assumes single-component attributes with no extra flags.
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		IC3_ATTR_NO_DISCARD inline constexpr VertexAttributeKey makeVertexAttributeKeyV(
				uint16 pBaseSlot,
				GCI::EVertexAttribFormat pBaseFormat,
				TBitmask<ESystemAttributeSemanticFlags> pSemanticFlags )
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
				TBitmask<ESystemAttributeSemanticFlags> pSemanticFlags )
		{
			return makeVertexAttributeKey( pBaseSlot, pComponentsNum, pBaseFormat, pSemanticFlags, eVertexAttributeKeyFlagPerInstanceBit );
		}

	}

	/// Pre-defined vertex position: three 32-bit floats, located at slot 0.
	inline constexpr auto cxStandardVertexAttributeKeyPosition =
		GCU::makeVertexAttributeKeyV( 0, GCI::EVertexAttribFormat::Vec3F32, eSystemAttributeSemanticFlagPositionBit );

	/// Pre-defined vertex normal: three 32-bit floats, located at slot 1.
	inline constexpr auto cxStandardVertexAttributeKeyNormal =
		GCU::makeVertexAttributeKeyV( 1, GCI::EVertexAttribFormat::Vec3F32, eSystemAttributeSemanticFlagNormalBit );

	/// Pre-defined vertex tangent: three 32-bit floats, located at slot 2.
	inline constexpr auto cxStandardVertexAttributeKeyTangent =
		GCU::makeVertexAttributeKeyV( 2, GCI::EVertexAttribFormat::Vec3F32, eSystemAttributeSemanticFlagTangentBit );

	/// Pre-defined vertex bi-tangent: three 32-bit floats, located at slot 3.
	inline constexpr auto cxStandardVertexAttributeKeyBiTangent =
		GCU::makeVertexAttributeKeyV( 3, GCI::EVertexAttribFormat::Vec3F32, eSystemAttributeSemanticFlagBiTangentBit );

	/// Pre-defined vertex fixed color: four 8-bit uint values (normalized), located at slot 4.
	inline constexpr auto cxStandardVertexAttributeKeyFixedColor =
		GCU::makeVertexAttributeKeyV( 4, GCI::EVertexAttribFormat::Vec4U8N, eSystemAttributeSemanticFlagFixedColorBit );

	/// Pre-defined vertex tex coord 0: two 32-bit floats, located at slot 5.
	inline constexpr auto cxStandardVertexAttributeKeyTexCoord0 =
		GCU::makeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec2F32, eSystemAttributeSemanticFlagTexCoord0Bit );

	/// Pre-defined vertex tex coord 1: two 32-bit floats, located at slot 6.
	inline constexpr auto cxStandardVertexAttributeKeyTexCoord1 =
		GCU::makeVertexAttributeKeyV( 6, GCI::EVertexAttribFormat::Vec2F32, eSystemAttributeSemanticFlagTexCoord1Bit );

	/// Pre-defined vertex tex coord 0&1 combined: four 32-bit floats, located at slot 5.
	inline constexpr auto cxStandardVertexAttributeKeyTexCoord01Combined =
		GCU::makeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec4F32, eSystemAttributeSemanticMaskTexCoord01Packed );

	/// Pre-defined vertex tex coord 2: two 32-bit floats, located at slot 7.
	inline constexpr auto cxStandardVertexAttributeKeyTexCoord2 =
		GCU::makeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec2F32, eSystemAttributeSemanticFlagTexCoord2Bit );

	/// Pre-defined vertex tex coord 2&3 combined: four 32-bit floats, located at slot 7.
	inline constexpr auto cxStandardVertexAttributeKeyTexCoord23Combined =
		GCU::makeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec4F32, eSystemAttributeSemanticMaskTexCoord23Packed );

	/// Pre-defined vertex blend indices
	inline constexpr auto cxStandardVertexAttributeKeyBlendIndices =
		GCU::makeVertexAttributeKeyV( 8, GCI::EVertexAttribFormat::Vec4U32, eSystemAttributeSemanticFlagBlendIndicesBit );

	/// Pre-defined vertex blend weights
	inline constexpr auto cxStandardVertexAttributeKeyBlendWeights =
		GCU::makeVertexAttributeKeyV( 9, GCI::EVertexAttribFormat::Vec4F32, eSystemAttributeSemanticFlagBlendWeightsBit );

	/// Pre-defined instance matrix
	inline constexpr auto cxStandardVertexAttributeKeyInstanceMatrix =
		GCU::makeVertexAttributeKeyI( 10, 4, GCI::EVertexAttribFormat::Vec4F32, eSystemAttributeSemanticFlagInstanceMatrixBit );

	/// Pre-defined instance user data
	inline constexpr auto cxStandardVertexAttributeKeyInstanceUserData =
		GCU::makeVertexAttributeKeyI( 14, 2, GCI::EVertexAttribFormat::Vec2F32, eSystemAttributeSemanticFlagInstanceUserDataBit );

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
