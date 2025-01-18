
#include "VK10UDriverAPI.h"
#include "Ic3/GPUapiVK/VKUDriver.h"
#include "Ic3/GPUapiVK/VKUDevice.h"
#include <Ic3/GPUapiVK/system/VK_presentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GPUDriverHandle VK10GPUDriverInterface::CreateDriver( const GPUDriverCreateInfo & pCreateInfo )
	{
		VKGPUDriverCreateInfo vk10CreateInfo;
		vk10CreateInfo.exfSystemContext = pCreateInfo.exfSystemContext;
		vk10CreateInfo.initFlags = pCreateInfo.initFlags;

		return VKGPUDriver::Create( vk10CreateInfo );
	}

	PresentationLayerHandle VK10GPUDriverInterface::CreateScreenPresentationLayer( GPUDevice * pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		VKPresentationLayerCreateInfo vk10CreateInfo;
		vk10CreateInfo.coreEngineState = pCreateInfo.coreEngineState;
		vk10CreateInfo.screenRect = pCreateInfo.screenRect;
		vk10CreateInfo.mVisualConfig = pCreateInfo.mVisualConfig;
		vk10CreateInfo.mDisplayConfigFlags = pCreateInfo.mDisplayConfigFlags;

		auto * deviceVK = pDevice->QueryInterface<VKGPUDevice>();
		return VKScreenPresentationLayer::Create( deviceVK, vk10CreateInfo );
	}

} // namespace Ic3::Graphics::GCI
