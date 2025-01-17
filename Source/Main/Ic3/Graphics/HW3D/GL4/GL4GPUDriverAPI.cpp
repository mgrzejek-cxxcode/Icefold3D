
#include "GL4GpuDriverAPI.h"
#include "GL4GpuDriver.h"
#include "GL4GpuDevice.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GpuDriverHandle GL4GpuDriverInterface::CreateDriver( const GpuDriverCreateInfo & pCreateInfo )
	{
		GL4GpuDriverCreateInfo gl4CreateInfo;
		gl4CreateInfo.sysContext = pCreateInfo.sysContext;
		gl4CreateInfo.sysContextCreateInfo = pCreateInfo.sysContextCreateInfo;
		gl4CreateInfo.configFlags = pCreateInfo.configFlags;

		return GL4GpuDriver::Create( gl4CreateInfo );
	}

	PresentationLayerHandle GL4GpuDriverInterface::CreateScreenPresentationLayer( GpuDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		GLPresentationLayerCreateInfo openglCreateInfo;
		openglCreateInfo.screenRect = pCreateInfo.screenRect;
		openglCreateInfo.visualConfig = pCreateInfo.visualConfig;
		openglCreateInfo.displayConfigFlags = pCreateInfo.displayConfigFlags;

		auto * deviceGL = pDevice.QueryInterface<GLGpuDevice>();
		return GLScreenPresentationLayer::Create( *deviceGL, openglCreateInfo );
	}

} // namespace Ic3::Graphics::GCI
