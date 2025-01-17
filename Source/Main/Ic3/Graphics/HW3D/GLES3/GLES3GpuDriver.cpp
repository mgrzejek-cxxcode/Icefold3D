
#include "GLES3GPUDriver.h"
#include "GLES3GPUDevice.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GLES3GPUDriver::GLES3GPUDriver( System::OpenGLSystemDriverHandle pSysGLDriver )
	: GLGPUDriver( pSysGLDriver )
	{ }

	GLES3GPUDriver::~GLES3GPUDriver() = default;

	GLES3GPUDriverHandle GLES3GPUDriver::Create( const GLES3GPUDriverCreateInfo & pCreateInfo )
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

		auto sysGLDriver = InitializeSysGLDriver( sysContext );
		if( !sysGLDriver )
		{
			return nullptr;
		}

		auto gles3Driver = CreateGfxObject<GLES3GPUDriver>( sysGLDriver );
		gles3Driver->SetConfigFlags( pCreateInfo.configFlags & gles3SupportedGPUConfigFlags );

		return gles3Driver;
	}

	DisplayManagerHandle GLES3GPUDriver::CreateDefaultDisplayManager()
	{
		return nullptr;
	}

	GPUDeviceHandle GLES3GPUDriver::CreateDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		GLES3GPUDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;

		return GLES3GPUDevice::Create( *this, createInfo );
	}

	EGPUDriverID GLES3GPUDriver::QueryGPUDriverID() const
	{
		return EGPUDriverID::GDIDGLES3;
	}

} // namespace Ic3::Graphics::GCI
