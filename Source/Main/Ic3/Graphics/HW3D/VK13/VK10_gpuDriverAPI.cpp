
#include "VK10UDriverAPI.h"
#include "ic3/GpuapiVK/VKUDriver.h"
#include "ic3/GpuapiVK/VKUDevice.h"
#include <Ic3/GpuapiVK/system/VK_presentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GpuDriverHandle VK10GpuDriverInterface::CreateDriver( const GpuDriverCreateInfo & pCreateInfo )
	{
		VKGpuDriverCreateInfo vk10CreateInfo;
		vk10CreateInfo.exfSystemContext = pCreateInfo.exfSystemContext;
		vk10CreateInfo.initFlags = pCreateInfo.initFlags;

		return VKGpuDriver::Create( vk10CreateInfo );
	}

	PresentationLayerHandle VK10GpuDriverInterface::CreateScreenPresentationLayer( GpuDevice * pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		VKPresentationLayerCreateInfo vk10CreateInfo;
		vk10CreateInfo.coreEngineState = pCreateInfo.coreEngineState;
		vk10CreateInfo.screenRect = pCreateInfo.screenRect;
		vk10CreateInfo.mVisualConfig = pCreateInfo.mVisualConfig;
		vk10CreateInfo.mDisplayConfigFlags = pCreateInfo.mDisplayConfigFlags;

		auto * deviceVK = pDevice->QueryInterface<VKGpuDevice>();
		return VKScreenPresentationLayer::Create( deviceVK, vk10CreateInfo );
	}

} // namespace Ic3::Graphics::GCI
