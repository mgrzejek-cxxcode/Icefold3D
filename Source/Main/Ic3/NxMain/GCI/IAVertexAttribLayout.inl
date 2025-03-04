
#if !defined( __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	inline VertexInputAttributeArrayConfig::operator bool() const noexcept
	{
		return !IsEmpty();
	}
	
	inline const GenericVertexInputAttribute & VertexInputAttributeArrayConfig::operator[]( native_uint pAttributeSlot ) const noexcept
	{
		Ic3DebugAssert( GCI::CXU::IAIsVertexAttributeSlotValid( pAttributeSlot ) );
		return _attributeArray[pAttributeSlot];
	}

	inline const GenericVertexInputAttribute & VertexInputAttributeArrayConfig::AttributeAt( native_uint pAttributeSlot ) const
	{
		Ic3DebugAssert( GCI::CXU::IAIsVertexAttributeSlotValid( pAttributeSlot ) );
		return _attributeArray.at( pAttributeSlot );
	}

	inline const GenericVertexInputAttribute * VertexInputAttributeArrayConfig::AttributePtr( native_uint pAttributeSlot ) const noexcept
	{
		return GCI::CXU::IAIsVertexAttributeSlotValid( pAttributeSlot ) ? &( _attributeArray[pAttributeSlot] ) : nullptr;
	}

	inline bool VertexInputAttributeArrayConfig::IsAttributeActive( native_uint pAttributeSlot ) const noexcept
	{
		Ic3DebugAssert( GCI::CXU::IAIsVertexAttributeSlotValid( pAttributeSlot ) );
		return _attributeArray[pAttributeSlot].IsActive();
	}

	inline gci_input_assembler_slot_t VertexInputAttributeArrayConfig::QueryBaseAttributeBySemantics(
			cppx::string_view pSemanticName ) const noexcept
	{
		const auto attributeSlot = cppx::get_map_value_ref_or_default(
				_semanticNameMap,
				pSemanticName,
				GCI::kIAVertexAttributeSlotUndefined );
		return cppx::numeric_cast<gci_input_assembler_slot_t>( attributeSlot );
	}

	inline gci_input_assembler_slot_t VertexInputAttributeArrayConfig::QueryBaseAttributeBySemantics(
			cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags ) const noexcept
	{
		const auto semanticName = GCIUtils::GetSemanticNameFromAttributeFlags( pAttributeSemanticFlags );
		return QueryBaseAttributeBySemantics( semanticName );
	}

	inline const GenericVertexAttributeArray & VertexInputAttributeArrayConfig::GetAttributeArray() const noexcept
	{
		return _attributeArray;
	}

	inline uint32 VertexInputAttributeArrayConfig::GetActiveBaseAttributesNum() const noexcept
	{
		return _activeBaseAttributesNum;
	}

	inline uint32 VertexInputAttributeArrayConfig::GetActiveAttributeSlotsNum() const noexcept
	{
		return _activeAttributeSlotsNum;
	}

	inline cppx::bitmask<GCI::EIAVertexAttributeFlags> VertexInputAttributeArrayConfig::GetActiveAttributesMask() const noexcept
	{
		return _activeAttributesMask;
	}

	inline const InputAssemblerSlotRange & VertexInputAttributeArrayConfig::GetActiveAttributesRange() const noexcept
	{
		return _activeAttributesRange;
	}

	inline const InputAssemblerSlotArray & VertexInputAttributeArrayConfig::GetActiveAttributesSlots() const noexcept
	{
		return _activeAttributesSlots;
	}

	inline bool VertexInputAttributeArrayConfig::IsActiveAttributesRangeContinuous() const noexcept
	{
		return !IsEmpty() && ( _activeAttributesRange.length() == _activeAttributeSlotsNum );
	}

	inline bool VertexInputAttributeArrayConfig::IsEmpty() const noexcept
	{
		return _activeAttributeSlotsNum == 0;
	}

} // namespace Ic3
