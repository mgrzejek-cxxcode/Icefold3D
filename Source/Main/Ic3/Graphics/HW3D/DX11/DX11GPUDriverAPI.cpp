
#include "DX11GPUDriverAPI.h"
#include "DX11GPUDriver.h"
#include "DX11GPUDevice.h"
#include "DX11presentationLayer.h"

namespace Ic3::Graphics::GCI
{

	GPUDriverHandle DX11GPUDriverInterface::CreateDriver( const GPUDriverCreateInfo & pCreateInfo )
	{
		DX11GPUDriverCreateInfo dx11CreateInfo;
		dx11CreateInfo.sysContext = pCreateInfo.sysContext;
		dx11CreateInfo.sysContextCreateInfo = pCreateInfo.sysContextCreateInfo;
		dx11CreateInfo.configFlags = pCreateInfo.configFlags;

		return DX11GPUDriver::Create( dx11CreateInfo );
	}

	PresentationLayerHandle DX11GPUDriverInterface::CreateScreenPresentationLayer( GPUDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		DX11PresentationLayerCreateInfo dx11CreateInfo;
		dx11CreateInfo.screenRect = pCreateInfo.screenRect;
		dx11CreateInfo.mVisualConfig = pCreateInfo.mVisualConfig;
		dx11CreateInfo.mDisplayConfigFlags = pCreateInfo.mDisplayConfigFlags;

		auto * deviceDX11 = pDevice.QueryInterface<DX11GPUDevice>();
		return DX11ScreenPresentationLayer::Create( *deviceDX11, dx11CreateInfo );
	}

} // namespace Ic3::Graphics::GCI
