
#if !defined( __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	inline VertexInputStream::operator bool() const noexcept
	{
		return IsActive();
	}

	inline bool VertexInputStream::IsActive() const noexcept
	{
		return ( streamIASlot != cxGCIVertexStreamIndexUndefined ) && ( activeAttributesNum > 0 );
	}

	inline bool VertexInputStream::CheckAttributeCompatibility( const VertexAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		return ( streamDataRate == EVertexDataRate::Undefined ) || ( streamDataRate == pAttributeDefinition.dataRate );
	}

	inline void VertexInputStream::Init( uint16 pStreamIASlot, EVertexDataRate pDataRate )
	{
		Ic3DebugAssert( streamIASlot == cxGCIVertexStreamIndexUndefined );
		Ic3DebugAssert( streamDataRate == EVertexDataRate::Undefined );
		streamIASlot = pStreamIASlot;
		streamDataRate = pDataRate;
	}

	inline void VertexInputStream::Reset()
	{
		streamIASlot = cxGCIVertexStreamIndexUndefined;
		activeAttributesMask.clear();
		activeAttributesNum = 0;
		dataStrideInBytes = 0;
		streamDataRate = EVertexDataRate::Undefined;
	}


	inline VertexStreamArrayConfig::operator bool() const noexcept
	{
		return !IsEmpty();
	}

	inline const VertexInputStream & VertexStreamArrayConfig::operator[]( native_uint pStreamIASlot ) const noexcept
	{
		Ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) );
		return _streamArray[pStreamIASlot];
	}

	inline const VertexInputStream & VertexStreamArrayConfig::StreamAt( native_uint pStreamIASlot ) const
	{
		Ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) );
		return _streamArray.at( pStreamIASlot );
	}

	inline const VertexInputStream * VertexStreamArrayConfig::StreamPtr( native_uint pStreamIASlot ) const noexcept
	{
		return cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) ? &( _streamArray[pStreamIASlot] ) : nullptr;
	}

	inline bool VertexStreamArrayConfig::IsStreamActive( native_uint pStreamIASlot ) const noexcept
	{
		Ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) );
		return _streamArray[pStreamIASlot].IsActive();
	}

	inline const VertexInputStreamArray & VertexStreamArrayConfig::GetStreamArray() const noexcept
	{
		return _streamArray;
	}

	inline uint32 VertexStreamArrayConfig::GetActiveStreamsNum() const noexcept
	{
		return _activeStreamsNum;
	}

	inline cppx::bitmask<GCI::EIAVertexStreamBindingFlags> VertexStreamArrayConfig::GetActiveAttributesMask() const noexcept
	{
		return _activeStreamsMask;
	}

	inline const InputAssemblerSlotRange & VertexStreamArrayConfig::GetActiveAttributesRange() const noexcept
	{
		return _activeStreamsRange;
	}

	inline const InputAssemblerSlotArray & VertexStreamArrayConfig::GetActiveStreamsSlots() const noexcept
	{
		return _activeStreamsSlots;
	}

	inline bool VertexStreamArrayConfig::IsActiveStreamsRangeContinuous() const noexcept
	{
		return !IsEmpty() && ( _activeStreamsRange.length() == _activeStreamsNum );
	}

	inline bool VertexStreamArrayConfig::IsEmpty() const noexcept
	{
		return _activeStreamsNum == 0;
	}

} // namespace Ic3
