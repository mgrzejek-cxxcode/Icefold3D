
#include "GLGPUBuffer.h"
#include "../GLAPITranslationLayer.h"
#include "../GLGPUDevice.h"

namespace Ic3::Graphics::GCI
{

#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
	// OpenGL ES 3.1 exposes neither the immutable storage API (glBufferStorage), nor the persistent mapping
	// (as a natural consequence of the former). Thus, explicit coherency/persistence flags are not used.
	static constexpr uint32 sSupportedGPUMemoryFlags = E_GPU_MEMORY_ACCESS_MASK_CPU_READ_WRITE | E_GPU_MEMORY_ACCESS_MASK_GPU_READ_WRITE;
#else
	// Core supports full set of features, including immutable storage, persistent mapping and explicit flushes.
	static constexpr uint32 sSupportedGPUMemoryFlags =
			eGPUMemoryAccessMaskCPUReadWrite |
			eGPUMemoryAccessMaskGPUReadWrite |
			eGPUMemoryHeapPropertyFlagCPUCoherentBit |
			eGPUMemoryHeapPropertyFlagGPUCoherentBit |
			eGPUMemoryHeapPropertyFlagPersistentMapBit;
#endif


	GLGPUBuffer::GLGPUBuffer(
			GLGPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GPUBufferProperties & pBufferProperties,
			GLBufferObjectHandle pGLBufferObject )
	: GPUBuffer( pGPUDevice, pResourceMemory, pBufferProperties )
	, mGLBufferObject( std::move( pGLBufferObject ) )
	{}

	GLGPUBuffer::~GLGPUBuffer() = default;

	GLGPUBufferHandle GLGPUBuffer::CreateInstance( GLGPUDevice & pGPUDevice, const GPUBufferCreateInfo & pCreateInfo )
	{
		auto createInfo = pCreateInfo;
		if( !ValidateBufferCreateInfo( createInfo ) )
		{
			return nullptr;
		}

		const auto initialBufferTarget = RCU::GetGPUBufferDefaultTargetFromBindFlags( createInfo.resourceFlags );
		if( initialBufferTarget == EGPUBufferTarget::Unknown )
		{
			return nullptr;
		}

		GLBufferCreateInfo openglCreateInfo;
		openglCreateInfo.bindTarget = ATL::GLTranslateBufferBindTarget( initialBufferTarget );
		openglCreateInfo.size = static_cast<GLuint>( createInfo.bufferSize );
		openglCreateInfo.resourceFlags = createInfo.resourceFlags;
		openglCreateInfo.memoryFlags = createInfo.memoryFlags;
		openglCreateInfo.initDataDesc = pCreateInfo.initDataDesc;

		GLBufferObjectHandle openglBufferObject = nullptr;
		if( pGPUDevice.IsCompatibilityDevice() )
		{
			openglBufferObject = GLBufferObject::CreateCompat( openglCreateInfo );
			Ic3DebugAssert( openglBufferObject );
		}
		else
		{
			openglBufferObject = GLBufferObject::CreateCore( openglCreateInfo );
			Ic3DebugAssert( openglBufferObject );
		}

		GPUBufferProperties bufferProperties;
		bufferProperties.byteSize = openglBufferObject->QuerySize();
		bufferProperties.resourceFlags = createInfo.resourceFlags;

		ResourceMemoryInfo bufferMemoryInfo;
		bufferMemoryInfo.sourceHeapRegion.offset = 0;
		bufferMemoryInfo.sourceHeapRegion.size = bufferProperties.byteSize;
		bufferMemoryInfo.baseAlignment = createInfo.memoryBaseAlignment;
		bufferMemoryInfo.memoryFlags = createInfo.memoryFlags;

		auto openglBuffer = CreateDynamicObject<GLGPUBuffer>(
				pGPUDevice,
				bufferMemoryInfo,
				bufferProperties,
				std::move( openglBufferObject ) );

		return openglBuffer;
	}

	bool GLGPUBuffer::ValidateBufferCreateInfo( GPUBufferCreateInfo & pCreateInfo )
	{
		if( !GPUBuffer::ValidateBufferCreateInfo( pCreateInfo ) )
		{
			return false;
		}

		// Unset all flags which are not supported by the current platform.
		pCreateInfo.memoryFlags = pCreateInfo.memoryFlags & sSupportedGPUMemoryFlags;

		return true;
	}

	bool GLGPUBuffer::MapRegion( void * , const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode )
	{
		void * mappedMemoryPtr = nullptr;

		if( mGLBufferObject->IsMappedPersistent() )
		{
			if( !mResourceMemory.memoryFlags.is_set( eGPUMemoryHeapPropertyFlagCPUCoherentBit ) )
			{
				glMemoryBarrier( GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT );
				Ic3OpenGLHandleLastError();
			}
			mappedMemoryPtr = mGLBufferObject->GetPersistentMapPtr();
		}
		else
		{
			const auto openglMapFlags = ATL::GLTranslateBufferMapFlags( pMapMode, mResourceMemory.memoryFlags );
			if( mGLBufferObject->Map( pRegion.offset, pRegion.size, openglMapFlags ) )
			{
				mappedMemoryPtr = mGLBufferObject->QueryMappedPtr();
			}
		}

		if( !mappedMemoryPtr )
		{
			return false;
		}

		ResourceMappedMemory mappedMemoryInfo;
		mappedMemoryInfo.pointer = mappedMemoryPtr;
		mappedMemoryInfo.mappedRegion = pRegion;
		mappedMemoryInfo.memoryMapFlags = static_cast<EGPUMemoryMapFlags>( pMapMode );
		SetMappedMemory( mappedMemoryInfo );

		return true;
	}

	void GLGPUBuffer::Unmap( void * )
	{
		if( const auto & mappedMemory = GetMappedMemory() )
		{
			if( mGLBufferObject->IsMappedPersistent() )
			{
				if( !mResourceMemory.memoryFlags.is_set( eGPUMemoryHeapPropertyFlagCPUCoherentBit ) )
				{
					mGLBufferObject->FlushMappedRegion( mappedMemory.mappedRegion.offset, mappedMemory.mappedRegion.size );
				}
			}
			else
			{
				mGLBufferObject->Unmap();
			}
			ResetMappedMemory();
		}
	}

	void GLGPUBuffer::FlushMappedRegion( void * , const GPUMemoryRegion & pRegion )
	{
		mGLBufferObject->FlushMappedRegion( pRegion.offset, pRegion.size );
	}

	void GLGPUBuffer::InvalidateRegion( void * , const GPUMemoryRegion & pRegion )
	{
		mGLBufferObject->InvalidateRegion( pRegion.offset, pRegion.size );
	}

	void GLGPUBuffer::UpdateSubDataCopy( void * , GPUBuffer & pSrcBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc )
	{
		auto * openglSourceBuffer = pSrcBuffer.QueryInterface<GLGPUBuffer>();
		if( !mGPUDevice.QueryInterface<GLGPUDevice>()->IsCompatibilityDevice() )
		{
			mGLBufferObject->UpdateCopyInvalidate( *openglSourceBuffer->mGLBufferObject, pCopyDesc );
		}
		else
		{
			mGLBufferObject->UpdateCopyOrphan( *openglSourceBuffer->mGLBufferObject, pCopyDesc );
		}
	}

	void GLGPUBuffer::UpdateSubDataUpload( void * , const GPUBufferSubDataUploadDesc & pUploadDesc )
	{
		if( !mGPUDevice.QueryInterface<GLGPUDevice>()->IsCompatibilityDevice() )
		{
			mGLBufferObject->UpdateUploadInvalidate( pUploadDesc );
		}
		else
		{
			mGLBufferObject->UpdateUploadOrphan( pUploadDesc );
		}
	}

	bool GLGPUBuffer::ValidateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode )
	{
		return GPUBuffer::ValidateMapRequest( pRegion, pMapMode );
	}

} // namespace Ic3::Graphics::GCI
