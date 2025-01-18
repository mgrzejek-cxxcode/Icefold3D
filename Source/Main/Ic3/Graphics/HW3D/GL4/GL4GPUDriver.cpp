
#include "GL4GPUDriver.h"
#include "GL4GPUDevice.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GL4GPUDriver::GL4GPUDriver( System::OpenGLSystemDriverHandle pSysGLDriver )
	: GLGPUDriver( pSysGLDriver )
	{}

	GL4GPUDriver::~GL4GPUDriver() = default;

	GL4GPUDriverHandle GL4GPUDriver::Create( const GL4GPUDriverCreateInfo & pCreateInfo )
	{
		constexpr auto gl4SupportedGPUConfigFlags =
				eGPUDriverConfigFlagEnableDebugLayerBit |
				eGPUDriverConfigFlagEnableShaderDebugInfoBit;

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

		auto gl4Driver = CreateGfxObject<GL4GPUDriver>( sysGLDriver );
		gl4Driver->SetConfigFlags( pCreateInfo.configFlags & gl4SupportedGPUConfigFlags );

		return gl4Driver;
	}

	DisplayManagerHandle GL4GPUDriver::_DrvCreateDefaultDisplayManager()
	{
		return nullptr;
	}

	GPUDeviceHandle GL4GPUDriver::_DrvCreateDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		GL4GPUDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;

		return GL4GPUDevice::Create( *this, createInfo );
	}

	EGPUDriverID GL4GPUDriver::QueryGPUDriverID() const noexcept
	{
		return EGPUDriverID::GDIOpenGLDesktop4;
	}

} // namespace Ic3::Graphics::GCI
