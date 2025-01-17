
#include "GpuDriverNull.h"
#include "GpuDeviceNull.h"

namespace Ic3::Graphics::GCI
{

	GpuDriver::GpuDriver( System::SysContextHandle pSysContext ) noexcept
	: mSysContext( pSysContext )
	{}

	GpuDriver::~GpuDriver() noexcept = default;

	EGpuDriverID GpuDriver::QueryGpuDriverID() const noexcept
	{
		return EGpuDriverID::GDIUnknown;
	}

	bool GpuDriver::IsNullDriver() const noexcept
	{
		return false;
	}

	DisplayManagerHandle GpuDriver::CreateDefaultDisplayManager()
	{
		return nullptr;
	}

	GpuDeviceHandle GpuDriver::CreateDevice( const GpuDeviceCreateInfo & pCreateInfo )
	{
		auto gpuDevice = _DrvCreateDevice( pCreateInfo );
		if( !gpuDevice )
		{
			return nullptr;
		}

		gpuDevice->InitializeCommandSystem();

		return gpuDevice;
	}

	GpuDriver & GpuDriver::GetNullDriver()
	{
		static const GpuDriverHandle sNullDriverInstance = CreateGfxObject<GpuDriverNull>();
		return *sNullDriverInstance;
	}

	void GpuDriver::SetConfigFlags( cppx::bitmask<EGpuDriverConfigFlags> pConfigFlags )
	{
		_configFlags = pConfigFlags;
	}

} // namespace Ic3::Graphics::GCI
