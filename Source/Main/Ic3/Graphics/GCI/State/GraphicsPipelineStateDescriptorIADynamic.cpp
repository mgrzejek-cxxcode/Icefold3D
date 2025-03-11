
#include "GraphicsPipelineStateDescriptorIADynamic.h"

namespace Ic3::Graphics::GCI
{

	VertexSourceBindingDescriptorDynamic::VertexSourceBindingDescriptorDynamic( GPUDevice & pGPUDevice )
	: PipelineStateDescriptorDynamic<VertexSourceBindingDescriptor>( pGPUDevice, _vertexSourceBinding )
	{}

	VertexSourceBindingDescriptorDynamic::~VertexSourceBindingDescriptorDynamic() = default;

	bool VertexSourceBindingDescriptorDynamic::IsEmpty() const noexcept
	{
		return _vertexSourceBinding.IsEmpty();
	}

	bool VertexSourceBindingDescriptorDynamic::IsIndexBufferActive() const noexcept
	{
		return _vertexSourceBinding.activeStreamsMask.is_set( eVertexSourceBindingFlagIndexBufferBit );
	}

	bool VertexSourceBindingDescriptorDynamic::IsVertexBufferActive( native_uint pVertexBufferIndex ) const noexcept
	{
		return _vertexSourceBinding.activeStreamsMask.is_set( CXU::IAMakeVertexSourceVertexBufferBindingFlag( pVertexBufferIndex ) );
	}

	bool VertexSourceBindingDescriptorDynamic::IsBufferActive( native_uint pBufferIndex ) const noexcept
	{
		return _vertexSourceBinding.activeStreamsMask.is_set( CXU::IAMakeVertexSourceBufferBindingFlag( pBufferIndex ) );
	}

	native_uint VertexSourceBindingDescriptorDynamic::CountActiveVertexBuffers() const noexcept
	{
		return ( _vertexSourceBinding.activeStreamsMask & eVertexSourceBindingMaskVertexBufferAllBits ).count_bits();
	}

	const IAVertexSourceBindingDefinition & VertexSourceBindingDescriptorDynamic::GetVertexSourceBindingDefinition() const noexcept
	{
		return _vertexSourceBinding;
	}

	IAVertexBufferReference * VertexSourceBindingDescriptorDynamic::UpdateActiveVertexBuffer( native_uint pVertexBufferIndex )
	{
		return _SetVertexBufferActive( pVertexBufferIndex );
	}

	IAIndexBufferReference * VertexSourceBindingDescriptorDynamic::UpdateActiveIndexBuffer()
	{
		return _SetIndexBufferActive();
	}

	void VertexSourceBindingDescriptorDynamic::SetStreamArrayConfiguration(
			const IAVertexSourceBindingDefinition & pStreamArrayConfiguration )
	{
		_SetVertexBufferReferences( 0, GCM::kIAMaxDataStreamVertexBuffersNum, pStreamArrayConfiguration.vertexBufferReferences.data() );
		_SetIndexBufferReference( pStreamArrayConfiguration.indexBufferReference );
	}

	void VertexSourceBindingDescriptorDynamic::SetVertexBufferReference(
			native_uint pVertexBufferIndex,
			const IAVertexBufferReference & pBufferReference )
	{
		_SetVertexBufferReferences( pVertexBufferIndex, 1, &pBufferReference );
	}

	void VertexSourceBindingDescriptorDynamic::SetVertexBufferReferences(
			const cppx::array_view<const IAVertexBufferReference> & pBufferReferences,
			native_uint pVertexBufferFirstIndex,
			native_uint pVertexBufferCount )
	{
		_SetVertexBufferReferences( pVertexBufferFirstIndex, pVertexBufferCount, pBufferReferences.data() );
	}

	void VertexSourceBindingDescriptorDynamic::SetIndexBufferReference( const IAIndexBufferReference & pBufferReference )
	{
		_SetIndexBufferReference( pBufferReference );
	}

	void VertexSourceBindingDescriptorDynamic::ResetAll()
	{
		_ResetVertexBufferReferences( 0, GCM::kIAMaxDataStreamVertexBuffersNum );
		_ResetIndexBufferReference();
	}

	void VertexSourceBindingDescriptorDynamic::ResetAllFlags()
	{
		_vertexSourceBinding.activeStreamsMask.clear();
		_vertexSourceBinding.activeStreamsNum = 0;
	}

	void VertexSourceBindingDescriptorDynamic::ResetVertexBufferReference( native_uint pVertexBufferIndex )
	{
		_ResetVertexBufferReferences( pVertexBufferIndex, 1 );
	}

	void VertexSourceBindingDescriptorDynamic::ResetVertexBufferReferences(
			native_uint pVertexBufferFirstIndex,
			native_uint pVertexBufferCount )
	{
		_ResetVertexBufferReferences( pVertexBufferFirstIndex, pVertexBufferCount );
	}

	void VertexSourceBindingDescriptorDynamic::ResetIndexBufferReference()
	{
		_ResetIndexBufferReference();
	}

	TGfxHandle<VertexSourceBindingDescriptorDynamic> VertexSourceBindingDescriptorDynamic::CreateNew( GPUDevice & pGPUDevice )
	{
		return CreateDynamicObject<VertexSourceBindingDescriptorDynamic>( pGPUDevice );
	}

	IAVertexBufferReference * VertexSourceBindingDescriptorDynamic::_SetVertexBufferActive( native_uint pVertexBufferIndex )
	{
		if( CXU::IAIsDataStreamVertexBufferSlotValid( pVertexBufferIndex ) )
		{
			const auto vertexBufferBit = CXU::IAMakeVertexSourceVertexBufferBindingFlag( pVertexBufferIndex );

			auto & vertexBufferReference = _vertexSourceBinding.vertexBufferReferences[pVertexBufferIndex];

			if( !_vertexSourceBinding.activeStreamsMask.is_set( vertexBufferBit ) )
			{
				_vertexSourceBinding.activeStreamsMask.set( vertexBufferBit );
				_vertexSourceBinding.activeStreamsNum += 1;
			}

			return &vertexBufferReference;
		}

		return nullptr;
	}

	void VertexSourceBindingDescriptorDynamic::_SetVertexBufferReferences(
			native_uint pVertexBufferFirstIndex,
			native_uint pVertexBufferCount,
			const IAVertexBufferReference * pBufferReferences )
	{
		if( ( pVertexBufferCount > 0 ) && CXU::IAIsDataStreamVertexBufferSlotValid( pVertexBufferFirstIndex ) )
		{
			for( uint32 nVertexBuffer = 0; nVertexBuffer < pVertexBufferCount ; ++nVertexBuffer )
			{
				const auto vertexBufferIndex = pVertexBufferFirstIndex + nVertexBuffer;
				if( !CXU::IAIsDataStreamVertexBufferSlotValid( vertexBufferIndex ) )
				{
					break;
				}

				const auto vertexBufferBit = CXU::IAMakeVertexSourceVertexBufferBindingFlag( vertexBufferIndex );

				auto & vertexBufferReference = _vertexSourceBinding.vertexBufferReferences[vertexBufferIndex];

				if( const auto & inputVertexBufferReference = pBufferReferences[nVertexBuffer] )
				{
					if( !_vertexSourceBinding.activeStreamsMask.is_set( vertexBufferBit ) )
					{
						_vertexSourceBinding.activeStreamsMask.set( vertexBufferBit );
						_vertexSourceBinding.activeStreamsNum += 1;
					}

					vertexBufferReference = inputVertexBufferReference;
				}
				else
				{
					if( _vertexSourceBinding.activeStreamsMask.is_set( vertexBufferBit ) )
					{
						_vertexSourceBinding.activeStreamsMask.unset( vertexBufferBit );
						_vertexSourceBinding.activeStreamsNum -= 1;
					}

					vertexBufferReference.Reset();
				}
			}
		}
	}

	IAIndexBufferReference * VertexSourceBindingDescriptorDynamic::_SetIndexBufferActive()
	{
		if( !_vertexSourceBinding.activeStreamsMask.is_set( eVertexSourceBindingFlagIndexBufferBit ) )
		{
			_vertexSourceBinding.activeStreamsMask.set( eVertexSourceBindingFlagIndexBufferBit );
			_vertexSourceBinding.activeStreamsNum += 1;
		}

		return &( _vertexSourceBinding.indexBufferReference );
	}

	void VertexSourceBindingDescriptorDynamic::_SetIndexBufferReference( const IAIndexBufferReference & pBufferReference )
	{
		if( pBufferReference )
		{
			if( !_vertexSourceBinding.activeStreamsMask.is_set( eVertexSourceBindingFlagIndexBufferBit ) )
			{
				_vertexSourceBinding.activeStreamsMask.set( eVertexSourceBindingFlagIndexBufferBit );
				_vertexSourceBinding.activeStreamsNum += 1;
			}

			_vertexSourceBinding.indexBufferReference = pBufferReference;
		}
		else
		{
			if( _vertexSourceBinding.activeStreamsMask.is_set( eVertexSourceBindingFlagIndexBufferBit ) )
			{
				_vertexSourceBinding.activeStreamsMask.unset( eVertexSourceBindingFlagIndexBufferBit );
				_vertexSourceBinding.activeStreamsNum -= 1;
			}

			_vertexSourceBinding.indexBufferReference.Reset();
		}
	}

	void VertexSourceBindingDescriptorDynamic::_ResetVertexBufferReferences(
			native_uint pVertexBufferFirstIndex,
			native_uint pVertexBufferCount )
	{
		if( ( pVertexBufferCount > 0 ) && CXU::IAIsDataStreamVertexBufferSlotValid( pVertexBufferFirstIndex ) )
		{
			for( uint32 nVertexBuffer = 0; nVertexBuffer < pVertexBufferCount; ++nVertexBuffer )
			{
				const auto vertexBufferIndex = pVertexBufferFirstIndex + nVertexBuffer;
				if( !CXU::IAIsDataStreamVertexBufferSlotValid( vertexBufferIndex ) )
				{
					break;
				}

				const auto vertexBufferBit = CXU::IAMakeVertexSourceVertexBufferBindingFlag( vertexBufferIndex );

				auto & vertexBufferReference = _vertexSourceBinding.vertexBufferReferences[vertexBufferIndex];

				if( _vertexSourceBinding.activeStreamsMask.is_set( vertexBufferBit ) )
				{
					_vertexSourceBinding.activeStreamsMask.unset( vertexBufferBit );
					_vertexSourceBinding.activeStreamsNum -= 1;
				}

				vertexBufferReference.Reset();
			}
		}
	}

	void VertexSourceBindingDescriptorDynamic::_ResetIndexBufferReference()
	{
		if( _vertexSourceBinding.activeStreamsMask.is_set( eVertexSourceBindingFlagIndexBufferBit ) )
		{
			_vertexSourceBinding.activeStreamsMask.unset( eVertexSourceBindingFlagIndexBufferBit );
			_vertexSourceBinding.activeStreamsNum -= 1;
		}

		_vertexSourceBinding.indexBufferReference.Reset();
	}

}
