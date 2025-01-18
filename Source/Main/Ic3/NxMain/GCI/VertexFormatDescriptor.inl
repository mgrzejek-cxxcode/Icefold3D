
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

	inline bool VertexFormatDescriptor::isIndexedVertexFormat() const noexcept
	{
		return _indexDataFormat != GCI::EIndexDataFormat::Undefined;
	}

	inline uint32 VertexFormatDescriptor::getIndexDataSize() const noexcept
	{
		return GCI::CxDef::getIndexDataFormatByteSize( _indexDataFormat );
	}

	inline bool VertexFormatDescriptor::hasAttributeWithSemantics( StringView pSemanticName ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticName, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeIndexUndefined;
	}

	inline bool VertexFormatDescriptor::hasAttributeWithSemantics( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemantics );
		return resolvedAttributeIndex != cxGCIVertexAttributeIndexUndefined;
	}

	inline bool VertexFormatDescriptor::hasAttributeWithSemantics( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSysSmtFlags, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeIndexUndefined;
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( native_uint pAttribIASlot ) const noexcept
	{
		return _attribArrayLayout.attributePtr( pAttribIASlot );
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemantics );
		return _attribArrayLayout.attributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pAttributeKey, pSemanticIndex );
		return _attribArrayLayout.attributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( StringView pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticName, pSemanticIndex );
		return _attribArrayLayout.attributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSysSmtFlags, pSemanticIndex );
		return _attribArrayLayout.attributePtr( resolvedAttributeIndex );
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( native_uint pAttribIASlot ) const noexcept
	{
		const auto * attributePtr = getAttribute( pAttribIASlot );
		return attributePtr ? getStream( attributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto * attributePtr = getAttribute( pAttributeKey );
		return attributePtr ? getStream( attributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto * attributePtr = getAttribute( pSemantics );
		return attributePtr ? getStream( attributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( StringView pSemanticName ) const noexcept
	{
		const auto * attributePtr = getAttribute( pSemanticName );
		return attributePtr ? getStream( attributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept
	{
		const auto * attributePtr = getAttribute( pSysSmtFlags );
		return attributePtr ? getStream( attributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStream( native_uint pStreamIASlot ) const noexcept
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

	inline bool VertexFormatDescriptor::isAttributeActive( native_uint pAttribIASlot ) const
	{
		return _attribArrayLayout.isAttributeActive( pAttribIASlot );
	}

	inline bool VertexFormatDescriptor::isVertexStreamActive( native_uint pStreamIASlot ) const
	{
		return _streamArrayConfig.isStreamActive( pStreamIASlot );
	}

} // namespace Ic3
