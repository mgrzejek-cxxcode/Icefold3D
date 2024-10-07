
#pragma once

#ifndef __IC3_NXMAIN_IA_COMMON_DEFS_H__
#define __IC3_NXMAIN_IA_COMMON_DEFS_H__

#include "ShaderInputSemantics.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>
#include <Ic3/Cppx/SortedArray.h>

namespace Ic3
{

	struct VertexAttributeComponent;
	struct VertexStreamComponent;

	using gci_input_assembler_slot_t = GCI::input_assembler_index_t;

	using InputAssemblerSlotArray = Cppx::TSortedArray<gci_input_assembler_slot_t>;
	using InputAssemblerSlotRange = TRange<gci_input_assembler_slot_t>;
	using VertexAttributeArray = Cppx::TSortedArray<VertexAttributeComponent>;
	using VertexStreamArray = Cppx::TSortedArray<VertexStreamComponent>;

	/**
	 * [Constant] Represents an invalid vertex attribute slot.
	 */
	constexpr auto cxGCIVertexAttributeSlotUndefined = GCI::cxIAVertexAttributeIndexUndefined;

	/**
	 * [Constant] Represents an invalid vertex stream slot.
	 */
	constexpr auto cxGCIVertexStreamSlotUndefined = GCI::cxIAVertexStreamIndexUndefined;

	/// Represents an invalid offset value for vertex attribute.
	constexpr auto cxGCIVertexAttributeRelativeOffsetInvalid = QLimits<uint32>::sMaxValue;

	///
	inline constexpr InputAssemblerSlotRange cxGCIValidVertexAttributeSlotRange{ 0u, GCM::cxIAMaxVertexAttributesNum - 1 };

	///
	inline constexpr InputAssemblerSlotRange cxGCIValidVertexAttributeComponentsNumberRange{ 1u, GCM::cxIAMaxVertexAttributeComponentsNum };

	///
	inline constexpr InputAssemblerSlotRange cxGCIValidInputAssemblerSlotRange{ 0u, GCM::cxIAMaxVertexStreamsNum - 1 };

	enum EVertexDataRate : uint16
	{
		PerVertex,
		PerInstance,
		Undefined = Cppx::QLimits<uint16>::sMaxValue
	};

	/**
	 * @brief Common part of all vertex attribute-related data structures. Contains basic properties of an attribute.
	 */
	struct CommonVertexAttributeData
	{
		/**
		 * Base format of an attribute. For multi-component attributes, this describes an individual component.
		 */
		GCI::EVertexAttribFormat baseFormat = GCI::EVertexAttribFormat::Undefined;

		/**
		 * Base attribute index. Allowed values are from 0 to (GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM - 1).
		 * For multi-component attributes, this is the index of the first occupied attribute slot.
		 */
		gci_input_assembler_slot_t attributeIASlot = cxGCIVertexAttributeSlotUndefined;

		/**
		 * Defines an extra padding (applied to each of the attribute's subcomponent. Affects the combined data stride.
		 * @example Consider an attribute which is a 3x3 float matrix. Such attribute would occupy three generic
		 * attribute slots, each containing a single 3-component float vector. In order to get each attribute aligned
		 * on a 16-byte boundary, we could place the data like this:\n
		 * [M00][M01][M02][_P_][M10][M11][M12][_P_][M20][M21][M22][_P_] (_P_ being 4 bytes of padding)\n
		 * In this case:
		 * - baseFormat should be GCI::EVertexAttribFormat::Vec3F32
		 * - semanticComponentsNum should be 3
		 * - padding should be 4
		 * @see CommonVertexAttributeData
		 */
		uint16 componentPadding = 0;

		/**
		 * An index of a vertex buffer slot this attribute is fetched from.
		 */
		gci_input_assembler_slot_t vertexStreamIASlot = cxGCIVertexStreamSlotUndefined;

		/**
		 * An offset from the start of the vertex buffer data to the beginning of the attribute's data.
		 * This is a *relative* offset from the start of the bound range of the buffer, not from it's physical base address.
		 */
		uint32 vertexStreamRelativeOffset = 0;

		/**
		 * Semantics of the attribute.
		 */
		ShaderSemantics shaderSemantics {};

		IC3_ATTR_NO_DISCARD uint32 sizeInBytes() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 dataStride() const noexcept;
	};

	/**
	 * @brief
	 */
	struct VertexAttributeDefinition : public CommonVertexAttributeData
	{
		/**
		 * Number of components (semantic sub-attributes) this attribute contains. It equals to the number of slots
		 * in the attribute array this attribute occupies. For base attributes this will be at least <1>, but no more
		 * than GCM::IA_MAX_VERTEX_ATTRIBUTE_COMPONENTS_NUM. For non-base attributes this is set to 0.
		 */
		uint8 semanticComponentsNum = 0;

		/**
		 *
		 */
		uint8 semanticIndex = 0;

		/**
		 *
		 */
		EVertexDataRate dataRate = EVertexDataRate::Undefined;

		/**
		 * @brief Returns true if the attribute definition uses automatic offset calculation or false otherwise.
		 * @return True if the attribute definition uses automatic offset calculation or false otherwise.
		 */
		IC3_ATTR_NO_DISCARD bool hasAppendAsRelativeOffset() const noexcept;

		IC3_ATTR_NO_DISCARD bool operator==( const VertexAttributeDefinition & pRhs ) const noexcept;
		IC3_ATTR_NO_DISCARD bool operator<( const VertexAttributeDefinition & pRhs ) const noexcept;

		/**
		 * @brief Returns true if the attribute specification is valid or false otherwise.
		 * A vertex definition is considered active if it represents an active attribute (active() returns true)
		 * and the number of components and their paddings have correct values.
		 * @return True if the attribute specification is valid or false otherwise.
		 */
		IC3_ATTR_NO_DISCARD bool valid() const noexcept;
	};


	namespace GCU
	{

		IC3_ATTR_NO_DISCARD bool isAttributeLocationAndSizeValid( uint32 pBaseAttributeIASlot, uint32 pComponentsNum );

	}

} // namespace Ic3

#include "IACommonDefs.inl"

#endif // __IC3_NXMAIN_IA_COMMON_DEFS_H__
