
#if !defined( __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	inline VertexStreamComponent::VertexStreamComponent( gci_input_assembler_slot_t pStreamIASlot )
	{
		streamIASlot = pStreamIASlot;
	}

	inline VertexStreamComponent::operator bool() const noexcept
	{
		return active();
	}

	inline bool VertexStreamComponent::operator==( const VertexStreamComponent & pRhs ) const noexcept
	{
		return streamIASlot == pRhs.streamIASlot;
	}

	inline bool VertexStreamComponent::operator<( const VertexStreamComponent & pRhs ) const noexcept
	{
		return streamIASlot < pRhs.streamIASlot;
	}

	inline bool VertexStreamComponent::active() const noexcept
	{
		return !activeAttributesMask.empty() && ( activeAttributesNum > 0 ) && ( streamDataRate != EVertexDataRate::Undefined );
	}

	inline bool VertexStreamComponent::checkAttributeCompatibility( const VertexAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		return ( streamDataRate == EVertexDataRate::Undefined ) || ( streamDataRate == pAttributeDefinition.dataRate );
	}

	inline void VertexStreamComponent::init( uint16 pStreamIASlot, EVertexDataRate pDataRate )
	{
		ic3DebugAssert( ( streamIASlot == cxGCIVertexStreamSlotUndefined ) || ( streamIASlot == pStreamIASlot ) );
		ic3DebugAssert( streamDataRate == EVertexDataRate::Undefined );
		streamIASlot = pStreamIASlot;
		streamDataRate = pDataRate;
	}

	inline void VertexStreamComponent::reset()
	{
		activeAttributesMask.clear();
		streamIASlot = cxGCIVertexStreamSlotUndefined;
		activeAttributesNum = 0;
		elementStrideInBytes = 0;
		streamDataRate = EVertexDataRate::Undefined;
	}


	inline VertexStreamArrayConfig::operator bool() const noexcept
	{
		return !empty();
	}

	inline const VertexStreamComponent & VertexStreamArrayConfig::operator[]( gci_input_assembler_slot_t pStreamIASlot ) const noexcept
	{
		const auto streamArrayIndex = findStreamAtSlot( pStreamIASlot );
		return _activeStreams[streamArrayIndex];
	}

	inline const VertexStreamComponent & VertexStreamArrayConfig::streamAt( gci_input_assembler_slot_t pStreamIASlot ) const
	{
		const auto streamArrayIndex = findStreamAtSlot( pStreamIASlot );
		return _activeStreams.at( streamArrayIndex );
	}

	inline const VertexStreamComponent * VertexStreamArrayConfig::streamPtr( gci_input_assembler_slot_t pStreamIASlot ) const noexcept
	{
		const auto streamArrayIndex = findStreamAtSlot( pStreamIASlot );
		return ( streamArrayIndex != cxInvalidPosition ) ? &( _activeStreams[streamArrayIndex] ) : nullptr;
	}

	inline bool VertexStreamArrayConfig::isStreamActive( gci_input_assembler_slot_t pStreamIASlot ) const noexcept
	{
		const auto streamArrayIndex = findStreamAtSlot( pStreamIASlot );
		return streamArrayIndex != cxInvalidPosition;
	}

	inline const VertexStreamArray & VertexStreamArrayConfig::getActiveStreams() const noexcept
	{
		return _activeStreams;
	}

	inline TBitmask<GCI::EIAVertexStreamBindingFlags> VertexStreamArrayConfig::getActiveAttributesMask() const noexcept
	{
		return _activeStreamsMask;
	}

	inline uint32 VertexStreamArrayConfig::getActiveStreamsNum() const noexcept
	{
		return _activeStreamsNum;
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
		return _activeStreams.empty();
	}

} // namespace Ic3
