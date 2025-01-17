
#include "GLBufferObject.h"
#include "../GLApiTranslationLayer.h"

namespace Ic3::Graphics::GCI
{

	GLBufferObject::GLBufferObject( GLuint pHandle, const GLBufferCreateInfo & pGLCreateInfo )
	: GLObject( GLObjectBaseType::Buffer, pHandle )
	, mGLBufferBindTarget( pGLCreateInfo.bindTarget )
	, size( pGLCreateInfo.size )
	{}

	GLBufferObject::~GLBufferObject() = default;

	GLBufferObjectHandle GLBufferObject::CreateCore( const GLBufferCreateInfo & pGLCreateInfo )
	{
		GLuint bufferHandle = 0;

		glGenBuffers( 1, &bufferHandle );
		ic3OpenGLHandleLastError();

		glBindBuffer( pGLCreateInfo.bindTarget, bufferHandle );
		ic3OpenGLHandleLastError();

		GLBufferObjectHandle openglBufferObject{ new GLBufferObject( bufferHandle, pGLCreateInfo ) };
		if( !openglBufferObject->InitializeCore( pGLCreateInfo ) )
		{
			return nullptr;
		}

		return openglBufferObject;
	}

	GLBufferObjectHandle GLBufferObject::CreateCompat( const GLBufferCreateInfo & pGLCreateInfo )
	{
		GLuint bufferHandle = 0;

		glGenBuffers( 1, &bufferHandle );
		ic3OpenGLHandleLastError();

		glBindBuffer( pGLCreateInfo.bindTarget, bufferHandle );
		ic3OpenGLHandleLastError();

		GLBufferObjectHandle openglBufferObject{ new GLBufferObject( bufferHandle, pGLCreateInfo ) };
		if( !openglBufferObject->InitializeCompat( pGLCreateInfo ) )
		{
			return nullptr;
		}

		return openglBufferObject;
	}

	bool GLBufferObject::Release()
	{
		glDeleteBuffers( 1, &mGLHandle );
		ic3OpenGLHandleLastError();

		return true;
	}

	bool GLBufferObject::ValidateHandle() const
	{
		auto isBuffer = glIsBuffer( mGLHandle );
		ic3OpenGLHandleLastError();

		return isBuffer != GL_FALSE;
	}

	bool GLBufferObject::QueryIsMapped( GLenum pActiveBindTarget ) const
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		GLint64 mappedState = 0;
		glGetBufferParameteri64v( bufferBindTarget, GL_BUFFER_MAPPED, &mappedState );
		ic3OpenGLHandleLastError();

		return mappedState != GL_FALSE;
	}

	byte * GLBufferObject::QueryMappedPtr( GLenum pActiveBindTarget ) const
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		void * mappedMemoryPtr = nullptr;
		glGetBufferPointerv( bufferBindTarget, GL_BUFFER_MAP_POINTER, &mappedMemoryPtr );
		ic3OpenGLHandleLastError();

		return reinterpret_cast<byte *>( mappedMemoryPtr );
	}

	MemoryRegion GLBufferObject::QueryMappedRegion( GLenum pActiveBindTarget ) const
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

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

	memory_size_t GLBufferObject::QuerySize( GLenum pActiveBindTarget ) const
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		GLint64 bufferSize = 0;
		glGetBufferParameteri64v( bufferBindTarget, GL_BUFFER_SIZE, &bufferSize );
		ic3OpenGLHandleLastError();

		return static_cast<memory_size_t>( bufferSize );
	}

	bool GLBufferObject::IsMappedPersistent() const
	{
		return _persistentMapPointer != nullptr;
	}

	void * GLBufferObject::GetPersistentMapPtr() const
	{
		return _persistentMapPointer;
	}

	bool GLBufferObject::Map( gpu_memory_size_t pOffset, gpu_memory_size_t pLength, GLenum pFlags, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		void * mapPointer = nullptr;
		glGetBufferPointerv( bufferBindTarget, GL_BUFFER_MAP_POINTER, &mapPointer );
		ic3OpenGLHandleLastError();

		if( mapPointer != nullptr )
		{
			ic3DebugInterrupt();
		}

		glMapBufferRange(
			bufferBindTarget,
			cppx::numeric_cast<GLintptr>( pOffset ),
			cppx::numeric_cast<GLsizeiptr>( pLength ),
			pFlags );
		ic3OpenGLHandleLastError();

		return QueryIsMapped();
	}

	bool GLBufferObject::MapPersistent( GLenum pFlags, GLenum pActiveBindTarget )
	{
	#if( IC3_GX_GL_FEATURE_SUPPORT_BUFFER_PERSISTENT_MAP )
		// Unset all flags which may be redundant and/or invalid (like INVALIDATE_BUFFER_RANGE or UNSYNCHRONIZED).
		pFlags &= ( GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );
		// Make sure a persistent version of the mapping will be requested.
		pFlags |= GL_MAP_PERSISTENT_BIT;
		// Map the buffer persistently
		return Map( 0, size, pFlags );
    #else
		return false;
    #endif
	}

	void GLBufferObject::Unmap( GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		glUnmapBuffer( bufferBindTarget );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::FlushMappedRegion( gpu_memory_size_t pOffset, gpu_memory_size_t pLength, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		void * mapPointer = nullptr;
		glGetBufferPointerv( bufferBindTarget, GL_BUFFER_MAP_POINTER, &mapPointer );
		ic3OpenGLHandleLastError();

		ic3DebugAssert( mapPointer );

		glFlushMappedBufferRange(
			bufferBindTarget,
			cppx::numeric_cast<GLintptr>( pOffset ),
			cppx::numeric_cast<GLsizeiptr>( pLength ) );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::InvalidateRegion( gpu_memory_size_t pOffset, gpu_memory_size_t pLength, GLenum pActiveBindTarget )
	{
	#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		glBufferSubData( bufferBindTarget, pOffset, pLength, nullptr );
		ic3OpenGLHandleLastError();
	#else
		glInvalidateBufferSubData(
			mGLHandle,
			cppx::numeric_cast<GLintptr>( pOffset ),
			cppx::numeric_cast<GLsizeiptr>( pLength ) );
		ic3OpenGLHandleLastError();
	#endif
	}

	void GLBufferObject::UpdateCopyInvalidate( GLBufferObject & pSrcBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		// If we copy data into the whole buffer, give driver a hint to invalidate the storage.
		// For ES, use storage orphaning since glInvalidateBuffer() is not available there.
		if( pCopyDesc.flags.is_set( eGpuBufferDataCopyFlagModeInvalidateBit ) )
		{
			if( pCopyDesc.sourceBufferRegion.size == size )
			{
				glInvalidateBufferData( mGLHandle );
				ic3OpenGLHandleLastError();
			}
			else
			{
				glInvalidateBufferSubData(
					mGLHandle,
					cppx::numeric_cast<GLintptr>( pCopyDesc.sourceBufferRegion.offset ),
			        cppx::numeric_cast<GLsizeiptr>( pCopyDesc.sourceBufferRegion.size ) );
				ic3OpenGLHandleLastError();
			}
		}

		glBindBuffer( GL_COPY_READ_BUFFER, pSrcBuffer.mGLHandle );
		ic3OpenGLHandleLastError();

		glCopyBufferSubData(
			GL_COPY_READ_BUFFER,
			bufferBindTarget,
			cppx::numeric_cast<GLintptr>( pCopyDesc.sourceBufferRegion.offset ),
			cppx::numeric_cast<GLintptr>( pCopyDesc.targetBufferOffset ),
			cppx::numeric_cast<GLsizeiptr>( pCopyDesc.sourceBufferRegion.size ) );
		ic3OpenGLHandleLastError();

		glBindBuffer( GL_COPY_READ_BUFFER, 0 );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::UpdateCopyOrphan( GLBufferObject & pSrcBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		// If we copy data into the whole buffer, give driver a hint to invalidate the storage.
		// For ES, use storage orphaning since glInvalidateBuffer() is not available there.
		if( pCopyDesc.flags.is_set( eGpuBufferDataCopyFlagModeInvalidateBit ) && ( pCopyDesc.sourceBufferRegion.size == size ) )
		{
			GLint bufferUsage = 0;
			glGetBufferParameteriv( bufferBindTarget, GL_BUFFER_USAGE, &bufferUsage );
			ic3OpenGLHandleLastError();

			glBufferData( bufferBindTarget, cppx::numeric_cast<GLsizeiptr>( size ), nullptr, bufferUsage );
			ic3OpenGLHandleLastError();
		}

		glBindBuffer( GL_COPY_READ_BUFFER, pSrcBuffer.mGLHandle );
		ic3OpenGLHandleLastError();

		glCopyBufferSubData(
			GL_COPY_READ_BUFFER,
			bufferBindTarget,
			cppx::numeric_cast<GLintptr>( pCopyDesc.sourceBufferRegion.offset ),
			cppx::numeric_cast<GLintptr>( pCopyDesc.targetBufferOffset ),
			cppx::numeric_cast<GLsizeiptr>( pCopyDesc.sourceBufferRegion.size ) );
		ic3OpenGLHandleLastError();

		glBindBuffer( GL_COPY_READ_BUFFER, 0 );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::UpdateUploadInvalidate( const GpuBufferSubDataUploadDesc & pUploadDesc, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		if( pUploadDesc.flags.is_set( eGpuBufferDataCopyFlagModeInvalidateBit ) )
		{
			if( pUploadDesc.bufferRegion.size == size )
			{
				glInvalidateBufferData( mGLHandle );
				ic3OpenGLHandleLastError();
			}
			else
			{
				glInvalidateBufferSubData(
					mGLHandle,
					cppx::numeric_cast<GLintptr>( pUploadDesc.bufferRegion.offset ),
					cppx::numeric_cast<GLsizeiptr>( pUploadDesc.bufferRegion.size ) );
				ic3OpenGLHandleLastError();
			}
		}

		glBufferSubData(
			bufferBindTarget,
			cppx::numeric_cast<GLintptr>( pUploadDesc.bufferRegion.offset ),
			cppx::numeric_cast<GLsizeiptr>( pUploadDesc.inputDataDesc.size ),
			pUploadDesc.inputDataDesc.pointer );
		ic3OpenGLHandleLastError();
	}

	void GLBufferObject::UpdateUploadOrphan( const GpuBufferSubDataUploadDesc & pUploadDesc, GLenum pActiveBindTarget )
	{
		auto bufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		// If we copy data into the whole buffer, give driver a hint to invalidate the storage.
		// For ES, use storage orphaning since glInvalidateBuffer() is not available there.
		if( pUploadDesc.flags.is_set( eGpuBufferDataCopyFlagModeInvalidateBit ) && ( pUploadDesc.bufferRegion.size == size ) )
		{
			GLint bufferUsage = 0;
			glGetBufferParameteriv( bufferBindTarget, GL_BUFFER_USAGE, &bufferUsage );
			ic3OpenGLHandleLastError();

			glBufferData( bufferBindTarget, cppx::numeric_cast<GLsizeiptr>( size ), pUploadDesc.inputDataDesc.pointer, bufferUsage );
			ic3OpenGLHandleLastError();
		}

		glBufferSubData(
			bufferBindTarget,
			cppx::numeric_cast<GLintptr>( pUploadDesc.bufferRegion.offset ),
			cppx::numeric_cast<GLsizeiptr>( pUploadDesc.inputDataDesc.size ),
			pUploadDesc.inputDataDesc.pointer );
		ic3OpenGLHandleLastError();
	}

	bool GLBufferObject::InitializeCore( const GLBufferCreateInfo & pGLCreateInfo )
	{
		const bool nonEmptyInitData = pGLCreateInfo.initDataDesc ? true : false;
		const bool copyDataOnInit = nonEmptyInitData && ( pGLCreateInfo.initDataDesc.size == pGLCreateInfo.size );

		const auto storageInitFlags = ATL::ChooseGLBufferStorageFlags( pGLCreateInfo.bindTarget, pGLCreateInfo.resourceFlags, pGLCreateInfo.memoryFlags );
		const auto * storageInitDataPtr = copyDataOnInit ? pGLCreateInfo.initDataDesc.pointer : nullptr;

		glBufferStorage(
			pGLCreateInfo.bindTarget,
			cppx::numeric_cast<GLsizeiptr>( pGLCreateInfo.size ),
			storageInitDataPtr,
			storageInitFlags );
		ic3OpenGLHandleLastError();

		if( !copyDataOnInit && nonEmptyInitData )
		{
			const auto initDataSize = cppx::get_min_of( pGLCreateInfo.size, pGLCreateInfo.initDataDesc.size );

			glBufferSubData(
				pGLCreateInfo.bindTarget,
				0,
				cppx::numeric_cast<GLsizeiptr>( initDataSize ),
		        pGLCreateInfo.initDataDesc.pointer );
			ic3OpenGLHandleLastError();
		}

		if( pGLCreateInfo.memoryFlags.is_set( eGpuMemoryHeapPropertyFlagPersistentMapBit ) )
		{
			// Map with the access specified for the buffer storage.
			const auto mapMode =
		        static_cast<EGpuMemoryMapMode>( static_cast<uint32>( pGLCreateInfo.memoryFlags & eGpuMemoryAccessMaskCpuAll ) );

			auto openglMapFlags = ATL::TranslateGLBufferMapFlags( mapMode, pGLCreateInfo.memoryFlags );
			if( MapPersistent( openglMapFlags ) )
			{
				_persistentMapPointer = QueryMappedPtr();
			}
		}

		return true;
	}

	bool GLBufferObject::InitializeCompat( const GLBufferCreateInfo & pGLCreateInfo )
	{
		const bool nonEmptyInitData = pGLCreateInfo.initDataDesc ? true : false;
		const bool copyDataOnInit = nonEmptyInitData && ( pGLCreateInfo.initDataDesc.size == pGLCreateInfo.size );

		const auto usagePolicy = ATL::ChooseGLBufferUsagePolicy( pGLCreateInfo.bindTarget, pGLCreateInfo.resourceFlags );

		glBufferData(
			pGLCreateInfo.bindTarget,
			cppx::numeric_cast<GLsizeiptr>( pGLCreateInfo.size ),
			copyDataOnInit ? pGLCreateInfo.initDataDesc.pointer : nullptr,
			usagePolicy );
		ic3OpenGLHandleLastError();

		if( !copyDataOnInit && nonEmptyInitData )
		{
			const auto initDataSize = cppx::get_min_of( pGLCreateInfo.size, pGLCreateInfo.initDataDesc.size );

			glBufferSubData(
				pGLCreateInfo.bindTarget,
				0,
				cppx::numeric_cast<GLsizeiptr>( initDataSize ),
				pGLCreateInfo.initDataDesc.pointer );
			ic3OpenGLHandleLastError();
		}

		return true;
	}

	GLenum GLBufferObject::CheckActiveBindTarget( GLenum pBindTarget ) const
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
