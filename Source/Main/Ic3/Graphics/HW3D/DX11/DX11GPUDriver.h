
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_DX11_GPU_DRIVER__

#include "DX11Prerequisites.h"
#include <Ic3/Graphics/HW3D/DX/DXgpuDriver.h>

namespace Ic3::Graphics::GCI
{

	struct DX11GPUDriverCreateInfo : public GPUDriverCreateInfo
	{
	};

	class IC3_GX_DX11_CLASS DX11GPUDriver final : public DXGPUDriver
	{
	public:
		explicit DX11GPUDriver( System::SysContextHandle pSysContext ) noexcept;
		virtual ~DX11GPUDriver() noexcept;

		virtual EGPUDriverID QueryGPUDriverID() const noexcept override final;

		static DX11GPUDriverHandle Create( const DX11GPUDriverCreateInfo & pCreateInfo );

	private:
		virtual DisplayManagerHandle _DrvCreateDefaultDisplayManager() override;

		virtual GPUDeviceHandle _DrvCreateDevice( const GPUDeviceCreateInfo & pCreateInfo ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_GPU_DRIVER__
