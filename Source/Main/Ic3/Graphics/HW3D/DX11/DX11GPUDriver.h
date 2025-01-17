
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_DX11_GPU_DRIVER__

#include "DX11Prerequisites.h"
#include <Ic3/Graphics/HW3D/DX/DXgpuDriver.h>

namespace Ic3::Graphics::GCI
{

	struct DX11GpuDriverCreateInfo : public GpuDriverCreateInfo
	{
	};

	class IC3_GX_DX11_CLASS DX11GpuDriver final : public DXGpuDriver
	{
	public:
		explicit DX11GpuDriver( System::SysContextHandle pSysContext ) noexcept;
		virtual ~DX11GpuDriver() noexcept;

		virtual EGpuDriverID QueryGpuDriverID() const noexcept override final;

		static DX11GpuDriverHandle Create( const DX11GpuDriverCreateInfo & pCreateInfo );

	private:
		virtual DisplayManagerHandle _DrvCreateDefaultDisplayManager() override;

		virtual GpuDeviceHandle _DrvCreateDevice( const GpuDeviceCreateInfo & pCreateInfo ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_GPU_DRIVER__
