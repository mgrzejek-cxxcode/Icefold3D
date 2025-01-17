
#include "GL4GpuDriver.h"
#include "GL4GpuDevice.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GL4GpuDriver::GL4GpuDriver( System::OpenGLSystemDriverHandle pSysGLDriver )
	: GLGpuDriver( pSysGLDriver )
	{}

	GL4GpuDriver::~GL4GpuDriver() = default;

	GL4GpuDriverHandle GL4GpuDriver::Create( const GL4GpuDriverCreateInfo & pCreateInfo )
	{
		constexpr auto gl4SupportedGpuConfigFlags =
				eGpuDriverConfigFlagEnableDebugLayerBit |
				eGpuDriverConfigFlagEnableShaderDebugInfoBit;

		auto sysContext = pCreateInfo.sysContext;
		if( !sysContext )
		{
			sysContext = System::Platform::CreateSysContext( pCreateInfo.sysContextCreateInfo );
			if( !sysContext )
			{
				return nullptr;
			}
		}

		auto sysGLDriver = InitializeSysGLDriver( sysContext );
		if( !sysGLDriver )
		{
			return nullptr;
		}

		auto gl4Driver = CreateGfxObject<GL4GpuDriver>( sysGLDriver );
		gl4Driver->SetConfigFlags( pCreateInfo.configFlags & gl4SupportedGpuConfigFlags );

		return gl4Driver;
	}

	DisplayManagerHandle GL4GpuDriver::_DrvCreateDefaultDisplayManager()
	{
		return nullptr;
	}

	GpuDeviceHandle GL4GpuDriver::_DrvCreateDevice( const GpuDeviceCreateInfo & pCreateInfo )
	{
		GL4GpuDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;

		return GL4GpuDevice::Create( *this, createInfo );
	}

	EGpuDriverID GL4GpuDriver::QueryGpuDriverID() const noexcept
	{
		return EGpuDriverID::GDIOpenGLDesktop4;
	}

} // namespace Ic3::Graphics::GCI
