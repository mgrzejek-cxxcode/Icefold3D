
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_BUFFER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_BUFFER_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/Resources/GPUBuffer.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( MetalGPUBuffer );

	class MetalGPUBuffer : public GPUBuffer
	{
	public:
		id<MTLDevice> const mMTLDevice;
		id<MTLBuffer> const mMTLBuffer;

	public:
		MetalGPUBuffer(
			MetalGPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GPUBufferProperties & pBufferProperties,
			id<MTLBuffer> pMTLBuffer );

		virtual ~MetalGPUBuffer();

		static MetalGPUBufferHandle create( MetalGPUDevice & pGPUDevice, const GPUBufferCreateInfo & pCreateInfo );

	private:
	    virtual bool mapRegion( void * pCommandObject, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode ) override;

		virtual void unmap( void * pCommandObject ) override;

		virtual void flushMappedRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) override;

		virtual void invalidateRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) override;

		virtual void updateSubDataCopy( void * pCommandObject, GPUBuffer & pSrcBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc ) override;

		virtual void updateSubDataUpload( void * pCommandObject, const GPUBufferSubDataUploadDesc & pUploadDesc ) override;
		
		virtual bool validateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode ) override;

		static id<MTLCommandBuffer> getMTLCommandBuffer( void * pCommandObject );

		static bool checkCommandListTransfer( void * pCommandObject );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_BUFFER_H__
