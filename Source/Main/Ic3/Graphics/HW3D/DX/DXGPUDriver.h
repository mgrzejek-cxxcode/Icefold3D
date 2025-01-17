
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_GPU_DRIVER__

#include "DXPrerequisites.h"
#include <Ic3/Graphics/GCI/GpuDriver.h>

namespace Ic3::Graphics::GCI
{

	class IC3_GX_DXCOMMON_CLASS DXGpuDriver : public GpuDriver
	{
	public:
		explicit DXGpuDriver( System::SysContextHandle pSysContext ) noexcept;
		virtual ~DXGpuDriver() noexcept;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_GPU_DRIVER__
