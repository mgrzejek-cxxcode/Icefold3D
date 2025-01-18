
#include "MTLGPUBuffer.h"
#include "../MTL_commandList.h"
#include "../MTLUDevice.h"

#include <Metal/MTLDevice.h>

namespace Ic3::Graphics::GCI
{

	MetalGPUBuffer::MetalGPUBuffer(
			MetalGPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GPUBufferProperties & pBufferProperties,
			id<MTLBuffer> pMTLBuffer )
	: GPUBuffer( pGPUDevice, pResourceMemory, pBufferProperties )
	, mMTLDevice( [pMTLBuffer device] )
	, mMTLBuffer( pMTLBuffer )
	{}

	MetalGPUBuffer::~MetalGPUBuffer() = default;

	MetalGPUBufferHandle MetalGPUBuffer::Create( MetalGPUDevice & pGPUDevice, const GPUBufferCreateInfo & pCreateInfo )
	{}

	bool MetalGPUBuffer::MapRegion( void *, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode )
	{
		auto * baseBufferPointer = [mMTLBuffer contents];
		if( !baseBufferPointer )
		{
			return false;
		}

		ResourceMappedMemory mappedMemoryInfo;
		mappedMemoryInfo.pointer = baseBufferPointer;
		mappedMemoryInfo.mappedRegion = pRegion;
		mappedMemoryInfo.memoryMapFlags = static_cast<EGPUMemoryMapFlags>( pMapMode );
		SetMappedMemory( mappedMemoryInfo );

		return true;
	}

	void MetalGPUBuffer::Unmap( void * )
	{
		if( const auto & mappedMemory = GetMappedMemory() )
		{
			NSRange updateRange;
			updateRange.location = cppx::numeric_cast<NSUInteger>( mappedMemory.mappedRegion.offset );
			updateRange.length = cppx::numeric_cast<NSUInteger>( mappedMemory.mappedRegion.size );

			[mMTLBuffer didModifyRange:updateRange];

			ResetMappedMemory();
		}
	}

	void MetalGPUBuffer::FlushMappedRegion( void *, const GPUMemoryRegion & pRegion )
	{
		if( const auto & mappedMemory = GetMappedMemory() )
		{
			NSRange updateRange;
			updateRange.location = cppx::numeric_cast<NSUInteger>( mappedMemory.mappedRegion.offset );
			updateRange.length = cppx::numeric_cast<NSUInteger>( mappedMemory.mappedRegion.size );

			[mMTLBuffer didModifyRange:updateRange];
		}
	}

	void MetalGPUBuffer::InvalidateRegion( void * pCommandObject, const GPUMemoryRegion & pRegion )
	{
	}

	void MetalGPUBuffer::UpdateSubDataCopy( void * pCommandObject, GPUBuffer & pSrcBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc )
	{
		auto * metalCommandList = reinterpret_cast<MetalCommandList *>( pCommandObject );
		if( !metalCommandList->CheckCommandClassSupport( ECommandQueueClass::Transfer ) )
		{
			throw 0;
		}

		auto * sourceMTLBuffer = pSrcBuffer.QueryInterface<MetalGPUBuffer>()->mMTLBuffer;

		id<MTLBlitCommandEncoder> blitCommandEncoder = [metalCommandList->mMTLCommandBuffer blitCommandEncoder];

		[blitCommandEncoder copyFromBuffer:sourceMTLBuffer
		                      sourceOffset:pCopyDesc.sourceBufferRegion.offset
		                          toBuffer:mMTLBuffer
		                 destinationOffset:pCopyDesc.targetBufferOffset
		                              size:pCopyDesc.sourceBufferRegion.size];

		[blitCommandEncoder endEncoding];

	}

	void MetalGPUBuffer::UpdateSubDataUpload( void * pCommandObject, const GPUBufferSubDataUploadDesc & pUploadDesc )
	{
		auto * metalCommandList = reinterpret_cast<MetalCommandList *>( pCommandObject );
		if( !metalCommandList->CheckCommandClassSupport( ECommandQueueClass::Transfer ) )
		{
			throw 0;
		}

		id<MTLBuffer> uploadBuffer = [mMTLDevice newBufferWithBytes:pUploadDesc.inputDataDesc.pointer
									                         length:pUploadDesc.inputDataDesc.size
									                        options:MTLResourceStorageModePrivate | MTLResourceCpuCacheModeDefaultCache];

		id<MTLBlitCommandEncoder> blitCommandEncoder = [metalCommandList->mMTLCommandBuffer blitCommandEncoder];

		[blitCommandEncoder copyFromBuffer:uploadBuffer
		                      sourceOffset:0
		                          toBuffer:mMTLBuffer
		                 destinationOffset:pUploadDesc.bufferRegion.offset
		                              size:pUploadDesc.bufferRegion.size];

		[blitCommandEncoder endEncoding];
	}

	bool MetalGPUBuffer::ValidateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode )
	{
		return true;
	}

	id<MTLCommandBuffer> MetalGPUBuffer::GetMTLCommandBuffer( void * pCommandObject )
	{
		return reinterpret_cast<MetalCommandList *>( pCommandObject )->mMTLCommandBuffer;
	}

}
