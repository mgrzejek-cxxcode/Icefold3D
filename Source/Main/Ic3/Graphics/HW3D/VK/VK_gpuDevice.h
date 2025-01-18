
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
	class IC3_GX_VKCOMMON_CLASS VKGPUDevice : public GPUDevice
	{
	public:
		VkDevice const mVkDevice = nullptr;

		VKGPUDevice( VKGPUDriver * pDriver, VkDevice pVkDevice );
		virtual ~VKGPUDevice();

		static VKGPUDeviceHandle Create( VKGPUDriver * pDriver, const VKGPUDeviceCreateInfo & pCreateInfo );

	private:
		virtual void InitializeCommandSystem() override;

	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_GPU_DEVICE_H__
