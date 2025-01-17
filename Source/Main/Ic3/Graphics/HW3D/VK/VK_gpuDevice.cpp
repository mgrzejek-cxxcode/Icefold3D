
#include "VKUDevice.h"
#include "VKUDriver.h"

namespace Ic3::Graphics::GCI
{

	VKGpuDevice::VKGpuDevice( VKGpuDriver * pDriver, VkDevice pVkDevice )
	: GpuDevice( pDriver )
	, mVkDevice( pVkDevice )
	{ }

	VKGpuDevice::~VKGpuDevice() = default;

	VKGpuDeviceHandle VKGpuDevice::Create( VKGpuDriver * pDriver, const VKGpuDeviceCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	void VKGpuDevice::InitializeCommandSystem()
	{
	}

} // namespace Ic3::Graphics::GCI
