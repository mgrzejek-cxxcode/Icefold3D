
#include "IAVertexStreamLayout.h"
#include "IAVertexAttribLayout.h"

namespace Ic3
{

	bool VertexInputStreamArrayConfig::CheckAttributeDefinitionCompatibility(
			const VertexInputAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		if( const auto & targetAttributeStream = StreamAt( pAttributeDefinition.vertexStreamSlot ) )
		{
			return targetAttributeStream.CheckAttributeCompatibility( pAttributeDefinition );
		}

		return true;
	}

	bool VertexInputStreamArrayConfig::AddActiveStream(
			gci_input_assembler_slot_t pStreamSlot,
			GCI::EIAVertexAttributeDataRate pStreamDataRate )
	{
		if( pStreamDataRate == GCI::EIAVertexAttributeDataRate::Undefined )
		{
			return false;
		}

		if( _streamArray[pStreamSlot].IsInitialized() )
		{
			return true;
		}

		_AddStreamImpl( pStreamSlot, pStreamDataRate );

		return true;
	}

	bool VertexInputStreamArrayConfig::AppendAttribute(
			gci_input_assembler_slot_t pStreamSlot,
			const GenericVertexInputAttribute & pAttribute )
	{
		if( !GCI::CXU::IAIsDataStreamVertexBufferSlotValid( pStreamSlot ) )
		{
			return false;
		}

		if( !_streamArray[pStreamSlot].IsInitialized() )
		{
			return false;
		}

		_AppendAttributeImpl( _streamArray[pStreamSlot], pAttribute );

		return true;
	}

	bool VertexInputStreamArrayConfig::AppendAttributeAuto(
			gci_input_assembler_slot_t pStreamSlot,
			GCI::EIAVertexAttributeDataRate pStreamDataRate,
			const GenericVertexInputAttribute & pAttribute )
	{
		if( !GCI::CXU::IAIsDataStreamVertexBufferSlotValid( pStreamSlot ) )
		{
			return false;
		}

		if( !_streamArray[pStreamSlot].IsInitialized() )
		{
			_AddStreamImpl( pStreamSlot, pStreamDataRate );
		}

		_AppendAttributeImpl( _streamArray[pStreamSlot], pAttribute );

		return true;
	}

	void VertexInputStreamArrayConfig::Reset()
	{
		for( auto & vertexStream : _streamArray )
		{
			vertexStream.Reset();
		}

		_activeStreamsNum = 0;
		_activeStreamsMask.clear();
		_activeStreamsRange = InputAssemblerSlotRange::empty_range();
		_activeStreamsSlots.clear();
	}

	void VertexInputStreamArrayConfig::_AddStreamImpl( gci_input_assembler_slot_t pStreamSlot, GCI::EIAVertexAttributeDataRate pStreamDataRate )
	{
		auto & vertexStream = _streamArray[pStreamSlot];
		vertexStream.Init( pStreamSlot, pStreamDataRate );

		_activeStreamsNum += 1;
		_activeStreamsMask.set( GCI::CXU::IAMakeVertexSourceVertexBufferBindingFlag( pStreamSlot ) );
		_activeStreamsRange.add( InputAssemblerSlotRange{ pStreamSlot, pStreamSlot } );
		_activeStreamsSlots.insert( pStreamSlot );
	}

	void VertexInputStreamArrayConfig::_AppendAttributeImpl( VertexInputStream & pStream, const GenericVertexInputAttribute & pAttribute )
	{
		for( uint32 nComponent = 0; nComponent < pAttribute.semanticGroupSize; ++nComponent )
		{
			const auto genericAttributeSlot = pAttribute.attributeSlot + nComponent;
			const auto genericAttributeDataStride = pAttribute.GetDataStride();;

			pStream.activeAttributesNum += 1;
			pStream.activeAttributesMask.set( GCI::CXU::IAMakeVertexAttributeFlag( genericAttributeSlot ) );
			pStream.dataStrideInBytes += genericAttributeDataStride;
		}
	}

}
