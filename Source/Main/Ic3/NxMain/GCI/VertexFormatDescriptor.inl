
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
		return _attribArrayLayout.IsEmpty();
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
		return GCI::CxDef::GetIndexDataFormatByteSize( _indexDataFormat );
	}

	inline bool VertexFormatDescriptor::hasAttributeWithSemantics( cppx::string_view pSemanticName ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticName, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeIndexUndefined;
	}

	inline bool VertexFormatDescriptor::hasAttributeWithSemantics( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemantics );
		return resolvedAttributeIndex != cxGCIVertexAttributeIndexUndefined;
	}

	inline bool VertexFormatDescriptor::hasAttributeWithSemantics( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSysSmtFlags, 0 );
		return resolvedAttributeIndex != cxGCIVertexAttributeIndexUndefined;
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( native_uint pAttribIASlot ) const noexcept
	{
		return _attribArrayLayout.AttributePtr( pAttribIASlot );
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemantics );
		return _attribArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pAttributeKey, pSemanticIndex );
		return _attribArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( cppx::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSemanticName, pSemanticIndex );
		return _attribArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexAttribute * VertexFormatDescriptor::getAttribute( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = resolveAttributeRef( pSysSmtFlags, pSemanticIndex );
		return _attribArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( native_uint pAttribIASlot ) const noexcept
	{
		const auto * AttributePtr = getAttribute( pAttribIASlot );
		return AttributePtr ? getStream( AttributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto * AttributePtr = getAttribute( pAttributeKey );
		return AttributePtr ? getStream( AttributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto * AttributePtr = getAttribute( pSemantics );
		return AttributePtr ? getStream( AttributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( cppx::string_view pSemanticName ) const noexcept
	{
		const auto * AttributePtr = getAttribute( pSemanticName );
		return AttributePtr ? getStream( AttributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStreamForAttribute( cppx::bitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept
	{
		const auto * AttributePtr = getAttribute( pSysSmtFlags );
		return AttributePtr ? getStream( AttributePtr->streamIASlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::getStream( native_uint pStreamIASlot ) const noexcept
	{
		return _streamArrayConfig.StreamPtr( pStreamIASlot );
	}

	inline InputAssemblerSlotArray VertexFormatDescriptor::GetActiveAttributesSlots() const noexcept
	{
		return _attribArrayLayout.GetActiveAttributesSlots();
	}

	inline InputAssemblerSlotArray VertexFormatDescriptor::GetActiveStreamsSlots() const noexcept
	{
		return _streamArrayConfig.GetActiveStreamsSlots();
	}

	inline bool VertexFormatDescriptor::IsAttributeActive( native_uint pAttribIASlot ) const
	{
		return _attribArrayLayout.IsAttributeActive( pAttribIASlot );
	}

	inline bool VertexFormatDescriptor::isVertexStreamActive( native_uint pStreamIASlot ) const
	{
		return _streamArrayConfig.IsStreamActive( pStreamIASlot );
	}

} // namespace Ic3
