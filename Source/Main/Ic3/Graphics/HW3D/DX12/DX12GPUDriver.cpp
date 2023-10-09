
#include "DX12GPUDriver.h"
#include "DX12GPUDevice.h"
#include <Ic3/System/DisplayDriverNative.h>

namespace Ic3::Graphics::GCI
{

	DX12GPUDriver::DX12GPUDriver( System::temContext * pExfSystemContext )
	: DXGPUDriver( pExfSystemContext )
	{}

	DX12GPUDriver::~DX12GPUDriver()
	{}

	DX12GPUDriverHandle DX12GPUDriver::create( const DX12GPUDriverCreateInfo & pCreateInfo )
	{
		constexpr auto dx12SupportedGPUConfigFlags =
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_SHADER_DEBUG_INFO_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_DISABLE_MULTI_THREAD_ACCESS_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_USE_REFERENCE_DRIVER_BIT;

		auto dx12GPUDriver = createGPUAPIObject<DX12GPUDriver>( pCreateInfo.exfSystemContext );
		dx12GPUDriver->setConfigFlags( pCreateInfo.configFlags & dx12SupportedGPUConfigFlags );

		return dx12GPUDriver;
	}

	DisplayManagerHandle DX12GPUDriver::createDefaultDisplayManager()
	{
		auto * exfDisplayManager = mExfSystemContext->getDisplayManager();

		System::DisplayDriverCreateInfoDXGI dxgiDriverCreateInfo;

		return nullptr;
	}

	GPUDeviceHandle DX12GPUDriver::createDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		DX12GPUDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;
		return DX12GPUDevice::create( *this, createInfo );
	}

} // namespace Ic3::Graphics::GCI
