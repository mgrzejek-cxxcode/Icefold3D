
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
#define __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__

#include "IACommonDefs.h"

namespace Ic3
{

	/**
	 * Additional attribute flags.
	 */
	enum EVertexAttributeKeyFlags : uint8
	{
		/// Specifies that the attribute is a per-instance attribute instead of per-vertex.
		eVertexAttributeKeyFlagPerInstanceBit = 0x01,
		eVertexAttributeKeyMaskAll            = 0x01,
	};

	union VertexAttributeKey
	{
		struct
		{
			/// Base slot (index) of the attribute. 4 bits, values from 0 to 15.
			uint8 uBaseSlot : 4;

			/// Semantic group size (number of attributes with the same semantic). 4 bits, values from 1 to 4.
			uint8 uSemanticGroupSize : 4;

			/// Additional vertex key flags.
			/// @see EVertexAttributeKeyFlags
			cppx::bitmask<EVertexAttributeKeyFlags> uAttributeKeyFlags;

			/// Semantic flags of the attribute.
			/// @see EVertexAttributeSemanticFlags
			cppx::bitmask<EVertexAttributeSemanticFlags> uSemanticFlags;

			/// Base attribute format, i.e. the format of the data stored in a single attribute slot.
			GCI::EVertexAttribFormat uBaseDataFormat;
		};

		/// Attribute key info encoded into a single 64-bit value.
		vertex_attribute_key_value_t value;

		constexpr VertexAttributeKey( vertex_attribute_key_value_t pValue = 0 )
		: value( pValue )
		{}

		constexpr VertexAttributeKey(
				native_uint pBaseSlot,
				native_uint pSemanticGroupSize,
				GCI::EVertexAttribFormat pBaseDataFormat,
				cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags,
				cppx::bitmask<EVertexAttributeKeyFlags> pAttributeKeyFlags )
		: uBaseSlot( static_cast<uint8>( pBaseSlot ) & 0xF )
		, uSemanticGroupSize( static_cast<uint8>( pSemanticGroupSize ) & 0xF )
		, uSemanticFlags( pSemanticFlags & eVertexAttributeSemanticMaskAll )
		, uAttributeKeyFlags( pAttributeKeyFlags & eVertexAttributeKeyMaskAll )
		, uBaseDataFormat( pBaseDataFormat )
		{}

		CPPX_ATTR_NO_DISCARD operator vertex_attribute_key_value_t() const noexcept
		{
			return value;
		}

		CPPX_ATTR_NO_DISCARD uint32 GetBaseByteSize() const noexcept
		{
			return GCI::CXU::GetVertexAttribFormatByteSize( uBaseDataFormat );
		}

		CPPX_ATTR_NO_DISCARD GCI::EIAVertexAttributeDataRate GetDataRate() const noexcept
		{
			return IsAttributePerInstance() ? GCI::EIAVertexAttributeDataRate::PerInstance : GCI::EIAVertexAttributeDataRate::PerVertex;
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<EVertexAttributeSemanticFlags> GetSemanticFlags() const noexcept
		{
			return uSemanticFlags;
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<EVertexAttributeKeyFlags> GetAttributeKeyFlags() const noexcept
		{
			return uAttributeKeyFlags;
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
			return uAttributeKeyFlags.is_set( eVertexAttributeKeyFlagPerInstanceBit );
		}
	};

	namespace GCIUtils
	{
		using namespace GCI::CXU;

		/**
		 * Builds a VertexAttributeKey value from the specified attribute properties.
		 * @param pBaseSlot           Base IA slot of the attribute (4 bits).
		 * @param pSemanticGroupSize  Number of semantic components (i.e. number of occupied IA slots) (4 bits).
		 * @param pBaseFormat         Format of an individual component of the attribute (32 bits).
		 * @param pSemanticFlags      Semantic flags of the attribute (24 bits).
		 * @param pAttributeKeyFlags  Additional attribute flags. @see EVertexAttributeKeyFlags
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr VertexAttributeKey MakeVertexAttributeKey(
				native_uint pBaseSlot,
				native_uint pSemanticGroupSize,
				GCI::EVertexAttribFormat pBaseDataFormat,
				cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags,
				cppx::bitmask<EVertexAttributeKeyFlags> pAttributeKeyFlags = 0 )
		{
			return VertexAttributeKey{ pBaseSlot, pSemanticGroupSize, pBaseDataFormat, pSemanticFlags, pAttributeKeyFlags };
		}

		/**
		 * Builds a per-vertex VertexAttributeKey value from the specified attribute properties. This function is a more
		 * specialized version of MakeVertexAttributeKey, but assumes single-component attributes with no extra flags.
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr VertexAttributeKey MakeVertexAttributeKeyV(
				uint16 pBaseSlot,
				GCI::EVertexAttribFormat pBaseDataFormat,
				cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags )
		{
			return MakeVertexAttributeKey( pBaseSlot, 1, pBaseDataFormat, pSemanticFlags, 0u );
		}

		/**
		 * Builds a per-instance VertexAttributeKey value from the specified attribute properties. This function is a more
		 * specialized version of MakeVertexAttributeKey, but assumes per-instance attribute flag.
		 * @return A VertexAttributeKey value with encoded attribute information.
		 */
		CPPX_ATTR_NO_DISCARD inline constexpr VertexAttributeKey MakeVertexAttributeKeyI(
				uint16 pBaseSlot,
				uint16 pComponentsNum,
				GCI::EVertexAttribFormat pBaseDataFormat,
				cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags )
		{
			return MakeVertexAttributeKey( pBaseSlot, pComponentsNum, pBaseDataFormat, pSemanticFlags, eVertexAttributeKeyFlagPerInstanceBit );
		}

	}

	/// Pre-defined vertex position: three 32-bit floats, located at slot 0.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysPosition =
			GCIUtils::MakeVertexAttributeKeyV( 0, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagPositionBit );

	/// Pre-defined vertex normal: three 32-bit floats, located at slot 1.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysNormal =
			GCIUtils::MakeVertexAttributeKeyV( 1, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagNormalBit );

	/// Pre-defined vertex tangent: three 32-bit floats, located at slot 2.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTangent =
			GCIUtils::MakeVertexAttributeKeyV( 2, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTangentBit );

	/// Pre-defined vertex bi-tangent: three 32-bit floats, located at slot 3.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysBiTangent =
			GCIUtils::MakeVertexAttributeKeyV( 3, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagBiTangentBit );

	/// Pre-defined vertex fixed color: four 32-bit float values, located at slot 4.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysFixedColorF32 =
			GCIUtils::MakeVertexAttributeKeyV( 4, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticFlagFixedColorBit );

	/// Pre-defined vertex fixed color: four 8-bit uint values (normalized), located at slot 4.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysFixedColorU8N =
			GCIUtils::MakeVertexAttributeKeyV( 4, GCI::EVertexAttribFormat::Vec4U8N, eVertexAttributeSemanticFlagFixedColorBit );

	/// Pre-defined 2D vertex tex coord 0: two 32-bit floats, located at slot 5.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2D0 =
			GCIUtils::MakeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagTexCoord0Bit );

	/// Pre-defined 2D vertex tex coord 1: two 32-bit floats, located at slot 6.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2D1 =
			GCIUtils::MakeVertexAttributeKeyV( 6, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagTexCoord1Bit );

	/// Pre-defined 2D vertex tex coord 0&1 combined: four 32-bit floats, located at slot 5.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2D01Combined =
			GCIUtils::MakeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticMaskTexCoordP01 );

	/// Pre-defined 2D vertex tex coord 2: two 32-bit floats, located at slot 7.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2 =
			GCIUtils::MakeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagTexCoord2Bit );

	/// Pre-defined 2D vertex tex coord 3: two 32-bit floats, located at slot 8.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3 =
			GCIUtils::MakeVertexAttributeKeyV( 8, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagTexCoord3Bit );

	/// Pre-defined 2D vertex tex coord 2&3 combined: four 32-bit floats, located at slot 7.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2D23Combined =
			GCIUtils::MakeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticMaskTexCoordP23 );

	/// Pre-defined 3D vertex tex coord 0: three 32-bit floats, located at slot 5.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3D0 =
			GCIUtils::MakeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTexCoord0Bit );

	/// Pre-defined 3D vertex tex coord 1: three 32-bit floats, located at slot 6.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3D1 =
			GCIUtils::MakeVertexAttributeKeyV( 6, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTexCoord1Bit );

	/// Pre-defined 3D vertex tex coord 2: three 32-bit floats, located at slot 7.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3D2 =
			GCIUtils::MakeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTexCoord2Bit );

	/// Pre-defined 3D vertex tex coord 3: three 32-bit floats, located at slot 8.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3D3 =
			GCIUtils::MakeVertexAttributeKeyV( 8, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTexCoord3Bit );

	/// Pre-defined vertex blend indices: four 32-bit floats, located at slot 8.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysBlendIndices =
			GCIUtils::MakeVertexAttributeKeyV( 8, GCI::EVertexAttribFormat::Vec4U32, eVertexAttributeSemanticFlagBlendIndicesBit );

	/// Pre-defined vertex blend weights: four 32-bit floats, located at slot 9.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysBlendWeights =
			GCIUtils::MakeVertexAttributeKeyV( 9, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticFlagBlendWeightsBit );

	/// Pre-defined instance matrix
	inline constexpr VertexAttributeKey kVertexAttributeKeySysInstanceMatrix =
			GCIUtils::MakeVertexAttributeKeyI( 10, 4, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticFlagInstanceMatrixBit );

	/// Pre-defined instance user data
	inline constexpr VertexAttributeKey kVertexAttributeKeySysInstanceUserData =
			GCIUtils::MakeVertexAttributeKeyI( 14, 2, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagInstanceUserDataBit );

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
