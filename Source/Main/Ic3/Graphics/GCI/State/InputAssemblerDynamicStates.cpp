
#include "InputAssemblerDynamicStates.h"

namespace Ic3::Graphics::GCI
{

	IAVertexStreamDynamicState::IAVertexStreamDynamicState()
	{}

	IAVertexStreamDynamicState::IAVertexStreamDynamicState( const IAVertexStreamDefinition & pDefinition )
	: _vertexStreamDefinition( pDefinition )
	{}

	IAVertexStreamDynamicState::~IAVertexStreamDynamicState() = default;

	bool IAVertexStreamDynamicState::empty() const noexcept
	{
		return _vertexStreamDefinition.activeBindingsMask.empty();
	}

	native_uint IAVertexStreamDynamicState::countActiveVertexBuffers() const noexcept
	{
		return popCount( _vertexStreamDefinition.activeBindingsMask & eIAVertexStreamBindingMaskVertexBufferAllBits );
	}

	const IAVertexStreamDefinition & IAVertexStreamDynamicState::getVertexStreamDefinition() const noexcept
	{
		return _vertexStreamDefinition;
	}

	void IAVertexStreamDynamicState::assign( const IAVertexStreamDefinition & pDefinition )
	{
		_vertexStreamDefinition = pDefinition;
	}

	IAVertexBufferReference & IAVertexStreamDynamicState::setVertexBufferRef( native_uint pIndex )
	{
		ic3DebugAssert( CxDef::isIAVertexBufferIndexValid( pIndex ) );
		_vertexStreamDefinition.activeBindingsMask.set( CxDef::makeIAVertexBufferFlag( pIndex ) );
		return _vertexStreamDefinition.vertexBufferReferences[pIndex];

	}

	void IAVertexStreamDynamicState::setVertexBufferRef( native_uint pIndex, const IAVertexBufferReference & pVBReference )
	{
		_setVertexBufferRefs( pIndex, 1u, &pVBReference );
	}

	void IAVertexStreamDynamicState::setVertexBufferRefs( const IAVertexBufferReferenceArray & pVBReferences )
	{
		_setVertexBufferRefs( 0, pVBReferences.size(), pVBReferences.data() );
	}

	void IAVertexStreamDynamicState::setVertexBufferRefs( native_uint pFirstIndex, native_uint pCount, const IAVertexBufferReference * pVBReferences )
	{
		_setVertexBufferRefs( pFirstIndex, pCount, pVBReferences );
	}

	IAIndexBufferReference & IAVertexStreamDynamicState::setIndexBufferRef()
	{
		_vertexStreamDefinition.activeBindingsMask.set( eIAVertexStreamBindingFlagIndexBufferBit );
		return _vertexStreamDefinition.indexBufferReference;
	}

	void IAVertexStreamDynamicState::setIndexBufferRef( const IAIndexBufferReference & pIBReference )
	{
		_vertexStreamDefinition.activeBindingsMask.setOrUnset( eIAVertexStreamBindingFlagIndexBufferBit, !pIBReference.empty() );
		_vertexStreamDefinition.indexBufferReference = pIBReference;
	}

	void IAVertexStreamDynamicState::resetVertexBufferRef( native_uint pIndex )
	{
		_resetVertexBufferRefs( pIndex, 1 );
	}

	void IAVertexStreamDynamicState::resetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount )
	{
		_resetVertexBufferRefs( pFirstIndex, pCount );
	}

	void IAVertexStreamDynamicState::resetVertexBufferRefs()
	{
		_resetVertexBufferRefs( 0, GCM::cxIAMaxVertexBufferBindingsNum );
	}

	void IAVertexStreamDynamicState::resetIndexBufferRef()
	{
		_vertexStreamDefinition.activeBindingsMask.unset( eIAVertexStreamBindingFlagIndexBufferBit );
		_vertexStreamDefinition.indexBufferReference.reset();
	}

	void IAVertexStreamDynamicState::resetAllBufferRefs()
	{
		resetIndexBufferRef();
		resetVertexBufferRefs();
	}

	void IAVertexStreamDynamicState::_setVertexBufferRefs( native_uint pFirstIndex, native_uint pCount, const IAVertexBufferReference * pVBReferences )
	{
		for( native_uint vbIndex = pFirstIndex; ( vbIndex < GCM::cxIAMaxVertexBufferBindingsNum ) && (pCount != 0 ); ++vbIndex, --pCount )
		{
			const auto & sourceVBReference = pVBReferences[vbIndex - pFirstIndex];

			const auto vertexBufferBit = CxDef::makeIAVertexBufferFlag( vbIndex );

			_vertexStreamDefinition.vertexBufferReferences[vbIndex] = sourceVBReference;
			_vertexStreamDefinition.activeBindingsMask.setOrUnset( vertexBufferBit, !sourceVBReference.empty() );
		}
	}

	void IAVertexStreamDynamicState::_resetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount )
	{
		for( native_uint vbIndex = pFirstIndex; ( vbIndex < GCM::cxIAMaxVertexBufferBindingsNum ) && (pCount != 0 ); ++vbIndex, --pCount )
		{
			const auto vertexBufferBit = CxDef::makeIAVertexBufferFlag( vbIndex );

			_vertexStreamDefinition.vertexBufferReferences[vbIndex].reset();
			_vertexStreamDefinition.activeBindingsMask.unset( vertexBufferBit );
		}
	}

}
