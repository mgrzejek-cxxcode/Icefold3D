
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

	GL4GPUDeviceHandle GL4GPUDevice::Create( GL4GPUDriver & pDriver, const GL4GPUDeviceCreateInfo & pCreateInfo )
	{
		auto driverConfigFlags = pDriver.GetConfigFlags();
		auto gl4GPUDevice = CreateGfxObject<GL4GPUDevice>( pDriver );

		if( driverConfigFlags.is_set( eGPUDriverConfigFlagEnableDebugLayerBit ) )
		{
			gl4GPUDevice->InitializeGLDebugOutput();
		}

		return gl4GPUDevice;
	}

} // namespace Ic3::Graphics::GCI
