
#if !defined( __IC3_NXMAIN_VERTEX_FORMAT_DESCRIPTOR_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{
	inline VertexFormatDescriptor::operator bool() const noexcept
	{
		return IsValid();
	}

	inline bool VertexFormatDescriptor::IsEmpty() const noexcept
	{
		return _inputAttributeArrayLayout.IsEmpty();
	}

	inline bool VertexFormatDescriptor::IsValid() const noexcept
	{
		return !IsEmpty() &&
				( _indexDataFormat != GCI::EIndexDataFormat::Undefined ) &&
				( _primitiveTopology != GCI::EPrimitiveTopology::Undefined );
	}

	inline bool VertexFormatDescriptor::IsIndexedVertexFormat() const noexcept
	{
		return _indexDataFormat != GCI::EIndexDataFormat::Undefined;
	}

	inline uint32 VertexFormatDescriptor::GetIndexDataSize() const noexcept
	{
		return GCI::CXU::GetIndexDataFormatByteSize( _indexDataFormat );
	}

	inline bool VertexFormatDescriptor::HasAttributeWithSemantics( cppx::string_view pSemanticName ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSemanticName, 0 );
		return resolvedAttributeIndex != kGCIVertexInputAttributeSlotUndefined;
	}

	inline bool VertexFormatDescriptor::HasAttributeWithSemantics( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSemantics );
		return resolvedAttributeIndex != kGCIVertexInputAttributeSlotUndefined;
	}

	inline bool VertexFormatDescriptor::HasAttributeWithSemantics( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSystemSemanticFlags, 0 );
		return resolvedAttributeIndex != kGCIVertexInputAttributeSlotUndefined;
	}

	inline const GenericVertexInputAttribute * VertexFormatDescriptor::GetAttribute( native_uint pAttributeSlot ) const noexcept
	{
		return _inputAttributeArrayLayout.AttributePtr( pAttributeSlot );
	}

	inline const GenericVertexInputAttribute * VertexFormatDescriptor::GetAttribute( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSemantics );
		return _inputAttributeArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexInputAttribute * VertexFormatDescriptor::GetAttribute( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pAttributeKey, pSemanticIndex );
		return _inputAttributeArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexInputAttribute * VertexFormatDescriptor::GetAttribute( cppx::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSemanticName, pSemanticIndex );
		return _inputAttributeArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexInputAttribute * VertexFormatDescriptor::GetAttribute( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSystemSemanticFlags, pSemanticIndex );
		return _inputAttributeArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const VertexInputStream * VertexFormatDescriptor::GetStreamForAttribute( native_uint pAttributeSlot ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pAttributeSlot );
		return attributePtr ? GetStream( attributePtr->streamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::GetStreamForAttribute( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pAttributeKey );
		return attributePtr ? GetStream( attributePtr->streamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::GetStreamForAttribute( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pSemantics );
		return attributePtr ? GetStream( attributePtr->streamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::GetStreamForAttribute( cppx::string_view pSemanticName ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pSemanticName );
		return attributePtr ? GetStream( attributePtr->streamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::GetStreamForAttribute( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pSystemSemanticFlags );
		return attributePtr ? GetStream( attributePtr->streamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatDescriptor::GetStream( native_uint pStreamSlot ) const noexcept
	{
		return _inputStreamArrayConfig.StreamPtr( pStreamSlot );
	}

	inline InputAssemblerSlotArray VertexFormatDescriptor::GetActiveAttributesSlots() const noexcept
	{
		return _inputAttributeArrayLayout.GetActiveAttributesSlots();
	}

	inline InputAssemblerSlotArray VertexFormatDescriptor::GetActiveStreamsSlots() const noexcept
	{
		return _inputStreamArrayConfig.GetActiveStreamsSlots();
	}

	inline bool VertexFormatDescriptor::IsAttributeActive( native_uint pAttributeSlot ) const
	{
		return _inputAttributeArrayLayout.IsAttributeActive( pAttributeSlot );
	}

	inline bool VertexFormatDescriptor::IsVertexStreamActive( native_uint pStreamSlot ) const
	{
		return _inputStreamArrayConfig.IsStreamActive( pStreamSlot );
	}

} // namespace Ic3
