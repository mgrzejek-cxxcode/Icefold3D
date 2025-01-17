
#include "MTLGpuBuffer.h"
#include "../MTL_commandList.h"
#include "../MTLUDevice.h"

#include <Metal/MTLDevice.h>

namespace Ic3::Graphics::GCI
{

	MetalGpuBuffer::MetalGpuBuffer(
			MetalGpuDevice & pGpuDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GpuBufferProperties & pBufferProperties,
			id<MTLBuffer> pMTLBuffer )
	: GpuBuffer( pGpuDevice, pResourceMemory, pBufferProperties )
	, mMTLDevice( [pMTLBuffer device] )
	, mMTLBuffer( pMTLBuffer )
	{}

	MetalGpuBuffer::~MetalGpuBuffer() = default;

	MetalGpuBufferHandle MetalGpuBuffer::Create( MetalGpuDevice & pGpuDevice, const GpuBufferCreateInfo & pCreateInfo )
	{}

	bool MetalGpuBuffer::MapRegion( void *, const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode )
	{
		auto * baseBufferPointer = [mMTLBuffer contents];
		if( !baseBufferPointer )
		{
			return false;
		}

		ResourceMappedMemory mappedMemoryInfo;
		mappedMemoryInfo.pointer = baseBufferPointer;
		mappedMemoryInfo.mappedRegion = pRegion;
		mappedMemoryInfo.memoryMapFlags = static_cast<EGpuMemoryMapFlags>( pMapMode );
		SetMappedMemory( mappedMemoryInfo );

		return true;
	}

	void MetalGpuBuffer::Unmap( void * )
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

	void MetalGpuBuffer::FlushMappedRegion( void *, const GpuMemoryRegion & pRegion )
	{
		if( const auto & mappedMemory = GetMappedMemory() )
		{
			NSRange updateRange;
			updateRange.location = cppx::numeric_cast<NSUInteger>( mappedMemory.mappedRegion.offset );
			updateRange.length = cppx::numeric_cast<NSUInteger>( mappedMemory.mappedRegion.size );

			[mMTLBuffer didModifyRange:updateRange];
		}
	}

	void MetalGpuBuffer::InvalidateRegion( void * pCommandObject, const GpuMemoryRegion & pRegion )
	{
	}

	void MetalGpuBuffer::UpdateSubDataCopy( void * pCommandObject, GpuBuffer & pSrcBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc )
	{
		auto * metalCommandList = reinterpret_cast<MetalCommandList *>( pCommandObject );
		if( !metalCommandList->CheckCommandClassSupport( ECommandQueueClass::Transfer ) )
		{
			throw 0;
		}

		auto * sourceMTLBuffer = pSrcBuffer.QueryInterface<MetalGpuBuffer>()->mMTLBuffer;

		id<MTLBlitCommandEncoder> blitCommandEncoder = [metalCommandList->mMTLCommandBuffer blitCommandEncoder];

		[blitCommandEncoder copyFromBuffer:sourceMTLBuffer
		                      sourceOffset:pCopyDesc.sourceBufferRegion.offset
		                          toBuffer:mMTLBuffer
		                 destinationOffset:pCopyDesc.targetBufferOffset
		                              size:pCopyDesc.sourceBufferRegion.size];

		[blitCommandEncoder endEncoding];

	}

	void MetalGpuBuffer::UpdateSubDataUpload( void * pCommandObject, const GpuBufferSubDataUploadDesc & pUploadDesc )
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

	bool MetalGpuBuffer::ValidateMapRequest( const GpuMemoryRegion & pRegion, const EGpuMemoryMapMode & pMapMode )
	{
		return true;
	}

	id<MTLCommandBuffer> MetalGpuBuffer::GetMTLCommandBuffer( void * pCommandObject )
	{
		return reinterpret_cast<MetalCommandList *>( pCommandObject )->mMTLCommandBuffer;
	}

}
