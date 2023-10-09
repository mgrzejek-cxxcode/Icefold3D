
#include "VKUDevice.h"
#include "VKUDriver.h"

namespace Ic3::Graphics::GCI
{

	VKGPUDevice::VKGPUDevice( VKGPUDriver * pDriver, VkDevice pVkDevice )
	: GPUDevice( pDriver )
	, mVkDevice( pVkDevice )
	{ }

	VKGPUDevice::~VKGPUDevice() = default;

	VKGPUDeviceHandle VKGPUDevice::create( VKGPUDriver * pDriver, const VKGPUDeviceCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	void VKGPUDevice::initializeCommandSystem()
	{
	}

} // namespace Ic3::Graphics::GCI
