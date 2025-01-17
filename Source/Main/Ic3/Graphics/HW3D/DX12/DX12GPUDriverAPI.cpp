
#include "DX12GpuDriverAPI.h"
#include "DX12GpuDriver.h"
#include "DX12GpuDevice.h"
#include <Ic3/Graphics/HW3D/DX12/system/DX12_presentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GpuDriverHandle DX12GpuDriverInterface::CreateDriver( const GpuDriverCreateInfo & pCreateInfo )
	{
		DX12GpuDriverCreateInfo dx12CreateInfo;
		dx12CreateInfo.exfSystemContext = pCreateInfo.exfSystemContext;
		dx12CreateInfo.configFlags = pCreateInfo.configFlags;

		return DX12GpuDriver::Create( dx12CreateInfo );
	}

	PresentationLayerHandle DX12GpuDriverInterface::CreateScreenPresentationLayer( GpuDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		DX12PresentationLayerCreateInfo dx12CreateInfo;
		dx12CreateInfo.coreEngineState = pCreateInfo.coreEngineState;
		dx12CreateInfo.screenRect = pCreateInfo.screenRect;
		dx12CreateInfo.mVisualConfig = pCreateInfo.mVisualConfig;
		dx12CreateInfo.mDisplayConfigFlags = pCreateInfo.mDisplayConfigFlags;

		if( pDevice.IsDebugDevice() )
		{
			dx12CreateInfo.dxgiFlags.set( DXGI_CREATE_FACTORY_DEBUG );
		}

		auto * deviceDX12 = pDevice.QueryInterface<DX12GpuDevice>();
		return DX12ScreenPresentationLayer::Create( *deviceDX12, dx12CreateInfo );
	}

} // namespace Ic3::Graphics::GCI
