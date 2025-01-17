

#include "DXGPUDriver.h"

namespace Ic3::Graphics::GCI
{

	DXGPUDriver::DXGPUDriver( System::SysContextHandle pSysContext ) noexcept
	: GPUDriver( pSysContext )
	{}

	DXGPUDriver::~DXGPUDriver() noexcept = default;

} // namespace Ic3::Graphics::GCI
