
#include "GL4GPUDevice.h"
#include "GL4GPUDriver.h"
#include "GL4GPUCmdCore.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GL4GPUDevice::GL4GPUDevice( GL4GPUDriver & pDriver )
	: GL4GPUDeviceBase( pDriver )
	{}

	GL4GPUDevice::~GL4GPUDevice() = default;

	GL4GPUDeviceHandle GL4GPUDevice::create( GL4GPUDriver & pDriver, const GL4GPUDeviceCreateInfo & pCreateInfo )
	{
		auto driverConfigFlags = pDriver.getConfigFlags();
		auto gl4GPUDevice = createGPUAPIObject<GL4GPUDevice>( pDriver );

		if( driverConfigFlags.isSet( E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT ) )
		{
			gl4GPUDevice->initializeGLDebugOutput();
		}

		return gl4GPUDevice;
	}

} // namespace Ic3::Graphics::GCI
