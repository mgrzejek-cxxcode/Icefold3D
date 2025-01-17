
#include "GLGpuBuffer.h"
#include "../GLApiTranslationLayer.h"
#include "../GLGpuDevice.h"

namespace Ic3::Graphics::GCI
{

#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
	// OpenGL ES 3.1 exposes neither the immutable storage API (glBufferStorage), nor the persistent mapping
	// (as a natural consequence of the former). Thus, explicit coherency/persistence flags are not used.
	static constexpr uint32 sSupportedGpuMemoryFlags = E_GPU_MEMORY_ACCESS_MASK_CPU_READ_WRITE | E_GPU_MEMORY_ACCESS_MASK_GPU_READ_WRITE;
#else
	// Core supports full set of features, including immutable storage, persistent mapping and explicit flushes.
	static constexpr uint32 sSupportedGpuMemoryFlags =
			eGpuMemoryAccessMaskCpuReadWrite |
			eGpuMemoryAccessMaskGpuReadWrite |
			eGpuMemoryHeapPropertyFlagCpuCoherentBit |
			eGpuMemoryHeapPropertyFlagGpuCoherentBit |
			eGpuMemoryHeapPropertyFlagPersistentMapBit;
#endif


	GLGpuBuffer::GLGpuBuffer(
			GLGpuDevice & pGpuDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GpuBufferProperties & pBufferProperties,
			GLBufferObjectHandle pGLBufferObject )
	: GpuBuffer( pGpuDevice, pResourceMemory, pBufferProperties )
	, mGLBufferObject( std::move( pGLBufferObject ) )
	{}

	GLGpuBuffer::~GLGpuBuffer() = default;

	GLGpuBufferHandle GLGpuBuffer::CreateInstance( GLGpuDevice & pGpuDevice, const GpuBufferCreateInfo & pCreateInfo )
	{
		auto createInfo = pCreateInfo;
		if( !ValidateBufferCreateInfo( createInfo ) )
		{
			return nullptr;
		}

		const auto initialBufferTarget = RCU::GetGpuBufferDefaultTargetFromBindFlags( createInfo.resourceFlags );
		if( initialBufferTarget == EGpuBufferTarget::Unknown )
		{
			return nullptr;
		}

		GLBufferCreateInfo openglCreateInfo;
		openglCreateInfo.bindTarget = ATL::TranslateGLBufferBindTarget( initialBufferTarget );
		openglCreateInfo.size = static_cast<GLuint>( createInfo.bufferSize );
		openglCreateInfo.resourceFlags = createInfo.resourceFlags;
		openglCreateInfo.memoryFlags = createInfo.memoryFlags;
		openglCreateInfo.initDataDesc = pCreateInfo.initDataDesc;

		GLBufferObjectHandle openglBufferObject = nullptr;
		if( pGpuDevice.IsCompatibilityDevice() )
		{
			openglBufferObject = GLBufferObject::CreateCompat( openglCreateInfo );
			ic3DebugAssert( openglBufferObject );
		}
		else
		{
			openglBufferObject = GLBufferObject::CreateCore( openglCreateInfo );
			ic3DebugAssert( openglBufferObject );
		}

		GpuBufferProperties bufferProperties;
		bufferProperties.byteSize = openglBufferObject->QuerySize();
		bufferProperties.resourceFlags = createInfo.resourceFlags;

		ResourceMemoryInfo bufferMemoryInfo;
		bufferMemoryInfo.sourceHeapRegion.offset = 0;
		bufferMemoryInfo.sourceHeapRegion.size = bufferProperties.byteSize;
		bufferMemoryInfo.baseAlignment = createInfo.memoryBaseAlignment;
		bufferMemoryInfo.memoryFlags = createInfo.memoryFlags;

		auto openglBuffer = CreateDynamicObject<GLGpuBuffer>(
				pGpuDevice,
				bufferMemoryInfo,
				bufferProperties,
				std::move( openglBufferObject ) );

		return openglBuffer;
	}

	bool GLGpuBuffer::ValidateBufferCreateInfo( GpuBufferCreateInfo & pCreateInfo )
	{
		if( !GpuBuffer::ValidateBufferCreateInfo( pCreateInfo ) )
		{
			return false;
		}

		// Unset all flags which are not supported by the current platform.
		pCreateInfo.memoryFlags = pCreateInfo.memoryFlags & sSupportedGpuMemoryFlags;

		return true;
	}

	bool GLGpuBuffer::MapRegion( void * , const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode )
	{
		void * mappedMemoryPtr = nullptr;

		if( mGLBufferObject->IsMappedPersistent() )
		{
			if( !mResourceMemory.memoryFlags.is_set( eGpuMemoryHeapPropertyFlagCpuCoherentBit ) )
			{
				glMemoryBarrier( GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT );
				ic3OpenGLHandleLastError();
			}
			mappedMemoryPtr = mGLBufferObject->GetPersistentMapPtr();
		}
		else
		{
			const auto openglMapFlags = ATL::TranslateGLBufferMapFlags( pMapMode, mResourceMemory.memoryFlags );
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
		mappedMemoryInfo.memoryMapFlags = static_cast<EGpuMemoryMapFlags>( pMapMode );
		SetMappedMemory( mappedMemoryInfo );

		return true;
	}

	void GLGpuBuffer::Unmap( void * )
	{
		if( const auto & mappedMemory = GetMappedMemory() )
		{
			if( mGLBufferObject->IsMappedPersistent() )
			{
				if( !mResourceMemory.memoryFlags.is_set( eGpuMemoryHeapPropertyFlagCpuCoherentBit ) )
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

	void GLGpuBuffer::FlushMappedRegion( void * , const GpuMemoryRegion & pRegion )
	{
		mGLBufferObject->FlushMappedRegion( pRegion.offset, pRegion.size );
	}

	void GLGpuBuffer::InvalidateRegion( void * , const GpuMemoryRegion & pRegion )
	{
		mGLBufferObject->InvalidateRegion( pRegion.offset, pRegion.size );
	}

	void GLGpuBuffer::UpdateSubDataCopy( void * , GpuBuffer & pSrcBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc )
	{
		auto * openglSourceBuffer = pSrcBuffer.QueryInterface<GLGpuBuffer>();
		if( !mGpuDevice.QueryInterface<GLGpuDevice>()->IsCompatibilityDevice() )
		{
			mGLBufferObject->UpdateCopyInvalidate( *openglSourceBuffer->mGLBufferObject, pCopyDesc );
		}
		else
		{
			mGLBufferObject->UpdateCopyOrphan( *openglSourceBuffer->mGLBufferObject, pCopyDesc );
		}
	}

	void GLGpuBuffer::UpdateSubDataUpload( void * , const GpuBufferSubDataUploadDesc & pUploadDesc )
	{
		if( !mGpuDevice.QueryInterface<GLGpuDevice>()->IsCompatibilityDevice() )
		{
			mGLBufferObject->UpdateUploadInvalidate( pUploadDesc );
		}
		else
		{
			mGLBufferObject->UpdateUploadOrphan( pUploadDesc );
		}
	}

	bool GLGpuBuffer::ValidateMapRequest( const GpuMemoryRegion & pRegion, const EGpuMemoryMapMode & pMapMode )
	{
		return GpuBuffer::ValidateMapRequest( pRegion, pMapMode );
	}

} // namespace Ic3::Graphics::GCI
