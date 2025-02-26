
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_FORMAT_DESCRIPTOR_H__
#define __IC3_NXMAIN_VERTEX_FORMAT_DESCRIPTOR_H__

#include "IAVertexAttribLayout.h"
#include "IAVertexStreamLayout.h"
#include "VertexAttributeCommonDefs.h"
#include <cppx/hash.h>

namespace Ic3
{

	class VertexFormatDescriptor;

	/// @brief Stores the layout configuration for the vertex data, i.e. attribute array and stream usage.
	///
	/// This is a high-level equivalent of the Input Assembler (IA) primitives from the GCI with some additional
	/// functionalities that allow easy access to attribute/stream properties, their layout and dependencies.
	/// Each instance of this class defines the following properties of the IA stage:
	/// - attribute layout data (VertexInputAttributeArrayConfig),
	/// - stream layout data (VertexDataInputConfig),
	/// - format of the index data (GCI::EIndexDataFormat),
	/// - primitive topology (GCI::EPrimitiveTopology).
	/// Note, that VertexFormatDescriptor does not describe the location of the vertex data - it only refers to how the data
	/// is laid out in memory, which attributes are fetched from which stream, what are their offsets, strides, rates
	/// at which they are read by the input pipeline and so on. Consequently, the vertex stream layout describes the
	/// usage of each stream, but not their data directly.
	/// One VertexFormatDescriptor can be used with multiple geometry buffer sets, as long as their internal layout
	/// matches the one described by the descriptor.
	/// @see VertexInputAttributeArrayConfig
	/// @see VertexInputStreamArrayConfig
	/// @see GCI::EIndexDataFormat
	/// @see GCI::EPrimitiveTopology
	class IC3_NXMAIN_CLASS VertexFormatDescriptor final : public IDynamicObject
	{
		friend class VertexFormatDescriptorBuilder;
		
	public:
		/// Read-only reference to the format of the index data.
		const GCI::EIndexDataFormat & mIndexDataFormat;

		///
		const GCI::EPrimitiveTopology & mPrimitiveTopology;

		/// Read-only reference to the attribute layout data.
		const VertexInputAttributeArrayConfig & mInputAttributeArrayLayout;

		/// Read-only reference to the stream layout data.
		const VertexInputStreamArrayConfig & mInputStreamArrayConfig;

	public:
		VertexFormatDescriptor();

		VertexFormatDescriptor( VertexFormatDescriptor && pSource ) noexcept;
		VertexFormatDescriptor & operator=( VertexFormatDescriptor && pRhs ) noexcept;

		VertexFormatDescriptor( const VertexFormatDescriptor & pSource );
		VertexFormatDescriptor & operator=( const VertexFormatDescriptor & pRhs );

		virtual ~VertexFormatDescriptor();

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		/// @brief Returns true if the layout is empty, i.e. contains no active attribute definition, or false otherwise.
		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		//
		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD bool Equals( const VertexFormatDescriptor & pOther ) const noexcept;

		/// @brief Returns true if the layout describes an indexed geometry, or false otherwise.
		CPPX_ATTR_NO_DISCARD bool IsIndexedVertexFormat() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD uint32 GetIndexDataSize() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttribute(
				native_uint pAttributeSlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttribute(
				cppx::string_view pSemanticName ) const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttribute(
				VertexAttributeKey pAttributeKey ) const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttribute(
				cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags ) const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttributeUnchecked( native_uint pAttributeSlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttributeUnchecked( VertexAttributeKey pAttributeKey ) const noexcept;

		/// @brief Returns true if the specified semantic description refers to an active attribute in the attribute array.
		CPPX_ATTR_NO_DISCARD bool HasAttributeWithSemantics(
				cppx::string_view pSemanticName ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasAttributeWithSemantics(
				const ShaderSemantics & pSemantics ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasAttributeWithSemantics(
				cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags ) const noexcept;

		/// @brief Returns a zero-based index in the attribute array that refers to the attribute with the specified properties.
		CPPX_ATTR_NO_DISCARD uint32 ResolveAttributeRef(
				const ShaderSemantics & pSemantics ) const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 ResolveAttributeRef(
				cppx::string_view pSemanticName,
				uint32 pSemanticIndex = 0 ) const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 ResolveAttributeRef(
				VertexAttributeKey pAttributeKey,
				uint32 pSemanticIndex = 0 ) const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 ResolveAttributeRef(
				cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags,
				uint32 pSemanticIndex = 0 ) const noexcept;

		/// @brief Returns the usage info of a vertex stream with the specified index. If the index is not valid, a null pointer is returned.
		/// @param pAttributeSlot A zero-based index of a vertex attribute to retrieve.
		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute * GetAttribute(
				native_uint pAttributeSlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute * GetAttribute(
				const ShaderSemantics & pSemantics ) const noexcept;

		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute * GetAttribute(
				cppx::string_view pSemanticName,
				uint32 pSemanticIndex = 0 ) const noexcept;

		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute * GetAttribute(
				VertexAttributeKey pAttributeKey,
				uint32 pSemanticIndex = 0 ) const noexcept;

		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute * GetAttribute(
				cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags,
				uint32 pSemanticIndex = 0 ) const noexcept;

		/// @brief Returns the usage info of a vertex stream which contains the described vertex attribute.
		/// @return A pointer to the described attribute's stream or a null pointer if such attribute does not exist.
		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStreamForAttribute( native_uint pAttributeSlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStreamForAttribute( const ShaderSemantics & pSemantics ) const noexcept;
		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStreamForAttribute( cppx::string_view pSemanticName ) const noexcept;
		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStreamForAttribute( VertexAttributeKey pAttributeKey ) const noexcept;

		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStreamForAttribute(
				cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags ) const noexcept;

		/// @brief Returns the usage info of a vertex stream with the specified index. If the index is not valid, a null pointer is returned.
		/// @param pAttributeSlot A zero-based index of a vertex attribute to retrieve.
		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStream( native_uint pStreamSlot ) const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD InputAssemblerSlotArray GetActiveAttributesSlots() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD InputAssemblerSlotArray GetActiveStreamsSlots() const noexcept;

		/// @brief Returns true if the specified index is an index of an active vertex attribute, or false otherwise.
		CPPX_ATTR_NO_DISCARD bool IsAttributeActive( native_uint pAttributeSlot ) const;

		/// @brief Returns true if the specified index is an index of an active vertex stream, or false otherwise.
		CPPX_ATTR_NO_DISCARD bool IsVertexStreamActive( native_uint pStreamSlot ) const;

		/// @brief Returns the string representation of the attribute layout.
		CPPX_ATTR_NO_DISCARD std::string GenerateVertexFormatStringID() const noexcept;

		/// @brief Returns an array of VertexAttributeDefinition values which can be used to re-Create the current attribute/stream layout.
		///
		/// The returned array is the exact representation of the current layout state. Formally speaking, the behaviour
		/// of this function is so that:
		/// - if L is an existing, non-empty VFD (Vertex Format Descriptor) with an index data format F
		/// - S is an empty layout with a matching index data format F
		CPPX_ATTR_NO_DISCARD std::vector<VertexInputAttributeDefinition> GenerateAttributeDefinitionArray() const noexcept;

	private:
		uint32 _ResolveAttributeRefImpl(
				cppx::string_view pSemanticName,
				uint32 pSemanticIndex ) const noexcept;

		uint32 _ResolveAttributeRefImpl(
				cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags,
				uint32 pSemanticIndex ) const noexcept;

	private:
		/// Format of the index buffer data.
		GCI::EIndexDataFormat _indexDataFormat = GCI::EIndexDataFormat::Undefined;

		///
		GCI::EPrimitiveTopology _primitiveTopology = GCI::EPrimitiveTopology::Undefined;

		/// Layout of the vertex attribute array.
		VertexInputAttributeArrayConfig _inputAttributeArrayLayout;

		/// Layout of the vertex stream array.
		VertexInputStreamArrayConfig _inputStreamArrayConfig;
	};

	namespace GCU
	{

		IC3_NXMAIN_API_NO_DISCARD std::string GenerateVertexFormatLayoutString(
				const VertexInputAttributeArrayConfig & pAttributeLayout,
				const VertexInputStreamArrayConfig & pStreamConfig );

	}

} // namespace Ic3

#include "VertexFormatDescriptor.inl"

#endif // __IC3_NXMAIN_VERTEX_FORMAT_DESCRIPTOR_H__
