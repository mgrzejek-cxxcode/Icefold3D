
#include "VKUDriver.h"
#include "VKUDevice.h"

namespace Ic3::Graphics::GCI
{

	VKGPUDriver::VKGPUDriver( System::temContext * pExfSystemContext )
	: GPUDriver( pExfSystemContext )
	{ }

	VKGPUDriver::~VKGPUDriver()
	{ }

	VKGPUDriverHandle VKGPUDriver::create( const VKGPUDriverCreateInfo & pCreateInfo )
	{
		return createGPUAPIObject<VKGPUDriver>( pCreateInfo.exfSystemContext );
	}

	GPUDeviceHandle VKGPUDriver::createDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		VKGPUDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;
		return VKGPUDevice::create( this, createInfo );
	}

} // namespace Ic3::Graphics::GCI
