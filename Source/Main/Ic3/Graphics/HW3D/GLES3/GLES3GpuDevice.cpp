
#include "GLES3GPUDevice.h"
#include "GLES3GPUDriver.h"
#include "GLES3GPUCmdCore.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GLES3GPUDevice::GLES3GPUDevice( GLES3GPUDriver & pDriver )
	: GLGPUDevice( pDriver )
	{}

	GLES3GPUDevice::~GLES3GPUDevice() = default;

	GLES3GPUDeviceHandle GLES3GPUDevice::Create( GLES3GPUDriver & pDriver, const GLES3GPUDeviceCreateInfo & pCreateInfo )
	{
		auto driverConfigFlags = pDriver.GetConfigFlags();
		auto gles3GPUDevice = CreateGfxObject<GLES3GPUDevice>( pDriver );

		gles3GPUDevice->InitializeCommandSystem();

		return gles3GPUDevice;
	}

} // namespace Ic3::Graphics::GCI
