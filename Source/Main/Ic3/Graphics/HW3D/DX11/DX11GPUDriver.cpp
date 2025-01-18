
#include "DX11GPUDriver.h"
#include "DX11GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	DX11GPUDriver::DX11GPUDriver( System::SysContextHandle pSysContext ) noexcept
	: DXGPUDriver( pSysContext )
	{}

	DX11GPUDriver::~DX11GPUDriver() noexcept = default;

	DX11GPUDriverHandle DX11GPUDriver::Create( const DX11GPUDriverCreateInfo & pCreateInfo )
	{
		constexpr auto dx11SupportedGPUConfigFlags =
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_SHADER_DEBUG_INFO_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_DISABLE_MULTI_THREAD_ACCESS_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_USE_REFERENCE_DRIVER_BIT;

		auto dx11GPUDriver = CreateGfxObject<DX11GPUDriver>( pCreateInfo.sysContext );
		dx11GPUDriver->SetConfigFlags( pCreateInfo.configFlags & dx11SupportedGPUConfigFlags );
		return dx11GPUDriver;
	}

	EGPUDriverID DX11GPUDriver::QueryGPUDriverID() const noexcept
	{
		return EGPUDriverID::GDIDirectX11;
	}

	DisplayManagerHandle DX11GPUDriver::_DrvCreateDefaultDisplayManager()
	{
		return nullptr;
	}

	GPUDeviceHandle DX11GPUDriver::_DrvCreateDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		DX11GPUDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;
		return DX11GPUDevice::Create( *this, createInfo );
	}

} // namespace Ic3::Graphics::GCI
