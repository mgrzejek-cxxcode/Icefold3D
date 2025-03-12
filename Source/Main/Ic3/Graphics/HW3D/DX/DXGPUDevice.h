
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_GPU_DEVICE_H__

#include "DXPrerequisites.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	class IC3_GX_DX_CLASS DXGPUDevice : public GPUDevice
	{
	public:
		ComPtr<IDXGIFactory2> const mDXGIFactory2;
		ComPtr<IDXGIDebug> const mDXGIDebug;
		ComPtr<IDXGIInfoQueue> const mDXGIInfoQueue;

		explicit DXGPUDevice(
				DXGPUDriver & pDXGPUDriver,
				GPUDeviceFeatureQuery * pFeatureQueryInterface,
				PipelineStateDescriptorManager * pPipelineStateDescriptorManager,
				ComPtr<IDXGIFactory2> pDXGIFactory2 );
		virtual ~DXGPUDevice();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_GPU_DEVICE_H__
