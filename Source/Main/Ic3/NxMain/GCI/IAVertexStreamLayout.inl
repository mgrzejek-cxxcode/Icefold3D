
#if !defined( __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	inline VertexInputStream::operator bool() const noexcept
	{
		return isActive();
	}

	inline bool VertexInputStream::isActive() const noexcept
	{
		return ( streamIASlot != cxGCIVertexStreamIndexUndefined ) && ( activeAttributesNum > 0 );
	}

	inline bool VertexInputStream::checkAttributeCompatibility( const VertexAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		return ( streamDataRate == EVertexDataRate::Undefined ) || ( streamDataRate == pAttributeDefinition.dataRate );
	}

	inline void VertexInputStream::init( uint16 pStreamIASlot, EVertexDataRate pDataRate )
	{
		ic3DebugAssert( streamIASlot == cxGCIVertexStreamIndexUndefined );
		ic3DebugAssert( streamDataRate == EVertexDataRate::Undefined );
		streamIASlot = pStreamIASlot;
		streamDataRate = pDataRate;
	}

	inline void VertexInputStream::reset()
	{
		streamIASlot = cxGCIVertexStreamIndexUndefined;
		activeAttributesMask.clear();
		activeAttributesNum = 0;
		dataStrideInBytes = 0;
		streamDataRate = EVertexDataRate::Undefined;
	}


	inline VertexStreamArrayConfig::operator bool() const noexcept
	{
		return !empty();
	}

	inline const VertexInputStream & VertexStreamArrayConfig::operator[]( native_uint pStreamIASlot ) const noexcept
	{
		ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) );
		return _streamArray[pStreamIASlot];
	}

	inline const VertexInputStream & VertexStreamArrayConfig::streamAt( native_uint pStreamIASlot ) const
	{
		ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) );
		return _streamArray.at( pStreamIASlot );
	}

	inline const VertexInputStream * VertexStreamArrayConfig::streamPtr( native_uint pStreamIASlot ) const noexcept
	{
		return cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) ? &( _streamArray[pStreamIASlot] ) : nullptr;
	}

	inline bool VertexStreamArrayConfig::isStreamActive( native_uint pStreamIASlot ) const noexcept
	{
		ic3DebugAssert( cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) );
		return _streamArray[pStreamIASlot].isActive();
	}

	inline const VertexInputStreamArray & VertexStreamArrayConfig::getStreamArray() const noexcept
	{
		return _streamArray;
	}

	inline uint32 VertexStreamArrayConfig::getActiveStreamsNum() const noexcept
	{
		return _activeStreamsNum;
	}

	inline TBitmask<GCI::EIAVertexStreamBindingFlags> VertexStreamArrayConfig::getActiveAttributesMask() const noexcept
	{
		return _activeStreamsMask;
	}

	inline const InputAssemblerSlotRange & VertexStreamArrayConfig::getActiveAttributesRange() const noexcept
	{
		return _activeStreamsRange;
	}

	inline const InputAssemblerSlotArray & VertexStreamArrayConfig::getActiveStreamsSlots() const noexcept
	{
		return _activeStreamsSlots;
	}

	inline bool VertexStreamArrayConfig::isActiveStreamsRangeContinuous() const noexcept
	{
		return !empty() && ( _activeStreamsRange.length() == _activeStreamsNum );
	}

	inline bool VertexStreamArrayConfig::empty() const noexcept
	{
		return _activeStreamsNum == 0;
	}

} // namespace Ic3
