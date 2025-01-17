
#include "GLES3GpuDriver.h"
#include "GLES3GpuDevice.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GLES3GpuDriver::GLES3GpuDriver( System::OpenGLSystemDriverHandle pSysGLDriver )
	: GLGpuDriver( pSysGLDriver )
	{ }

	GLES3GpuDriver::~GLES3GpuDriver() = default;

	GLES3GpuDriverHandle GLES3GpuDriver::Create( const GLES3GpuDriverCreateInfo & pCreateInfo )
	{
		constexpr auto gles3SupportedGpuConfigFlags =
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

		auto sysGLDriver = InitializeSysGLDriver( sysContext );
		if( !sysGLDriver )
		{
			return nullptr;
		}

		auto gles3Driver = CreateGfxObject<GLES3GpuDriver>( sysGLDriver );
		gles3Driver->SetConfigFlags( pCreateInfo.configFlags & gles3SupportedGpuConfigFlags );

		return gles3Driver;
	}

	DisplayManagerHandle GLES3GpuDriver::CreateDefaultDisplayManager()
	{
		return nullptr;
	}

	GpuDeviceHandle GLES3GpuDriver::CreateDevice( const GpuDeviceCreateInfo & pCreateInfo )
	{
		GLES3GpuDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;

		return GLES3GpuDevice::Create( *this, createInfo );
	}

	EGpuDriverID GLES3GpuDriver::QueryGpuDriverID() const
	{
		return EGpuDriverID::GDIDGLES3;
	}

} // namespace Ic3::Graphics::GCI
