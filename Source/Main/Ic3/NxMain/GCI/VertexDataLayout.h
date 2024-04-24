
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_PIPELINE_LAYOUTS_H__
#define __IC3_NXMAIN_VERTEX_PIPELINE_LAYOUTS_H__

#include "VertexPipelineDefs.h"

namespace Ic3
{

	/// @brief Stores the layout configuration for the vertex data, i.e. attribute array and stream usage.
	///
	/// This is a high-level equivalent of the Input Assembler (IA) primitives from the GCI with some additional
	/// functionalities that allow easy access to attribute/stream properties, their layout and dependencies.
	/// Each instance of this class contains three main objects:
	/// - attribute layout data (SVertexAttributeArrayLayoutData),
	/// - stream layout data (SVertexStreamArrayLayoutData),
	/// - format of the index data (GCI::EIndexDataFormat),
	/// - primitive topology (GCI::EPrimitiveTopology).
	/// Note, that CVertexDataLayout does not describe the location of the vertex data - it only refers to how the data
	/// is laid out in memory, which attributes are fetched from which stream, what are their offsets, strides, rates
	/// at which they are read by the input pipeline and so on. Consequently, the vertex stream layout describes the
	/// usage of each stream, but not their data directly.
	/// One CVertexDataLayout (just like in case of CVertexPipelineConfig) can be used with multiple geometry buffer
	/// sets, as long as their internal layout matches the one described by this layout objects.
	/// @see SVertexAttributeArrayLayoutData
	/// @see SVertexStreamArrayLayoutData
	/// @see GCI::EIndexDataFormat
	/// @see GCI::EPrimitiveTopology
	class IC3_NXMAIN_CLASS CVertexDataLayout
	{
	public:
		/// Read-only reference to the format of the index data.
		const GCI::EIndexDataFormat & mIndexDataFormat;

		///
		const GCI::EPrimitiveTopology & mPrimitiveTopology;

		/// Read-only reference to the attribute layout data.
		const SVertexAttributeArrayLayoutData & mAttribLayoutData;

		/// Read-only reference to the stream layout data.
		const SVertexStreamArrayLayoutData & mStreamLayoutData;

	public:
		CVertexDataLayout();

		CVertexDataLayout( CVertexDataLayout && pSource ) noexcept;
		CVertexDataLayout & operator=( CVertexDataLayout && pRhs ) noexcept;

		CVertexDataLayout( const CVertexDataLayout & pSource );
		CVertexDataLayout & operator=( const CVertexDataLayout & pRhs );

		~CVertexDataLayout();

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		/// @brief Returns true if the layout is empty, i.e. contains no active attribute definition, or false otherwise.
		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		/// @brief
		IC3_ATTR_NO_DISCARD bool equals( const CVertexDataLayout & pOther ) const noexcept;

		/// @brief Returns true if the layout describes an indexed geometry, or false otherwise.
		IC3_ATTR_NO_DISCARD bool isIndexedGeometry() const noexcept;

		/// @brief Returns true if the specified index is an index of an active vertex attribute, or false otherwise.
		IC3_ATTR_NO_DISCARD bool isAttributeActive( uint32 pAttributeIndex ) const;

		/// @brief Returns true if the specified index is an index of an active vertex stream, or false otherwise.
		IC3_ATTR_NO_DISCARD bool isVertexStreamActive( uint32 pStreamIndex ) const;

		IC3_ATTR_NO_DISCARD uint32 getIndexDataSize() const noexcept;

		IC3_ATTR_NO_DISCARD const SortedArray<uint32> & getActiveAttributeIndexList() const noexcept;

		IC3_ATTR_NO_DISCARD const SortedArray<uint32> & getActiveStreamIndexList() const noexcept;

		/// @brief Returns true if the specified semantic description refers to an active attribute in the attribute array.
		/// @param pAttributeID The semantic ID of an attribute.
		IC3_ATTR_NO_DISCARD bool hasAttributeWithSemantics( EShaderInputSemanticID pSemanticID ) const noexcept;

		/// @brief Returns true if the specified semantic description refers to an active attribute in the attribute array.
		/// @param pSemanticName The semantic name of an attribute.
		IC3_ATTR_NO_DISCARD bool hasAttributeWithSemantics( std::string_view pSemanticName ) const noexcept;

		/// @brief Returns true if the specified semantic description refers to an active attribute in the attribute array.
		/// @param pAttributeID The attribute semantics.
		IC3_ATTR_NO_DISCARD bool hasAttributeWithSemantics( const SShaderSemantics & pSemantics ) const noexcept;

		/// @brief Returns a zero-based index in the attribute array that refers to the attribute with the specified properties.
		/// @param pAttributeID An ID of one of the standard attributes.
		/// @param pSubIndex An optional index of a sub-component (valid only for multi-component attributes).
		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( EStandardVertexAttributeID pAttributeID, uint32 pSubIndex = 0 ) const noexcept;

		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( EShaderInputSemanticID pSemanticID, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( std::string_view pSemanticName, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( const SShaderSemantics & pSemantics, uint32 pSubIndex = 0  ) const noexcept;

		/// @brief Returns the usage info of a vertex stream with the specified index. If the index is not valid, a null pointer is returned.
		/// @param pAttributeIndex A zero-based index of a vertex attribute to retrieve.
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( uint32 pAttributeIndex ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( EStandardVertexAttributeID pAttributeID, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( EShaderInputSemanticID pSemanticID, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( std::string_view pSemanticName, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( const SShaderSemantics & pSemantics, uint32 pSubIndex = 0 ) const noexcept;

		/// @brief Returns the usage info of a vertex stream which contains the described vertex attribute.
		/// @return A pointer to the described attribute's stream or a null pointer if such attribute does not exist.
		IC3_ATTR_NO_DISCARD const SVertexStreamUsageInfo * getStreamForAttribute( uint32 pAttributeIndex ) const noexcept;

		IC3_ATTR_NO_DISCARD const SVertexStreamUsageInfo * getStreamForAttribute( EStandardVertexAttributeID pAttributeID, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD const SVertexStreamUsageInfo * getStreamForAttribute( EShaderInputSemanticID pSemanticID, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD const SVertexStreamUsageInfo * getStreamForAttribute( std::string_view pSemanticName, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD const SVertexStreamUsageInfo * getStreamForAttribute( const SShaderSemantics & pSemantics, uint32 pSubIndex = 0 ) const noexcept;

		/// @brief Returns the usage info of a vertex stream with the specified index. If the index is not valid, a null pointer is returned.
		/// @param pAttributeIndex A zero-based index of a vertex attribute to retrieve.
		IC3_ATTR_NO_DISCARD const SVertexStreamUsageInfo * getStream( uint32 pStreamIndex ) const noexcept;

		/// @brief Returns the string representation of the attribute layout.
		IC3_ATTR_NO_DISCARD std::string generateVertexFormatStringID() const noexcept;

		/// @brief Returns an array of SVertexAttributeDefinition values which can be used to re-create the current attribute/stream layout.
		///
		/// The returned array is the exact representation of the current layout state. Formally speaking, the behaviour
		/// of this function is so that:
		/// - if L is an existing, non-empty VDL (Vertex Data Layout) with an index data format F
		/// - S is an empty layout with a matching index data format F
		IC3_ATTR_NO_DISCARD std::vector<SVertexAttributeDefinition> generateAttributeDefinitionArray() const noexcept;

		void setIndexDataFormat( GCI::EIndexDataFormat pIndexDataFormat );

		void setPrimitiveTopology( GCI::EPrimitiveTopology pPrimitiveTopology );

		const SGenericVertexAttributeInfo * addAttribute( SVertexAttributeDefinition pAttributeDefinition );

		const SGenericVertexAttributeInfo * addStandardAttribute(
				EStandardVertexAttributeID pAttributeID,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset = GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND,
				uint32 pDataStride = 0 );

		template <typename TVertex, typename TAttribute>
		bool addAttribute(
				uint32 pBaseIndex,
				TAttribute TVertex::* pAttributePtr,
				SShaderSemantics pSemantics,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset = GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND,
				uint32 pInstanceRate = 0 );

		template <typename TVertex, typename TAttribute>
		bool addStandardAttribute(
				EStandardVertexAttributeID pAttributeID,
				TAttribute TVertex::* pAttributePtr,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset = GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND,
				uint32 pDataStride = 0 );

		SortedArray<uint32> addAttributes( ArrayView<SVertexAttributeDefinition> pAttributeDefinitions );

		SortedArray<uint32> setAttributes( ArrayView<SVertexAttributeDefinition> pAttributeDefinitions );

		void resetIndexDataFormat();

		void resetPrimitiveTopology();

		void resetLayouts();

		void resetAll();

	private:
		/// Format of the index buffer data.
		GCI::EIndexDataFormat _indexDataFormat = GCI::EIndexDataFormat::Undefined;

		///
		GCI::EPrimitiveTopology _primitiveTopology = GCI::EPrimitiveTopology::Undefined;

		/// Layout of the vertex attribute array.
		SVertexAttributeArrayLayoutData _attribLayoutData;

		/// Layout of the vertex stream array.
		SVertexStreamArrayLayoutData _streamLayoutData;
	};

	inline bool CVertexDataLayout::hasAttributeWithSemantics( EShaderInputSemanticID pSemanticID ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticID, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeIndexUndefined;
	}

	inline bool CVertexDataLayout::hasAttributeWithSemantics( std::string_view pSemanticName ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticName, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeIndexUndefined;
	}

	inline bool CVertexDataLayout::hasAttributeWithSemantics( const SShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemantics, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeIndexUndefined;
	}

	inline const SGenericVertexAttributeInfo * CVertexDataLayout::getAttribute( uint32 pAttributeIndex ) const noexcept
	{
		return _attribLayoutData.attributePtr( pAttributeIndex );
	}

	inline const SGenericVertexAttributeInfo * CVertexDataLayout::getAttribute( EStandardVertexAttributeID pAttributeID, uint32 pSubIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pAttributeID, pSubIndex );
		return _attribLayoutData.attributePtr( resolvedAttributeIndex );
	}

	inline const SGenericVertexAttributeInfo * CVertexDataLayout::getAttribute( EShaderInputSemanticID pSemanticID, uint32 pSubIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticID, pSubIndex );
		return _attribLayoutData.attributePtr( resolvedAttributeIndex );
	}

	inline const SGenericVertexAttributeInfo * CVertexDataLayout::getAttribute( std::string_view pSemanticName, uint32 pSubIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticName, pSubIndex );
		return _attribLayoutData.attributePtr( resolvedAttributeIndex );
	}

	inline const SGenericVertexAttributeInfo * CVertexDataLayout::getAttribute( const SShaderSemantics & pSemantics, uint32 pSubIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemantics, pSubIndex );
		return _attribLayoutData.attributePtr( resolvedAttributeIndex );
	}

	inline const SVertexStreamUsageInfo * CVertexDataLayout::getStreamForAttribute( uint32 pAttributeIndex ) const noexcept
	{
		const auto * attributePtr = getAttribute( pAttributeIndex );
		return attributePtr ? getStream( attributePtr->vertexStreamIndex ) : nullptr;
	}

	inline const SVertexStreamUsageInfo * CVertexDataLayout::getStreamForAttribute( EStandardVertexAttributeID pAttributeID, uint32 pSubIndex ) const noexcept
	{
		const auto * attributePtr = getAttribute( pAttributeID );
		return attributePtr ? getStream( attributePtr->vertexStreamIndex ) : nullptr;
	}

	inline const SVertexStreamUsageInfo * CVertexDataLayout::getStreamForAttribute( EShaderInputSemanticID pSemanticID, uint32 pSubIndex ) const noexcept
	{
		const auto * attributePtr = getAttribute( pSemanticID, pSubIndex );
		return attributePtr ? getStream( attributePtr->vertexStreamIndex ) : nullptr;
	}

	inline const SVertexStreamUsageInfo * CVertexDataLayout::getStreamForAttribute( std::string_view pSemanticName, uint32 pSubIndex ) const noexcept
	{
		const auto * attributePtr = getAttribute( pSemanticName, pSubIndex );
		return attributePtr ? getStream( attributePtr->vertexStreamIndex ) : nullptr;
	}

	inline const SVertexStreamUsageInfo * CVertexDataLayout::getStreamForAttribute( const SShaderSemantics & pSemantics, uint32 pSubIndex ) const noexcept
	{
		const auto * attributePtr = getAttribute( pSemantics, pSubIndex );
		return attributePtr ? getStream( attributePtr->vertexStreamIndex ) : nullptr;
	}

	inline const SVertexStreamUsageInfo * CVertexDataLayout::getStream( uint32 pStreamIndex ) const noexcept
	{
		return _streamLayoutData.streamPtr( pStreamIndex );
	}

	template <typename TVertex, typename TAttribute>
	inline bool CVertexDataLayout::addAttribute(
			uint32 pBaseIndex,
			TAttribute TVertex::* pAttributePtr,
			SShaderSemantics pSemantics,
			uint32 pStreamIndex,
			uint32 pStreamRelativeOffset,
			uint32 pInstanceRate )
	{
		using Traits = GCI::QVertexAttribFormatDataTypeTraits<TAttribute>;

		SVertexAttributeDefinition attributeDefinition{};
		attributeDefinition.baseAttributeIndex = pBaseIndex;
		attributeDefinition.baseFormat = Traits::sBaseAttribFormat;
		attributeDefinition.componentsNum = Traits::sAttribComponentsNum;
		attributeDefinition.componentStride = Traits::sComponentSizeInBytes;
		attributeDefinition.instanceRate = 0;
		attributeDefinition.componentPadding = 0;
		attributeDefinition.vertexStreamIndex = pStreamIndex;
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.semantics = std::move( pSemantics );

		return addAttribute( std::move( attributeDefinition ) );
	}

	template <typename TVertex, typename TAttribute>
	inline bool CVertexDataLayout::addStandardAttribute(
			EStandardVertexAttributeID pAttributeID,
			TAttribute TVertex::* pAttributePtr,
			uint32 pStreamIndex,
			uint32 pStreamRelativeOffset,
			uint32 pDataStride )
	{
		using Traits = GCI::QVertexAttribFormatDataTypeTraits<TAttribute>;

		const auto fixedAttributeFormat = CxDef::getStandardVertexAttributeBaseFormat( pAttributeID );
		const auto fixedAttributeComponentsNum = CxDef::getStandardVertexAttributeComponentsNum( pAttributeID );

		if( !GCIUtils::checkAttributeAutoDataFormat<Traits>( fixedAttributeFormat, fixedAttributeComponentsNum ) )
		{
			return false;
		}

		SVertexAttributeDefinition attributeDefinition{};
		attributeDefinition.baseAttributeIndex = CxDef::getStandardVertexAttributeBaseIndex( pAttributeID );
		attributeDefinition.baseFormat = Traits::sBaseAttribFormat;
		attributeDefinition.componentsNum = Traits::sAttribComponentsNum;
		attributeDefinition.componentStride = Traits::sComponentSizeInBytes;
		attributeDefinition.instanceRate = 0;
		attributeDefinition.componentPadding = 0;
		attributeDefinition.vertexStreamIndex = pStreamIndex;
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.semantics = SShaderSemantics{ pAttributeID };

		return addAttribute( std::move( attributeDefinition ) );
	}

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_PIPELINE_LAYOUTS_H__
