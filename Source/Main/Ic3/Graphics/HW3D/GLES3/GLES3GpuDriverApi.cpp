
#include "GLES3GpuDriverAPI.h"
#include "GLES3GpuDriver.h"
#include "GLES3GpuDevice.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GpuDriverHandle GLES3GpuDriverInterface::CreateDriver( const GpuDriverCreateInfo & pCreateInfo )
	{
		GLES3GpuDriverCreateInfo GLES3CreateInfo;
		GLES3CreateInfo.sysContext = pCreateInfo.sysContext;
		GLES3CreateInfo.sysContextCreateInfo = pCreateInfo.sysContextCreateInfo;
		GLES3CreateInfo.configFlags = pCreateInfo.configFlags;

		return GLES3GpuDriver::Create( GLES3CreateInfo );
	}

	PresentationLayerHandle GLES3GpuDriverInterface::CreateScreenPresentationLayer( GpuDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		GLPresentationLayerCreateInfo openglCreateInfo;
		openglCreateInfo.screenRect = pCreateInfo.screenRect;
		openglCreateInfo.mVisualConfig = pCreateInfo.mVisualConfig;
		openglCreateInfo.mDisplayConfigFlags = pCreateInfo.mDisplayConfigFlags;

		auto * deviceGL = pDevice.QueryInterface<GLGpuDevice>();
		return GLScreenPresentationLayer::Create( *deviceGL, openglCreateInfo );
	}

} // namespace Ic3::Graphics::GCI
