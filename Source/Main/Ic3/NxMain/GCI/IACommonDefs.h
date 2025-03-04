
#pragma once

#ifndef __IC3_NXMAIN_IA_COMMON_DEFS_H__
#define __IC3_NXMAIN_IA_COMMON_DEFS_H__

#include "CommonGCIDefs.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>
#include <cppx/sortedArray.h>

namespace Ic3
{

	struct GenericVertexInputAttribute;
	struct VertexInputAttributeDefinition;
	struct VertexInputStream;

	using gci_input_assembler_slot_t = GCI::input_assembler_index_t;

	using InputAssemblerSlotArray = cppx::sorted_array<gci_input_assembler_slot_t>;
	using InputAssemblerSlotRange = cppx::range<gci_input_assembler_slot_t>;

	using GenericVertexAttributeArray = std::array<GenericVertexInputAttribute, GCM::kIAMaxVertexAttributesNum>;
	using VertexInputStreamArray = std::array<VertexInputStream, GCM::kIAMaxDataStreamVertexBuffersNum>;

	enum EVertexAttributeSemanticFlags : uint16
	{
		eVertexAttributeSemanticFlagNone                = 0,

		eVertexAttributeSemanticFlagPositionBit         = 0x0001,
		eVertexAttributeSemanticFlagNormalBit           = 0x0002,
		eVertexAttributeSemanticFlagTangentBit          = 0x0004,
		eVertexAttributeSemanticFlagBiTangentBit        = 0x0008,
		eVertexAttributeSemanticMaskGeometryDataAll     = 0x000F,

		eVertexAttributeSemanticFlagTexCoord0Bit        = 0x0010,
		eVertexAttributeSemanticFlagTexCoord1Bit        = 0x0020,
		eVertexAttributeSemanticFlagTexCoord2Bit        = 0x0040,
		eVertexAttributeSemanticFlagTexCoord3Bit        = 0x0080,
		eVertexAttributeSemanticFlagTexCoord4Bit        = 0x0100,
		eVertexAttributeSemanticFlagTexCoord5Bit        = 0x0200,
		eVertexAttributeSemanticMaskTexCoordAll         = 0x03F0,

		eVertexAttributeSemanticFlagBlendIndicesBit     = 0x0400,
		eVertexAttributeSemanticFlagBlendWeightsBit     = 0x0800,
		eVertexAttributeSemanticMaskSkinDataAll         = 0x0C00,

		eVertexAttributeSemanticFlagFixedColorBit       = 0x1000,

		eVertexAttributeSemanticFlagInstanceMatrixBit   = 0x2000,
		eVertexAttributeSemanticFlagInstanceUserDataBit = 0x4000,
		eVertexAttributeSemanticMaskInstanceDataAll     = 0x6000,

		eVertexAttributeSemanticFlagCustomAttributeBit  = 0x8000,

		eVertexAttributeSemanticMaskStandardAttribsAll  = 0x7FFF,
		eVertexAttributeSemanticMaskAll                 = 0xFFFF,

		eVertexAttributeSemanticMaskTexCoordP01 =
			eVertexAttributeSemanticFlagTexCoord0Bit | eVertexAttributeSemanticFlagTexCoord1Bit,

		eVertexAttributeSemanticMaskTexCoordP23 =
			eVertexAttributeSemanticFlagTexCoord2Bit | eVertexAttributeSemanticFlagTexCoord3Bit,
	};

	static_assert( sizeof( EVertexAttributeSemanticFlags ) == sizeof( uint16 ) );

	enum class EVertexAttributeSemanticID : uint32
	{
		Unknown             = 0,
		SIDPosition         = eVertexAttributeSemanticFlagPositionBit,
		SIDNormal           = eVertexAttributeSemanticFlagNormalBit,
		SIDTangent          = eVertexAttributeSemanticFlagTangentBit,
		SIDBiTangent        = eVertexAttributeSemanticFlagBiTangentBit,
		SIDTexCoord0        = eVertexAttributeSemanticFlagTexCoord0Bit,
		SIDTexCoord1        = eVertexAttributeSemanticFlagTexCoord1Bit,
		SIDTexCoord2        = eVertexAttributeSemanticFlagTexCoord2Bit,
		SIDTexCoord3        = eVertexAttributeSemanticFlagTexCoord3Bit,
		SIDTexCoord4        = eVertexAttributeSemanticFlagTexCoord4Bit,
		SIDTexCoord5        = eVertexAttributeSemanticFlagTexCoord5Bit,
		SIDTexCoordP01      = eVertexAttributeSemanticMaskTexCoordP01,
		SIDTexCoordP23      = eVertexAttributeSemanticMaskTexCoordP23,
		SIDBlendIndices     = eVertexAttributeSemanticFlagBlendIndicesBit,
		SIDBlendWeights     = eVertexAttributeSemanticFlagBlendWeightsBit,
		SIDFixedColor       = eVertexAttributeSemanticFlagFixedColorBit,
		SIDInstanceMatrix   = eVertexAttributeSemanticFlagInstanceMatrixBit,
		SIDInstanceUserData = eVertexAttributeSemanticFlagInstanceUserDataBit,
		SIDCustomAttribute  = eVertexAttributeSemanticFlagCustomAttributeBit,
	};

	/**
	 *
	 */
	struct VertexInputAttributeDefinition
	{
		/// @copydoc GCI::IAVertexAttributeInfo::attributeSlot
		uint8 attributeSlot = GCI::kIAVertexAttributeSlotUndefined;

		/// Size of the semantic group, i.e. number of attributes with the semantics, i.e. number of IA slots this attribute occupies.
		/// @see GenericVertexInputAttribute::semanticGroupSize
		uint8 semanticGroupSize = 1;

		GCI::EVertexAttribFormat baseDataFormat = GCI::EVertexAttribFormat::Undefined;

		uint8 dataPadding = 0;

		/// @copydoc GenericVertexInputAttribute::semanticName
		cppx::immutable_string semanticName{};

		///
		cppx::bitmask<EVertexAttributeSemanticFlags> semanticFlags;

		GCI::EIAVertexAttributeDataRate dataRate = GCI::EIAVertexAttributeDataRate::Undefined;

		uint8 vertexStreamSlot = GCI::kIAVertexStreamSlotUndefined;

		uint32 vertexStreamRelativeOffset = 0;

		CPPX_ATTR_NO_DISCARD uint32 GetDataSizeInBytes() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasAppendAsRelativeOffset() const noexcept;
	};

	/**
	 * Represents a generic vertex attribute, i.e. contents of a single IA-level attribute slot.
	 */
	struct GenericVertexInputAttribute : public GCI::IAVertexAttributeInfo
	{
		/**
		 * Semantic index of the attribute. Semantic group is a group of 2, 3 or 4 attributes that
		 * share the same semantic name. Semantic group occupies continuous range of IA slots and
		 * each of the attributes must have a different semantic index.
		 * An example could be an 4x4 instance matrix, that would be stored as 4 vertex attributes,
		 * each of type Vec4 and with semantic indices 0, 1, 2 and 3 (and identical semantic name).
		 */
		uint8 semanticIndex = 0;

		/**
		 * Size of the semantic group this attribute belongs to. If this is 1, the attribute is a
		 * single-slot attribute with unique semantic name. If >1, this is one of up to 4 attributes
		 * that share the same semantic name and occupy adjacent slots.
		 * @see GCI::IAVertexAttributeInfo::semanticIndex
		 */
		uint8 semanticGroupSize;

		/**
		 * Semantic name of the attribute.
		 * @see VertexAttributeShaderSemantics:: semanticName
		 */
		cppx::immutable_string semanticName;

		/**
		 *
		 */
		cppx::bitmask<EVertexAttributeSemanticFlags> semanticFlags;

		/**
		 * Data padding placed after the attribute's data in its data stream. The final data stride for an attribute
		 * is calculated as: (size of dataFormat in bytes) + dataPadding.
		 */
		uint8 dataPadding;

		/**
		 * An index of a vertex buffer slot this attribute is fetched from.
		 */
		uint8 vertexStreamSlot;

		/**
		 * An offset from the start of the vertex buffer data to the beginning of the attribute's data.
		 * This is a *relative* offset from the start of the bound range of the buffer, not from it's physical base address.
		 */
		uint32 vertexStreamRelativeOffset = 0;

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept;

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD bool IsBaseAttribute() const noexcept;

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD bool IsSemanticGroupAttribute() const noexcept;

		/**
		 *
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD bool IsSameAs( const GenericVertexInputAttribute & pOther ) const noexcept;

		/**
		 *
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD bool HasSameFormatAs( const GenericVertexInputAttribute & pOther ) const noexcept;

		/**
		 *
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD bool HasSameSemanticsAs( const GenericVertexInputAttribute & pOther ) const noexcept;

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD GCI::EBaseDataType GetBaseDataType() const noexcept;

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD uint32 GetDataSizeInBytes() const noexcept;

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD uint32 GetDataStride() const noexcept;

		/**
		 *
		 */
		void InitBaseAttributeFromDefinition( const VertexInputAttributeDefinition & pDefinition );

		/**
		 *
		 */
		void InitSemanticSubAttributeFromBaseAttribute( const GenericVertexInputAttribute & pBaseAttribute, uint32 pSemanticIndex );

		/**
		 *
		 */
		void Reset();
	};

	// Size and layout of variables within GenericVertexInputAttribute are precisely designed to fit into 32 bytes
	// of storage, so VertexInputAttributeArrayConfig (and, as a consequence, VertexFormatSignature as well) are
	// optimized in terms of the size - there can be lots of them in some scenarios, so we want to keep them as small
	// as possible to reduce the overall memory consumption.
	static_assert( sizeof( GenericVertexInputAttribute ) <= 32 );


	namespace GCIUtils
	{
		using namespace GCI::CXU;

		/**
		 *
		 * @param pBaseAttributeSlot
		 * @param pSemanticGroupSize
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD bool IsAttributeLocationAndSizeValid( uint32 pBaseAttributeSlot, uint32 pSemanticGroupSize );

		/**
		 *
		 * @param pAttributeSemanticID
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD cppx::string_view GetSemanticNameFromAttributeID(
				EVertexAttributeSemanticID pAttributeSemanticID );

		/**
		 *
		 * @param pAttributeSemanticFlags
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD cppx::string_view GetSemanticNameFromAttributeFlags(
				cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags );

		/**
		 *
		 * @param pAttributeSemanticName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD cppx::bitmask<EVertexAttributeSemanticFlags> GetSemanticFlagsFromAttributeName(
				const cppx::string_view & pAttributeSemanticName );

		/**
		 *
		 * @param pAttributeSemanticName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD EVertexAttributeSemanticID GetSemanticIDFromAttributeName(
				const cppx::string_view & pAttributeSemanticName );

		/**
		 *
		 * @param pSemanticFlags
		 * @return
		 */
		inline constexpr bool IsStandardSystemAttribute( cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags )
		{
			return // Standard attributes, by default are single-slot attributes with unique semantics.
			       ( pSemanticFlags.is_set_any_of( eVertexAttributeSemanticMaskStandardAttribsAll ) && ( pSemanticFlags.count_bits() == 1 ) ) ||
			       // Exception: packed texture coordinates, where a single attribute holds two set of texture coords.
			       ( ( pSemanticFlags & eVertexAttributeSemanticMaskTexCoordAll ).count_bits() == 2 );
		}

	}

} // namespace Ic3

#include "IACommonDefs.inl"

#endif // __IC3_NXMAIN_IA_COMMON_DEFS_H__
