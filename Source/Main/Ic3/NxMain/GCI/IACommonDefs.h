
#pragma once

#ifndef __IC3_NXMAIN_IA_COMMON_DEFS_H__
#define __IC3_NXMAIN_IA_COMMON_DEFS_H__

#include "ShaderInputSemantics.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>
#include <Ic3/Cppx/SortedArray.h>

namespace Ic3
{

	struct GenericVertexAttribute;
	struct VertexAttributeDefinition;
	struct VertexInputStream;

	using gci_input_assembler_slot_t = GCI::input_assembler_index_t;

	using InputAssemblerSlotArray = Cppx::TSortedArray<gci_input_assembler_slot_t>;
	using InputAssemblerSlotRange = TRange<gci_input_assembler_slot_t>;

	using GenericVertexAttributeArray = std::array<GenericVertexAttribute, GCM::cxIAMaxVertexAttributesNum>; // Cppx::TSortedArray<VertexAttributeComponent>;
	using VertexInputStreamArray = std::array<VertexInputStream, GCM::cxIAMaxVertexStreamsNum>; // Cppx::TSortedArray<VertexInputStream>;

	/**
	 * [Constant] Represents an invalid vertex attribute slot.
	 */
	constexpr auto cxGCIVertexAttributeIndexUndefined = GCI::cxIAVertexAttributeIndexUndefined;

	/**
	 * [Constant] Represents an invalid vertex stream slot.
	 */
	constexpr auto cxGCIVertexStreamIndexUndefined = GCI::cxIAVertexStreamIndexUndefined;

	/// Represents an invalid offset value for vertex attribute.
	constexpr auto cxGCIVertexAttributeOffsetInvalid = QLimits<uint32>::sMaxValue;

	///
	constexpr auto cxGCIVertexAttributeOffsetAppend = GCI::cxIAVertexAttributeOffsetAppend;

	///
	inline constexpr InputAssemblerSlotRange cxGCIValidInputAssemblerSlotIndexRange{ 0u, GCM::cxIAGenericInputArraySize - 1 };

	///
	inline constexpr InputAssemblerSlotRange cxGCIValidVertexAttributeComponentsNumRange{ 1u, GCM::cxIAMaxVertexAttributeComponentsNum };

	///
	inline constexpr InputAssemblerSlotRange cxGCIValidVertexAttributeSemanticGroupSizeRange{ 1u, GCM::cxIAMaxVertexAttributeSemanticGroupSize };

	/**
	 *
	 */
	enum EVertexDataRate : uint16
	{
		PerVertex,
		PerInstance,
		Undefined = Cppx::QLimits<uint16>::sMaxValue
	};

	/**
	 *
	 */
	struct VertexAttributeDefinition
	{
		gci_input_assembler_slot_t attributeIASlot = cxGCIVertexAttributeIndexUndefined;

		gci_input_assembler_slot_t streamIASlot = cxGCIVertexStreamIndexUndefined;

		GCI::EVertexAttribFormat dataFormat = GCI::EVertexAttribFormat::Undefined;

		uint16 dataPadding = 0;

		uint16 semanticGroupSize = 1;

		uint32 vertexStreamRelativeOffset = 0;

		EVertexDataRate dataRate = EVertexDataRate::Undefined;

		ShaderSemantics shaderSemantics{};

		IC3_ATTR_NO_DISCARD bool isValid() const noexcept;

		IC3_ATTR_NO_DISCARD bool hasAppendAsRelativeOffset() const noexcept;
	};

	/**
	 * Represents a generic vertex attribute, i.e. contents of a single IA-level attribute slot.
	 */
	struct GenericVertexAttribute
	{
		/**
		 * Format of the attribute's data.
		 */
		GCI::EVertexAttribFormat dataFormat = GCI::EVertexAttribFormat::Undefined;

		/**
		 * Data padding placed after the attribute's data in its data stream. The final data stride for an attribute
		 * is calculated as: (size of dataFormat in bytes) + dataPadding.
		 */
		uint16 dataPadding;

		/**
		 * Size of the semantic group this attribute belongs to. If this is 1, the attribute is a
		 * single-slot attribute with unique semantic name. If >1, this is one of up to 4 attributes
		 * that share the same semantic name and occupy adjacent slot range.
		 * @see semanticGroupIndex
		 */
		 uint8 semanticGroupSize;

		/**
		 * Semantic index of the attribute. Semantic group is a group of 2, 3 or 4 attributes that
		 * share the same semantic name. Semantic group occupies continuous range of IA slots and
		 * each of the attributes must have a different semantic index.
		 * An example could be an 4x4 instance matrix, that would be stored as 4 vertex attributes,
		 * each of type Vec4 and with semantic indices 0, 1, 2 and 3 (and identical semantic name).
		 */
		uint8 semanticIndex;

		/**
		 * Base attribute index. Allowed values are from 0 to (GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM - 1).
		 * For multi-component attributes, this is the index of the first occupied attribute slot.
		 */
		gci_input_assembler_slot_t attributeIASlot;

		/**
		 * An index of a vertex buffer slot this attribute is fetched from.
		 */
		gci_input_assembler_slot_t streamIASlot;

		/**
		 * An offset from the start of the vertex buffer data to the beginning of the attribute's data.
		 * This is a *relative* offset from the start of the bound range of the buffer, not from it's physical base address.
		 */
		uint32 vertexStreamRelativeOffset;

		/**
		 * Semantics of the attribute.
		 */
		ShaderSemantics shaderSemantics{};

		/**
		 *
		 */
		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		/**
		 *
		 */
		IC3_ATTR_NO_DISCARD bool isActive() const noexcept;

		/**
		 *
		 */
		IC3_ATTR_NO_DISCARD bool isBaseAttribute() const noexcept;

		/**
		 *
		 */
		IC3_ATTR_NO_DISCARD bool isSemanticGroupAttribute() const noexcept;

		/**
		 *
		 * @return
		 */
		IC3_ATTR_NO_DISCARD bool isSameAs( const GenericVertexAttribute & pOther ) const noexcept;

		/**
		 *
		 * @return
		 */
		IC3_ATTR_NO_DISCARD bool hasSameFormatAs( const GenericVertexAttribute & pOther ) const noexcept;

		/**
		 *
		 * @return
		 */
		IC3_ATTR_NO_DISCARD bool hasSameSemanticsAs( const GenericVertexAttribute & pOther ) const noexcept;

		/**
		 *
		 */
		IC3_ATTR_NO_DISCARD GCI::EBaseDataType getBaseDataType() const noexcept;

		/**
		 *
		 */
		IC3_ATTR_NO_DISCARD uint32 getDataSizeInBytes() const noexcept;

		/**
		 *
		 */
		IC3_ATTR_NO_DISCARD uint32 getDataStride() const noexcept;

		/**
		 *
		 */
		void initBaseAttributeFromDefinition( const VertexAttributeDefinition & pDefinition );

		/**
		 *
		 */
		void initSemanticSubAttributeFromBaseAttribute( const GenericVertexAttribute & pBaseAttribute, uint32 pSemanticIndex );

		/**
		 *
		 */
		void reset();
	};


	namespace GCU
	{

		IC3_ATTR_NO_DISCARD bool isAttributeLocationAndSizeValid( uint32 pBaseAttributeIASlot, uint32 pComponentsNum );

	}

} // namespace Ic3

#include "IACommonDefs.inl"

#endif // __IC3_NXMAIN_IA_COMMON_DEFS_H__
