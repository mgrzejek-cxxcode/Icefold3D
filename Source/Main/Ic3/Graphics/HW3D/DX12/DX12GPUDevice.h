
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_DX12_GPU_DEVICE_H__

#include "DX12Prerequisites.h"
#include <Ic3/Graphics/HW3D/DX/DXgpuDevice.h>

namespace Ic3::Graphics::GCI
{

	struct DX12GPUDeviceCreateInfo : public GPUDeviceCreateInfo
	{
	};

	/// @brief
	class IC3_GX_DX12_CLASS DX12GPUDevice final : public DXGPUDevice
	{
	public:
		ComPtr<IDXGIFactory3> const mDXGIFactory3;
		ComPtr<ID3D12Debug> const mD3D12DebugInterface;
		ComPtr<ID3D12Device> const mD3D12Device;

		explicit DX12GPUDevice(
				DX12GPUDriver & pDriver,
				ComPtr<IDXGIFactory3> pDXGIFactory3,
				ComPtr<ID3D12Device> pD3D12Device,
				ComPtr<ID3D12Debug> pD3D12DebugInterface );

		virtual ~DX12GPUDevice();

		ID3D12CommandQueue * GetD3D12DeviceQueue( gpu_cmd_device_queue_id_t pQueueID ) const;

		static DX12GPUDeviceHandle Create( DX12GPUDriver & pDX12Driver, const DX12GPUDeviceCreateInfo & pCreateInfo );

	private:
		virtual void InitializeCommandSystem() override;

		void InitializeDefaultPresentDeviceQueue();

	private:
		ComPtr<ID3D12DescriptorHeap> _globalCbvSrvUavHeap;
		ComPtr<ID3D12DescriptorHeap> _globalDsvHeap;
		ComPtr<ID3D12DescriptorHeap> _globalRtvHeap;
		ComPtr<ID3D12DescriptorHeap> _globalSamplerHeap;

	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_GPU_DEVICE_H__
