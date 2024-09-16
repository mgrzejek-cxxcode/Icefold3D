
#include "GPUDriverNull.h"
#include "GPUDeviceNull.h"

namespace Ic3::Graphics::GCI
{

	GPUDriver::GPUDriver( System::SysContextHandle pSysContext ) noexcept
	: mSysContext( pSysContext )
	{}

	GPUDriver::~GPUDriver() noexcept = default;

	EGPUDriverID GPUDriver::queryGPUDriverID() const noexcept
	{
		return EGPUDriverID::GDIUnknown;
	}

	bool GPUDriver::isNullDriver() const noexcept
	{
		return false;
	}

	DisplayManagerHandle GPUDriver::createDefaultDisplayManager()
	{
		return nullptr;
	}

	GPUDeviceHandle GPUDriver::createDevice( const GPUDeviceCreateInfo & pCreateInfo )
	{
		auto gpuDevice = _drvCreateDevice( pCreateInfo );
		if( !gpuDevice )
		{
			return nullptr;
		}

		gpuDevice->initializeCommandSystem();

		return gpuDevice;
	}

	GPUDriver & GPUDriver::nullDriver()
	{
		static const GPUDriverHandle sNullDriverInstance = createGPUAPIObject<GPUDriverNull>();
		return *sNullDriverInstance;
	}

	void GPUDriver::setConfigFlags( TBitmask<EGPUDriverConfigFlags> pConfigFlags )
	{
		_configFlags = pConfigFlags;
	}

} // namespace Ic3::Graphics::GCI
