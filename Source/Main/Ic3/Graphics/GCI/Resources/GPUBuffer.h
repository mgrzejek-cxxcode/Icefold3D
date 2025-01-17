
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_BUFFER_H__
#define __IC3_GRAPHICS_GCI_GPU_BUFFER_H__

#include "GpuResource.h"
#include "GpuBufferReference.h"

namespace Ic3::Graphics::GCI
{

	struct GpuBufferProperties : public GpuResourceProperties
	{
		gpu_memory_size_t byteSize;
	};

	class GpuBuffer : public GpuResource
	{
		friend class CommandList;
		friend class GpuBufferReference;

	public:
		GpuBufferProperties const mBufferProperties;

	public:
		GpuBuffer(
			GpuDevice & pGpuDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GpuBufferProperties & pBufferProperties );

		virtual ~GpuBuffer();

		CPPX_ATTR_NO_DISCARD virtual const GpuResourceProperties & GetProperties() const override final;

		CPPX_ATTR_NO_DISCARD GpuMemoryRegion GetWholeBufferRegion() const;

	protected:
		virtual bool MapRegion( void * pCommandObject, const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode ) = 0;

		virtual void Unmap( void * pCommandObject ) = 0;

		virtual void FlushMappedRegion( void * pCommandObject, const GpuMemoryRegion & pRegion ) = 0;

		virtual void InvalidateRegion( void * pCommandObject, const GpuMemoryRegion & pRegion ) = 0;

		virtual void UpdateSubDataCopy( void * pCommandObject, GpuBuffer & pSourceBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc ) = 0;

		virtual void UpdateSubDataUpload( void * pCommandObject, const GpuBufferSubDataUploadDesc & pUploadDesc ) = 0;

		virtual bool ValidateMapRequest( const GpuMemoryRegion & pRegion, const EGpuMemoryMapMode & pMapMode );

		static bool ValidateBufferCreateInfo( GpuBufferCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_BUFFER_H__
