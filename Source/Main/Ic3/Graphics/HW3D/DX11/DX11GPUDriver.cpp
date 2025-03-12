
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
			eGPUDriverConfigFlagEnableDebugLayerBit |
			eGPUDriverConfigFlagEnableShaderDebugInfoBit |
			eGPUDriverConfigFlagDisableMultiThreadAccessBit |
			eGPUDriverConfigFlagUseReferenceDriverBit;

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
