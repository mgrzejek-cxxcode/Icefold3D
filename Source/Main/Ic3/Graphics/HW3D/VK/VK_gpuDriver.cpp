
#include "VKUDriver.h"
#include "VKUDevice.h"

namespace Ic3::Graphics::GCI
{

	VKGPUDriver::VKGPUDriver( System::SysContext * pExfSystemContext )
	: GPUDriver( pExfSystemContext )
	{ }

	VKGPUDriver::~VKGPUDriver()
	{ }

	VKGPUDriverHandle VKGPUDriver::Create( const VKGPUDriverCreateInfo & pCreateInfo )
	{
		return CreateGfxObject<VKGPUDriver>( pCreateInfo.exfSystemContext );
	}

	GPUDeviceHandle VKGPUDriver::CreateDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		VKGPUDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;
		return VKGPUDevice::Create( this, createInfo );
	}

} // namespace Ic3::Graphics::GCI
