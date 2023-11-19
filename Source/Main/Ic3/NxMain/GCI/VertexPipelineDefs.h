
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_PIPELINE_DEFS_H__
#define __IC3_NXMAIN_VERTEX_PIPELINE_DEFS_H__

#include "VertexAttributeCommonDefs.h"
#include <Ic3/Cppx/SortedArray.h>

namespace Ic3
{

	/// @brief A definition of a generic vertex attribute. This is a per-slot definition - for multi-component attributes
	/// it contains properties of a single component of such attribute (as opposed to SBaseVertexAttributeInfo).
	struct SGenericVertexAttributeInfo : public SCommonVertexAttributeInfo
	{
		/// Attribute class - either a base attribute or a sub-component.
		EVertexAttributeClass attributeClass = EVertexAttributeClass::Undefined;

		/// Index of the sub-component. For base components and single-component attributes this is zero.
		uint16 subComponentIndex = 0;

		uint16 subComponentsNum = 1;

		/// Pointer to the base component (with subComponentIndex=0).
		SGenericVertexAttributeInfo * baseAttribute = nullptr;

		/// Pointer to the next component of the attribute. For single-component attributes and the last component of
		/// multi-component attribute this is nullptr.
		SGenericVertexAttributeInfo * nextComponent = nullptr;

		/// Returns true if the attribute defined by this instance is a base attribute, i.e. either a single-component
		/// attribute or a base component
		IC3_ATTR_NO_DISCARD bool isBaseAttribute() const noexcept
		{
			return attributeClass == EVertexAttributeClass::BaseAttribute;
		}

		/// Returns the absolute, zero-based index of the slot in the IA attribute array.
		IC3_ATTR_NO_DISCARD uint16 slotIndex() const noexcept
		{
			return baseAttributeIndex + subComponentIndex;
		}

		/// Return the sub-component with the specified index. Returns nullptr if this attribute is not one of the
		/// components of a multi-component attribute for which the specified index is defined. This also means that
		/// for single-component attributes only '0' is accepted (for which the attribute itself is returned).
		/// @param pComponentIndex The index of an attribute's component to retrieve.
		IC3_NXMAIN_API_NO_DISCARD SGenericVertexAttributeInfo * getComponent( uint32 pComponentIndex ) const noexcept;
	};

	/// @brief
	struct SVertexStreamInfo
	{
		Bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask {};
		uint16 activeAttributesNum = 0;
		uint16 elementStrideInBytes = 0;

		IC3_ATTR_NO_DISCARD bool active() const noexcept
		{
			return !activeAttributesMask.empty() && ( activeAttributesNum > 0 );
		}

		void reset()
		{
			activeAttributesMask.clear();
			activeAttributesNum = 0;
			elementStrideInBytes = 0;
		}
	};

	/// @brief
	struct SVertexAttributeArrayLayoutData
	{
		using AttributeArray = std::array<SGenericVertexAttributeInfo, GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM>;
		using SemanticIDMap = std::unordered_map<EShaderInputSemanticID, uint32>;
		using SemanticNameMap = std::unordered_map<std::string_view, uint32>;

		AttributeArray attributesArray;
		uint32 activeBaseAttributesNum = 0;
		uint32 activeGenericAttributesNum = 0;
		SVertexAttributeIndexRange activeAttributesRange = SVertexAttributeIndexRange::maxRange();
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

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return activeGenericAttributesNum == 0;
		}
	};

	/// @brief
	struct SVertexStreamArrayLayoutData
	{
		using StreamArray = std::array<SVertexStreamInfo, GCM::IA_MAX_VERTEX_STREAMS_NUM>;

		StreamArray streamArray;
		uint32 activeStreamsNum = 0;
		SVertexStreamIndexRange activeStreamsRange = SVertexStreamIndexRange::maxRange();
		Bitmask<GCI::EIAVertexStreamBindingFlags> activeStreamsMask;

		SVertexStreamInfo & operator[]( size_t pStreamIndex ) noexcept
		{
			ic3DebugAssert( cxGCIValidVertexStreamIndexRange.contains( pStreamIndex ) );
			return streamArray[pStreamIndex];
		}

		IC3_ATTR_NO_DISCARD const SVertexStreamInfo & operator[]( size_t pStreamIndex ) const noexcept
		{
			ic3DebugAssert( cxGCIValidVertexStreamIndexRange.contains( pStreamIndex ) );
			return streamArray[pStreamIndex];
		}

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return activeStreamsNum == 0;
		}
	};

	namespace GCIUtils
	{

		IC3_NXMAIN_API_NO_DISCARD const char * getAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat );

		IC3_NXMAIN_API_NO_DISCARD const char * getShaderSemanticShortName( const std::string_view & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD std::string generateVertexAttributeFormatString( const SGenericVertexAttributeInfo & pAttributeInfo );

		IC3_NXMAIN_API_NO_DISCARD std::string generateVertexAttributeLayoutString( const SVertexAttributeArrayLayoutData & pLayoutData );

		template <typename TAttribute>
		IC3_ATTR_NO_DISCARD inline bool checkAttributeAutoDataFormat(
				GCI::EVertexAttribFormat pAttributeBaseFormat,
				uint32 pAttributeComponentsNum = 0 ) noexcept
		{
			using AttributeFormatTraits = GCI::VertexAttribFormatDataTypeTraits<TAttribute>;

			if( AttributeFormatTraits::sBaseAttribFormat != pAttributeBaseFormat )
			{
				return false;
			}

			if( ( pAttributeComponentsNum != 0 ) && ( AttributeFormatTraits::sAttribComponentsNum != pAttributeComponentsNum ) )
			{
				return false;
			}

			const auto componentByteSize = GCI::CxDef::getVertexAttribFormatByteSize( pAttributeBaseFormat );
			const auto attributeByteSize = componentByteSize * AttributeFormatTraits::sAttribComponentsNum;

			if( AttributeFormatTraits::sSizeInBytes != attributeByteSize )
			{
				return false;
			}

			return true;
		}

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_PIPELINE_DEFS_H__
