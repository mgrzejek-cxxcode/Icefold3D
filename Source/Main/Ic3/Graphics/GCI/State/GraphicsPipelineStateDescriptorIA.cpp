
#include "GraphicsPipelineStateDescriptorIA.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	IAInputLayoutStateDescriptor::IAInputLayoutStateDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const IAInputLayoutCommonConfig & pCommonInputLayoutConfig )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	, mCommonInputLayoutConfig( pCommonInputLayoutConfig )
	{}

	IAInputLayoutStateDescriptor::~IAInputLayoutStateDescriptor() = default;

	EPipelineStateDescriptorType IAInputLayoutStateDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTIAInputLayout;
	}


	IAVertexStreamStateDescriptor::IAVertexStreamStateDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const IAVertexStreamCommonConfig & pCommonVertexStreamConfig )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	, mCommonVertexStreamConfig( pCommonVertexStreamConfig )
	{}

	IAVertexStreamStateDescriptor::~IAVertexStreamStateDescriptor() = default;

	EPipelineStateDescriptorType IAVertexStreamStateDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTIAVertexStream;
	}


	IAVertexStreamStateDescriptorDynamic::IAVertexStreamStateDescriptorDynamic(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	: IAVertexStreamStateDescriptor( pGPUDevice, pDescriptorID, _vertexStreamConfiguration )
	{}

	IAVertexStreamStateDescriptorDynamic::~IAVertexStreamStateDescriptorDynamic() = default;

	bool IAVertexStreamStateDescriptorDynamic::IsDynamicDescriptor() const noexcept
	{
		return true;
	}

	bool IAVertexStreamStateDescriptorDynamic::IsEmpty() const noexcept
	{
		return _vertexStreamConfiguration.IsEmpty();
	}

	bool IAVertexStreamStateDescriptorDynamic::IsIndexBufferActive() const noexcept
	{
		return _vertexStreamConfiguration.activeStreamsMask.is_set( eIAVertexStreamBindingFlagIndexBufferBit );
	}

	native_uint IAVertexStreamStateDescriptorDynamic::CountActiveVertexBuffers() const noexcept
	{
		return ( _vertexStreamConfiguration.activeStreamsMask & eIAVertexStreamBindingMaskVertexBufferAllBits ).count_bits();
	}

	const IAVertexStreamConfiguration & IAVertexStreamStateDescriptorDynamic::GetStreamArrayConfiguration() const noexcept
	{
		return _vertexStreamConfiguration;
	}

	IAVertexBufferReference * IAVertexStreamStateDescriptorDynamic::UpdateActiveVertexBuffer( native_uint pVertexBufferIndex )
	{
		return _SetVertexBufferActive( pVertexBufferIndex );
	}

	IAIndexBufferReference * IAVertexStreamStateDescriptorDynamic::UpdateActiveIndexBuffer()
	{
		return _SetIndexBufferActive();
	}

	void IAVertexStreamStateDescriptorDynamic::SetStreamArrayConfiguration(
			const IAVertexStreamConfiguration & pStreamArrayConfiguration )
	{
		_SetVertexBufferReferences( 0, GCM::kIAMaxDataStreamVertexBuffersNum, pStreamArrayConfiguration.vertexBufferReferences.data() );
		_SetIndexBufferReference( pStreamArrayConfiguration.indexBufferReference );
	}

	void IAVertexStreamStateDescriptorDynamic::SetVertexBufferReference(
			native_uint pVertexBufferIndex,
			const IAVertexBufferReference & pBufferReference )
	{
		_SetVertexBufferReferences( pVertexBufferIndex, 1, &pBufferReference );
	}

	void IAVertexStreamStateDescriptorDynamic::SetVertexBufferReferences(
			const cppx::array_view<const IAVertexBufferReference> & pBufferReferences,
			native_uint pVertexBufferFirstIndex,
			native_uint pVertexBufferCount )
	{
		_SetVertexBufferReferences( pVertexBufferFirstIndex, pVertexBufferCount, pBufferReferences.data() );
	}

	void IAVertexStreamStateDescriptorDynamic::SetIndexBufferReference( const IAIndexBufferReference & pBufferReference )
	{
		_SetIndexBufferReference( pBufferReference );
	}

	void IAVertexStreamStateDescriptorDynamic::ResetAll()
	{
		_ResetVertexBufferReferences( 0, GCM::kIAMaxDataStreamVertexBuffersNum );
		_ResetIndexBufferReference();
	}

	void IAVertexStreamStateDescriptorDynamic::ResetAllFlags()
	{
		_vertexStreamConfiguration.activeStreamsMask.clear();
		_vertexStreamConfiguration.activeStreamsNum = 0;
	}

	void IAVertexStreamStateDescriptorDynamic::ResetVertexBufferReference( native_uint pVertexBufferIndex )
	{
		_ResetVertexBufferReferences( pVertexBufferIndex, 1 );
	}

	void IAVertexStreamStateDescriptorDynamic::ResetVertexBufferReferences(
			native_uint pVertexBufferFirstIndex,
			native_uint pVertexBufferCount )
	{
		_ResetVertexBufferReferences( pVertexBufferFirstIndex, pVertexBufferCount );
	}

	void IAVertexStreamStateDescriptorDynamic::ResetIndexBufferReference()
	{
		_ResetIndexBufferReference();
	}

	IAVertexBufferReference * IAVertexStreamStateDescriptorDynamic::_SetVertexBufferActive( native_uint pVertexBufferIndex )
	{
		if( CXU::IAIsDataStreamVertexBufferIndexValid( pVertexBufferIndex ) )
		{
			const auto vertexBufferBit = CXU::IAMakeDataStreamVertexBufferBindingFlag( pVertexBufferIndex );

			auto & vertexBufferReference = _vertexStreamConfiguration.vertexBufferReferences[pVertexBufferIndex];

			if( !_vertexStreamConfiguration.activeStreamsMask.is_set( vertexBufferBit ) )
			{
				_vertexStreamConfiguration.activeStreamsMask.set( vertexBufferBit );
				_vertexStreamConfiguration.activeStreamsNum += 1;
			}

			return &vertexBufferReference;
		}

		return nullptr;
	}

	void IAVertexStreamStateDescriptorDynamic::_SetVertexBufferReferences(
			native_uint pVertexBufferFirstIndex,
			native_uint pVertexBufferCount,
			const IAVertexBufferReference * pBufferReferences )
	{
		if( ( pVertexBufferCount > 0 ) && CXU::IAIsDataStreamVertexBufferIndexValid( pVertexBufferFirstIndex ) )
		{
			for( uint32 nVertexBuffer = 0; nVertexBuffer < pVertexBufferCount; ++nVertexBuffer )
			{
				const auto vertexBufferIndex = pVertexBufferFirstIndex + nVertexBuffer;
				if( !CXU::IAIsDataStreamVertexBufferIndexValid( vertexBufferIndex ) )
				{
					break;
				}

				const auto vertexBufferBit = CXU::IAMakeDataStreamVertexBufferBindingFlag( vertexBufferIndex );

				auto & vertexBufferReference = _vertexStreamConfiguration.vertexBufferReferences[vertexBufferIndex];

				if( const auto & inputVertexBufferReference = pBufferReferences[nVertexBuffer] )
				{
					if( !_vertexStreamConfiguration.activeStreamsMask.is_set( vertexBufferBit ) )
					{
						_vertexStreamConfiguration.activeStreamsMask.set( vertexBufferBit );
						_vertexStreamConfiguration.activeStreamsNum += 1;
					}

					vertexBufferReference = inputVertexBufferReference;
				}
				else
				{
					if( _vertexStreamConfiguration.activeStreamsMask.is_set( vertexBufferBit ) )
					{
						_vertexStreamConfiguration.activeStreamsMask.unset( vertexBufferBit );
						_vertexStreamConfiguration.activeStreamsNum -= 1;
					}

					vertexBufferReference.Reset();
				}
			}
		}
	}

	IAIndexBufferReference * IAVertexStreamStateDescriptorDynamic::_SetIndexBufferActive()
	{
		if( !_vertexStreamConfiguration.activeStreamsMask.is_set( eIAVertexStreamBindingFlagIndexBufferBit ) )
		{
			_vertexStreamConfiguration.activeStreamsMask.set( eIAVertexStreamBindingFlagIndexBufferBit );
			_vertexStreamConfiguration.activeStreamsNum += 1;
		}

		return &( _vertexStreamConfiguration.indexBufferReference );
	}

	void IAVertexStreamStateDescriptorDynamic::_SetIndexBufferReference( const IAIndexBufferReference & pBufferReference )
	{
		if( pBufferReference )
		{
			if( !_vertexStreamConfiguration.activeStreamsMask.is_set( eIAVertexStreamBindingFlagIndexBufferBit ) )
			{
				_vertexStreamConfiguration.activeStreamsMask.set( eIAVertexStreamBindingFlagIndexBufferBit );
				_vertexStreamConfiguration.activeStreamsNum += 1;
			}

			_vertexStreamConfiguration.indexBufferReference = pBufferReference;
		}
		else
		{
			if( _vertexStreamConfiguration.activeStreamsMask.is_set( eIAVertexStreamBindingFlagIndexBufferBit ) )
			{
				_vertexStreamConfiguration.activeStreamsMask.unset( eIAVertexStreamBindingFlagIndexBufferBit );
				_vertexStreamConfiguration.activeStreamsNum -= 1;
			}

			_vertexStreamConfiguration.indexBufferReference.Reset();
		}
	}

	void IAVertexStreamStateDescriptorDynamic::_ResetVertexBufferReferences(
			native_uint pVertexBufferFirstIndex,
			native_uint pVertexBufferCount )
	{
		if( ( pVertexBufferCount > 0 ) && CXU::IAIsDataStreamVertexBufferIndexValid( pVertexBufferFirstIndex ) )
		{
			for( uint32 nVertexBuffer = 0; nVertexBuffer < pVertexBufferCount; ++nVertexBuffer )
			{
				const auto vertexBufferIndex = pVertexBufferFirstIndex + nVertexBuffer;
				if( !CXU::IAIsDataStreamVertexBufferIndexValid( vertexBufferIndex ) )
				{
					break;
				}

				const auto vertexBufferBit = CXU::IAMakeDataStreamVertexBufferBindingFlag( vertexBufferIndex );

				auto & vertexBufferReference = _vertexStreamConfiguration.vertexBufferReferences[vertexBufferIndex];

				if( _vertexStreamConfiguration.activeStreamsMask.is_set( vertexBufferBit ) )
				{
					_vertexStreamConfiguration.activeStreamsMask.unset( vertexBufferBit );
					_vertexStreamConfiguration.activeStreamsNum -= 1;
				}

				vertexBufferReference.Reset();
			}
		}
	}

	void IAVertexStreamStateDescriptorDynamic::_ResetIndexBufferReference()
	{
		if( _vertexStreamConfiguration.activeStreamsMask.is_set( eIAVertexStreamBindingFlagIndexBufferBit ) )
		{
			_vertexStreamConfiguration.activeStreamsMask.unset( eIAVertexStreamBindingFlagIndexBufferBit );
			_vertexStreamConfiguration.activeStreamsNum -= 1;
		}

		_vertexStreamConfiguration.indexBufferReference.Reset();
	}

	namespace PIM
	{

		//!! IAInputLayoutStateDescriptorNative

		IAInputLayoutStateDescriptorNative::IAInputLayoutStateDescriptorNative(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAInputLayoutCommonConfig & pCommonInputLayoutConfig )
		: IAInputLayoutStateDescriptor( pGPUDevice, pDescriptorID, pCommonInputLayoutConfig )
		{}

		IAInputLayoutStateDescriptorNative::~IAInputLayoutStateDescriptorNative() = default;


		//!! IAVertexStreamStateDescriptorNative

		IAVertexStreamStateDescriptorNative::IAVertexStreamStateDescriptorNative(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAVertexStreamCommonConfig & pCommonVertexStreamConfig )
		: IAVertexStreamStateDescriptor( pGPUDevice, pDescriptorID, pCommonVertexStreamConfig )
		{}

		IAVertexStreamStateDescriptorNative::~IAVertexStreamStateDescriptorNative() = default;

	} // namespace PIM

}
