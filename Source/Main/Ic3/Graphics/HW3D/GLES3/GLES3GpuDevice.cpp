
#include "GLES3GpuDevice.h"
#include "GLES3GpuDriver.h"
#include "GLES3GpuCmdCore.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GLES3GpuDevice::GLES3GpuDevice( GLES3GpuDriver & pDriver )
	: GLGpuDevice( pDriver )
	{}

	GLES3GpuDevice::~GLES3GpuDevice() = default;

	GLES3GpuDeviceHandle GLES3GpuDevice::Create( GLES3GpuDriver & pDriver, const GLES3GpuDeviceCreateInfo & pCreateInfo )
	{
		auto driverConfigFlags = pDriver.GetConfigFlags();
		auto gles3GpuDevice = CreateGfxObject<GLES3GpuDevice>( pDriver );

		gles3GpuDevice->InitializeCommandSystem();

		return gles3GpuDevice;
	}

} // namespace Ic3::Graphics::GCI
