
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GPU_BUFFER_H__
#define __IC3_GRAPHICS_HW3D_DX11_GPU_BUFFER_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/GpuBuffer.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( DX11GpuBuffer );

	struct DX11GpuBufferDesc
	{
		cppx::bitmask<D3D11_BIND_FLAG> bindFlags;
		cppx::bitmask<D3D11_CPU_ACCESS_FLAG> cpuAccessFlags;
		cppx::bitmask<D3D11_RESOURCE_MISC_FLAG> resourceMiscFlags;
		D3D11_USAGE usage;
	};

	class DX11GpuBuffer : public GpuBuffer
	{
	public:
		ComPtr<ID3D11Buffer> const mD3D11Buffer;

		DX11GpuBuffer(
			DX11GpuDevice & pDX11GpuDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GpuBufferProperties & pBufferProperties,
			ComPtr<ID3D11Buffer> pD3D11Buffer );

		virtual ~DX11GpuBuffer();

		static DX11GpuBufferHandle Create( DX11GpuDevice & pDX11GpuDevice, const GpuBufferCreateInfo & pCreateInfo );

	private:
		static bool ValidateBufferCreateInfo( GpuBufferCreateInfo & pCreateInfo );

		static DX11GpuBufferDesc TranslateBufferDesc( const GpuBufferCreateInfo & pCreateInfo );

	    virtual bool MapRegion( void * pCommandObject, const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode ) override;

		virtual void Unmap( void * pCommandObject ) override;

		virtual void FlushMappedRegion( void * pCommandObject, const GpuMemoryRegion & pRegion ) override;

		virtual void InvalidateRegion( void * pCommandObject, const GpuMemoryRegion & pRegion ) override;

		virtual void UpdateSubDataCopy( void * pCommandObject, GpuBuffer & pSource, const GpuBufferSubDataCopyDesc & pCopyDesc ) override;

		virtual void UpdateSubDataUpload( void * pCommandObject, const GpuBufferSubDataUploadDesc & pUploadDesc ) override;

		virtual bool ValidateMapRequest( const GpuMemoryRegion & pRegion, const EGpuMemoryMapMode & pMapMode ) override;

	private:
		struct DX11BufferMapInfo
		{
			D3D11_MAP requestedMapMode;
			D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		};

		DX11BufferMapInfo _dx11MapInfo;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_GPU_BUFFER_H__
