
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DEVICE_H__

#include "VKPrerequisites.h"
#include <Ic3/Graphics/GCI/GpuDevice.h>

namespace Ic3::Graphics::GCI
{

	struct VKGpuDeviceCreateInfo : public GpuDeviceCreateInfo
	{
	};

	/// @brief
	class IC3_GX_VKCOMMON_CLASS VKGpuDevice : public GpuDevice
	{
	public:
		VkDevice const mVkDevice = nullptr;

		VKGpuDevice( VKGpuDriver * pDriver, VkDevice pVkDevice );
		virtual ~VKGpuDevice();

		static VKGpuDeviceHandle Create( VKGpuDriver * pDriver, const VKGpuDeviceCreateInfo & pCreateInfo );

	private:
		virtual void InitializeCommandSystem() override;

	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DEVICE_H__
