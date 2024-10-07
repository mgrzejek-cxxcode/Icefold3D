
#if !defined( __IC3_NXMAIN_IA_VERTEX_ATTRIB_LAYOUT_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	inline VertexAttributeComponent::VertexAttributeComponent( size_t pAttribIASlot )
	{
		attributeIASlot = pAttribIASlot;
	}

	inline VertexAttributeComponent::operator bool() const noexcept
	{
		return active();
	}

	inline bool VertexAttributeComponent::operator==( const VertexAttributeComponent & pRhs ) const noexcept
	{
		return attributeIASlot == pRhs.attributeIASlot;
	}

	inline bool VertexAttributeComponent::operator<( const VertexAttributeComponent & pRhs ) const noexcept
	{
		return attributeIASlot < pRhs.attributeIASlot;
	}

	inline bool VertexAttributeComponent::active() const noexcept
	{
		return ( baseFormat != GCI::EVertexAttribFormat::Undefined ) &&
		       ( attributeIASlot != cxGCIVertexAttributeSlotUndefined ) &&
		       ( vertexStreamIASlot != cxGCIVertexStreamSlotUndefined );
	}

	inline bool VertexAttributeComponent::isBaseAttribute() const noexcept
	{
		return shaderSemantics.smtIndex == 0;
	}

	inline bool VertexAttributeComponent::isSameAs( const VertexAttributeComponent & pOther ) const noexcept
	{
		return ( baseFormat == pOther.baseFormat ) &&
		       ( attributeIASlot == pOther.attributeIASlot ) &&
		       ( componentPadding == pOther.componentPadding ) &&
		       ( vertexStreamIASlot == pOther.vertexStreamIASlot ) &&
		       ( vertexStreamRelativeOffset == pOther.vertexStreamRelativeOffset ) &&
		       ( shaderSemantics == pOther.shaderSemantics );
	}

	inline bool VertexAttributeComponent::hasSameFormatAs( const VertexAttributeComponent & pOther ) const noexcept
	{
		return baseFormat == pOther.baseFormat;
	}

	inline bool VertexAttributeComponent::hasSameSemanticsAs( const VertexAttributeComponent & pOther ) const noexcept
	{
		return shaderSemantics.smtName == pOther.shaderSemantics.smtName;
	}

	inline void VertexAttributeComponent::initBaseAttributeFromDefinition( VertexAttributeDefinition pDefinition )
	{
		ic3DebugAssert(
				( attributeIASlot == cxGCIVertexAttributeSlotUndefined ) ||
				( attributeIASlot == pDefinition.attributeIASlot ) );

		baseFormat = pDefinition.baseFormat;
		attributeIASlot = pDefinition.attributeIASlot;
		componentPadding = pDefinition.componentPadding;
		vertexStreamIASlot = pDefinition.vertexStreamIASlot;
		vertexStreamRelativeOffset = pDefinition.vertexStreamRelativeOffset;
		shaderSemantics = ShaderSemantics( std::move( pDefinition.shaderSemantics ) );
	}

	inline void VertexAttributeComponent::initSubComponentFromBaseAttribute( const VertexAttributeComponent & pBaseAttribute, uint32 pSubIndex )
	{
		ic3DebugAssert(
				( attributeIASlot == cxGCIVertexAttributeSlotUndefined ) ||
				( attributeIASlot == pBaseAttribute.attributeIASlot + pSubIndex ) );

		baseFormat = pBaseAttribute.baseFormat;
		attributeIASlot = pBaseAttribute.attributeIASlot + pSubIndex;
		componentPadding = pBaseAttribute.componentPadding;
		vertexStreamIASlot = pBaseAttribute.vertexStreamIASlot;
		vertexStreamRelativeOffset = pBaseAttribute.vertexStreamRelativeOffset + ( pSubIndex * pBaseAttribute.dataStride() );
	}

	inline void VertexAttributeComponent::reset()
	{
		baseFormat = GCI::EVertexAttribFormat::Undefined;
		attributeIASlot = cxGCIVertexAttributeSlotUndefined;
		vertexStreamIASlot = cxGCIVertexStreamSlotUndefined;
		shaderSemantics.clear();
	}


	inline VertexAttributeArrayLayout::operator bool() const noexcept
	{
		return !empty();
	}
	
	inline const VertexAttributeComponent & VertexAttributeArrayLayout::operator[]( gci_input_assembler_slot_t pAttribIASlot ) const noexcept
	{
		const auto attribArrayIndex = findAttributeAtSlot( pAttribIASlot );
		return _activeAttributes[attribArrayIndex];
	}

	inline const VertexAttributeComponent & VertexAttributeArrayLayout::attributeAt( gci_input_assembler_slot_t pAttribIASlot ) const
	{
		const auto attribArrayIndex = findAttributeAtSlot( pAttribIASlot );
		return _activeAttributes.at( attribArrayIndex );
	}

	inline const VertexAttributeComponent * VertexAttributeArrayLayout::attributePtr( gci_input_assembler_slot_t pAttribIASlot ) const noexcept
	{
		const auto attribArrayIndex = findAttributeAtSlot( pAttribIASlot );
		return ( attribArrayIndex != cxInvalidPosition ) ? &( _activeAttributes[attribArrayIndex] ) : nullptr;
	}

	inline bool VertexAttributeArrayLayout::isAttributeActive( gci_input_assembler_slot_t pAttribIASlot ) const noexcept
	{
		const auto attribArrayIndex = findAttributeAtSlot( pAttribIASlot );
		return attribArrayIndex != cxInvalidPosition;
	}

	inline gci_input_assembler_slot_t VertexAttributeArrayLayout::resolveSemanticID( EShaderInputSemanticID pSemanticID ) const noexcept
	{
		const std::string_view semanticName = GCU::getShaderInputSemanticNameFromID( pSemanticID );
		return resolveSemanticName( semanticName );
	}

	inline gci_input_assembler_slot_t VertexAttributeArrayLayout::resolveSemanticName( std::string_view pSemanticName ) const noexcept
	{
		const auto attributeIndex = Cppx::getMapValueRefOrDefault( _semanticNameMap, pSemanticName, cxGCIVertexAttributeSlotUndefined );
		return numeric_cast<gci_input_assembler_slot_t>( attributeIndex );
	}

	inline const VertexAttributeArray & VertexAttributeArrayLayout::getActiveAttributes() const noexcept
	{
		return _activeAttributes;
	}

	inline TBitmask<GCI::EIAVertexAttributeFlags> VertexAttributeArrayLayout::getActiveAttributesMask() const noexcept
	{
		return _activeAttributesMask;
	}

	inline uint32 VertexAttributeArrayLayout::getActiveAttributesNum() const noexcept
	{
		return _activeAttributesNum;
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
		return !empty() && ( _activeAttributesRange.length() == _activeAttributesNum );
	}

	inline bool VertexAttributeArrayLayout::empty() const noexcept
	{
		return _activeAttributes.empty();
	}

} // namespace Ic3
