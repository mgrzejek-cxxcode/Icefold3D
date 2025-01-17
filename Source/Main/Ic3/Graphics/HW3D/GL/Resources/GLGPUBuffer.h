
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_BUFFER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_BUFFER_H__

#include "../Objects/GLBufferObject.h"
#include <Ic3/Graphics/GCI/Resources/GpuBuffer.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( GLGpuBuffer );

	class GLGpuBuffer : public GpuBuffer
	{
		friend class GLCommandContext;

	public:
		GLBufferObjectHandle const mGLBufferObject;

	public:
		GLGpuBuffer(
			GLGpuDevice & pGpuDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GpuBufferProperties & pBufferProperties,
			GLBufferObjectHandle pGLBufferObject );

		virtual ~GLGpuBuffer();

		static GLGpuBufferHandle CreateInstance( GLGpuDevice & pGpuDevice, const GpuBufferCreateInfo & pCreateInfo );

	protected:
		static bool ValidateBufferCreateInfo( GpuBufferCreateInfo & pCreateInfo );

		virtual bool MapRegion( void * pCommandObject, const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode ) override;

		virtual void Unmap( void * pCommandObject ) override;

		virtual void FlushMappedRegion( void * pCommandObject, const GpuMemoryRegion & pRegion ) override;

		virtual void InvalidateRegion( void * pCommandObject, const GpuMemoryRegion & pRegion ) override;

		virtual void UpdateSubDataCopy( void * pCommandObject, GpuBuffer & pSrcBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc ) override;

		virtual void UpdateSubDataUpload( void * pCommandObject, const GpuBufferSubDataUploadDesc & pUploadDesc ) override;
		
		virtual bool ValidateMapRequest( const GpuMemoryRegion & pRegion, const EGpuMemoryMapMode & pMapMode ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_BUFFER_H__
