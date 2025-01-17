
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_BUFFER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_BUFFER_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/Resources/GpuBuffer.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( MetalGpuBuffer );

	class MetalGpuBuffer : public GpuBuffer
	{
	public:
		id<MTLDevice> const mMTLDevice;
		id<MTLBuffer> const mMTLBuffer;

	public:
		MetalGpuBuffer(
			MetalGpuDevice & pGpuDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GpuBufferProperties & pBufferProperties,
			id<MTLBuffer> pMTLBuffer );

		virtual ~MetalGpuBuffer();

		static MetalGpuBufferHandle Create( MetalGpuDevice & pGpuDevice, const GpuBufferCreateInfo & pCreateInfo );

	private:
	    virtual bool MapRegion( void * pCommandObject, const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode ) override;

		virtual void Unmap( void * pCommandObject ) override;

		virtual void FlushMappedRegion( void * pCommandObject, const GpuMemoryRegion & pRegion ) override;

		virtual void InvalidateRegion( void * pCommandObject, const GpuMemoryRegion & pRegion ) override;

		virtual void UpdateSubDataCopy( void * pCommandObject, GpuBuffer & pSrcBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc ) override;

		virtual void UpdateSubDataUpload( void * pCommandObject, const GpuBufferSubDataUploadDesc & pUploadDesc ) override;
		
		virtual bool ValidateMapRequest( const GpuMemoryRegion & pRegion, const EGpuMemoryMapMode & pMapMode ) override;

		static id<MTLCommandBuffer> GetMTLCommandBuffer( void * pCommandObject );

		static bool checkCommandListTransfer( void * pCommandObject );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_BUFFER_H__
