
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_GPU_BUFFER_H__
#define __IC3_GRAPHICS_HW3D_GLC_GPU_BUFFER_H__

#include "../Objects/GLBufferObject.h"
#include <Ic3/Graphics/GCI/Resources/GPUBuffer.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( GLGPUBuffer );

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

		static GLGPUBufferHandle CreateInstance( GLGPUDevice & pGPUDevice, const GPUBufferCreateInfo & pCreateInfo );

	protected:
		static bool ValidateBufferCreateInfo( GPUBufferCreateInfo & pCreateInfo );

		virtual bool MapRegion( void * pCommandObject, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode ) override;

		virtual void Unmap( void * pCommandObject ) override;

		virtual void FlushMappedRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) override;

		virtual void InvalidateRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) override;

		virtual void UpdateSubDataCopy( void * pCommandObject, GPUBuffer & pSrcBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc ) override;

		virtual void UpdateSubDataUpload( void * pCommandObject, const GPUBufferSubDataUploadDesc & pUploadDesc ) override;
		
		virtual bool ValidateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_GPU_BUFFER_H__
