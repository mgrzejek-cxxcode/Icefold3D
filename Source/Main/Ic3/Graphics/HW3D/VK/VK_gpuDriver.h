
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DRIVER__

#include "VKPrerequisites.h"
#include <Ic3/Graphics/GCI/GPUDriver.h>

namespace Ic3::Graphics::GCI
{

	struct VKGPUDriverCreateInfo : public GPUDriverCreateInfo
	{
	};

	class ICFGX_VKCOMMON_CLASS VKGPUDriver : public GPUDriver
	{
	public:
		VkInstance const mVkInstance = nullptr;
		
		explicit VKGPUDriver( System::temContext * pExfSystemContext );
		virtual ~VKGPUDriver();

		virtual GPUDeviceHandle createDevice( const GPUDeviceCreateInfo & pCreateInfo ) override;

		static VKGPUDriverHandle create( const VKGPUDriverCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DRIVER__
