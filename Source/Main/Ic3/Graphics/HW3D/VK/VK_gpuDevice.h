
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DEVICE_H__

#include "VKPrerequisites.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	struct VKGPUDeviceCreateInfo : public GPUDeviceCreateInfo
	{
	};

	/// @brief
	class ICFGX_VKCOMMON_CLASS VKGPUDevice : public GPUDevice
	{
	public:
		VkDevice const mVkDevice = nullptr;

		VKGPUDevice( VKGPUDriver * pDriver, VkDevice pVkDevice );
		virtual ~VKGPUDevice();

		static VKGPUDeviceHandle create( VKGPUDriver * pDriver, const VKGPUDeviceCreateInfo & pCreateInfo );

	private:
		virtual void initializeCommandSystem() override;

	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DEVICE_H__
