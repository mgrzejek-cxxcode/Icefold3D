
#include <Ic3/Graphics/HW3D/GL/GLGpuDriver.h>
#include <Ic3/Graphics/HW3D/GL/GLGpuDevice.h>
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>
#include <Ic3/System/DisplaySystem.h>

namespace Ic3::Graphics::GCI
{

	GLGpuDriver::GLGpuDriver( System::OpenGLSystemDriverHandle pSysGLDriver )
	: GpuDriver( pSysGLDriver->mSysContext )
	, mSysGLDriver( std::move( pSysGLDriver ) )
	{ }

	GLGpuDriver::~GLGpuDriver() = default;

	System::OpenGLSystemDriverHandle GLGpuDriver::InitializeSysGLDriver( System::SysContextHandle pSysContext )
	{
		try
		{
		    auto sysDisplayManager = pSysContext->CreateDisplayManager();
		    auto sysGLDriver = pSysContext->CreateOpenGLSystemDriver( sysDisplayManager );

			sysGLDriver->initializePlatform();

			return sysGLDriver;
		}
		catch ( ... )
		{}

		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
