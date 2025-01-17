
#include "InputAssemblerDynamicStates.h"

namespace Ic3::Graphics::GCI
{

	IAVertexStreamDynamicState::IAVertexStreamDynamicState()
	{}

	IAVertexStreamDynamicState::IAVertexStreamDynamicState( const IAVertexStreamDefinition & pDefinition )
	: _vertexStreamDefinition( pDefinition )
	{}

	IAVertexStreamDynamicState::~IAVertexStreamDynamicState() = default;

	bool IAVertexStreamDynamicState::IsEmpty() const noexcept
	{
		return _vertexStreamDefinition.activeBindingsMask.empty();
	}

	native_uint IAVertexStreamDynamicState::CountActiveVertexBuffers() const noexcept
	{
		return pop_count( _vertexStreamDefinition.activeBindingsMask & eIAVertexStreamBindingMaskVertexBufferAllBits );
	}

	const IAVertexStreamDefinition & IAVertexStreamDynamicState::GetVertexStreamDefinition() const noexcept
	{
		return _vertexStreamDefinition;
	}

	void IAVertexStreamDynamicState::Assign( const IAVertexStreamDefinition & pDefinition )
	{
		_vertexStreamDefinition = pDefinition;
	}

	IAVertexBufferReference & IAVertexStreamDynamicState::SetVertexBufferRef( native_uint pIndex )
	{
		ic3DebugAssert( CxDef::IsIAVertexBufferIndexValid( pIndex ) );
		_vertexStreamDefinition.activeBindingsMask.set( CxDef::makeIAVertexBufferFlag( pIndex ) );
		return _vertexStreamDefinition.vertexBufferReferences[pIndex];

	}

	void IAVertexStreamDynamicState::SetVertexBufferRef( native_uint pIndex, const IAVertexBufferReference & pVBReference )
	{
		_SetVertexBufferRefs( pIndex, 1u, &pVBReference );
	}

	void IAVertexStreamDynamicState::SetVertexBufferRefs( const IAVertexBufferReferenceArray & pVBReferences )
	{
		_SetVertexBufferRefs( 0, pVBReferences.size(), pVBReferences.data() );
	}

	void IAVertexStreamDynamicState::SetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount, const IAVertexBufferReference * pVBReferences )
	{
		_SetVertexBufferRefs( pFirstIndex, pCount, pVBReferences );
	}

	IAIndexBufferReference & IAVertexStreamDynamicState::SetIndexBufferRef()
	{
		_vertexStreamDefinition.activeBindingsMask.set( eIAVertexStreamBindingFlagIndexBufferBit );
		return _vertexStreamDefinition.indexBufferReference;
	}

	void IAVertexStreamDynamicState::SetIndexBufferRef( const IAIndexBufferReference & pIBReference )
	{
		_vertexStreamDefinition.activeBindingsMask.set_or_unset(
				eIAVertexStreamBindingFlagIndexBufferBit,
				!pIBReference.IsEmpty());
		_vertexStreamDefinition.indexBufferReference = pIBReference;
	}

	void IAVertexStreamDynamicState::ResetVertexBufferRef( native_uint pIndex )
	{
		_ResetVertexBufferRefs( pIndex, 1 );
	}

	void IAVertexStreamDynamicState::ResetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount )
	{
		_ResetVertexBufferRefs( pFirstIndex, pCount );
	}

	void IAVertexStreamDynamicState::ResetVertexBufferRefs()
	{
		_ResetVertexBufferRefs( 0, GCM::cxIAMaxVertexBufferBindingsNum );
	}

	void IAVertexStreamDynamicState::ResetIndexBufferRef()
	{
		_vertexStreamDefinition.activeBindingsMask.unset( eIAVertexStreamBindingFlagIndexBufferBit );
		_vertexStreamDefinition.indexBufferReference.Reset();
	}

	void IAVertexStreamDynamicState::ResetAllBufferRefs()
	{
		ResetIndexBufferRef();
		ResetVertexBufferRefs();
	}

	void IAVertexStreamDynamicState::_SetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount, const IAVertexBufferReference * pVBReferences )
	{
		for( native_uint vbIndex = pFirstIndex; ( vbIndex < GCM::cxIAMaxVertexBufferBindingsNum ) && (pCount != 0 ); ++vbIndex, --pCount )
		{
			const auto & sourceVBReference = pVBReferences[vbIndex - pFirstIndex];

			const auto vertexBufferBit = CxDef::makeIAVertexBufferFlag( vbIndex );

			_vertexStreamDefinition.vertexBufferReferences[vbIndex] = sourceVBReference;
			_vertexStreamDefinition.activeBindingsMask.set_or_unset( vertexBufferBit, !sourceVBReference.IsEmpty() );
		}
	}

	void IAVertexStreamDynamicState::_ResetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount )
	{
		for( native_uint vbIndex = pFirstIndex; ( vbIndex < GCM::cxIAMaxVertexBufferBindingsNum ) && (pCount != 0 ); ++vbIndex, --pCount )
		{
			const auto vertexBufferBit = CxDef::makeIAVertexBufferFlag( vbIndex );

			_vertexStreamDefinition.vertexBufferReferences[vbIndex].Reset();
			_vertexStreamDefinition.activeBindingsMask.unset( vertexBufferBit );
		}
	}

}
