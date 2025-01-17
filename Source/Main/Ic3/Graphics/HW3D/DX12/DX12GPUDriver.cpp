
#include "DX12GPUDriver.h"
#include "DX12GPUDevice.h"
#include <Ic3/System/DisplayDriverNative.h>

namespace Ic3::Graphics::GCI
{

	DX12GPUDriver::DX12GPUDriver( System::SysContext * pExfSystemContext )
	: DXGPUDriver( pExfSystemContext )
	{}

	DX12GPUDriver::~DX12GPUDriver()
	{}

	DX12GPUDriverHandle DX12GPUDriver::Create( const DX12GPUDriverCreateInfo & pCreateInfo )
	{
		constexpr auto dx12SupportedGPUConfigFlags =
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_SHADER_DEBUG_INFO_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_DISABLE_MULTI_THREAD_ACCESS_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_USE_REFERENCE_DRIVER_BIT;

		auto dx12GPUDriver = CreateGfxObject<DX12GPUDriver>( pCreateInfo.exfSystemContext );
		dx12GPUDriver->SetConfigFlags( pCreateInfo.configFlags & dx12SupportedGPUConfigFlags );

		return dx12GPUDriver;
	}

	DisplayManagerHandle DX12GPUDriver::CreateDefaultDisplayManager()
	{
		auto * exfDisplayManager = mExfSystemContext->GetDisplayManager();

		System::DisplayDriverCreateInfoDXGI dxgiDriverCreateInfo;

		return nullptr;
	}

	GPUDeviceHandle DX12GPUDriver::CreateDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		DX12GPUDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;
		return DX12GPUDevice::Create( *this, createInfo );
	}

} // namespace Ic3::Graphics::GCI
