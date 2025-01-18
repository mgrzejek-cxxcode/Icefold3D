
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

		CPPX_ATTR_NO_DISCARD virtual const GPUResourceProperties & GetProperties() const override final;

		CPPX_ATTR_NO_DISCARD GPUMemoryRegion GetWholeBufferRegion() const;

	protected:
		virtual bool MapRegion( void * pCommandObject, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode ) = 0;

		virtual void Unmap( void * pCommandObject ) = 0;

		virtual void FlushMappedRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) = 0;

		virtual void InvalidateRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) = 0;

		virtual void UpdateSubDataCopy( void * pCommandObject, GPUBuffer & pSourceBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc ) = 0;

		virtual void UpdateSubDataUpload( void * pCommandObject, const GPUBufferSubDataUploadDesc & pUploadDesc ) = 0;

		virtual bool ValidateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode );

		static bool ValidateBufferCreateInfo( GPUBufferCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_BUFFER_H__
