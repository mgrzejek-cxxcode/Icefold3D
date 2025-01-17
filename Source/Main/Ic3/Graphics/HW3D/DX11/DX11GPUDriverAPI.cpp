
#include "DX11GpuDriverAPI.h"
#include "DX11GpuDriver.h"
#include "DX11GpuDevice.h"
#include "DX11presentationLayer.h"

namespace Ic3::Graphics::GCI
{

	GpuDriverHandle DX11GpuDriverInterface::CreateDriver( const GpuDriverCreateInfo & pCreateInfo )
	{
		DX11GpuDriverCreateInfo dx11CreateInfo;
		dx11CreateInfo.sysContext = pCreateInfo.sysContext;
		dx11CreateInfo.sysContextCreateInfo = pCreateInfo.sysContextCreateInfo;
		dx11CreateInfo.configFlags = pCreateInfo.configFlags;

		return DX11GpuDriver::Create( dx11CreateInfo );
	}

	PresentationLayerHandle DX11GpuDriverInterface::CreateScreenPresentationLayer( GpuDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		DX11PresentationLayerCreateInfo dx11CreateInfo;
		dx11CreateInfo.screenRect = pCreateInfo.screenRect;
		dx11CreateInfo.mVisualConfig = pCreateInfo.mVisualConfig;
		dx11CreateInfo.mDisplayConfigFlags = pCreateInfo.mDisplayConfigFlags;

		auto * deviceDX11 = pDevice.QueryInterface<DX11GpuDevice>();
		return DX11ScreenPresentationLayer::Create( *deviceDX11, dx11CreateInfo );
	}

} // namespace Ic3::Graphics::GCI
