
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
#define __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__

#include "IACommonDefs.h"
#include "ShaderInputSemantics.h"

namespace Ic3
{

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

		constexpr explicit VertexAttributeKey( vertex_attribute_key_value_t pValue = 0 )
		: value( pValue )
		{}

		constexpr VertexAttributeKey(
			native_uint pBaseSlot,
			native_uint pSemanticGroupSize,
			GCI::EVertexAttribFormat pBaseFormat,
			cppx::bitmask<uint32> pCombinedSemanticKeyFlags )
		: uBaseSlot( static_cast<uint8>( pBaseSlot ) & 0xF )
		, uSemanticGroupSize( static_cast<uint8>( pSemanticGroupSize ) & 0xF )
		, uCombinedSemanticKeyFlags( pCombinedSemanticKeyFlags & ( eSystemAttributeSemanticMaskAll | eVertexAttributeKeyMaskAll ) )
		, uBaseFormat( pBaseFormat )
		{}

		CPPX_ATTR_NO_DISCARD uint32 GetBaseByteSize() const noexcept
		{
			return GCI::CXU::GetVertexAttribFormatByteSize( uBaseFormat );
		}

		CPPX_ATTR_NO_DISCARD EVertexDataRate GetVertexDataRate() const noexcept
		{
			return IsAttributePerInstance() ? EVertexDataRate::PerInstance : EVertexDataRate::PerVertex;
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<uint32> GetFlags() const noexcept
		{
			return uCombinedSemanticKeyFlags;
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<ESystemAttributeSemanticFlags> GetSystemSemanticFlags() const noexcept
		{
			return ( uCombinedSemanticKeyFlags & eSystemAttributeSemanticMaskAll );
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<EVertexAttributeKeyFlags> GetVertexAttribKeyFlags() const noexcept
		{
			return ( uCombinedSemanticKeyFlags & eVertexAttributeKeyMaskAll );
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<GCI::EIAVertexAttributeFlags> GetAttributeMask() const noexcept
		{
			return GCI::CXU::IAMakeVertexAttributeFlag( uBaseSlot ) |
			       ( ( uSemanticGroupSize > 1 ) ? GCI::CXU::IAMakeVertexAttributeFlag( uBaseSlot + 1 ) : 0u ) |
			       ( ( uSemanticGroupSize > 2 ) ? GCI::CXU::IAMakeVertexAttributeFlag( uBaseSlot + 2 ) : 0u ) |
			       ( ( uSemanticGroupSize > 3 ) ? GCI::CXU::IAMakeVertexAttributeFlag( uBaseSlot + 3 ) : 0u );
		}

		CPPX_ATTR_NO_DISCARD bool IsAttributePerInstance() const noexcept
		{
			return cppx::make_bitmask( uCombinedSemanticKeyFlags ).is_set( eVertexAttributeKeyFlagPerInstanceBit );
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
		CPPX_ATTR_NO_DISCARD inline constexpr VertexAttributeKey MakeVertexAttributeKey(
				native_uint pBaseSlot,
				native_uint pSemanticGroupSize,
				GCI::EVertexAttribFormat pBaseFormat,
				cppx::bitmask<ESystemAttributeSemanticFlags> pSemanticFlags,
				cppx::bitmask<EVertexAttributeKeyFlags> pAttributeFlags = 0 )
		{
			return VertexAttributeKey{ pBaseSlot, pSemanticGroupSize, pBaseFormat, pSemanticFlags | pAttributeFlags };
		}

		/**
		 * Builds a per-vertex VertexAttributeKey value from the specified attribute properties. This function is a more
		 * specialized version of MakeVertexAttributeKey, but assumes single-component attributes with no extra flags.
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr VertexAttributeKey MakeVertexAttributeKeyV(
				uint16 pBaseSlot,
				GCI::EVertexAttribFormat pBaseFormat,
				cppx::bitmask<ESystemAttributeSemanticFlags> pSemanticFlags )
		{
			return MakeVertexAttributeKey( pBaseSlot, 1, pBaseFormat, pSemanticFlags, 0u );
		}

		/**
		 * Builds a per-instance VertexAttributeKey value from the specified attribute properties. This function is a more
		 * specialized version of MakeVertexAttributeKey, but assumes per-instance attribute flag.
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr VertexAttributeKey MakeVertexAttributeKeyI(
				uint16 pBaseSlot,
				uint16 pComponentsNum,
				GCI::EVertexAttribFormat pBaseFormat,
				cppx::bitmask<ESystemAttributeSemanticFlags> pSemanticFlags )
		{
			return MakeVertexAttributeKey( pBaseSlot, pComponentsNum, pBaseFormat, pSemanticFlags, eVertexAttributeKeyFlagPerInstanceBit );
		}

	}

	/// Pre-defined vertex position: three 32-bit floats, located at slot 0.
	inline constexpr auto kStandardVertexAttributeKeyPosition =
		GCU::MakeVertexAttributeKeyV( 0, GCI::EVertexAttribFormat::Vec3F32, eSystemAttributeSemanticFlagPositionBit );

	/// Pre-defined vertex normal: three 32-bit floats, located at slot 1.
	inline constexpr auto kStandardVertexAttributeKeyNormal =
		GCU::MakeVertexAttributeKeyV( 1, GCI::EVertexAttribFormat::Vec3F32, eSystemAttributeSemanticFlagNormalBit );

	/// Pre-defined vertex tangent: three 32-bit floats, located at slot 2.
	inline constexpr auto kStandardVertexAttributeKeyTangent =
		GCU::MakeVertexAttributeKeyV( 2, GCI::EVertexAttribFormat::Vec3F32, eSystemAttributeSemanticFlagTangentBit );

	/// Pre-defined vertex bi-tangent: three 32-bit floats, located at slot 3.
	inline constexpr auto kStandardVertexAttributeKeyBiTangent =
		GCU::MakeVertexAttributeKeyV( 3, GCI::EVertexAttribFormat::Vec3F32, eSystemAttributeSemanticFlagBiTangentBit );

	/// Pre-defined vertex fixed color: four 8-bit uint values (normalized), located at slot 4.
	inline constexpr auto kStandardVertexAttributeKeyFixedColor =
		GCU::MakeVertexAttributeKeyV( 4, GCI::EVertexAttribFormat::Vec4U8N, eSystemAttributeSemanticFlagFixedColorBit );

	/// Pre-defined vertex tex coord 0: two 32-bit floats, located at slot 5.
	inline constexpr auto kStandardVertexAttributeKeyTexCoord0 =
		GCU::MakeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec2F32, eSystemAttributeSemanticFlagTexCoord0Bit );

	/// Pre-defined vertex tex coord 1: two 32-bit floats, located at slot 6.
	inline constexpr auto kStandardVertexAttributeKeyTexCoord1 =
		GCU::MakeVertexAttributeKeyV( 6, GCI::EVertexAttribFormat::Vec2F32, eSystemAttributeSemanticFlagTexCoord1Bit );

	/// Pre-defined vertex tex coord 0&1 combined: four 32-bit floats, located at slot 5.
	inline constexpr auto kStandardVertexAttributeKeyTexCoord01Combined =
		GCU::MakeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec4F32, eSystemAttributeSemanticMaskTexCoord01Packed );

	/// Pre-defined vertex tex coord 2: two 32-bit floats, located at slot 7.
	inline constexpr auto kStandardVertexAttributeKeyTexCoord2 =
		GCU::MakeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec2F32, eSystemAttributeSemanticFlagTexCoord2Bit );

	/// Pre-defined vertex tex coord 2&3 combined: four 32-bit floats, located at slot 7.
	inline constexpr auto kStandardVertexAttributeKeyTexCoord23Combined =
		GCU::MakeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec4F32, eSystemAttributeSemanticMaskTexCoord23Packed );

	/// Pre-defined vertex blend indices: four 32-bit floats, located at slot 8.
	inline constexpr auto kStandardVertexAttributeKeyBlendIndices =
		GCU::MakeVertexAttributeKeyV( 8, GCI::EVertexAttribFormat::Vec4U32, eSystemAttributeSemanticFlagBlendIndicesBit );

	/// Pre-defined vertex blend weights: four 32-bit floats, located at slot 9.
	inline constexpr auto kStandardVertexAttributeKeyBlendWeights =
		GCU::MakeVertexAttributeKeyV( 9, GCI::EVertexAttribFormat::Vec4F32, eSystemAttributeSemanticFlagBlendWeightsBit );

	/// Pre-defined instance matrix
	inline constexpr auto kStandardVertexAttributeKeyInstanceMatrix =
		GCU::MakeVertexAttributeKeyI( 10, 4, GCI::EVertexAttribFormat::Vec4F32, eSystemAttributeSemanticFlagInstanceMatrixBit );

	/// Pre-defined instance user data
	inline constexpr auto kStandardVertexAttributeKeyInstanceUserData =
		GCU::MakeVertexAttributeKeyI( 14, 2, GCI::EVertexAttribFormat::Vec2F32, eSystemAttributeSemanticFlagInstanceUserDataBit );

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
