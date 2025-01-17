
#include "GL4GpuDevice.h"
#include "GL4GpuDriver.h"
#include "GL4GpuCmdCore.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GL4GpuDevice::GL4GpuDevice( GL4GpuDriver & pDriver )
	: GL4GpuDeviceBase( pDriver )
	{}

	GL4GpuDevice::~GL4GpuDevice() = default;

	GL4GpuDeviceHandle GL4GpuDevice::Create( GL4GpuDriver & pDriver, const GL4GpuDeviceCreateInfo & pCreateInfo )
	{
		auto driverConfigFlags = pDriver.GetConfigFlags();
		auto gl4GpuDevice = CreateGfxObject<GL4GpuDevice>( pDriver );

		if( driverConfigFlags.is_set( eGpuDriverConfigFlagEnableDebugLayerBit ) )
		{
			gl4GpuDevice->InitializeGLDebugOutput();
		}

		return gl4GpuDevice;
	}

} // namespace Ic3::Graphics::GCI
