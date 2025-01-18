
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
	/// - attribute layout data (VertexAttributeArrayLayout),
	/// - stream layout data (VertexDataInputConfig),
	/// - format of the index data (GCI::EIndexDataFormat),
	/// - primitive topology (GCI::EPrimitiveTopology).
	/// Note, that VertexFormatDescriptor does not describe the location of the vertex data - it only refers to how the data
	/// is laid out in memory, which attributes are fetched from which stream, what are their offsets, strides, rates
	/// at which they are read by the input pipeline and so on. Consequently, the vertex stream layout describes the
	/// usage of each stream, but not their data directly.
	/// One VertexFormatDescriptor (just like in case of CVertexPipelineConfig) can be used with multiple geometry buffer
	/// sets, as long as their internal layout matches the one described by this layout objects.
	/// @see VertexAttributeArrayLayout
	/// @see VertexStreamArrayConfig
	/// @see GCI::EIndexDataFormat
	/// @see GCI::EPrimitiveTopology
	class IC3_NXMAIN_CLASS VertexFormatDescriptor : public IDynamicObject
	{
		friend class VertexFormatDescriptorBuilder;
		
	public:
		/// Read-only reference to the format of the index data.
		const GCI::EIndexDataFormat & mIndexDataFormat;

		///
		const GCI::EPrimitiveTopology & mPrimitiveTopology;

		/// Read-only reference to the attribute layout data.
		const VertexAttributeArrayLayout & mAttribArrayLayout;

		/// Read-only reference to the stream layout data.
		const VertexStreamArrayConfig & mStreamArrayConfig;

	public:
		VertexFormatDescriptor();

		VertexFormatDescriptor( VertexFormatDescriptor && pSource ) noexcept;
		VertexFormatDescriptor & operator=( VertexFormatDescriptor && pRhs ) noexcept;

		VertexFormatDescriptor( const VertexFormatDescriptor & pSource );
		VertexFormatDescriptor & operator=( const VertexFormatDescriptor & pRhs );

		~VertexFormatDescriptor();

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		/// @brief Returns true if the layout is empty, i.e. contains no active attribute definition, or false otherwise.
		CPPX_ATTR_NO_DISCARD bool empty() const noexcept;

		//
		CPPX_ATTR_NO_DISCARD bool valid() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD bool equals( const VertexFormatDescriptor & pOther ) const noexcept;

		/// @brief Returns true if the layout describes an indexed geometry, or false otherwise.
		CPPX_ATTR_NO_DISCARD bool isIndexedVertexFormat() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD uint32 getIndexDataSize() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD uint32 getElementStrideForAttribute( native_uint pAttribIASlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 getElementStrideForAttribute( cppx::string_view pSemanticName ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 getElementStrideForAttribute( VertexAttributeKey pAttributeKey ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 getElementStrideForAttribute( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD uint32 getElementStrideForAttributeUnchecked( native_uint pAttribIASlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 getElementStrideForAttributeUnchecked( VertexAttributeKey pAttributeKey ) const noexcept;

		/// @brief Returns true if the specified semantic description refers to an active attribute in the attribute array.
		/// @param pSemanticID The semantic ID of an attribute.
		CPPX_ATTR_NO_DISCARD bool hasAttributeWithSemantics( cppx::string_view pSemanticName ) const noexcept;
		CPPX_ATTR_NO_DISCARD bool hasAttributeWithSemantics( const ShaderSemantics & pSemantics ) const noexcept;
		CPPX_ATTR_NO_DISCARD bool hasAttributeWithSemantics( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept;

		/// @brief Returns a zero-based index in the attribute array that refers to the attribute with the specified properties.
		/// @param pAttributeKey An encoded key of the requested attribute.
		/// @param pSemanticIndex An optional index of a sub-component (valid only for multi-component attributes).
		CPPX_ATTR_NO_DISCARD uint32 resolveAttributeRef( const ShaderSemantics & pSemantics ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 resolveAttributeRef( cppx::string_view pSemanticName, uint32 pSemanticIndex = 0 ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 resolveAttributeRef( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex = 0 ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 resolveAttributeRef( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags, uint32 pSemanticIndex = 0 ) const noexcept;

		/// @brief Returns the usage info of a vertex stream with the specified index. If the index is not valid, a null pointer is returned.
		/// @param pAttributeIndex A zero-based index of a vertex attribute to retrieve.
		CPPX_ATTR_NO_DISCARD const GenericVertexAttribute * getAttribute( native_uint pAttribIASlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD const GenericVertexAttribute * getAttribute( const ShaderSemantics & pSemantics ) const noexcept;
		CPPX_ATTR_NO_DISCARD const GenericVertexAttribute * getAttribute( cppx::string_view pSemanticName, uint32 pSemanticIndex = 0 ) const noexcept;
		CPPX_ATTR_NO_DISCARD const GenericVertexAttribute * getAttribute( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex = 0 ) const noexcept;
		CPPX_ATTR_NO_DISCARD const GenericVertexAttribute * getAttribute( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags, uint32 pSemanticIndex = 0 ) const noexcept;

		/// @brief Returns the usage info of a vertex stream which contains the described vertex attribute.
		/// @return A pointer to the described attribute's stream or a null pointer if such attribute does not exist.
		CPPX_ATTR_NO_DISCARD const VertexInputStream * getStreamForAttribute( native_uint pAttribIASlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD const VertexInputStream * getStreamForAttribute( const ShaderSemantics & pSemantics ) const noexcept;
		CPPX_ATTR_NO_DISCARD const VertexInputStream * getStreamForAttribute( cppx::string_view pSemanticName ) const noexcept;
		CPPX_ATTR_NO_DISCARD const VertexInputStream * getStreamForAttribute( VertexAttributeKey pAttributeKey ) const noexcept;
		CPPX_ATTR_NO_DISCARD const VertexInputStream * getStreamForAttribute( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept;

		/// @brief Returns the usage info of a vertex stream with the specified index. If the index is not valid, a null pointer is returned.
		/// @param pAttributeIndex A zero-based index of a vertex attribute to retrieve.
		CPPX_ATTR_NO_DISCARD const VertexInputStream * getStream( native_uint pStreamIASlot ) const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD InputAssemblerSlotArray GetActiveAttributesSlots() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD InputAssemblerSlotArray GetActiveStreamsSlots() const noexcept;

		/// @brief Returns true if the specified index is an index of an active vertex attribute, or false otherwise.
		CPPX_ATTR_NO_DISCARD bool IsAttributeActive( native_uint pAttribIASlot ) const;

		/// @brief Returns true if the specified index is an index of an active vertex stream, or false otherwise.
		CPPX_ATTR_NO_DISCARD bool isVertexStreamActive( native_uint pStreamIASlot ) const;

		/// @brief Returns the string representation of the attribute layout.
		CPPX_ATTR_NO_DISCARD std::string generateVertexFormatStringID() const noexcept;

		/// @brief Returns an array of VertexAttributeDefinition values which can be used to re-Create the current attribute/stream layout.
		///
		/// The returned array is the exact representation of the current layout state. Formally speaking, the behaviour
		/// of this function is so that:
		/// - if L is an existing, non-empty VFD (Vertex Format Descriptor) with an index data format F
		/// - S is an empty layout with a matching index data format F
		CPPX_ATTR_NO_DISCARD std::vector<VertexAttributeDefinition> generateAttributeDefinitionArray() const noexcept;

	private:
		uint32 _resolveAttributeRefImpl( cppx::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept;
		uint32 _resolveAttributeRefImpl( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags, uint32 pSemanticIndex ) const noexcept;

	private:
		/// Format of the index buffer data.
		GCI::EIndexDataFormat _indexDataFormat = GCI::EIndexDataFormat::Undefined;

		///
		GCI::EPrimitiveTopology _primitiveTopology = GCI::EPrimitiveTopology::Undefined;

		/// Layout of the vertex attribute array.
		VertexAttributeArrayLayout _attribArrayLayout;

		/// Layout of the vertex stream array.
		VertexStreamArrayConfig _streamArrayConfig;
	};

	namespace GCU
	{

		IC3_NXMAIN_API_NO_DISCARD std::string generateVertexFormatLayoutString(
				const VertexAttributeArrayLayout & pAttributeLayout,
				const VertexStreamArrayConfig & pStreamConfig );

	}

} // namespace Ic3

#include "VertexFormatDescriptor.inl"

#endif // __IC3_NXMAIN_VERTEX_FORMAT_DESCRIPTOR_H__
