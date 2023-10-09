
#include <Ic3/Graphics/HW3D/GL/GLGPUDriver.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>
#include <Ic3/System/DisplaySystem.h>

namespace Ic3::Graphics::GCI
{

	GLGPUDriver::GLGPUDriver( System::OpenGLSystemDriverHandle pSysGLDriver )
	: GPUDriver( pSysGLDriver->mSysContext )
	, mSysGLDriver( std::move( pSysGLDriver ) )
	{ }

	GLGPUDriver::~GLGPUDriver() = default;

	System::OpenGLSystemDriverHandle GLGPUDriver::initializeSysGLDriver( System::SysContextHandle pSysContext )
	{
		try
		{
		    auto sysDisplayManager = pSysContext->createDisplayManager();
		    auto sysGLDriver = pSysContext->createOpenGLSystemDriver( sysDisplayManager );

			sysGLDriver->initializePlatform();

			return sysGLDriver;
		}
		catch ( ... )
		{}

		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
