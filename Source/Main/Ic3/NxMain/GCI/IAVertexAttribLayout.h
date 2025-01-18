
#pragma once

#ifndef __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__
#define __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__

#include "IACommonDefs.h"
#include <Ic3/Cppx/STLHelperAlgo.h>

namespace Ic3
{

	/**
	 * @brief
	 */
	class VertexAttributeArrayLayout
	{
		friend class VertexFormatDescriptor;

	public:
		VertexAttributeArrayLayout() = default;
		~VertexAttributeArrayLayout() = default;

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		IC3_ATTR_NO_DISCARD const GenericVertexAttribute & operator[]( native_uint pAttribIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD const GenericVertexAttribute & attributeAt( native_uint pAttribIASlot ) const;

		IC3_ATTR_NO_DISCARD const GenericVertexAttribute * attributePtr( native_uint pAttribIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD bool isAttributeActive( native_uint pAttribIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD gci_input_assembler_slot_t queryBaseAttributeBySemantics( StringView pSemanticName ) const noexcept;

		IC3_ATTR_NO_DISCARD gci_input_assembler_slot_t queryBaseAttributeBySemantics( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept;

		IC3_ATTR_NO_DISCARD const GenericVertexAttributeArray & getAttributeArray() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 getActiveBaseAttributesNum() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 getActiveAttributeSlotsNum() const noexcept;

		IC3_ATTR_NO_DISCARD TBitmask<GCI::EIAVertexAttributeFlags> getActiveAttributesMask() const noexcept;

		IC3_ATTR_NO_DISCARD const InputAssemblerSlotRange & getActiveAttributesRange() const noexcept;

		IC3_ATTR_NO_DISCARD const InputAssemblerSlotArray & getActiveAttributesSlots() const noexcept;

		IC3_ATTR_NO_DISCARD bool isActiveAttributesRangeContinuous() const noexcept;

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		IC3_NXMAIN_API_NO_DISCARD bool checkAttributeArraySpace( native_uint pBaseIASlot, size_t pSemanticGroupSize = 1 ) const noexcept;

		IC3_NXMAIN_API_NO_DISCARD bool checkAttributeDefinitionCompatibility( const VertexAttributeDefinition & pAttributeDefinition ) const noexcept;

		IC3_NXMAIN_API GenericVertexAttribute * addActiveAttribute( VertexAttributeDefinition pAttributeDefinition );

		IC3_NXMAIN_API void reset();

	private:
		using SemanticNameMap = std::unordered_map<std::string_view, gci_input_assembler_slot_t>;

		GenericVertexAttributeArray _attributeArray;

		uint16 _activeBaseAttributesNum = 0;

		uint16 _activeAttributeSlotsNum = 0;

		TBitmask<GCI::EIAVertexAttributeFlags> _activeAttributesMask;

		TBitmask<ESystemAttributeSemanticFlags> _activeAttributeSemanticsMask;

		InputAssemblerSlotRange _activeAttributesRange = InputAssemblerSlotRange::emptyRange();

		InputAssemblerSlotArray _activeAttributesSlots;

		SemanticNameMap _semanticNameMap;
	};

	namespace GCU
	{

		/*
		IC3_NXMAIN_API_NO_DISCARD GCI::EVertexAttribFormat getCombinedVertexAttribFormat(
				GCI::EVertexAttribFormat pFormat1,
				GCI::EVertexAttribFormat pFormat2 );

		IC3_NXMAIN_API_NO_DISCARD GCI::EVertexAttribFormat getCombinedVertexAttribFormat(
				const GCI::EVertexAttribFormat * pFormatArray,
				size_t pFormatsNum );

		template <size_t tpFormatsNum>
		IC3_ATTR_NO_DISCARD inline GCI::EVertexAttribFormat getCombinedVertexAttribFormat( GCI::EVertexAttribFormat ( &pFormats )[tpFormatsNum] )
		{
			return getCombinedVertexAttribFormat( &( pFormats[0] ), tpFormatsNum );
		}
		 */

		IC3_NXMAIN_API_NO_DISCARD GCI::EVertexAttribFormat getAttributeFormatFromStringIdentifier( const std::string & pAttribFormatStringID );

		IC3_NXMAIN_API_NO_DISCARD StringView getAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat );

		IC3_NXMAIN_API_NO_DISCARD StringView getShaderSemanticShortName( const StringView & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD StringView resolveShaderSemanticShortName( const StringView & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD std::string generateVertexAttributeFormatString( const GenericVertexAttribute & pAttribute );

		template <typename TPAttributeTraits>
		IC3_ATTR_NO_DISCARD inline bool checkAttributeAutoDataFormat(
				GCI::EVertexAttribFormat pAttributeBaseFormat,
				uint32 pAttributeComponentsNum = 0 ) noexcept
		{
			if( pAttributeBaseFormat != TPAttributeTraits::sBaseAttribFormat )
			{
				return false;
			}

			if( ( pAttributeComponentsNum != 0 ) && ( TPAttributeTraits::sAttribComponentsNum != pAttributeComponentsNum ) )
			{
				return false;
			}

			const auto specifiedComponentByteSize = GCI::CxDef::getVertexAttribFormatByteSize( pAttributeBaseFormat );
			const auto specifiedAttributeByteSize = specifiedComponentByteSize * pAttributeComponentsNum;

			if( specifiedAttributeByteSize != TPAttributeTraits::sSizeInBytes )
			{
				return false;
			}

			return true;
		}

	}

} // namespace Ic3

#include "IAVertexAttribLayout.inl"

#endif // __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__
