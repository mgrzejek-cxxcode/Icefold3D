
#include "IAVertexStreamLayout.h"
#include "IAVertexAttribLayout.h"

namespace Ic3
{

	struct VertexStreamIndexCmpEqual
	{
		CPPX_ATTR_NO_DISCARD bool operator()( const VertexInputStream & pLhs, size_t pRhs ) const noexcept
		{
			return pLhs.streamIASlot == pRhs;
		}
	};

	struct VertexStreamIndexCmpLess
	{
		CPPX_ATTR_NO_DISCARD bool operator()( const VertexInputStream & pLhs, size_t pRhs ) const noexcept
		{
			return pLhs.streamIASlot < pRhs;
		}
	};

	bool VertexStreamArrayConfig::CheckAttributeDefinitionCompatibility(
			const VertexAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		if( const auto & targetAttributeStream = StreamAt( pAttributeDefinition.streamIASlot ) )
		{
			return targetAttributeStream.CheckAttributeCompatibility( pAttributeDefinition );
		}

		return true;
	}

	bool VertexStreamArrayConfig::AddActiveStream(
			gci_input_assembler_slot_t pStreamIASlot,
			EVertexDataRate pStreamDataRate )
	{
		if( pStreamDataRate == EVertexDataRate::Undefined )
		{
			return false;
		}

		if( IsStreamActive( pStreamIASlot ) )
		{
			return true;
		}

		_AddStreamImpl( pStreamIASlot, pStreamDataRate );

		return true;
	}

	bool VertexStreamArrayConfig::AppendAttribute(
			gci_input_assembler_slot_t pStreamIASlot,
			const GenericVertexAttribute & pAttribute )
	{
		if( !cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) )
		{
			return false;
		}

		if( !IsStreamActive( pStreamIASlot ) )
		{
			return false;
		}

		_AppendAttributeImpl( _streamArray[pStreamIASlot], pAttribute );

		return true;
	}

	bool VertexStreamArrayConfig::AppendAttributeAuto(
			gci_input_assembler_slot_t pStreamIASlot,
			EVertexDataRate pStreamDataRate,
			const GenericVertexAttribute & pAttribute )
	{
		if( !cxGCIValidInputAssemblerSlotIndexRange.contains( pStreamIASlot ) )
		{
			return false;
		}

		if( !IsStreamActive( pStreamIASlot ) )
		{
			_AddStreamImpl( pStreamIASlot, pStreamDataRate );
		}

		_AppendAttributeImpl( _streamArray[pStreamIASlot], pAttribute );

		return true;
	}

	void VertexStreamArrayConfig::Reset()
	{
		for( auto & vertexStream : _streamArray )
		{
			vertexStream.Reset();
		}

		_activeStreamsNum = 0;
		_activeStreamsMask.clear();
		_activeStreamsRange = InputAssemblerSlotRange::max_range();
		_activeStreamsSlots.clear();
	}

	void VertexStreamArrayConfig::_AddStreamImpl( gci_input_assembler_slot_t pStreamIASlot, EVertexDataRate pStreamDataRate )
	{
		auto & vertexStream = _streamArray[pStreamIASlot];
		vertexStream.Init( pStreamIASlot, pStreamDataRate );

		_activeStreamsNum += 1;
		_activeStreamsMask.set( GCI::CxDef::makeIAVertexBufferFlag( pStreamIASlot ) );
		_activeStreamsRange.add( InputAssemblerSlotRange{ pStreamIASlot, pStreamIASlot } );
		_activeStreamsSlots.insert( pStreamIASlot );
	}

	void VertexStreamArrayConfig::_AppendAttributeImpl( VertexInputStream & pStream, const GenericVertexAttribute & pAttribute )
	{
		for( uint32 nComponent = 0; nComponent < pAttribute.semanticGroupSize; ++nComponent )
		{
			const auto genericAttributeSlotIndex = pAttribute.attributeIASlot + nComponent;

			pStream.activeAttributesNum += 1;
			pStream.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( genericAttributeSlotIndex ) );
			pStream.dataStrideInBytes += pAttribute.GetDataStride();
		}
	}

}
