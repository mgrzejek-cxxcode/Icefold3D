
#if !defined( __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	inline VertexAttributeArrayLayout::operator bool() const noexcept
	{
		return !empty();
	}
	
	inline const GenericVertexAttribute & VertexAttributeArrayLayout::operator[]( native_uint pAttribIASlot ) const noexcept
	{
		ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pAttribIASlot ) );
		return _attributeArray[pAttribIASlot];
	}

	inline const GenericVertexAttribute & VertexAttributeArrayLayout::attributeAt( native_uint pAttribIASlot ) const
	{
		ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pAttribIASlot ) );
		return _attributeArray.at( pAttribIASlot );
	}

	inline const GenericVertexAttribute * VertexAttributeArrayLayout::attributePtr( native_uint pAttribIASlot ) const noexcept
	{
		return cxGCIValidInputAssemblerSlotIndexRange.contains( pAttribIASlot ) ? &( _attributeArray[pAttribIASlot] ) : nullptr;
	}

	inline bool VertexAttributeArrayLayout::isAttributeActive( native_uint pAttribIASlot ) const noexcept
	{
		ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pAttribIASlot ) );
		return _attributeArray[pAttribIASlot].isActive();
	}

	inline gci_input_assembler_slot_t VertexAttributeArrayLayout::queryBaseAttributeBySemantics( StringView pSemanticName ) const noexcept
	{
		const auto attributeIndex = Cppx::getMapValueRefOrDefault( _semanticNameMap, pSemanticName, cxGCIVertexAttributeIndexUndefined );
		return numeric_cast<gci_input_assembler_slot_t>( attributeIndex );
	}

	inline gci_input_assembler_slot_t VertexAttributeArrayLayout::queryBaseAttributeBySemantics( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept
	{
		const auto semanticName = GCU::getStandardSemanticNameFromSystemFlags( pSysSmtFlags );
		return queryBaseAttributeBySemantics( semanticName );
	}

	inline const GenericVertexAttributeArray & VertexAttributeArrayLayout::getAttributeArray() const noexcept
	{
		return _attributeArray;
	}

	inline uint32 VertexAttributeArrayLayout::getActiveBaseAttributesNum() const noexcept
	{
		return _activeBaseAttributesNum;
	}

	inline uint32 VertexAttributeArrayLayout::getActiveAttributeSlotsNum() const noexcept
	{
		return _activeAttributeSlotsNum;
	}

	inline TBitmask<GCI::EIAVertexAttributeFlags> VertexAttributeArrayLayout::getActiveAttributesMask() const noexcept
	{
		return _activeAttributesMask;
	}

	inline const InputAssemblerSlotRange & VertexAttributeArrayLayout::getActiveAttributesRange() const noexcept
	{
		return _activeAttributesRange;
	}

	inline const InputAssemblerSlotArray & VertexAttributeArrayLayout::getActiveAttributesSlots() const noexcept
	{
		return _activeAttributesSlots;
	}

	inline bool VertexAttributeArrayLayout::isActiveAttributesRangeContinuous() const noexcept
	{
		return !empty() && ( _activeAttributesRange.length() == _activeAttributeSlotsNum );
	}

	inline bool VertexAttributeArrayLayout::empty() const noexcept
	{
		return _activeAttributeSlotsNum == 0;
	}

} // namespace Ic3
