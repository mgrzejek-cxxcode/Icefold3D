
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
#define __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__

#include "VertexFormatCommon.h"

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
		struct EncodedData
		{
			/// Base slot (index) of the attribute. 4 bits, values from 0 to 15.
			uint8 baseSlot : 4;

			/// Semantic group size (number of attributes with the same semantic). 4 bits, values from 1 to 4.
			uint8 semanticGroupSize : 4;

			/// Additional vertex key flags.
			/// @see EVertexAttributeKeyFlags
			cppx::bitmask<EVertexAttributeKeyFlags> attributeKeyFlags;

			/// Semantic flags of the attribute.
			/// @see EVertexAttributeSemanticFlags
			cppx::bitmask<EVertexAttributeSemanticFlags> semanticFlags;

			/// Base attribute format, i.e. the format of the data stored in a single attribute slot.
			GCI::EVertexAttribFormat baseDataFormat;
		};

		/// Attribute key info encoded into a single 64-bit value.
		vertex_attribute_key_value_t uValue;

		EncodedData uEncodedData;

		constexpr VertexAttributeKey( vertex_attribute_key_value_t pValue = 0 )
		: uValue( pValue )
		{}

		constexpr VertexAttributeKey(
				native_uint pBaseSlot,
				native_uint pSemanticGroupSize,
				GCI::EVertexAttribFormat pBaseDataFormat,
				cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags,
				cppx::bitmask<EVertexAttributeKeyFlags> pAttributeKeyFlags )
		: uEncodedData{
			static_cast<uint8>( pBaseSlot & 0xF ),
			static_cast<uint8>( pSemanticGroupSize & 0xF ),
			pAttributeKeyFlags & eVertexAttributeKeyMaskAll,
			pSemanticFlags & eVertexAttributeSemanticMaskAll,
			pBaseDataFormat }
		{}

		CPPX_ATTR_NO_DISCARD operator vertex_attribute_key_value_t() const noexcept
		{
			return uValue;
		}

		CPPX_ATTR_NO_DISCARD uint8 GetBaseSlot() const noexcept
		{
			return uEncodedData.baseSlot;
		}

		CPPX_ATTR_NO_DISCARD GCI::EVertexAttribFormat GetBaseDataFormat() const noexcept
		{
			return uEncodedData.baseDataFormat;
		}

		CPPX_ATTR_NO_DISCARD uint8 GetSemanticGroupSize() const noexcept
		{
			return uEncodedData.semanticGroupSize;
		}

		CPPX_ATTR_NO_DISCARD uint32 GetBaseByteSize() const noexcept
		{
			return GCI::CXU::GetVertexAttribFormatByteSize( uEncodedData.baseDataFormat );
		}

		CPPX_ATTR_NO_DISCARD GCI::EIAVertexAttributeDataRate GetDataRate() const noexcept
		{
			return IsAttributePerInstance() ? GCI::EIAVertexAttributeDataRate::PerInstance : GCI::EIAVertexAttributeDataRate::PerVertex;
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<EVertexAttributeSemanticFlags> GetSemanticFlags() const noexcept
		{
			return uEncodedData.semanticFlags;
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<EVertexAttributeKeyFlags> GetAttributeKeyFlags() const noexcept
		{
			return uEncodedData.attributeKeyFlags;
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<GCI::EIAVertexAttributeFlags> GetAttributeMask() const noexcept
		{
			return GCI::CXU::IAMakeVertexAttributeFlag( uEncodedData.baseSlot ) |
			       ( ( uEncodedData.semanticGroupSize > 1 ) ? GCI::CXU::IAMakeVertexAttributeFlag( uEncodedData.baseSlot + 1 ) : 0u ) |
			       ( ( uEncodedData.semanticGroupSize > 2 ) ? GCI::CXU::IAMakeVertexAttributeFlag( uEncodedData.baseSlot + 2 ) : 0u ) |
			       ( ( uEncodedData.semanticGroupSize > 3 ) ? GCI::CXU::IAMakeVertexAttributeFlag( uEncodedData.baseSlot + 3 ) : 0u );
		}

		CPPX_ATTR_NO_DISCARD bool IsAttributePerInstance() const noexcept
		{
			return uEncodedData.attributeKeyFlags.is_set( eVertexAttributeKeyFlagPerInstanceBit );
		}
	};

	namespace VertexFormat
	{

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

		/**
		 *
		 * @param pAttributeKey
		 * @param pSemanticIndex
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD inline VertexAttributeShaderSemantics GetShaderSemanticsForAttributeKey(
			VertexAttributeKey pAttributeKey,
			uint32 pSemanticIndex = 0 )
		{
			return GetSemanticsFromAttributeFlags( pAttributeKey.GetSemanticFlags(), pSemanticIndex );
		}

		IC3_NXMAIN_API_NO_DISCARD VertexInputAttributeDefinition MakeAttributeDefinition(
			uint32 pVertexStreamSlot,
			VertexAttributeKey pAttributeKey,
			uint32 pDataPadding = 0,
			uint32 pVertexStreamRelativeOffset = kVertexAttributeOffsetAppend );

		/**
		 * @brief 
		 * @param pAttributeKey 
		 * @param pVertexStreamSlot 
		 * @param pVertexStreamRelativeOffset 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD inline GCI::IAVertexAttributeDesc GCIMakeVertexAttributeDesc(
			VertexAttributeKey pAttributeKey,
			uint32 pVertexStreamSlot,
			uint32 pVertexStreamRelativeOffset );

	}

	/// Pre-defined vertex position: three 32-bit floats, located at slot 0.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysPosition =
			VertexFormat::MakeVertexAttributeKeyV( 0, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagPositionBit );

	/// Pre-defined vertex normal: three 32-bit floats, located at slot 1.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysNormal =
			VertexFormat::MakeVertexAttributeKeyV( 1, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagNormalBit );

	/// Pre-defined vertex tangent: three 32-bit floats, located at slot 2.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTangent =
			VertexFormat::MakeVertexAttributeKeyV( 2, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTangentBit );

	/// Pre-defined vertex bi-tangent: three 32-bit floats, located at slot 3.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysBiTangent =
			VertexFormat::MakeVertexAttributeKeyV( 3, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagBiTangentBit );

	/// Pre-defined vertex fixed color: four 32-bit float values, located at slot 4.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysFixedColorF32 =
			VertexFormat::MakeVertexAttributeKeyV( 4, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticFlagFixedColorBit );

	/// Pre-defined vertex fixed color: four 8-bit uint values (normalized), located at slot 4.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysFixedColorU8N =
			VertexFormat::MakeVertexAttributeKeyV( 4, GCI::EVertexAttribFormat::Vec4U8N, eVertexAttributeSemanticFlagFixedColorBit );

	/// Pre-defined 2D vertex tex coord 0: two 32-bit floats, located at slot 5.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2D0 =
			VertexFormat::MakeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagTexCoord0Bit );

	/// Pre-defined 2D vertex tex coord 1: two 32-bit floats, located at slot 6.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2D1 =
			VertexFormat::MakeVertexAttributeKeyV( 6, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagTexCoord1Bit );

	/// Pre-defined 2D vertex tex coord 0&1 combined: four 32-bit floats, located at slot 5.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2D01Combined =
			VertexFormat::MakeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticMaskTexCoordP01 );

	/// Pre-defined 2D vertex tex coord 2: two 32-bit floats, located at slot 7.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2 =
			VertexFormat::MakeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagTexCoord2Bit );

	/// Pre-defined 2D vertex tex coord 3: two 32-bit floats, located at slot 8.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3 =
			VertexFormat::MakeVertexAttributeKeyV( 8, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagTexCoord3Bit );

	/// Pre-defined 2D vertex tex coord 2&3 combined: four 32-bit floats, located at slot 7.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord2D23Combined =
			VertexFormat::MakeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticMaskTexCoordP23 );

	/// Pre-defined 3D vertex tex coord 0: three 32-bit floats, located at slot 5.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3D0 =
			VertexFormat::MakeVertexAttributeKeyV( 5, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTexCoord0Bit );

	/// Pre-defined 3D vertex tex coord 1: three 32-bit floats, located at slot 6.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3D1 =
			VertexFormat::MakeVertexAttributeKeyV( 6, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTexCoord1Bit );

	/// Pre-defined 3D vertex tex coord 2: three 32-bit floats, located at slot 7.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3D2 =
			VertexFormat::MakeVertexAttributeKeyV( 7, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTexCoord2Bit );

	/// Pre-defined 3D vertex tex coord 3: three 32-bit floats, located at slot 8.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysTexCoord3D3 =
			VertexFormat::MakeVertexAttributeKeyV( 8, GCI::EVertexAttribFormat::Vec3F32, eVertexAttributeSemanticFlagTexCoord3Bit );

	/// Pre-defined vertex blend indices: four 32-bit floats, located at slot 8.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysBlendIndices =
			VertexFormat::MakeVertexAttributeKeyV( 8, GCI::EVertexAttribFormat::Vec4U32, eVertexAttributeSemanticFlagBlendIndicesBit );

	/// Pre-defined vertex blend weights: four 32-bit floats, located at slot 9.
	inline constexpr VertexAttributeKey kVertexAttributeKeySysBlendWeights =
			VertexFormat::MakeVertexAttributeKeyV( 9, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticFlagBlendWeightsBit );

	/// Pre-defined instance matrix
	inline constexpr VertexAttributeKey kVertexAttributeKeySysInstanceMatrix =
			VertexFormat::MakeVertexAttributeKeyI( 10, 4, GCI::EVertexAttribFormat::Vec4F32, eVertexAttributeSemanticFlagInstanceMatrixBit );

	/// Pre-defined instance user data
	inline constexpr VertexAttributeKey kVertexAttributeKeySysInstanceUserData =
			VertexFormat::MakeVertexAttributeKeyI( 14, 2, GCI::EVertexAttribFormat::Vec2F32, eVertexAttributeSemanticFlagInstanceUserDataBit );

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
