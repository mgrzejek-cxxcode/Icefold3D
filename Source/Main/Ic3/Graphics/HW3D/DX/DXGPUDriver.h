
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_GPU_DRIVER__

#include "DXPrerequisites.h"
#include <Ic3/Graphics/GCI/GPUDriver.h>

namespace Ic3::Graphics::GCI
{

	class IC3_GX_DXCOMMON_CLASS DXGPUDriver : public GPUDriver
	{
	public:
		explicit DXGPUDriver( System::SysContextHandle pSysContext ) noexcept;
		virtual ~DXGPUDriver() noexcept;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_GPU_DRIVER__
