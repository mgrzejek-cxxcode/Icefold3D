
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_BUFFER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_BUFFER_H__

#include "../Objects/GLBufferObject.h"
#include <Ic3/Graphics/GCI/Resources/GPUBuffer.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( GLGPUBuffer );

	class GLGPUBuffer : public GPUBuffer
	{
		friend class GLCommandContext;

	public:
		GLBufferObjectHandle const mGLBufferObject;

	public:
		GLGPUBuffer(
			GLGPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GPUBufferProperties & pBufferProperties,
			GLBufferObjectHandle pGLBufferObject );

		virtual ~GLGPUBuffer();

		static GLGPUBufferHandle createInstance( GLGPUDevice & pGPUDevice, const GPUBufferCreateInfo & pCreateInfo );

	protected:
		static bool validateBufferCreateInfo( GPUBufferCreateInfo & pCreateInfo );

		virtual bool mapRegion( void * pCommandObject, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode ) override;

		virtual void unmap( void * pCommandObject ) override;

		virtual void flushMappedRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) override;

		virtual void invalidateRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) override;

		virtual void updateSubDataCopy( void * pCommandObject, GPUBuffer & pSrcBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc ) override;

		virtual void updateSubDataUpload( void * pCommandObject, const GPUBufferSubDataUploadDesc & pUploadDesc ) override;
		
		virtual bool validateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_BUFFER_H__
