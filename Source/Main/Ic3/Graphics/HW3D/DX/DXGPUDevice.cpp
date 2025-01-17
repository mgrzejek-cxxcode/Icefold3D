
#include "DXGpuDevice.h"
#include "DXGpuDriver.h"
#include "DXApiTranslationLayer.h"

namespace Ic3::Graphics::GCI
{

	DXGpuDevice::DXGpuDevice( DXGpuDriver & pDXGpuDriver, ComPtr<IDXGIFactory2> pDXGIFactory2 )
	: GpuDevice( pDXGpuDriver )
	, mDXGIFactory2( std::move( pDXGIFactory2 ) )
	, mDXGIDebug( ATL::QueryDXGIDebugInterface( pDXGpuDriver.GetConfigFlags() ) )
	, mDXGIInfoQueue( ATL::QueryDXGIDebugInfoQueue( pDXGpuDriver.GetConfigFlags() ) )
	{}

	DXGpuDevice::~DXGpuDevice() = default;

} // namespace Ic3::Graphics::GCI
