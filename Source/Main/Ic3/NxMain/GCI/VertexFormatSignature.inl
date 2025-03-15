
#if !defined( __IC3_NXMAIN_VERTEX_FORMAT_SIGNATURE_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{
	inline VertexFormatSignatureBase::operator bool() const noexcept
	{
		return IsValid();
	}

	inline bool VertexFormatSignatureBase::IsEmpty() const noexcept
	{
		return mInternalLayout.IsEmpty();
	}

	inline bool VertexFormatSignatureBase::IsValid() const noexcept
	{
		return !mInternalLayout.IsEmpty() &&
		       ( _indexDataFormat != GCI::EIndexDataFormat::Undefined ) &&
		       ( _primitiveTopology != GCI::EPrimitiveTopology::Undefined );
	}

	inline bool VertexFormatSignatureBase::IsIndexedVertexFormat() const noexcept
	{
		return _indexDataFormat != GCI::EIndexDataFormat::Undefined;
	}

	inline uint32 VertexFormatSignatureBase::GetIndexDataSize() const noexcept
	{
		return GCI::CXU::GetIndexDataFormatByteSize( _indexDataFormat );
	}

	inline bool VertexFormatSignatureBase::HasAttributeWithSemantics( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept
	{
		return false;
	}

	inline bool VertexFormatSignatureBase::HasAttributeWithSemantics( cppx::string_view pSemanticName ) const noexcept
	{
		return false;
	}

	inline const GenericVertexInputAttribute * VertexFormatSignatureBase::GetAttribute( native_uint pAttributeSlot ) const noexcept
	{
		return mInternalLayout.GetAttributeAtSlot( pAttributeSlot );
	}

	inline const GenericVertexInputAttribute * VertexFormatSignatureBase::GetAttribute( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept
	{
		return nullptr;
	}

	inline const GenericVertexInputAttribute * VertexFormatSignatureBase::GetAttribute( cppx::string_view pSemanticName ) const noexcept
	{
		return nullptr;
	}

	inline const VertexInputStream * VertexFormatSignatureBase::GetStreamForAttribute( native_uint pAttributeSlot ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pAttributeSlot );
		return attributePtr ? GetStream( attributePtr->vertexStreamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatSignatureBase::GetStreamForAttribute( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept
	{
		return nullptr;
	}

	inline const VertexInputStream * VertexFormatSignatureBase::GetStreamForAttribute( cppx::string_view pSemanticName ) const noexcept
	{
		const auto * attributePtr = GetAttribute( pSemanticName );
		return attributePtr ? GetStream( attributePtr->vertexStreamSlot ) : nullptr;
	}

	inline const VertexInputStream * VertexFormatSignatureBase::GetStream( native_uint pStreamSlot ) const noexcept
	{
		return mInternalLayout.GetStreamAtSlot( pStreamSlot );
	}

	cppx::array_view<input_assembler_slot_t> VertexFormatSignatureBase::GetActiveAttributeSlots() const noexcept
	{
		return mInternalLayout.GetActiveAttributeSlotsView();
	}

	cppx::array_view<input_assembler_slot_t> VertexFormatSignatureBase::GetActiveStreamSlots() const noexcept
	{
		return mInternalLayout.GetActiveStreamSlotsView();
	}

	inline bool VertexFormatSignatureBase::IsAttributeActive( native_uint pAttributeSlot ) const noexcept
	{
		return mInternalLayout.IsAttributeActive( pAttributeSlot );
	}

	inline bool VertexFormatSignatureBase::IsStreamActive( native_uint pStreamSlot ) const noexcept
	{
		return mInternalLayout.IsStreamActive( pStreamSlot );
	}

} // namespace Ic3
