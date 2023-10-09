
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_BUFFER_H__
#define __IC3_GRAPHICS_GCI_GPU_BUFFER_H__

#include "GPUResource.h"
#include "GPUBufferReference.h"

namespace Ic3::Graphics::GCI
{

	struct GPUBufferProperties : public GPUResourceProperties
	{
		gpu_memory_size_t byteSize;
	};

	class GPUBuffer : public GPUResource
	{
		friend class CommandList;
		friend class GPUBufferReference;

	public:
		GPUBufferProperties const mBufferProperties;

	public:
		GPUBuffer(
			GPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GPUBufferProperties & pBufferProperties );

		virtual ~GPUBuffer();

		IC3_ATTR_NO_DISCARD virtual const GPUResourceProperties & getProperties() const override final;

		IC3_ATTR_NO_DISCARD GPUMemoryRegion getWholeBufferRegion() const;

	protected:
		virtual bool mapRegion( void * pCommandObject, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode ) = 0;

		virtual void unmap( void * pCommandObject ) = 0;

		virtual void flushMappedRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) = 0;

		virtual void invalidateRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) = 0;

		virtual void updateSubDataCopy( void * pCommandObject, GPUBuffer & pSourceBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc ) = 0;

		virtual void updateSubDataUpload( void * pCommandObject, const GPUBufferSubDataUploadDesc & pUploadDesc ) = 0;

		virtual bool validateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode );

		static bool validateBufferCreateInfo( GPUBufferCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_BUFFER_H__
