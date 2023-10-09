
#include "GL4GPUDriver.h"
#include "GL4GPUDevice.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GL4GPUDriver::GL4GPUDriver( System::OpenGLSystemDriverHandle pSysGLDriver )
	: GLGPUDriver( pSysGLDriver )
	{}

	GL4GPUDriver::~GL4GPUDriver() = default;

	GL4GPUDriverHandle GL4GPUDriver::create( const GL4GPUDriverCreateInfo & pCreateInfo )
	{
		constexpr auto gl4SupportedGPUConfigFlags =
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_SHADER_DEBUG_INFO_BIT;

		auto sysContext = pCreateInfo.sysContext;
		if( !sysContext )
		{
			sysContext = System::platform::createSysContext( pCreateInfo.sysContextCreateInfo );
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

		auto gl4Driver = createGPUAPIObject<GL4GPUDriver>( sysGLDriver );
		gl4Driver->setConfigFlags( pCreateInfo.configFlags & gl4SupportedGPUConfigFlags );

		return gl4Driver;
	}

	DisplayManagerHandle GL4GPUDriver::_drvCreateDefaultDisplayManager()
	{
		return nullptr;
	}

	GPUDeviceHandle GL4GPUDriver::_drvCreateDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		GL4GPUDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;

		return GL4GPUDevice::create( *this, createInfo );
	}

	EGPUDriverID GL4GPUDriver::queryGPUDriverID() const noexcept
	{
		return EGPUDriverID::GDIOpenGLDesktop4;
	}

} // namespace Ic3::Graphics::GCI
