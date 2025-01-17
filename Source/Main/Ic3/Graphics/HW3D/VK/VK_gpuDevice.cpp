
#include "VKUDevice.h"
#include "VKUDriver.h"

namespace Ic3::Graphics::GCI
{

	VKGPUDevice::VKGPUDevice( VKGPUDriver * pDriver, VkDevice pVkDevice )
	: GPUDevice( pDriver )
	, mVkDevice( pVkDevice )
	{ }

	VKGPUDevice::~VKGPUDevice() = default;

	VKGPUDeviceHandle VKGPUDevice::Create( VKGPUDriver * pDriver, const VKGPUDeviceCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	void VKGPUDevice::InitializeCommandSystem()
	{
	}

} // namespace Ic3::Graphics::GCI
