
#include "VKUDriver.h"
#include "VKUDevice.h"

namespace Ic3::Graphics::GCI
{

	VKGpuDriver::VKGpuDriver( System::SysContext * pExfSystemContext )
	: GpuDriver( pExfSystemContext )
	{ }

	VKGpuDriver::~VKGpuDriver()
	{ }

	VKGpuDriverHandle VKGpuDriver::Create( const VKGpuDriverCreateInfo & pCreateInfo )
	{
		return CreateGfxObject<VKGpuDriver>( pCreateInfo.exfSystemContext );
	}

	GpuDeviceHandle VKGpuDriver::CreateDevice( const GpuDeviceCreateInfo & pCreateInfo )
	{
		VKGpuDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;
		return VKGpuDevice::Create( this, createInfo );
	}

} // namespace Ic3::Graphics::GCI
