
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DRIVER__

#include "VKPrerequisites.h"
#include <Ic3/Graphics/GCI/GpuDriver.h>

namespace Ic3::Graphics::GCI
{

	struct VKGpuDriverCreateInfo : public GpuDriverCreateInfo
	{
	};

	class IC3_GX_VKCOMMON_CLASS VKGpuDriver : public GpuDriver
	{
	public:
		VkInstance const mVkInstance = nullptr;
		
		explicit VKGpuDriver( System::SysContext * pExfSystemContext );
		virtual ~VKGpuDriver();

		virtual GpuDeviceHandle CreateDevice( const GpuDeviceCreateInfo & pCreateInfo ) override;

		static VKGpuDriverHandle Create( const VKGpuDriverCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DRIVER__
