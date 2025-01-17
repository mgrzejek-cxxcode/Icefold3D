

#include "DXGpuDriver.h"

namespace Ic3::Graphics::GCI
{

	DXGpuDriver::DXGpuDriver( System::SysContextHandle pSysContext ) noexcept
	: GpuDriver( pSysContext )
	{}

	DXGpuDriver::~DXGpuDriver() noexcept = default;

} // namespace Ic3::Graphics::GCI
