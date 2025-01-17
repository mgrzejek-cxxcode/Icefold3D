
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER__

#include "DX12Prerequisites.h"
#include <Ic3/Graphics/HW3D/DX/DXgpuDriver.h>


namespace Ic3::Graphics::GCI
{

	struct DX12GpuDriverCreateInfo : public GpuDriverCreateInfo
	{
	};

	class IC3_GX_DX12_CLASS DX12GpuDriver final : public DXGpuDriver
	{
	public:
		explicit DX12GpuDriver( System::SysContext * pExfSystemContext );
		virtual ~DX12GpuDriver();

		virtual DisplayManagerHandle CreateDefaultDisplayManager() override;

		virtual GpuDeviceHandle CreateDevice( const GpuDeviceCreateInfo & pCreateInfo ) override;

		static DX12GpuDriverHandle Create( const DX12GpuDriverCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER__
