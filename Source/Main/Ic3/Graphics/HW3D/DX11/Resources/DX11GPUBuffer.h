
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GPU_BUFFER_H__
#define __IC3_GRAPHICS_HW3D_DX11_GPU_BUFFER_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/GPUBuffer.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( DX11GPUBuffer );

	struct DX11GPUBufferDesc
	{
		cppx::bitmask<D3D11_BIND_FLAG> bindFlags;
		cppx::bitmask<D3D11_CPU_ACCESS_FLAG> cpuAccessFlags;
		cppx::bitmask<D3D11_RESOURCE_MISC_FLAG> resourceMiscFlags;
		D3D11_USAGE usage;
	};

	class IC3_GX_DX11_CLASS DX11GPUBuffer : public GPUBuffer
	{
	public:
		ComPtr<ID3D11Buffer> const mD3D11Buffer;

		DX11GPUBuffer(
			DX11GPUDevice & pDX11GPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GPUBufferProperties & pBufferProperties,
			ComPtr<ID3D11Buffer> pD3D11Buffer );

		virtual ~DX11GPUBuffer();

		static DX11GPUBufferHandle Create( DX11GPUDevice & pDX11GPUDevice, const GPUBufferCreateInfo & pCreateInfo );

	private:
		static bool ValidateBufferCreateInfo( GPUBufferCreateInfo & pCreateInfo );

		static DX11GPUBufferDesc TranslateBufferDesc( const GPUBufferCreateInfo & pCreateInfo );

	    virtual bool MapRegion( void * pCommandObject, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode ) override;

		virtual void Unmap( void * pCommandObject ) override;

		virtual void FlushMappedRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) override;

		virtual void InvalidateRegion( void * pCommandObject, const GPUMemoryRegion & pRegion ) override;

		virtual void UpdateSubDataCopy( void * pCommandObject, GPUBuffer & pSource, const GPUBufferSubDataCopyDesc & pCopyDesc ) override;

		virtual void UpdateSubDataUpload( void * pCommandObject, const GPUBufferSubDataUploadDesc & pUploadDesc ) override;

		virtual bool ValidateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode ) override;

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
