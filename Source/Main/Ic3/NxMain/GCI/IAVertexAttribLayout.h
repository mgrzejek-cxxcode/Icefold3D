
#pragma once

#ifndef __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__
#define __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__

#include "VertexAttributeShaderSemantics.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3
{

	/**
	 * @brief
	 */
	class IC3_NXMAIN_CLASS VertexInputAttributeArrayConfig
	{
		friend class VertexFormatSignature;

	public:
		VertexInputAttributeArrayConfig() = default;
		~VertexInputAttributeArrayConfig() = default;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute & operator[]( native_uint pAttributeSlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute & AttributeAt( native_uint pAttributeSlot ) const;

		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute * AttributePtr( native_uint pAttributeSlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsAttributeActive( native_uint pAttributeSlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD gci_input_assembler_slot_t QueryBaseAttributeBySemantics(
				cppx::string_view pSemanticName ) const noexcept;

		CPPX_ATTR_NO_DISCARD gci_input_assembler_slot_t QueryBaseAttributeBySemantics(
				cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags ) const noexcept;

		CPPX_ATTR_NO_DISCARD const GenericVertexAttributeArray & GetAttributeArray() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 GetActiveBaseAttributesNum() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 GetActiveAttributeSlotsNum() const noexcept;

		CPPX_ATTR_NO_DISCARD cppx::bitmask<GCI::EIAVertexAttributeFlags> GetActiveAttributesMask() const noexcept;

		CPPX_ATTR_NO_DISCARD const InputAssemblerSlotRange & GetActiveAttributesRange() const noexcept;

		CPPX_ATTR_NO_DISCARD const InputAssemblerSlotArray & GetActiveAttributesSlots() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsActiveAttributesRangeContinuous() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		CPPX_ATTR_NO_DISCARD bool CheckAttributeArraySpace(
				uint32 pBaseAttributeSlot,
				uint32 pSemanticGroupSize = 1 ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool CheckAttributeDefinitionCompatibility(
				const VertexInputAttributeDefinition & pAttributeDefinition ) const noexcept;

		GenericVertexInputAttribute * AddAttribute(
				const VertexInputAttributeDefinition & pAttributeDefinition );

		void Reset();

	private:
		GenericVertexInputAttribute * _AddAttributeImpl(
				const VertexInputAttributeDefinition & pAttributeDefinition );

	private:
		using SemanticNameMap = std::unordered_map<std::string_view, gci_input_assembler_slot_t>;

		GenericVertexAttributeArray _attributeArray;

		uint16 _activeBaseAttributesNum = 0;

		uint16 _activeAttributeSlotsNum = 0;

		cppx::bitmask<GCI::EIAVertexAttributeFlags> _activeAttributesMask;

		cppx::bitmask<EVertexAttributeSemanticFlags> _activeAttributeSemanticsMask;

		InputAssemblerSlotRange _activeAttributesRange = InputAssemblerSlotRange::empty_range();

		InputAssemblerSlotArray _activeAttributesSlots;

		SemanticNameMap _semanticNameMap;
	};

	namespace GCIUtils
	{
		using namespace GCI::CXU;

		IC3_NXMAIN_API_NO_DISCARD GCI::IAVertexAttributeDesc MakeGCIAttributeDesc(
				VertexAttributeKey pAttributeKey,
				uint32 pVertexStreamSlot,
				uint32 pVertexStreamRelativeOffset );

		IC3_NXMAIN_API_NO_DISCARD GCI::EVertexAttribFormat GetAttributeFormatFromStringIdentifier( const std::string & pAttribFormatStringID );

		IC3_NXMAIN_API_NO_DISCARD cppx::string_view GetAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat );

		IC3_NXMAIN_API_NO_DISCARD cppx::string_view GetShaderSemanticShortName( const cppx::string_view & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD cppx::string_view ResolveShaderSemanticShortName( const cppx::string_view & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD std::string GenerateVertexAttributeFormatString( const GenericVertexInputAttribute & pAttribute );

		template <typename TPAttributeTraits>
		CPPX_ATTR_NO_DISCARD inline bool CheckAttributeAutoDataFormat(
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

			const auto specifiedComponentByteSize = GCI::CXU::GetVertexAttribFormatByteSize( pAttributeBaseFormat );
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
