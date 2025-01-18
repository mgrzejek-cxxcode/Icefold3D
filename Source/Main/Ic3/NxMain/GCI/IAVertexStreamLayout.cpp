
#include "IAVertexStreamLayout.h"
#include "IAVertexAttribLayout.h"

namespace Ic3
{

	struct VertexStreamIndexCmpEqual
	{
		IC3_ATTR_NO_DISCARD bool operator()( const VertexInputStream & pLhs, size_t pRhs ) const noexcept
		{
			return pLhs.streamIASlot == pRhs;
		}
	};

	struct VertexStreamIndexCmpLess
	{
		IC3_ATTR_NO_DISCARD bool operator()( const VertexInputStream & pLhs, size_t pRhs ) const noexcept
		{
			return pLhs.streamIASlot < pRhs;
		}
	};

	bool VertexStreamArrayConfig::checkAttributeDefinitionCompatibility(
			const VertexAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		if( const auto & targetAttributeStream = streamAt( pAttributeDefinition.streamIASlot ) )
		{
			return targetAttributeStream.checkAttributeCompatibility( pAttributeDefinition );
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
			const GenericVertexAttribute & pAttribute )
	{
		if( !cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) )
		{
			return false;
		}

		if( !isStreamActive( pStreamIASlot ) )
		{
			return false;
		}

		_appendAttributeImpl( _streamArray[pStreamIASlot], pAttribute );

		return true;
	}

	bool VertexStreamArrayConfig::appendAttributeAuto(
			gci_input_assembler_slot_t pStreamIASlot,
			EVertexDataRate pStreamDataRate,
			const GenericVertexAttribute & pAttribute )
	{
		if( !cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) )
		{
			return false;
		}

		if( !isStreamActive( pStreamIASlot ) )
		{
			_addStreamImpl( pStreamIASlot, pStreamDataRate );
		}

		_appendAttributeImpl( _streamArray[pStreamIASlot], pAttribute );

		return true;
	}

	void VertexStreamArrayConfig::reset()
	{
		for( auto & vertexStream : _streamArray )
		{
			vertexStream.reset();
		}

		_activeStreamsNum = 0;
		_activeStreamsMask.clear();
		_activeStreamsRange = InputAssemblerSlotRange::maxRange();
		_activeStreamsSlots.clear();
	}

	void VertexStreamArrayConfig::_addStreamImpl( gci_input_assembler_slot_t pStreamIASlot, EVertexDataRate pStreamDataRate )
	{
		auto & vertexStream = _streamArray[pStreamIASlot];
		vertexStream.init( pStreamIASlot, pStreamDataRate );

		_activeStreamsNum += 1;
		_activeStreamsMask.set( GCI::CxDef::makeIAVertexBufferFlag( pStreamIASlot ) );
		_activeStreamsRange.add( InputAssemblerSlotRange{ pStreamIASlot, pStreamIASlot } );
		_activeStreamsSlots.insert( pStreamIASlot );
	}

	void VertexStreamArrayConfig::_appendAttributeImpl( VertexInputStream & pStream, const GenericVertexAttribute & pAttribute )
	{
		for( uint32 nComponent = 0; nComponent < pAttribute.semanticGroupSize; ++nComponent )
		{
			const auto genericAttributeSlotIndex = pAttribute.attributeIASlot + nComponent;

			pStream.activeAttributesNum += 1;
			pStream.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( genericAttributeSlotIndex ) );
			pStream.dataStrideInBytes += pAttribute.getDataStride();
		}
	}

}
