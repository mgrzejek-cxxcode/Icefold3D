
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_PIPELINE_DEFS_H__
#define __IC3_NXMAIN_VERTEX_PIPELINE_DEFS_H__

#include "VertexAttributeCommonDefs.h"
#include <Ic3/Cppx/SortedArray.h>

namespace Ic3
{

	/// @brief A definition of a generic vertex attribute. This is a per-slot definition - for multi-component attributes
	/// it contains properties of a single component of such attribute (as opposed to SBaseVertexAttributeInfo).
	struct SGenericVertexAttributeInfo : public SCommonVertexAttributeData
	{
		/// Attribute class - either a base attribute or a sub-component.
		EVertexAttributeClass attributeClass = EVertexAttributeClass::Undefined;

		/// Index of the sub-component. For base components and single-component attributes this is zero.
		uint16 componentIndex = 0;

		/// Data stride for this attribute. It is at least baseSize(), but may be greater if either an additional,
		/// per-component padding (SVertexAttributeDefinition::componentPadding) or an explicit component stride
		/// (SVertexAttributeDefinition::componentStride) have been specified.
		/// @see SVertexAttributeDefinition::componentPadding
		/// @see SVertexAttributeDefinition::componentStride
		uint16 elementStride = 0;

		/// Returns true if the attribute data describes an active attribute.
		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return active();
		}

		///
		IC3_ATTR_NO_DISCARD bool active() const noexcept
		{
			return ( attributeClass != EVertexAttributeClass::Undefined ) &&
			       ( baseFormat != GCI::EVertexAttribFormat::Undefined ) &&
			       ( baseAttributeIndex != GCI::CxDef::IA_VERTEX_ATTRIBUTE_INDEX_UNDEFINED ) &&
			       ( vertexStreamIndex != GCI::CxDef::IA_VERTEX_BUFFER_BINDING_INDEX_UNDEFINED );
		}

		///
		IC3_ATTR_NO_DISCARD uint32 attributeArrayIndex() const noexcept
		{
			return baseAttributeIndex + componentIndex;
		}

		/// Returns true if the attribute defined by this instance is a base attribute, i.e. either a single-component
		/// attribute or a base component
		IC3_ATTR_NO_DISCARD bool isBaseAttribute() const noexcept
		{
			return attributeClass == EVertexAttributeClass::BaseAttribute;
		}

		///
		IC3_ATTR_NO_DISCARD bool equals( const SGenericVertexAttributeInfo & pOther ) const noexcept
		{
			return ( baseFormat == pOther.baseFormat ) &&
			       ( baseAttributeIndex == pOther.baseAttributeIndex ) &&
			       ( componentsNum == pOther.componentsNum ) &&
			       ( instanceRate == pOther.instanceRate ) &&
			       ( vertexStreamIndex == pOther.vertexStreamIndex ) &&
			       ( vertexStreamRelativeOffset == pOther.vertexStreamRelativeOffset ) &&
			       ( semantics.smtName == pOther.semantics.smtName ) &&
			       ( attributeClass == pOther.attributeClass ) &&
			       ( componentIndex == pOther.componentIndex ) &&
			       ( elementStride == pOther.elementStride );
		}

		void initBaseAttributeFromDefinition( SVertexAttributeDefinition pDefinition )
		{
			baseFormat = pDefinition.baseFormat;
			baseAttributeIndex = pDefinition.baseAttributeIndex;
			componentsNum = pDefinition.componentsNum;
			instanceRate = pDefinition.instanceRate;
			vertexStreamIndex = pDefinition.vertexStreamIndex;
			vertexStreamRelativeOffset = pDefinition.vertexStreamRelativeOffset;
			semantics = std::move( pDefinition.semantics );
			attributeClass = EVertexAttributeClass::BaseAttribute;
			componentIndex = 0;
			elementStride = pDefinition.componentStride;
		}

		void initSubComponentFromBaseAttribute( const SGenericVertexAttributeInfo & pBaseAttribute, uint32 pComponentIndex )
		{
			baseFormat = pBaseAttribute.baseFormat;
			baseAttributeIndex = pBaseAttribute.baseAttributeIndex;
			componentsNum = 0;
			instanceRate = pBaseAttribute.instanceRate;
			vertexStreamIndex = pBaseAttribute.vertexStreamIndex;
			vertexStreamRelativeOffset = pBaseAttribute.vertexStreamRelativeOffset + ( pComponentIndex * pBaseAttribute.elementStride );
			attributeClass = EVertexAttributeClass::SubComponent;
			componentIndex = pComponentIndex;
			elementStride = pBaseAttribute.elementStride;
		}

		void reset()
		{
			attributeClass = EVertexAttributeClass::Undefined;
			baseFormat = GCI::EVertexAttribFormat::Undefined;
			baseAttributeIndex = GCI::CxDef::IA_VERTEX_ATTRIBUTE_INDEX_UNDEFINED;
			vertexStreamIndex = GCI::CxDef::IA_VERTEX_BUFFER_BINDING_INDEX_UNDEFINED;
			semantics.clear();
		}
	};

	/// @brief
	struct SVertexStreamUsageInfo
	{
		Bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask {};

		uint16 activeAttributesNum = 0;

		uint16 elementStrideInBytes = 0;

		EVertexDataRate streamDataRate = EVertexDataRate::Undefined;

		/// Returns true if this instance describes an active vertex data stream.
		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return active();
		}

		IC3_ATTR_NO_DISCARD bool active() const noexcept
		{
			return !activeAttributesMask.empty() && ( activeAttributesNum > 0 ) && ( streamDataRate != EVertexDataRate::Undefined );
		}

		IC3_ATTR_NO_DISCARD bool checkAttributeCompatibility( const SCommonVertexAttributeData & pAttributeData ) const noexcept
		{
			return ( streamDataRate == EVertexDataRate::Undefined ) || ( streamDataRate == pAttributeData.getVertexDataRate() );
		}

		void initWithFirstAttribute( const SCommonVertexAttributeData & pAttributeData )
		{
			streamDataRate = pAttributeData.getVertexDataRate();
		}

		void reset()
		{
			activeAttributesMask.clear();
			activeAttributesNum = 0;
			elementStrideInBytes = 0;
			streamDataRate = EVertexDataRate::Undefined;
		}
	};

	/// @brief
	struct SVertexAttributeArrayLayoutData
	{
		using AttributeArray = std::array<SGenericVertexAttributeInfo, GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM>;
		using AttributeIndexArray = SortedArray<uint32>;
		using SemanticIDMap = std::unordered_map<EShaderInputSemanticID, uint32>;
		using SemanticNameMap = std::unordered_map<std::string_view, uint32>;

		AttributeArray attributesArray;
		uint32 activeBaseAttributesNum = 0;
		uint32 activeGenericAttributesNum = 0;
		SVertexAttributeIndexRange activeAttributesRange = SVertexAttributeIndexRange::maxRange();
		AttributeIndexArray activeAttributes;
		Bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask;
		Bitmask<EShaderInputSemanticFlags> activeAttributeSemanticsMask;
		SemanticIDMap semanticIDMap;
		SemanticNameMap semanticNameMap;

		SGenericVertexAttributeInfo & operator[]( size_t pAttributeIndex ) noexcept
		{
			ic3DebugAssert( cxGCIValidVertexAttributeIndexRange.contains( pAttributeIndex ) );
			return attributesArray[pAttributeIndex];
		}

		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo & operator[]( size_t pAttributeIndex ) const noexcept
		{
			ic3DebugAssert( cxGCIValidVertexAttributeIndexRange.contains( pAttributeIndex ) );
			return attributesArray[pAttributeIndex];
		}

		SGenericVertexAttributeInfo & attributeAt( size_t pAttributeIndex )
		{
			ic3DebugAssert( cxGCIValidVertexAttributeIndexRange.contains( pAttributeIndex ) );
			return attributesArray.at( pAttributeIndex );
		}

		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo & attributeAt( size_t pAttributeIndex ) const
		{
			ic3DebugAssert( cxGCIValidVertexAttributeIndexRange.contains( pAttributeIndex ) );
			return attributesArray.at( pAttributeIndex );
		}

		IC3_ATTR_NO_DISCARD SGenericVertexAttributeInfo * attributePtr( size_t pAttributeIndex )
		{
			return cxGCIValidVertexAttributeIndexRange.contains( pAttributeIndex ) ? &( attributesArray[pAttributeIndex] ) : nullptr;
		}

		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * attributePtr( size_t pAttributeIndex ) const
		{
			return cxGCIValidVertexAttributeIndexRange.contains( pAttributeIndex ) ? &( attributesArray[pAttributeIndex] ) : nullptr;
		}

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return activeGenericAttributesNum == 0;
		}

		void reset()
		{
			for( auto & attributeInfo : attributesArray )
			{
				attributeInfo.reset();
			}

			activeBaseAttributesNum = 0;
			activeGenericAttributesNum = 0;
			activeAttributesRange = SVertexAttributeIndexRange::maxRange();
			activeAttributes.clear();
			activeAttributesMask.clear();
			activeAttributeSemanticsMask.clear();
			semanticIDMap.clear();
			semanticNameMap.clear();
		}
	};

	/// @brief
	struct SVertexStreamArrayLayoutData
	{
		using StreamArray = std::array<SVertexStreamUsageInfo, GCM::IA_MAX_VERTEX_STREAMS_NUM>;
		using StreamIndexArray = SortedArray<uint32>;

		StreamArray streamArray;
		uint32 activeStreamsNum = 0;
		SVertexStreamIndexRange activeStreamsRange = SVertexStreamIndexRange::maxRange();
		StreamIndexArray activeStreams;
		Bitmask<GCI::EIAVertexStreamBindingFlags> activeStreamsMask;

		SVertexStreamUsageInfo & operator[]( size_t pStreamIndex ) noexcept
		{
			ic3DebugAssert( cxGCIValidVertexStreamIndexRange.contains( pStreamIndex ) );
			return streamArray[pStreamIndex];
		}

		IC3_ATTR_NO_DISCARD const SVertexStreamUsageInfo & operator[]( size_t pStreamIndex ) const noexcept
		{
			ic3DebugAssert( cxGCIValidVertexStreamIndexRange.contains( pStreamIndex ) );
			return streamArray[pStreamIndex];
		}

		SVertexStreamUsageInfo & streamAt( size_t pStreamIndex )
		{
			ic3DebugAssert( cxGCIValidVertexStreamIndexRange.contains( pStreamIndex ) );
			return streamArray.at( pStreamIndex );
		}

		IC3_ATTR_NO_DISCARD const SVertexStreamUsageInfo & streamAt( size_t pStreamIndex ) const
		{
			ic3DebugAssert( cxGCIValidVertexStreamIndexRange.contains( pStreamIndex ) );
			return streamArray.at( pStreamIndex );
		}

		IC3_ATTR_NO_DISCARD SVertexStreamUsageInfo * streamPtr( size_t pStreamIndex ) noexcept
		{
			return cxGCIValidVertexStreamIndexRange.contains( pStreamIndex ) ? &( streamArray[pStreamIndex] ) : nullptr;
		}

		IC3_ATTR_NO_DISCARD const SVertexStreamUsageInfo * streamPtr( size_t pStreamIndex ) const noexcept
		{
			return cxGCIValidVertexStreamIndexRange.contains( pStreamIndex ) ? &( streamArray[pStreamIndex] ) : nullptr;
		}

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return activeStreamsNum == 0;
		}

		void reset()
		{
			for( auto & streamUsage : streamArray )
			{
				streamUsage.reset();
			}

			activeStreamsNum = 0;
			activeStreamsRange = SVertexStreamIndexRange::maxRange();
			activeStreams.clear();
			activeStreamsMask.clear();
		}
	};

	namespace GCIUtils
	{

		IC3_NXMAIN_API_NO_DISCARD bool checkAttributeLayoutArraySlotsFree(
				const SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				uint32 pAttributeBaseIndex,
				uint32 pAttributeComponentsNum );

		IC3_NXMAIN_API_NO_DISCARD bool validateVertexAttributeDefinition(
				const SVertexAttributeDefinition & pAttributeDefinition,
				const SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				const SVertexStreamArrayLayoutData * pStreamLayoutData = nullptr );

		IC3_NXMAIN_API_NO_DISCARD bool validateVertexAttributeDefinitionWithUpdate(
				SVertexAttributeDefinition & pAttributeDefinition,
				const SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				const SVertexStreamArrayLayoutData * pStreamLayoutData = nullptr );

		IC3_NXMAIN_API SGenericVertexAttributeInfo * appendVertexLayoutAttribute(
				SVertexAttributeDefinition pAttributeDefinition,
				SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				SVertexStreamArrayLayoutData * pStreamLayoutData = nullptr );

		IC3_NXMAIN_API bool appendVertexStreamAttribute(
				SVertexStreamArrayLayoutData & pStreamLayoutData,
				const SGenericVertexAttributeInfo & pBaseAttribute );

		IC3_NXMAIN_API bool createVertexStreamLayoutFromAttributeLayout(
				const SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				SVertexStreamArrayLayoutData & pStreamLayoutData );

		IC3_NXMAIN_API bool createVertexStreamLayoutFromAttributeLayoutWithUpdate(
				SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				SVertexStreamArrayLayoutData & pStreamLayoutData );

		IC3_NXMAIN_API_NO_DISCARD const char * getAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat );

		IC3_NXMAIN_API_NO_DISCARD const char * getShaderSemanticShortName( const std::string_view & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD std::string generateVertexAttributeFormatString( const SGenericVertexAttributeInfo & pAttributeInfo );

		IC3_NXMAIN_API_NO_DISCARD std::string generateVertexAttributeLayoutString( const SVertexAttributeArrayLayoutData & pLayoutData );

		template <typename TAttributeFormatTraits>
		IC3_ATTR_NO_DISCARD inline bool checkAttributeAutoDataFormat(
				GCI::EVertexAttribFormat pAttributeBaseFormat,
				uint32 pAttributeComponentsNum = 0 ) noexcept
		{
			if( pAttributeBaseFormat != TAttributeFormatTraits::sBaseAttribFormat )
			{
				return false;
			}

			if( ( pAttributeComponentsNum != 0 ) && ( TAttributeFormatTraits::sAttribComponentsNum != pAttributeComponentsNum ) )
			{
				return false;
			}

			const auto specifiedComponentByteSize = GCI::CxDef::getVertexAttribFormatByteSize( pAttributeBaseFormat );
			const auto specifiedAttributeByteSize = specifiedComponentByteSize * pAttributeComponentsNum;

			if( specifiedAttributeByteSize != TAttributeFormatTraits::sSizeInBytes )
			{
				return false;
			}

			return true;
		}

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_PIPELINE_DEFS_H__
