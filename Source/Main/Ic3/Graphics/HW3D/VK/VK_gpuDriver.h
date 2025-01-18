
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

	class IC3_GX_VKCOMMON_CLASS VKGPUDriver : public GPUDriver
	{
	public:
		VkInstance const mVkInstance = nullptr;
		
		explicit VKGPUDriver( System::SysContext * pExfSystemContext );
		virtual ~VKGPUDriver();

		virtual GPUDeviceHandle CreateDevice( const GPUDeviceCreateInfo & pCreateInfo ) override;

		static VKGPUDriverHandle Create( const VKGPUDriverCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DRIVER__
