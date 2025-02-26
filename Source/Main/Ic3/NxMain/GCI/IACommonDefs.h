
#pragma once

#ifndef __IC3_NXMAIN_IA_COMMON_DEFS_H__
#define __IC3_NXMAIN_IA_COMMON_DEFS_H__

#include "ShaderInputSemantics.h"
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

	using GenericVertexAttributeArray = std::array<GenericVertexInputAttribute, GCM::kIAMaxVertexAttributesNum>; // cppx::sorted_array<VertexAttributeComponent>;
	using VertexInputStreamArray = std::array<VertexInputStream, GCM::kIAMaxDataStreamVertexBuffersNum>; // cppx::sorted_array<VertexInputStream>;

	/**
	 * [Constant] Represents an invalid vertex attribute slot.
	 */
	constexpr auto kGCIVertexInputAttributeSlotUndefined = GCI::kIAVertexAttributeIndexUndefined;

	/**
	 * [Constant] Represents an invalid vertex stream slot.
	 */
	constexpr auto kGCIVertexInputStreamSlotUndefined = GCI::kIAVertexStreamIndexUndefined;

	/// Represents an invalid offset value for vertex attribute.
	constexpr auto kGCIVertexAttributeOffsetInvalid = cppx::meta::limits<uint32>::max_value;

	///
	constexpr auto kGCIVertexAttributeOffsetAppend = GCI::kIAVertexAttributeOffsetAppend;

	/**
	 *
	 */
	enum EVertexDataRate : uint16
	{
		PerVertex,
		PerInstance,
		Undefined = cppx::meta::limits<uint16>::max_value
	};

	/**
	 *
	 */
	struct VertexInputAttributeDefinition
	{
		GCI::EVertexAttribFormat dataFormat = GCI::EVertexAttribFormat::Undefined;

		uint16 attributeSlot = kGCIVertexInputAttributeSlotUndefined;

		uint16 streamSlot = kGCIVertexInputStreamSlotUndefined;

		uint16 dataPadding = 0;

		uint16 semanticGroupSize = 1;

		uint32 vertexStreamRelativeOffset = 0;

		EVertexDataRate dataRate = EVertexDataRate::Undefined;

		ShaderSemantics shaderSemantics{};

		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasAppendAsRelativeOffset() const noexcept;
	};

	/**
	 * Represents a generic vertex attribute, i.e. contents of a single IA-level attribute slot.
	 */
	struct GenericVertexInputAttribute
	{
		/**
		 * Format of the attribute's data.
		 */
		GCI::EVertexAttribFormat dataFormat = GCI::EVertexAttribFormat::Undefined;

		/**
		 * Base attribute index. Allowed values are from 0 to (GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM - 1).
		 * For multi-component attributes, this is the index of the first occupied attribute slot.
		 */
		gci_input_assembler_slot_t attributeSlot;

		/**
		 * An index of a vertex buffer slot this attribute is fetched from.
		 */
		gci_input_assembler_slot_t streamSlot;

		/**
		 * Data padding placed after the attribute's data in its data stream. The final data stride for an attribute
		 * is calculated as: (size of dataFormat in bytes) + dataPadding.
		 */
		uint8 dataPadding;

		/**
		 * Size of the semantic group this attribute belongs to. If this is 1, the attribute is a
		 * single-slot attribute with unique semantic name. If >1, this is one of up to 4 attributes
		 * that share the same semantic name and occupy adjacent slots.
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


	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD bool IsAttributeLocationAndSizeValid( uint32 pBaseAttributeIASlot, uint32 pComponentsNum );

	}

} // namespace Ic3

#include "IACommonDefs.inl"

#endif // __IC3_NXMAIN_IA_COMMON_DEFS_H__
