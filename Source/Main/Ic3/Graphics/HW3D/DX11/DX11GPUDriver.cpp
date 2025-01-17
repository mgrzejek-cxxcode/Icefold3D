
#include "DX11GpuDriver.h"
#include "DX11GpuDevice.h"

namespace Ic3::Graphics::GCI
{

	DX11GpuDriver::DX11GpuDriver( System::SysContextHandle pSysContext ) noexcept
	: DXGpuDriver( pSysContext )
	{}

	DX11GpuDriver::~DX11GpuDriver() noexcept = default;

	DX11GpuDriverHandle DX11GpuDriver::Create( const DX11GpuDriverCreateInfo & pCreateInfo )
	{
		constexpr auto dx11SupportedGpuConfigFlags =
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_ENABLE_SHADER_DEBUG_INFO_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_DISABLE_MULTI_THREAD_ACCESS_BIT |
			E_GPU_DRIVER_CONFIG_FLAG_USE_REFERENCE_DRIVER_BIT;

		auto dx11GpuDriver = CreateGfxObject<DX11GpuDriver>( pCreateInfo.sysContext );
		dx11GpuDriver->SetConfigFlags( pCreateInfo.configFlags & dx11SupportedGpuConfigFlags );
		return dx11GpuDriver;
	}

	EGpuDriverID DX11GpuDriver::QueryGpuDriverID() const noexcept
	{
		return EGpuDriverID::GDIDirectX11;
	}

	DisplayManagerHandle DX11GpuDriver::_DrvCreateDefaultDisplayManager()
	{
		return nullptr;
	}

	GpuDeviceHandle DX11GpuDriver::_DrvCreateDevice( const GpuDeviceCreateInfo & pCreateInfo )
	{
		DX11GpuDeviceCreateInfo createInfo;
		createInfo.adapterID = pCreateInfo.adapterID;
		createInfo.flags = pCreateInfo.flags;
		return DX11GpuDevice::Create( *this, createInfo );
	}

} // namespace Ic3::Graphics::GCI
