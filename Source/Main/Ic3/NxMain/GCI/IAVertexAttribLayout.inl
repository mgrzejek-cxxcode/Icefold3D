
#if !defined( __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	inline VertexAttributeArrayLayout::operator bool() const noexcept
	{
		return !IsEmpty();
	}
	
	inline const GenericVertexAttribute & VertexAttributeArrayLayout::operator[]( native_uint pAttribIASlot ) const noexcept
	{
		Ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pAttribIASlot ) );
		return _attributeArray[pAttribIASlot];
	}

	inline const GenericVertexAttribute & VertexAttributeArrayLayout::AttributeAt( native_uint pAttribIASlot ) const
	{
		Ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pAttribIASlot ) );
		return _attributeArray.at( pAttribIASlot );
	}

	inline const GenericVertexAttribute * VertexAttributeArrayLayout::AttributePtr( native_uint pAttribIASlot ) const noexcept
	{
		return cxGCIValidInputAssemblerSlotIndexRange.contains( pAttribIASlot ) ? &( _attributeArray[pAttribIASlot] ) : nullptr;
	}

	inline bool VertexAttributeArrayLayout::IsAttributeActive( native_uint pAttribIASlot ) const noexcept
	{
		Ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pAttribIASlot ) );
		return _attributeArray[pAttribIASlot].IsActive();
	}

	inline gci_input_assembler_slot_t VertexAttributeArrayLayout::QueryBaseAttributeBySemantics( cppx::string_view pSemanticName ) const noexcept
	{
		const auto attributeIndex = cppx::get_map_value_ref_or_default( _semanticNameMap, pSemanticName, cxGCIVertexAttributeIndexUndefined );
		return cppx::numeric_cast<gci_input_assembler_slot_t>( attributeIndex );
	}

	inline gci_input_assembler_slot_t VertexAttributeArrayLayout::QueryBaseAttributeBySemantics( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept
	{
		const auto semanticName = GCU::getStandardSemanticNameFromSystemFlags( pSysSmtFlags );
		return QueryBaseAttributeBySemantics( semanticName );
	}

	inline const GenericVertexAttributeArray & VertexAttributeArrayLayout::GetAttributeArray() const noexcept
	{
		return _attributeArray;
	}

	inline uint32 VertexAttributeArrayLayout::GetActiveBaseAttributesNum() const noexcept
	{
		return _activeBaseAttributesNum;
	}

	inline uint32 VertexAttributeArrayLayout::GetActiveAttributeSlotsNum() const noexcept
	{
		return _activeAttributeSlotsNum;
	}

	inline cppx::bitmask<GCI::EIAVertexAttributeFlags> VertexAttributeArrayLayout::GetActiveAttributesMask() const noexcept
	{
		return _activeAttributesMask;
	}

	inline const InputAssemblerSlotRange & VertexAttributeArrayLayout::GetActiveAttributesRange() const noexcept
	{
		return _activeAttributesRange;
	}

	inline const InputAssemblerSlotArray & VertexAttributeArrayLayout::GetActiveAttributesSlots() const noexcept
	{
		return _activeAttributesSlots;
	}

	inline bool VertexAttributeArrayLayout::IsActiveAttributesRangeContinuous() const noexcept
	{
		return !IsEmpty() && ( _activeAttributesRange.length() == _activeAttributeSlotsNum );
	}

	inline bool VertexAttributeArrayLayout::IsEmpty() const noexcept
	{
		return _activeAttributeSlotsNum == 0;
	}

} // namespace Ic3
