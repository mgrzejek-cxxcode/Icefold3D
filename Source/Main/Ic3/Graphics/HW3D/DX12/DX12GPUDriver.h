
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER__

#include "DX12Prerequisites.h"
#include <Ic3/Graphics/HW3D/DX/DXgpuDriver.h>


namespace Ic3::Graphics::GCI
{

	struct DX12GPUDriverCreateInfo : public GPUDriverCreateInfo
	{
	};

	class IC3_GX_DX12_CLASS DX12GPUDriver final : public DXGPUDriver
	{
	public:
		explicit DX12GPUDriver( System::SysContext * pExfSystemContext );
		virtual ~DX12GPUDriver();

		virtual DisplayManagerHandle CreateDefaultDisplayManager() override;

		virtual GPUDeviceHandle CreateDevice( const GPUDeviceCreateInfo & pCreateInfo ) override;

		static DX12GPUDriverHandle Create( const DX12GPUDriverCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER__
