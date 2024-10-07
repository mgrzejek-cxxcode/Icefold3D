
#include "IAVertexStreamLayout.h"
#include "IAVertexAttribLayout.h"

namespace Ic3
{

	struct VertexStreamIndexCmpEqual
	{
		IC3_ATTR_NO_DISCARD bool operator()( const VertexStreamComponent & pLhs, size_t pRhs ) const noexcept
		{
			return pLhs.streamIASlot == pRhs;
		}
	};

	struct VertexStreamIndexCmpLess
	{
		IC3_ATTR_NO_DISCARD bool operator()( const VertexStreamComponent & pLhs, size_t pRhs ) const noexcept
		{
			return pLhs.streamIASlot < pRhs;
		}
	};

	size_t VertexStreamArrayConfig::findStreamAtSlot( gci_input_assembler_slot_t pStreamIASlot ) const noexcept
	{
		auto streamIter = _activeStreams.find( pStreamIASlot, VertexStreamIndexCmpLess{}, VertexStreamIndexCmpEqual{} );
		return ( streamIter != _activeStreams.end() ) ? ( streamIter - _activeStreams.begin() ) : cxInvalidPosition;
	}

	bool VertexStreamArrayConfig::checkAttributeDefinitionCompatibility(
			const VertexAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		if( const auto * attributeStreamPtr = streamPtr( pAttributeDefinition.vertexStreamIASlot ) )
		{
			return attributeStreamPtr->checkAttributeCompatibility( pAttributeDefinition );
		}

		return true;
	}

	bool VertexStreamArrayConfig::addActiveStream(
			gci_input_assembler_slot_t pStreamIASlot,
			EVertexDataRate pStreamDataRate )
	{
		if( pStreamDataRate == EVertexDataRate::Undefined )
		{
			return false;
		}

		if( isStreamActive( pStreamIASlot ) )
		{
			return true;
		}

		_addStreamImpl( pStreamIASlot, pStreamDataRate );

		return true;
	}

	bool VertexStreamArrayConfig::appendAttribute(
			gci_input_assembler_slot_t pStreamIASlot,
			const VertexAttributeComponent & pAttribute )
	{
		const auto streamArrayIndex = findStreamAtSlot( pStreamIASlot );

		if( streamArrayIndex == cxInvalidPosition )
		{
			return false;
		}

		_appendAttributeImpl( _activeStreams[streamArrayIndex], pAttribute );

		return true;
	}

	bool VertexStreamArrayConfig::appendAttributeAuto(
			gci_input_assembler_slot_t pStreamIASlot,
			EVertexDataRate pStreamDataRate,
			const VertexAttributeComponent & pAttribute )
	{
		auto streamArrayIndex = findStreamAtSlot( pStreamIASlot );

		if( streamArrayIndex == cxInvalidPosition )
		{
			streamArrayIndex = _addStreamImpl( pStreamIASlot, pStreamDataRate );
		}

		_appendAttributeImpl( _activeStreams[streamArrayIndex], pAttribute );

		return true;
	}

	void VertexStreamArrayConfig::reserveAttributeArraySpace( size_t pActiveStreamsNum )
	{
		_activeStreams.reserve( pActiveStreamsNum );
	}

	void VertexStreamArrayConfig::reset()
	{
		for( auto & streamComponent : _activeStreams )
		{
			streamComponent.reset();
		}

		_activeStreamsNum = 0;
		_activeStreamsMask.clear();
		_activeStreamsRange = InputAssemblerSlotRange::maxRange();
		_activeStreamsSlots.clear();
	}

	size_t VertexStreamArrayConfig::_addStreamImpl( gci_input_assembler_slot_t pStreamIASlot, EVertexDataRate pStreamDataRate )
	{
		auto streamIter = _activeStreams.insert( VertexStreamComponent{ pStreamIASlot } );
		streamIter->init( pStreamIASlot, pStreamDataRate );

		const auto streamArrayIndex = streamIter - _activeStreams.begin();

		_activeStreamsNum += 1;
		_activeStreamsMask.set( GCI::CxDef::makeIAVertexBufferFlag( pStreamIASlot ) );
		_activeStreamsRange.add( InputAssemblerSlotRange{ pStreamIASlot, pStreamIASlot } );
		_activeStreamsSlots.insert( pStreamIASlot );

		return streamArrayIndex;
	}

	void VertexStreamArrayConfig::_appendAttributeImpl( VertexStreamComponent & pStream, const VertexAttributeComponent & pAttribute )
	{
		for( uint32 nComponent = 0; nComponent < pAttribute.semanticComponentsNum; ++nComponent )
		{
			const auto attributeComponentSlot = pAttribute.attributeIASlot + nComponent;

			pStream.activeAttributesNum += 1;
			pStream.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( attributeComponentSlot ) );
			pStream.elementStrideInBytes += pAttribute.dataStride();
		}
	}

}
