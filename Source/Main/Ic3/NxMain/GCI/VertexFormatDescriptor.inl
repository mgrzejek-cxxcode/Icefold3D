
#if !defined( __IC3_NXMAIN_VERTEX_FORMAT_DESCRIPTOR_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	inline VertexFormatDescriptor::operator bool() const noexcept
	{
		return valid();
	}

	inline bool VertexFormatDescriptor::empty() const noexcept
	{
		return _attribArrayLayout.empty();
	}

	inline bool VertexFormatDescriptor::valid() const noexcept
	{
		return !empty() &&
		       ( _indexDataFormat != GCI::EIndexDataFormat::Undefined ) &&
		       ( _primitiveTopology != GCI::EPrimitiveTopology::Undefined );
	}

	inline bool VertexFormatDescriptor::isIndexedGeometry() const noexcept
	{
		return _indexDataFormat != GCI::EIndexDataFormat::Undefined;
	}

	inline uint32 VertexFormatDescriptor::getIndexDataSize() const noexcept
	{
		return GCI::CxDef::getIndexDataFormatByteSize( _indexDataFormat );
	}

	inline bool VertexFormatDescriptor::hasAttributeWithSemantics( EShaderInputSemanticID pSemanticID ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticID, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeSlotUndefined;
	}

	inline bool VertexFormatDescriptor::hasAttributeWithSemantics( std::string_view pSemanticName ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticName, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeSlotUndefined;
	}

	inline bool VertexFormatDescriptor::hasAttributeWithSemantics( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemantics, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeSlotUndefined;
	}

	inline const VertexAttributeComponent * VertexFormatDescriptor::getAttribute( gci_input_assembler_slot_t pAttribIASlot ) const noexcept
	{
		return _attribArrayLayout.attributePtr( pAttribIASlot );
	}

	inline const VertexAttributeComponent * VertexFormatDescriptor::getAttribute( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pAttributeKey, pSemanticIndex );
		return _attribArrayLayout.attributePtr( resolvedAttributeIndex );
	}

	inline const VertexAttributeComponent * VertexFormatDescriptor::getAttribute( EShaderInputSemanticID pSemanticID, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticID, pSemanticIndex );
		return _attribArrayLayout.attributePtr( resolvedAttributeIndex );
	}

	inline const VertexAttributeComponent * VertexFormatDescriptor::getAttribute( std::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticName, pSemanticIndex );
		return _attribArrayLayout.attributePtr( resolvedAttributeIndex );
	}

	inline const VertexAttributeComponent * VertexFormatDescriptor::getAttribute( const ShaderSemantics & pSemantics, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemantics, pSemanticIndex );
		return _attribArrayLayout.attributePtr( resolvedAttributeIndex );
	}

	inline const VertexStreamComponent * VertexFormatDescriptor::getStreamForAttribute( gci_input_assembler_slot_t pAttribIASlot ) const noexcept
	{
		const auto * attributePtr = getAttribute( pAttribIASlot );
		return attributePtr ? getStream( attributePtr->vertexStreamIASlot ) : nullptr;
	}

	inline const VertexStreamComponent * VertexFormatDescriptor::getStreamForAttribute( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto * attributePtr = getAttribute( pAttributeKey );
		return attributePtr ? getStream( attributePtr->vertexStreamIASlot ) : nullptr;
	}

	inline const VertexStreamComponent * VertexFormatDescriptor::getStreamForAttribute( EShaderInputSemanticID pSemanticID, uint32 pSemanticIndex ) const noexcept
	{
		const auto * attributePtr = getAttribute( pSemanticID, pSemanticIndex );
		return attributePtr ? getStream( attributePtr->vertexStreamIASlot ) : nullptr;
	}

	inline const VertexStreamComponent * VertexFormatDescriptor::getStreamForAttribute( std::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		const auto * attributePtr = getAttribute( pSemanticName, pSemanticIndex );
		return attributePtr ? getStream( attributePtr->vertexStreamIASlot ) : nullptr;
	}

	inline const VertexStreamComponent * VertexFormatDescriptor::getStreamForAttribute( const ShaderSemantics & pSemantics, uint32 pSemanticIndex ) const noexcept
	{
		const auto * attributePtr = getAttribute( pSemantics, pSemanticIndex );
		return attributePtr ? getStream( attributePtr->vertexStreamIASlot ) : nullptr;
	}

	inline const VertexStreamComponent * VertexFormatDescriptor::getStream( gci_input_assembler_slot_t pStreamIASlot ) const noexcept
	{
		return _streamArrayConfig.streamPtr( pStreamIASlot );
	}

	inline InputAssemblerSlotArray VertexFormatDescriptor::getActiveAttributesSlots() const noexcept
	{
		return _attribArrayLayout.getActiveAttributesSlots();
	}

	inline InputAssemblerSlotArray VertexFormatDescriptor::getActiveStreamsSlots() const noexcept
	{
		return _streamArrayConfig.getActiveStreamsSlots();
	}

	inline bool VertexFormatDescriptor::isAttributeActive( gci_input_assembler_slot_t pAttribIASlot ) const
	{
		return _attribArrayLayout.isAttributeActive( pAttribIASlot );
	}

	inline bool VertexFormatDescriptor::isVertexStreamActive( gci_input_assembler_slot_t pStreamIASlot ) const
	{
		return _streamArrayConfig.isStreamActive( pStreamIASlot );
	}

} // namespace Ic3
