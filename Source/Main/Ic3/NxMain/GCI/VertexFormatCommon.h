
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_FORMAT_COMMON_H__
#define __IC3_NXMAIN_VERTEX_FORMAT_COMMON_H__

#include "VertexAttributeShaderSemantics.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>

namespace Ic3
{

	struct GenericVertexInputAttribute;
	struct VertexInputAttributeDefinition;
	struct VertexInputStream;

	using input_assembler_slot_t = GCI::input_assembler_index_t;
	using vertex_attribute_padding_t = GCI::vertex_attribute_padding_t;

	using InputAssemblerSlotRange = cppx::range<input_assembler_slot_t>;

	inline constexpr auto kVertexAttributeOffsetAppend = cppx::meta::limits<uint32>::max_value;

	inline constexpr auto kVertexAttributeOffsetAppendAligned16 = cppx::meta::limits<uint32>::max_value - 1;

	/**
	 *
	 */
	constexpr auto kVertexAttributePaddingAlign16 = cppx::meta::limits<vertex_attribute_padding_t>::max_value;

	/**
	 * @brief Vertex attribute semantics included in VertexInputAttributeDefinition.
	 * 
	 * This structure is simillar to GCI::IAVertexAttributeSemantics and serves the same purpose, but there are few
	 * differences in how certain attributes can be specified (see the description for individual members).
	 * 
	 * It is not required to set both name and flags for an attribute - specified semantics are automatically resolved
	 * based on their contents. Rules for semantics resolving:
	 * 1. If 'semanticName' is not empty, it is used for semantics definition
	 *   - If the name is an indexed smantic name (e.g. "TEXCOORD3"), it is automatically split later on into a base name
	 *     and a semantic index ("TEXCOORD" and "3").
	 *   - In this case semantic flags are ignored and overwritten (see below). Corectness of the semantics definition
	 *     is the responsibility of the client.
	 *   - Additionally, semanticName is then analysed - if the name is one of the standard semantic names ("POSITION",
	 *     "NORMAL", "TEXCOORD0", etc.), flags are updated with the correct semantics flags. Otherwise, a special flag
	 *     is set instead: eVertexAttributeSemanticFlagCustomAttributeBit.
	 * 2. If 'semanticName' is empty, semantics are resolved based on the flags:
	 *   - If 'semanticFlags' contains a valid combination of bits matching one of the standard ("system") semantics,
	 *     the name is updated accrodingly.
	 *   - If 'semanticFlags' is empty, contains invalid combination of bits or has eVertexAttributeSemanticFlagCustomAttributeBit
	 *     bit set, name is not resolved and the final semantics are invalid.
	 * 
	 * Invalid semantics will produce an error whenever an attempt is made to use such configuration at the GCI level.
	 */
	struct VertexInputAttributeSemantics
	{
		/**
		 * @brief Semantic name of the attribute.
		 * 
		 * Similarly to GCI::IAVertexAttributeSemantics::semanticName this is the name that identifies the attribute
		 * in the shader. However it is fully allowed to set also indexed semantics as the value for this member.
		 * It is also one of the reason why at this level semanticIndex is not present:
		 * - For semantic groups, semanticIndex of each sub-attribute is computed
		 *   automatically based on its index within the group.
		 * - For indexed semantics, the index is specified directly as a part of the name.
		 */
		cppx::immutable_string semanticName;

		/**
		 * @brief Semantic flags of the attribute.
		 * 
		 * Semantic flags are an alternative way of describing the role of an attribute without using explicit names.
		 * There is a set of pre-defined, standard "system" attributes (position, normal, texCoords etc) that can be
		 * used to automatically configure the most frequently used attributes.
		 */
		cppx::bitmask<EVertexAttributeSemanticFlags> semanticFlags;
	};

	/**
	 * High-level definition of a vertex attribute - a generic property of a vertex streamed
	 * and processed through the pipeline to produce an output geometry.
	 * 
	 * Unlike GCI::IAVertexAttributeDesc which contains a configuration strictly limited to a single IA attribute slot,
	 * this struct can also describe a "logical" attribute, including multi-slot attributes (aka semantic groups).
	 * Semantic group is simply an attribute that occupies more than one slot of the IA attribute array. For example,
	 * an attribute defined in the shader as 4x4 matrix of floats (or a 3-element array of 4-components vectors) are
	 * typical examples of semantic groups. They have the following properties:
	 * - Every semantic group occupies multiple IA slots (in the above example: 4 and 3, respectively).
	 * - Size of the semantic group can be either 2, 3, or 4 (1 means a "classic", single attribute).
	 * - Base type is the data type of an individual attribute in the group.
	 * - Semantic group unwraps to S physical IA attributes with identical base type where S is the group size.
	 * - Every attribute in the group has the same semantic name and a different semantic index from 0 to S-1.
	 * 
	 * @note
	 * This structure is a user-provided description and used only to specify the attribute layout to the runtime.
	 * The runtime processes the provided list of attribute definitions and creates (assuming the attribute definition
	 * is valid and API succeeds) an array of GenericVertexInputAttribute, where each element describes a single physical
	 * attribute. GenericVertexInputAttribute is the primary type for working with attribute-related APIs.
	 */
	struct VertexInputAttributeDefinition
	{
		/**
		 * @brief Base format of the attribute's data.
		 * 
		 * "Base" refers to the format of a single "physical" attribute, i.e. if this struct defines
		 * a semantic attribute group (multi-sloit attribute), this is the format of a SINGLE element.
		 * Simply speaking, this is the format of a single IA attribute.
		 */
		GCI::EVertexAttribFormat baseDataFormat = GCI::EVertexAttribFormat::Undefined;

		/**
		 * @brief Index of the input attribute array slot in the IA.
		 * @see GCI::IAVertexAttributeInfo::attributeSlot
		 */
		uint8 attributeSlot = GCI::kIAVertexAttributeSlotUndefined;

		/**
		 * @brief 
		 */
		GCI::EIAVertexAttributeDataRate dataRate = GCI::EIAVertexAttributeDataRate::Undefined;

		/**
		 * @brief 
		 */
		uint16 instanceStepRate = 0;

		/**
		 * @brief Semantics of this attribute.
		 * @see VertexInputAttributeSemantics
		 */
		VertexInputAttributeSemantics semantics;

		/**
		 * @brief Size of the semantic group.
		 *
		 * If set to 1, the definition refers to a single attribute occupying a single IA attribute slot. If the value
		 * is 2, 3 or 4 (4 is the maximum allowed), the definition describes a group of attributes with identical format,
		 * and semantic name, occupying number of IA attribute slots equal to the group size. All propeerties specified
		 * except vertexStreamRelativeOffset (baseDataFormat, dataRate, instanceStepRate, dataPadding and vertexStreamSlot)
		 * are copied for each attribute in the group. The value of vertexStreamRelativeOffset is computed automatically,
		 * based on the offset specified and total stride of a single element.
		 * 
		 * Throughout the code and documentation whenever term "sub-attribute" is used, it refers to an attribute which is
		 * part of a semantic group. Semantic groups are sometimes also referred to as multi-slot attributes (MSA).
		 */
		uint8 semanticGroupSize = 1;

		/**
		 * @brief Additional data padding after the attribute data.
		 * 
		 * If non-zero it means that the every instance of this attribute is padded with 'dataPadding' bytes in the
		 * associated vertex buffer. Note that the maximum size of a single attribute is 16 bytes. If size of the
		 * attribute's data (let's call it S) with added padding is greater than 16, padding value is trimmed to 16-S,
		 * so the size limit for a single slot is not exceeded.
		 * This allows for easy setup if all attributes are 16-bytes aligned (you can set dataPadding for all attributes
		 * to, let's say, cppx::meta::limits<uint>::max_value), but watch out for potential mistakes, otherwise the data
		 * will not be fetched correctly and you can end up reading the data beyond the bound range of the vertex buffer.
		 * 
		 * @note
		 * For semantic groups/MSAs, dataPadding applies to ALL sub-attributes individually. For example, if you define
		 * an attribute as a 3x3 float matrix (12 bytes per slot, 36 bytes in total) and set dataPadding to 4 it will
		 * cause each sub-attribute to be assumed having padding of 4 bytes (16 bytes per slot, 48 bytes in total).
		 */
		uint8 dataPadding = 0;

		/**
		 * @brief An index of a vertex stream (vertex buffer slot) this attribute is fetched from.
		 * @see GCI::IAVertexAttributeStreamBinding::streamSlot.
		 */
		uint8 vertexStreamSlot = GCI::kIAVertexStreamSlotUndefined;

		/**
		 * An offset, in bytes, from the start of the buffer range bound to the associated stream.
		 * to the beginning of the data for this attribute.
		 * @see GCI::IAVertexAttributeStreamBinding::streamRelativeOffset
		 * 
		 * @note
		 * There are two special values accepted for the offset:
		 * - kVertexAttributeRelativeOffsetAppend: attribute is placed directly after the previous one
		 * - kVertexAttributeRelativeOffsetAppend16: attribute is placed directly after the previous one and 16B-aligned
		 * Note, that using kVertexAttributeRelativeOffsetAppend16 will cause the runtime to ignore the value of dataPadding.
		 * Similarly to dataPadding, in case of MSA 16B alignment forced by kVertexAttributeRelativeOffsetAppend16 applies
		 * each sub-attribute individually.
		 */
		uint32 vertexStreamRelativeOffset = 0;

		CPPX_ATTR_NO_DISCARD uint32 GetDataSizeInBytes() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 GetDataStride() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasAppendAsRelativeOffset() const noexcept;
	};

	/**
	 * @brief Contains properties of a generic vertex attribute, i.e. an attribute occupying a single IA-level attribute slot.
	 * 
	 * This struct contains checked, validated and resolved properties of a single vertex attribute. It should not be created
	 * manually, but rather via dedicated API which is supplied with an array of VertexInputAttributeDefinition values and
	 * translates them into an array of GenericVertexInputAttribute elements.
	 * 
	 * This is the primary type used by the engine to work with vertex attributes and is also part of the VertexFormatSignature
	 * (signature contains an array of GenericVertexInputAttribute for all attributes used by a given vertex format).
	 */
	struct GenericVertexInputAttribute
	{
		/**
		 * Format of the attribute's data.
		 * @see VertexInputAttributeDefinition::baseDataFormat
		 */
		GCI::EVertexAttribFormat dataFormat = GCI::EVertexAttribFormat::Undefined;

		/**
		 * @brief Index of the input attribute array slot in the IA.
		 * @see VertexInputAttributeDefinition::attributeSlot
		 */
		GCI::input_assembler_index_t attributeSlot = GCI::kIAVertexAttributeSlotUndefined;

		/**
		 * @brief An index of a vertex stream (vertex buffer slot) this attribute is fetched from.
		 * @see VertexInputAttributeDefinition::vertexStreamSlot
		 */
		uint8 vertexStreamSlot;

		/**
		 * @brief Additional data padding after the attribute data.
		 * @see VertexInputAttributeDefinition::dataPadding
		 */
		uint8 dataPadding;

		/**
		 *
		 * @see VertexInputAttributeDefinition::dataRate
		 */
		GCI::EIAVertexAttributeDataRate dataRate = GCI::EIAVertexAttributeDataRate::Undefined;

		/**
		 * Semantic name of the attribute.
		 * @see VertexInputAttributeDefinition::semantics
		 * @see GCI::IAVertexAttributeSemantics::semanticName
		 */
		cppx::immutable_string semanticName;

		/**
		 * Semantic index of the attribute.
		 * @see VertexInputAttributeDefinition::semantics
		 * @see GCI::IAVertexAttributeSemantics::semanticIndex
		 */
		uint8 semanticIndex = 0;

		/**
		 * Size of the semantic group this attribute belongs to. If this is 1, the attribute is a single-slot attribute
		 * with unique semantic name. If >1, this is one of up to 4 attributes that share the same semantic name and
		 * occupy adjacent IA slots.
		 * @see VertexInputAttributeDefinition::semanticGroupSize
		 */
		uint8 semanticGroupSize;

		/**
		 *
		 */
		cppx::bitmask<EVertexAttributeSemanticFlags> semanticFlags;

		/**
		 * 
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
	static_assert( sizeof( GenericVertexInputAttribute ) <= 24 );

	/**
	 *
	 */
	struct VertexInputStream
	{
		/**
		 *
		 */
		cppx::bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask;

		/**
		 */
		GCI::input_assembler_index_t streamSlot = GCI::kIAVertexStreamSlotUndefined;

		/**
		 *
		 */
		GCI::EIAVertexAttributeDataRate streamDataRate = GCI::EIAVertexAttributeDataRate::Undefined;

		/**
		 *
		 */
		uint16 dataStrideInBytes = 0;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsInitialized() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsSameAs( const VertexInputStream & pOther ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool CheckAttributeCompatibility( const VertexInputAttributeDefinition & pAttributeDefinition ) const noexcept;

		void Init( uint16 pStreamSlot, GCI::EIAVertexAttributeDataRate pDataRate );

		void Reset();
	};


	namespace VertexFormat
	{
		
		/**
		 * @brief 
		 * @param pVertexFormatAttributeArrayLayout 
		 * @param pVertexFormatStreamArrayLayout 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD GCI::IAVertexAttributeLayoutDefinition GCICreateVertexAttributeLayoutDefinition(
				const cppx::array_view<VertexInputAttributeDefinition> & pVertexAttributeDefinitions );

		/**
		 * @brief 
		 * @param pVertexFormatAttributeArrayLayout 
		 * @param pVertexFormatStreamArrayLayout 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD GCI::IAVertexAttributeLayoutDefinition GCICreateVertexAttributeLayoutDefinition(
				const cppx::array_view<GenericVertexInputAttribute> & pVertexAttributes );

		/**
		 * @brief 
		 * @param pVertexFormatAttributeArrayLayout 
		 * @param pVertexFormatStreamArrayLayout 
		 * @param pGCIVertexAttributeLayoutDefinition 
		 * @return 
		 */
		bool GCICreateVertexAttributeLayoutDefinition(
				const cppx::array_view<VertexInputAttributeDefinition> & pVertexAttributeDefinitions,
				GCI::IAVertexAttributeLayoutDefinition & pOutGCIAttributeLayoutDefinition );

		/**
		 * @brief 
		 * @param pVertexFormatAttributeArrayLayout 
		 * @param pVertexFormatStreamArrayLayout 
		 * @param pGCIVertexAttributeLayoutDefinition 
		 * @return 
		 */
		bool GCICreateVertexAttributeLayoutDefinition(
				const cppx::array_view<GenericVertexInputAttribute> & pVertexAttributes,
				GCI::IAVertexAttributeLayoutDefinition & pOutGCIAttributeLayoutDefinition );

	}

} // namespace Ic3

#include "VertexFormatCommon.inl"

#endif // __IC3_NXMAIN_VERTEX_FORMAT_COMMON_H__
