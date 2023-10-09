
#include "GLES3GPUDriver.h"
#include "GLES3GPUDevice.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GLES3GPUDriver::GLES3GPUDriver( System::OpenGLSystemDriverHandle pSysGLDriver )
	: GLGPUDriver( pSysGLDriver )
	{ }

	GLES3GPUDriver::~GLES3GPUDriver() = default;

	GLES3GPUDriverHandle GLES3GPUDriver::create( const GLES3GPUDriverCreateInfo & pCreateInfo )
	{
		constexpr auto gles3SupportedGPUConfigFlags =
				E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT |
				E_GPU_DRIVER_CONFIG_FLAG_ENABLE_SHADER_DEBUG_INFO_BIT |
				E_GPU_DRIVER_CONFIG_FLAG_FORCE_COMPATIBILITY_BIT |
				E_GPU_DRIVER_CONFIG_FLAG_FORCE_CORE_PROFILE_BIT;

		auto sysContext = pCreateInfo.sysContext;
		if( !sysContext )
		{
			sysContext = System::createSysContext( pCreateInfo.sysContextCreateInfo );
			if( !sysContext )
			{
				return nullptr;
			}
		}

		auto sysGLDriver = initializeSysGLDriver( sysContext );
		if( !sysGLDriver )
		{
			return nullptr;
		}

		auto gles3Driver = createGPUAPIObject<GLES3GPUDriver>( sysGLDriver );
		gles3Driver->setConfigFlags( pCreateInfo.configFlags & gles3SupportedGPUConfigFlags );

		return gles3Driver;
	}

	DisplayManagerHandle GLES3GPUDriver::createDefaultDisplayManager()
	{
		return nullptr;
	}

	GPUDeviceHandle GLES3GPUDriver::createDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		GLES3GPUDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;

		return GLES3GPUDevice::create( *this, createInfo );
	}

	EGPUDriverID GLES3GPUDriver::queryGPUDriverID() const
	{
		return EGPUDriverID::GDIDGLES3;
	}

} // namespace Ic3::Graphics::GCI
