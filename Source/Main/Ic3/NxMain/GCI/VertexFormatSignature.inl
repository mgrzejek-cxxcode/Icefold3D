
#if !defined( __IC3_NXMAIN_VERTEX_FORMAT_SIGNATURE_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{
	inline VertexFormatSignature::operator bool() const noexcept
	{
		return IsValid();
	}

	inline bool VertexFormatSignature::IsEmpty() const noexcept
	{
		return _inputAttributeArrayLayout.IsEmpty();
	}

	inline bool VertexFormatSignature::IsValid() const noexcept
	{
		return !IsEmpty() &&
				( _indexDataFormat != GCI::EIndexDataFormat::Undefined ) &&
				( _primitiveTopology != GCI::EPrimitiveTopology::Undefined );
	}

	inline bool VertexFormatSignature::IsIndexedVertexFormat() const noexcept
	{
		return _indexDataFormat != GCI::EIndexDataFormat::Undefined;
	}

	inline uint32 VertexFormatSignature::GetIndexDataSize() const noexcept
	{
		return GCI::CXU::GetIndexDataFormatByteSize( _indexDataFormat );
	}

	inline bool VertexFormatSignature::HasAttributeWithSemantics( cppx::string_view pSemanticName ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSemanticName, 0 );
		return resolvedAttributeIndex != GCI::kIAVertexAttributeSlotUndefined;
	}

	inline bool VertexFormatSignature::HasAttributeWithSemantics( const VertexAttributeShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSemantics );
		return resolvedAttributeIndex != GCI::kIAVertexAttributeSlotUndefined;
	}

	inline bool VertexFormatSignature::HasAttributeWithSemantics( cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pAttributeSemanticFlags, 0 );
		return resolvedAttributeIndex != GCI::kIAVertexAttributeSlotUndefined;
	}

	inline const GenericVertexInputAttribute * VertexFormatSignature::GetAttribute( native_uint pAttributeSlot ) const noexcept
	{
		return _inputAttributeArrayLayout.AttributePtr( pAttributeSlot );
	}

	inline const GenericVertexInputAttribute * VertexFormatSignature::GetAttribute( const VertexAttributeShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSemantics );
		return _inputAttributeArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexInputAttribute * VertexFormatSignature::GetAttribute( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pAttributeKey, pSemanticIndex );
		return _inputAttributeArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexInputAttribute * VertexFormatSignature::GetAttribute( cppx::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pSemanticName, pSemanticIndex );
		return _inputAttributeArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const GenericVertexInputAttribute * VertexFormatSignature::GetAttribute( cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags, uint32 pSemanticIndex ) const noexcept
	{
		const auto resolvedAttributeIndex = ResolveAttributeRef( pAttributeSemanticFlags, pSemanticIndex );
		return _inputAttributeArrayLayout.AttributePtr( resolvedAttributeIndex );
	}

	inline const VertexInputStream * VertexFormatSignature::GetStreamForAttribute( native_uint pAttributeSlot ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pAttributeSlot );
		return attributePtr ? GetStream( attributePtr->vertexStreamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatSignature::GetStreamForAttribute( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pAttributeKey );
		return attributePtr ? GetStream( attributePtr->vertexStreamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatSignature::GetStreamForAttribute( const VertexAttributeShaderSemantics & pSemantics ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pSemantics );
		return attributePtr ? GetStream( attributePtr->vertexStreamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatSignature::GetStreamForAttribute( cppx::string_view pSemanticName ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pSemanticName );
		return attributePtr ? GetStream( attributePtr->vertexStreamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatSignature::GetStreamForAttribute( cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pAttributeSemanticFlags );
		return attributePtr ? GetStream( attributePtr->vertexStreamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatSignature::GetStream( native_uint pStreamSlot ) const noexcept
	{
		return _inputStreamArrayConfig.StreamPtr( pStreamSlot );
	}

	inline InputAssemblerSlotArray VertexFormatSignature::GetActiveAttributesSlots() const noexcept
	{
		return _inputAttributeArrayLayout.GetActiveAttributesSlots();
	}

	inline InputAssemblerSlotArray VertexFormatSignature::GetActiveStreamsSlots() const noexcept
	{
		return _inputStreamArrayConfig.GetActiveStreamsSlots();
	}

	inline bool VertexFormatSignature::IsAttributeActive( native_uint pAttributeSlot ) const
	{
		return _inputAttributeArrayLayout.IsAttributeActive( pAttributeSlot );
	}

	inline bool VertexFormatSignature::IsVertexStreamActive( native_uint pStreamSlot ) const
	{
		return _inputStreamArrayConfig.IsStreamActive( pStreamSlot );
	}

} // namespace Ic3
