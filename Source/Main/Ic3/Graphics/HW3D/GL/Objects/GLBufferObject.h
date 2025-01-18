
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_BUFFER_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_BUFFER_OBJECT_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLBufferObject );

	struct GLBufferCreateInfo
	{
		GLenum bindTarget = 0;
		GLuint size = 0;
		cppx::bitmask<resource_flags_value_t> resourceFlags = 0;
		cppx::bitmask<EGPUMemoryFlags> memoryFlags = 0;
		GPUBufferInitDataDesc initDataDesc;
	};

	class GLBufferObject : public GLObject
	{
	public:
		GLenum const mGLBufferBindTarget = CX_GL_BIND_TARGET_UNKNOWN;
		GLuint const size;

		GLBufferObject( GLuint pHandle, const GLBufferCreateInfo & pGLCreateInfo );
		virtual ~GLBufferObject();

		virtual bool Release();
		virtual bool ValidateHandle() const;

		CPPX_ATTR_NO_DISCARD bool QueryIsMapped( GLenum pActiveBindTarget = 0 ) const;

		CPPX_ATTR_NO_DISCARD byte * QueryMappedPtr( GLenum pActiveBindTarget = 0 ) const;

		CPPX_ATTR_NO_DISCARD MemoryRegion QueryMappedRegion( GLenum pActiveBindTarget = 0 ) const;

		CPPX_ATTR_NO_DISCARD memory_size_t QuerySize( GLenum pActiveBindTarget = 0 ) const;

		CPPX_ATTR_NO_DISCARD bool IsMappedPersistent() const;

		CPPX_ATTR_NO_DISCARD void * GetPersistentMapPtr() const;

		bool Map( gpu_memory_size_t pOffset, gpu_memory_size_t pLength, GLenum pFlags, GLenum pActiveBindTarget = 0 );
		bool MapPersistent( GLenum pFlags, GLenum pActiveBindTarget = 0 );
		void Unmap( GLenum pActiveBindTarget = 0 );

		void FlushMappedRegion( gpu_memory_size_t pOffset, gpu_memory_size_t pLength, GLenum pActiveBindTarget = 0 );
		void InvalidateRegion( gpu_memory_size_t pOffset, gpu_memory_size_t pLength, GLenum pActiveBindTarget = 0 );

		void UpdateCopyInvalidate( GLBufferObject & pSrcBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget = 0 );
		void UpdateCopyOrphan( GLBufferObject & pSrcBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget = 0 );

		void UpdateUploadInvalidate( const GPUBufferSubDataUploadDesc & pUploadDesc, GLenum pActiveBindTarget = 0 );
		void UpdateUploadOrphan( const GPUBufferSubDataUploadDesc & pUploadDesc, GLenum pActiveBindTarget = 0 );

		static GLBufferObjectHandle CreateCore( const GLBufferCreateInfo & pGLCreateInfo );
		static GLBufferObjectHandle CreateCompat( const GLBufferCreateInfo & pGLCreateInfo );

	private:
		static GLBufferObjectHandle CreateBase( const GLBufferCreateInfo & pGLCreateInfo );

		GLenum CheckActiveBindTarget( GLenum pBindTarget ) const;

		bool InitializeCore( const GLBufferCreateInfo & pGLCreateInfo );

		bool InitializeCompat( const GLBufferCreateInfo & pGLCreateInfo );

	private:
		void * _persistentMapPointer = nullptr;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_BUFFER_OBJECT_H__
