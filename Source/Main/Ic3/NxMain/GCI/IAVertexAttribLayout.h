
#pragma once

#ifndef __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__
#define __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__

#include "IACommonDefs.h"
#include <Ic3/Cppx/StaticVector.h>
#include <Ic3/Cppx/STLHelperAlgo.h>

namespace Ic3
{

	/**
	 * @brief A definition of a generic vertex attribute. This is a per-slot definition - for multi-component
	 * attributes, it contains properties of a single component of such attribute.
	 */
	struct VertexAttributeComponent : public CommonVertexAttributeData
	{
		explicit VertexAttributeComponent( size_t pAttribIASlot = cxGCIVertexAttributeSlotUndefined );

		/**
		 * @brief Returns true if the attribute data describes an active attribute.
		 */
		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		IC3_ATTR_NO_DISCARD bool operator==( const VertexAttributeComponent & pRhs ) const noexcept;

		IC3_ATTR_NO_DISCARD bool operator<( const VertexAttributeComponent & pRhs ) const noexcept;

		/**
		 * @brief Return true if the instance represents an active attribute or false otherwise.
		 * Attribute is considered valid if:
		 * - format is not GCI::EVertexAttribFormat::Undefined
		 * - attributeIASlot is not cxGCIVertexAttributeSlotUndefined
		 * - vertexStreamIASlot is not cxGCIVertexStreamSlotUndefined
		 */
		IC3_ATTR_NO_DISCARD bool active() const noexcept;

		/**
		 * @brief Returns true if the attribute defined by this instance is a base attribute, or false otherwise.
		 * Base attribute is either a single-slot attribute (e.g. position or normal) or the first component of a
		 * multi-slot attribute (e.g. first vector of a 4x4 instance matrix). Formally speaking, an attribute is a
		 * base attribute if its semantic index is 0 (zero).
		 * @return True if the attribute defined by this instance is a base attribute, or false otherwise.
		 */
		IC3_ATTR_NO_DISCARD bool isBaseAttribute() const noexcept;

		/**
		 *
		 * @return
		 */
		IC3_ATTR_NO_DISCARD bool isSameAs( const VertexAttributeComponent & pOther ) const noexcept;

		/**
		 *
		 * @return
		 */
		IC3_ATTR_NO_DISCARD bool hasSameFormatAs( const VertexAttributeComponent & pOther ) const noexcept;

		/**
		 *
		 * @return
		 */
		IC3_ATTR_NO_DISCARD bool hasSameSemanticsAs( const VertexAttributeComponent & pOther ) const noexcept;

		/**
		 *
		 */
		void initBaseAttributeFromDefinition( VertexAttributeDefinition pDefinition );

		/**
		 *
		 */
		void initSubComponentFromBaseAttribute( const VertexAttributeComponent & pBaseAttribute, uint32 pSubIndex );

		/**
		 *
		 */
		void reset();
	};

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

		IC3_ATTR_NO_DISCARD const VertexAttributeComponent & operator[]( gci_input_assembler_slot_t pAttribIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD const VertexAttributeComponent & attributeAt( gci_input_assembler_slot_t pAttribIASlot ) const;

		IC3_ATTR_NO_DISCARD const VertexAttributeComponent * attributePtr( gci_input_assembler_slot_t pAttribIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD bool isAttributeActive( gci_input_assembler_slot_t pAttribIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD gci_input_assembler_slot_t resolveSemanticID( EShaderInputSemanticID pSemanticID ) const noexcept;

		IC3_ATTR_NO_DISCARD gci_input_assembler_slot_t resolveSemanticName( std::string_view pSemanticName ) const noexcept;

		IC3_ATTR_NO_DISCARD const VertexAttributeArray & getActiveAttributes() const noexcept;

		IC3_ATTR_NO_DISCARD TBitmask<GCI::EIAVertexAttributeFlags> getActiveAttributesMask() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 getActiveAttributesNum() const noexcept;

		IC3_ATTR_NO_DISCARD const InputAssemblerSlotRange & getActiveAttributesRange() const noexcept;

		IC3_ATTR_NO_DISCARD const InputAssemblerSlotArray & getActiveAttributesSlots() const noexcept;

		IC3_ATTR_NO_DISCARD bool isActiveAttributesRangeContinuous() const noexcept;

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		IC3_NXMAIN_API_NO_DISCARD size_t findAttributeAtSlot( gci_input_assembler_slot_t pAttribIASlot ) const noexcept;

		IC3_NXMAIN_API_NO_DISCARD bool checkAttributeArraySpace(
				gci_input_assembler_slot_t pAttribBaseIASlot,
				size_t pComponentsNum ) const noexcept;

		IC3_NXMAIN_API_NO_DISCARD bool checkAttributeDefinitionCompatibility(
				const VertexAttributeDefinition & pAttributeDefinition ) const noexcept;

		IC3_NXMAIN_API VertexAttributeComponent * addActiveAttribute( VertexAttributeDefinition pAttributeDefinition );

		IC3_NXMAIN_API void reserveAttributeArraySpace( size_t pActiveAttributesNum );

		IC3_NXMAIN_API void reset();

	private:
		using SemanticNameMap = std::unordered_map<std::string_view, gci_input_assembler_slot_t>;

		VertexAttributeArray _activeAttributes;

		TBitmask<GCI::EIAVertexAttributeFlags> _activeAttributesMask;

		uint16 _activeAttributesNum = 0;

		TBitmask<EShaderInputSemanticFlags> _activeAttributeSemanticsMask;

		InputAssemblerSlotRange _activeAttributesRange = InputAssemblerSlotRange::emptyRange();

		InputAssemblerSlotArray _activeAttributesSlots;

		SemanticNameMap _semanticNameMap;
	};

	namespace GCU
	{

		IC3_NXMAIN_API_NO_DISCARD GCI::EVertexAttribFormat getAttributeFormatFromStringIdentifier( const std::string & pAttribFormatStringID );

		IC3_NXMAIN_API_NO_DISCARD StringView getAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat );

		IC3_NXMAIN_API_NO_DISCARD StringView getShaderSemanticShortName( const std::string_view & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD StringView resolveShaderSemanticShortName( const std::string_view & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD std::string generateVertexAttributeFormatString( const VertexAttributeComponent & pAttribute );

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
