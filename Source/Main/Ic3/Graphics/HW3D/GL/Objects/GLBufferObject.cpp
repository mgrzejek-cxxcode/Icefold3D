
#include "GLBufferObject.h"
#include "../GLAPITranslationLayer.h"

namespace Ic3::Graphics::GCI
{

	GLBufferObject::GLBufferObject( GLuint pHandle, const GLBufferCreateInfo & pGLCreateInfo )
	: GLObject( GLObjectBaseType::Buffer, pHandle )
	, mGLBufferBindTarget( pGLCreateInfo.bindTarget )
	, mSize( pGLCreateInfo.size )
	{}

	GLBufferObject::~GLBufferObject() = default;

	GLBufferObjectHandle GLBufferObject::createCore( const GLBufferCreateInfo & pGLCreateInfo )
	{
		GLuint bufferHandle = 0;

		glGenBuffers( 1, &bufferHandle );
		ic3OpenGLHandleLastError();

		glBindBuffer( pGLCreateInfo.bindTarget, bufferHandle );
		ic3OpenGLHandleLastError();

		GLBufferObjectHandle openglBufferObject{ new GLBufferObject( bufferHandle, pGLCreateInfo ) };
		if( !openglBufferObject->initializeCore( pGLCreateInfo ) )
		{
			return nullptr;
		}

		return openglBufferObject;
	}

	GLBufferObjectHandle GLBufferObject::createCompat( const GLBufferCreateInfo & pGLCreateInfo )
	{
		GLuint bufferHandle = 0;

		glGenBuffers( 1, &bufferHandle );
		ic3OpenGLHandleLastError();

		glBindBuffer( pGLCreateInfo.bindTarget, bufferHandle );
		ic3OpenGLHandleLastError();

		GLBufferObjectHandle openglBufferObject{ new GLBufferObject( bufferHandle, pGLCreateInfo ) };
		if( !openglBufferObject->initializeCompat( pGLCreateInfo ) )
		{
			return nullptr;
		}

		return openglBufferObject;
	}

	bool GLBufferObject::release()
	{
		glDeleteBuffers( 1, &mGLHandle );
		ic3OpenGLHandleLastError();

		return true;
	}

	bool GLBufferObject::validateHandle() const
	{
		auto isBuffer = glIsBuffer( mGLHandle );
		ic3OpenGLHandleLastError();

		return isBuffer != GL_FALSE;
	}

	bool GLBufferObject::queryIsMapped( GLenum pActiveBindTarget ) const
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		GLint64 mappedState = 0;
		glGetBufferParameteri64v( bufferBindTarget, GL_BUFFER_MAPPED, &mappedState );
		ic3OpenGLHandleLastError();

		return mappedState != GL_FALSE;
	}

	byte * GLBufferObject::queryMappedPtr( GLenum pActiveBindTarget ) const
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		void * mappedMemoryPtr = nullptr;
		glGetBufferPointerv( bufferBindTarget, GL_BUFFER_MAP_POINTER, &mappedMemoryPtr );
		ic3OpenGLHandleLastError();

		return reinterpret_cast<byte *>( mappedMemoryPtr );
	}

	MemoryRegion GLBufferObject::queryMappedRegion( GLenum pActiveBindTarget ) const
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		GLint64 mapOffset = 0;
		glGetBufferParameteri64v( bufferBindTarget, GL_BUFFER_MAP_OFFSET, &mapOffset );
		ic3OpenGLHandleLastError();

		GLint64 mapLength = 0;
		glGetBufferParameteri64v( bufferBindTarget, GL_BUFFER_MAP_LENGTH, &mapLength );
		ic3OpenGLHandleLastError();

		MemoryRegion mappedRegion;
		mappedRegion.offset = static_cast<memory_size_t>( mapOffset );
		mappedRegion.size = static_cast<memory_size_t>( mapLength );

		return mappedRegion;
	}

	memory_size_t GLBufferObject::querySize( GLenum pActiveBindTarget ) const
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		GLint64 bufferSize = 0;
		glGetBufferParameteri64v( bufferBindTarget, GL_BUFFER_SIZE, &bufferSize );
		ic3OpenGLHandleLastError();

		return static_cast<memory_size_t>( bufferSize );
	}

	bool GLBufferObject::isMappedPersistent() const
	{
		return _persistentMapPointer != nullptr;
	}

	void * GLBufferObject::getPersistentMapPtr() const
	{
		return _persistentMapPointer;
	}

	bool GLBufferObject::map( gpu_memory_size_t pOffset, gpu_memory_size_t pLength, GLenum pFlags, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		void * mapPointer = nullptr;
		glGetBufferPointerv( bufferBindTarget, GL_BUFFER_MAP_POINTER, &mapPointer );
		ic3OpenGLHandleLastError();

		if( mapPointer != nullptr )
		{
			ic3DebugInterrupt();
		}

		glMapBufferRange(
			bufferBindTarget,
			numeric_cast<GLintptr>( pOffset ),
			numeric_cast<GLsizeiptr>( pLength ),
			pFlags );
		ic3OpenGLHandleLastError();

		return queryIsMapped();
	}

	bool GLBufferObject::mapPersistent( GLenum pFlags, GLenum pActiveBindTarget )
	{
	#if( ICFGX_GL_FEATURE_SUPPORT_BUFFER_PERSISTENT_MAP )
		// Unset all flags which may be redundant and/or invalid (like INVALIDATE_BUFFER_RANGE or UNSYNCHRONIZED).
		pFlags &= ( GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );
		// Make sure a persistent version of the mapping will be requested.
		pFlags |= GL_MAP_PERSISTENT_BIT;
		// Map the buffer persistently
		return map( 0, mSize, pFlags );
    #else
		return false;
    #endif
	}

	void GLBufferObject::unmap( GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		glUnmapBuffer( bufferBindTarget );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::flushMappedRegion( gpu_memory_size_t pOffset, gpu_memory_size_t pLength, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		void * mapPointer = nullptr;
		glGetBufferPointerv( bufferBindTarget, GL_BUFFER_MAP_POINTER, &mapPointer );
		ic3OpenGLHandleLastError();

		ic3DebugAssert( mapPointer );

		glFlushMappedBufferRange(
			bufferBindTarget,
			numeric_cast<GLintptr>( pOffset ),
			numeric_cast<GLsizeiptr>( pLength ) );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::invalidateRegion( gpu_memory_size_t pOffset, gpu_memory_size_t pLength, GLenum pActiveBindTarget )
	{
	#if( ICFGX_GL_PLATFORM_TYPE == ICFGX_GL_PLATFORM_TYPE_ES )
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		glBufferSubData( bufferBindTarget, pOffset, pLength, nullptr );
		ic3OpenGLHandleLastError();
	#else
		glInvalidateBufferSubData(
			mGLHandle,
			numeric_cast<GLintptr>( pOffset ),
			numeric_cast<GLsizeiptr>( pLength ) );
		ic3OpenGLHandleLastError();
	#endif
	}

	void GLBufferObject::updateCopyInvalidate( GLBufferObject & pSrcBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		// If we copy data into the whole buffer, give driver a hint to invalidate the storage.
		// For ES, use storage orphaning since glInvalidateBuffer() is not available there.
		if( pCopyDesc.flags.isSet( E_GPU_BUFFER_DATA_COPY_FLAG_MODE_INVALIDATE_BIT ) )
		{
			if( pCopyDesc.sourceBufferRegion.size == mSize )
			{
				glInvalidateBufferData( mGLHandle );
				ic3OpenGLHandleLastError();
			}
			else
			{
				glInvalidateBufferSubData(
					mGLHandle,
					numeric_cast<GLintptr>( pCopyDesc.sourceBufferRegion.offset ),
			        numeric_cast<GLsizeiptr>( pCopyDesc.sourceBufferRegion.size ) );
				ic3OpenGLHandleLastError();
			}
		}

		glBindBuffer( GL_COPY_READ_BUFFER, pSrcBuffer.mGLHandle );
		ic3OpenGLHandleLastError();

		glCopyBufferSubData(
			GL_COPY_READ_BUFFER,
			bufferBindTarget,
			numeric_cast<GLintptr>( pCopyDesc.sourceBufferRegion.offset ),
			numeric_cast<GLintptr>( pCopyDesc.targetBufferOffset ),
			numeric_cast<GLsizeiptr>( pCopyDesc.sourceBufferRegion.size ) );
		ic3OpenGLHandleLastError();

		glBindBuffer( GL_COPY_READ_BUFFER, 0 );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::updateCopyOrphan( GLBufferObject & pSrcBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		// If we copy data into the whole buffer, give driver a hint to invalidate the storage.
		// For ES, use storage orphaning since glInvalidateBuffer() is not available there.
		if( pCopyDesc.flags.isSet( E_GPU_BUFFER_DATA_COPY_FLAG_MODE_INVALIDATE_BIT ) && ( pCopyDesc.sourceBufferRegion.size == mSize ) )
		{
			GLint bufferUsage = 0;
			glGetBufferParameteriv( bufferBindTarget, GL_BUFFER_USAGE, &bufferUsage );
			ic3OpenGLHandleLastError();

			glBufferData( bufferBindTarget, numeric_cast<GLsizeiptr>( mSize ), nullptr, bufferUsage );
			ic3OpenGLHandleLastError();
		}

		glBindBuffer( GL_COPY_READ_BUFFER, pSrcBuffer.mGLHandle );
		ic3OpenGLHandleLastError();

		glCopyBufferSubData(
			GL_COPY_READ_BUFFER,
			bufferBindTarget,
			numeric_cast<GLintptr>( pCopyDesc.sourceBufferRegion.offset ),
			numeric_cast<GLintptr>( pCopyDesc.targetBufferOffset ),
			numeric_cast<GLsizeiptr>( pCopyDesc.sourceBufferRegion.size ) );
		ic3OpenGLHandleLastError();

		glBindBuffer( GL_COPY_READ_BUFFER, 0 );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::updateUploadInvalidate( const GPUBufferSubDataUploadDesc & pUploadDesc, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		if( pUploadDesc.flags.isSet( E_GPU_BUFFER_DATA_COPY_FLAG_MODE_INVALIDATE_BIT ) )
		{
			if( pUploadDesc.bufferRegion.size == mSize )
			{
				glInvalidateBufferData( mGLHandle );
				ic3OpenGLHandleLastError();
			}
			else
			{
				glInvalidateBufferSubData(
					mGLHandle,
					numeric_cast<GLintptr>( pUploadDesc.bufferRegion.offset ),
					numeric_cast<GLsizeiptr>( pUploadDesc.bufferRegion.size ) );
				ic3OpenGLHandleLastError();
			}
		}

		glBufferSubData(
			bufferBindTarget,
			numeric_cast<GLintptr>( pUploadDesc.bufferRegion.offset ),
			numeric_cast<GLsizeiptr>( pUploadDesc.inputDataDesc.size ),
			pUploadDesc.inputDataDesc.pointer );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::updateUploadOrphan( const GPUBufferSubDataUploadDesc & pUploadDesc, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		// If we copy data into the whole buffer, give driver a hint to invalidate the storage.
		// For ES, use storage orphaning since glInvalidateBuffer() is not available there.
		if( pUploadDesc.flags.isSet( E_GPU_BUFFER_DATA_COPY_FLAG_MODE_INVALIDATE_BIT ) && ( pUploadDesc.bufferRegion.size == mSize ) )
		{
			GLint bufferUsage = 0;
			glGetBufferParameteriv( bufferBindTarget, GL_BUFFER_USAGE, &bufferUsage );
			ic3OpenGLHandleLastError();

			glBufferData( bufferBindTarget, numeric_cast<GLsizeiptr>( mSize ), pUploadDesc.inputDataDesc.pointer, bufferUsage );
			ic3OpenGLHandleLastError();
		}

		glBufferSubData(
			bufferBindTarget,
			numeric_cast<GLintptr>( pUploadDesc.bufferRegion.offset ),
			numeric_cast<GLsizeiptr>( pUploadDesc.inputDataDesc.size ),
			pUploadDesc.inputDataDesc.pointer );
		ic3OpenGLHandleLastError();
	}

	bool GLBufferObject::initializeCore( const GLBufferCreateInfo & pGLCreateInfo )
	{
		const bool nonEmptyInitData = pGLCreateInfo.initDataDesc ? true : false;
		const bool copyDataOnInit = nonEmptyInitData && ( pGLCreateInfo.initDataDesc.size == pGLCreateInfo.size );

		const auto storageInitFlags = ATL::chooseGLBufferStorageFlags( pGLCreateInfo.bindTarget, pGLCreateInfo.resourceFlags, pGLCreateInfo.memoryFlags );

		glBufferStorage(
			pGLCreateInfo.bindTarget,
			numeric_cast<GLsizeiptr>( pGLCreateInfo.size ),
			copyDataOnInit ? pGLCreateInfo.initDataDesc.pointer : nullptr,
			storageInitFlags );
		ic3OpenGLHandleLastError();

		if( !copyDataOnInit && nonEmptyInitData )
		{
			const auto initDataSize = getMinOf( pGLCreateInfo.size, pGLCreateInfo.initDataDesc.size );

			glBufferSubData(
				pGLCreateInfo.bindTarget,
				0,
				numeric_cast<GLsizeiptr>( initDataSize ),
		        pGLCreateInfo.initDataDesc.pointer );
			ic3OpenGLHandleLastError();
		}

		if( pGLCreateInfo.memoryFlags.isSet( E_GPU_MEMORY_HEAP_PROPERTY_FLAG_PERSISTENT_MAP_BIT ) )
		{
			// Map with the access specified for the buffer storage.
			const auto mapMode =
		        static_cast<EGPUMemoryMapMode>( static_cast<uint32>( pGLCreateInfo.memoryFlags & E_GPU_MEMORY_ACCESS_MASK_CPU_ALL ) );

			auto openglMapFlags = ATL::translateGLBufferMapFlags( mapMode, pGLCreateInfo.memoryFlags );
			if( mapPersistent( openglMapFlags ) )
			{
				_persistentMapPointer = queryMappedPtr();
			}
		}

		return true;
	}

	bool GLBufferObject::initializeCompat( const GLBufferCreateInfo & pGLCreateInfo )
	{
		const bool nonEmptyInitData = pGLCreateInfo.initDataDesc ? true : false;
		const bool copyDataOnInit = nonEmptyInitData && ( pGLCreateInfo.initDataDesc.size == pGLCreateInfo.size );

		const auto usagePolicy = ATL::chooseGLBufferUsagePolicy( pGLCreateInfo.bindTarget, pGLCreateInfo.resourceFlags );

		glBufferData(
			pGLCreateInfo.bindTarget,
			numeric_cast<GLsizeiptr>( pGLCreateInfo.size ),
			copyDataOnInit ? pGLCreateInfo.initDataDesc.pointer : nullptr,
			usagePolicy );
		ic3OpenGLHandleLastError();

		if( !copyDataOnInit && nonEmptyInitData )
		{
			const auto initDataSize = getMinOf( pGLCreateInfo.size, pGLCreateInfo.initDataDesc.size );

			glBufferSubData(
				pGLCreateInfo.bindTarget,
				0,
				numeric_cast<GLsizeiptr>( initDataSize ),
				pGLCreateInfo.initDataDesc.pointer );
			ic3OpenGLHandleLastError();
		}

		return true;
	}

	GLenum GLBufferObject::checkActiveBindTarget( GLenum pBindTarget ) const
	{
		if( pBindTarget == 0 )
		{
			pBindTarget = mGLBufferBindTarget;

			glBindBuffer( pBindTarget, mGLHandle );
			ic3OpenGLHandleLastError();
		}

		return pBindTarget;
	}

} // namespace Ic3::Graphics::GCI
