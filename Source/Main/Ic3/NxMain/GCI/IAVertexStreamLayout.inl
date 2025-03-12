
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
		return IsInitialized() && ( activeAttributesNum > 0 );
	}

	inline bool VertexInputStream::IsInitialized() const noexcept
	{
		return ( streamSlot != GCI::kIAVertexStreamSlotUndefined ) && ( streamDataRate != GCI::EIAVertexAttributeDataRate::Undefined );
	}

	inline bool VertexInputStream::CheckAttributeCompatibility( const VertexInputAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		return ( streamDataRate == GCI::EIAVertexAttributeDataRate::Undefined ) || ( streamDataRate == pAttributeDefinition.dataRate );
	}

	inline void VertexInputStream::Init( uint16 pStreamSlot, GCI::EIAVertexAttributeDataRate pDataRate )
	{
		Ic3DebugAssert( streamSlot == GCI::kIAVertexStreamSlotUndefined );
		Ic3DebugAssert( streamDataRate == GCI::EIAVertexAttributeDataRate::Undefined );
		streamSlot = cppx::numeric_cast<uint8>( pStreamSlot );
		streamDataRate = pDataRate;
	}

	inline void VertexInputStream::Reset()
	{
		streamSlot = GCI::kIAVertexStreamSlotUndefined;
		activeAttributesMask.clear();
		activeAttributesNum = 0;
		dataStrideInBytes = 0;
		streamDataRate = GCI::EIAVertexAttributeDataRate::Undefined;
	}


	inline VertexInputStreamArrayConfig::operator bool() const noexcept
	{
		return !IsEmpty();
	}

	inline const VertexInputStream & VertexInputStreamArrayConfig::operator[]( native_uint pStreamSlot ) const noexcept
	{
		Ic3DebugAssert( GCI::CXU::IAIsDataStreamVertexBufferSlotValid( pStreamSlot ) );
		return _streamArray[pStreamSlot];
	}

	inline const VertexInputStream & VertexInputStreamArrayConfig::StreamAt( native_uint pStreamSlot ) const
	{
		Ic3DebugAssert( GCI::CXU::IAIsDataStreamVertexBufferSlotValid( pStreamSlot ) );
		return _streamArray.at( pStreamSlot );
	}

	inline const VertexInputStream * VertexInputStreamArrayConfig::StreamPtr( native_uint pStreamSlot ) const noexcept
	{
		return GCI::CXU::IAIsDataStreamVertexBufferSlotValid( pStreamSlot ) ? &( _streamArray[pStreamSlot] ) : nullptr;
	}

	inline bool VertexInputStreamArrayConfig::IsStreamActive( native_uint pStreamSlot ) const noexcept
	{
		Ic3DebugAssert( GCI::CXU::IAIsDataStreamVertexBufferSlotValid( pStreamSlot ) );
		return _streamArray[pStreamSlot].IsActive();
	}

	inline const VertexInputStreamArray & VertexInputStreamArrayConfig::GetStreamArray() const noexcept
	{
		return _streamArray;
	}

	inline uint32 VertexInputStreamArrayConfig::GetActiveStreamsNum() const noexcept
	{
		return _activeStreamsNum;
	}

	inline cppx::bitmask<GCI::EIAVertexSourceBindingFlags> VertexInputStreamArrayConfig::GetActiveStreamsMask() const noexcept
	{
		return _activeStreamsMask;
	}

	inline const InputAssemblerSlotRange & VertexInputStreamArrayConfig::GetActiveStreamsRange() const noexcept
	{
		return _activeStreamsRange;
	}

	inline const InputAssemblerSlotArray & VertexInputStreamArrayConfig::GetActiveStreamsSlots() const noexcept
	{
		return _activeStreamsSlots;
	}

	inline bool VertexInputStreamArrayConfig::IsActiveStreamsRangeContinuous() const noexcept
	{
		return !IsEmpty() && ( _activeStreamsRange.length() == _activeStreamsNum );
	}

	inline bool VertexInputStreamArrayConfig::IsEmpty() const noexcept
	{
		return _activeStreamsNum == 0;
	}

} // namespace Ic3
