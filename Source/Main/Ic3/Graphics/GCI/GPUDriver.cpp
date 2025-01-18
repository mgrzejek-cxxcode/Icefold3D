
#include "GPUDriverNull.h"
#include "GPUDeviceNull.h"

namespace Ic3::Graphics::GCI
{

	GPUDriver::GPUDriver( System::SysContextHandle pSysContext ) noexcept
	: mSysContext( pSysContext )
	{}

	GPUDriver::~GPUDriver() noexcept = default;

	EGPUDriverID GPUDriver::QueryGPUDriverID() const noexcept
	{
		return EGPUDriverID::GDIUnknown;
	}

	bool GPUDriver::IsNullDriver() const noexcept
	{
		return false;
	}

	DisplayManagerHandle GPUDriver::CreateDefaultDisplayManager()
	{
		return nullptr;
	}

	GPUDeviceHandle GPUDriver::CreateDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		auto gpuDevice = _DrvCreateDevice( pCreateInfo );
		if( !gpuDevice )
		{
			return nullptr;
		}

		gpuDevice->InitializeCommandSystem();

		return gpuDevice;
	}

	GPUDriver & GPUDriver::GetNullDriver()
	{
		static const GPUDriverHandle sNullDriverInstance = CreateGfxObject<GPUDriverNull>();
		return *sNullDriverInstance;
	}

	void GPUDriver::SetConfigFlags( cppx::bitmask<EGPUDriverConfigFlags> pConfigFlags )
	{
		_configFlags = pConfigFlags;
	}

} // namespace Ic3::Graphics::GCI
