
#include "DX12GpuDriver.h"
#include "DX12GpuDevice.h"
#include <Ic3/System/DisplayDriverNative.h>

namespace Ic3::Graphics::GCI
{

	DX12GpuDriver::DX12GpuDriver( System::SysContext * pExfSystemContext )
	: DXGpuDriver( pExfSystemContext )
	{}

	DX12GpuDriver::~DX12GpuDriver()
	{}

	DX12GpuDriverHandle DX12GpuDriver::Create( const DX12GpuDriverCreateInfo & pCreateInfo )
	{
		constexpr auto dx12SupportedGpuConfigFlags =
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_SHADER_DEBUG_INFO_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_DISABLE_MULTI_THREAD_ACCESS_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_USE_REFERENCE_DRIVER_BIT;

		auto dx12GpuDriver = CreateGfxObject<DX12GpuDriver>( pCreateInfo.exfSystemContext );
		dx12GpuDriver->SetConfigFlags( pCreateInfo.configFlags & dx12SupportedGpuConfigFlags );

		return dx12GpuDriver;
	}

	DisplayManagerHandle DX12GpuDriver::CreateDefaultDisplayManager()
	{
		auto * exfDisplayManager = mExfSystemContext->GetDisplayManager();

		System::DisplayDriverCreateInfoDXGI dxgiDriverCreateInfo;

		return nullptr;
	}

	GpuDeviceHandle DX12GpuDriver::CreateDevice( const GpuDeviceCreateInfo & pCreateInfo )
	{
		DX12GpuDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;
		return DX12GpuDevice::Create( *this, createInfo );
	}

} // namespace Ic3::Graphics::GCI
