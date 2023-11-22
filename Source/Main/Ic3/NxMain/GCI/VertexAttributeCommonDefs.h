
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
#define __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__

#include "ShaderInputSemantics.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>

namespace Ic3
{

	struct SVertexAttributeArrayLayoutData;
	struct SVertexStreamArrayLayoutData;

	class CVertexPipelineConfig;
	class CVertexPipelineConfigBuilder;

	using SVertexAttributeIndexRange = SRange<uint32>;
	using SVertexStreamIndexRange = SRange<uint32>;


	/// Represents an invalid vertex attribute index.
	constexpr auto cxGCIVertexAttributeIndexUndefined = GCI::CxDef::IA_VERTEX_ATTRIBUTE_INDEX_UNDEFINED;

	///
	inline constexpr SVertexAttributeIndexRange cxGCIValidVertexAttributeIndexRange{ 0u, GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM - 1 };

	///
	inline constexpr SVertexAttributeIndexRange cxGCIValidVertexAttributeComponentsNumberRange{ 1u, GCM::IA_MAX_VERTEX_ATTRIBUTE_COMPONENTS_NUM };

	///
	inline constexpr SVertexStreamIndexRange cxGCIValidVertexStreamIndexRange{ 0u, GCM::IA_MAX_VERTEX_STREAMS_NUM - 1 };


	/// @brief Vertex attribute class. Used to identify the meaning of an attribute definition.
	enum class EVertexAttributeClass : uint16
	{
		/// Undefined class. Used to identify uninitialised data.
		/// @see SGenericVertexAttributeInfo
		Undefined,

		/// Base attribute. This is used for two types of definitions:
		/// - Single-component attributes: attributes that occupy a single slot in the IA attribute array
		/// - Base components of a multi-component attributes: the first, base component of attributes which occupy more
		///   than one slot in the IA attribute array.
		/// Generally speaking, this class applies to all attributes with componentIndex set to 0.
		/// @see SGenericVertexAttributeInfo::componentIndex
		BaseAttribute,

		/// A sub-component of a multi-component attribute (an attribute that occupies more than one slot in the IA attribute
		/// array). Corresponding attribute definitions will have a non-zero componentIndex.
		/// @see SGenericVertexAttributeInfo::componentIndex
		SubComponent,
	};

	enum EVertexDataRate : uint32
	{
		PerVertex,
		PerInstance,
		Undefined = QLimits<uint32>::maxValue
	};

	/// @brief Common part of all vertex attribute-related data structures. Contains basic properties of an attribute.
	struct SCommonVertexAttributeData
	{
		/// Base format of an attribute. For multi-component attributes, this describes an individual component.
		GCI::EVertexAttribFormat baseFormat = GCI::EVertexAttribFormat::Undefined;

		/// Base attribute index. Allowed values are from 0 to (GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM - 1).
		/// For multi-component attributes, this is the index of the first occupied attribute slot.
		uint16 baseAttributeIndex = GCI::CxDef::IA_VERTEX_ATTRIBUTE_INDEX_UNDEFINED;

		/// Number of components (semantic sub-attributes) this attribute contains. It equals to the number of slots
		/// in the attribute array this attribute occupies. For base attributes this will be at least <1>, but no more
		/// than GCM::IA_MAX_VERTEX_ATTRIBUTE_COMPONENTS_NUM. For non-base attributes this is set to 0.
		uint16 componentsNum = 1;

		/// Instance rate. A value of 0 means the attribute contains per-vertex data.
		uint16 instanceRate = 0;

		/// An index of a vertex buffer slot this attribute is fetched from.
		uint16 vertexStreamIndex = GCI::CxDef::IA_VERTEX_BUFFER_BINDING_INDEX_UNDEFINED;

		/// An offset from the start of the vertex buffer data to the beginning of the attribute's data.
		/// This is a *relative* offset from the start of the bound range of the buffer, not from it's physical base address.
		uint32 vertexStreamRelativeOffset = 0;

		/// Semantics of the attribute.
		SShaderSemantics semantics {};

		/// Returns the base size of the attribute described, i.e. the size of a single component (which equals the total
		/// attribute size for the most common, single-component attributes).
		IC3_ATTR_NO_DISCARD uint32 baseSize() const noexcept
		{
			return GCI::CxDef::getVertexAttribFormatByteSize( baseFormat );
		}

		///
		IC3_ATTR_NO_DISCARD EVertexDataRate getVertexDataRate() const noexcept
		{
			return ( instanceRate == 0 ) ? EVertexDataRate::PerVertex : EVertexDataRate::PerInstance;
		}
	};

	/// @brief
	struct SVertexAttributeDefinition : public SCommonVertexAttributeData
	{
		/// Defines an extra padding applied to each of the attribute's subcomponent. Affects the combined data stride.
		/// @example Consider an attribute which is a 3x3 float matrix. Such attribute would occupy three generic
		/// attribute slots, each containing a single 3-component float vector. In order to get each attribute aligned
		/// on a 16-byte boundary, we could place the data like this:
		/// |-----------------------------------------------------------|
		/// [M00][M01][M02][PAD][M10][M11][M12][PAD][M20][M21][M22][PAD]
		/// |-----------------------------------------------------------|
		/// (PAD being 4 bytes of padding). In this case:
		/// - baseFormat is GCI::EVertexAttribFormat::Vec3F32
		/// - dataStride should be 48 (or 0 which will result in 64 computed automatically)
		/// - subComponentPadding should be 4
		/// - subComponentsNum should be 3
		uint16 componentPadding = 0;

		/// If 0, set to baseSize() + componentPadding.
		uint16 componentStride = 0;

		///
		IC3_ATTR_NO_DISCARD bool hasAppendAsRelativeOffset() const noexcept
		{
			return ( vertexStreamRelativeOffset == GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND ) ||
			       ( vertexStreamRelativeOffset == GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 );
		}

		/// Returns true if the attribute specification is valid, i.e. active (SCommonVertexAttributeData::active()
		/// returns true) and the number of components and their padding have correct values.
		IC3_ATTR_NO_DISCARD bool valid() const noexcept
		{
			return //
			       ( baseFormat != GCI::EVertexAttribFormat::Undefined ) &&
			       ( baseAttributeIndex != GCI::CxDef::IA_VERTEX_ATTRIBUTE_INDEX_UNDEFINED ) &&
			       ( vertexStreamIndex != GCI::CxDef::IA_VERTEX_BUFFER_BINDING_INDEX_UNDEFINED ) &&
			       // Vertex attribute index should be in the valid range of supported values.
			       cxGCIValidVertexAttributeIndexRange.contains( baseAttributeIndex ) &&
			       // Vertex stream index should be in the valid range of supported values.
			       cxGCIValidVertexStreamIndexRange.contains( vertexStreamIndex ) &&
			       // Each component has to have at least one component and no more than GCM::IA_MAX_VERTEX_ATTRIBUTE_COMPONENTS_NUM.
			       cxGCIValidVertexAttributeComponentsNumberRange.contains( componentsNum ) &&
			       // Attributes can have multiple components (e.g. a 4x4 matrix is a 4-component attribute, with each component
			       // being a 4-element vector). Even though the base index is valid, we need to check all potential sub-attributes.
			       cxGCIValidVertexAttributeIndexRange.contains( baseAttributeIndex + componentsNum );

		}
	};
} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_ATTRIBUTE_COMMON_DEFS_H__
