
#include "GLES3GPUDriverAPI.h"
#include "GLES3GPUDriver.h"
#include "GLES3GPUDevice.h"
#include <Ic3/Graphics/HW3D/GL/GLPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GPUDriverHandle GLES3GPUDriverInterface::CreateDriver( const GPUDriverCreateInfo & pCreateInfo )
	{
		GLES3GPUDriverCreateInfo GLES3CreateInfo;
		GLES3CreateInfo.sysContext = pCreateInfo.sysContext;
		GLES3CreateInfo.sysContextCreateInfo = pCreateInfo.sysContextCreateInfo;
		GLES3CreateInfo.configFlags = pCreateInfo.configFlags;

		return GLES3GPUDriver::Create( GLES3CreateInfo );
	}

	PresentationLayerHandle GLES3GPUDriverInterface::CreateScreenPresentationLayer( GPUDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		GLPresentationLayerCreateInfo openglCreateInfo;
		openglCreateInfo.screenRect = pCreateInfo.screenRect;
		openglCreateInfo.mVisualConfig = pCreateInfo.mVisualConfig;
		openglCreateInfo.mDisplayConfigFlags = pCreateInfo.mDisplayConfigFlags;

		auto * deviceGL = pDevice.QueryInterface<GLGPUDevice>();
		return GLScreenPresentationLayer::Create( *deviceGL, openglCreateInfo );
	}

} // namespace Ic3::Graphics::GCI
