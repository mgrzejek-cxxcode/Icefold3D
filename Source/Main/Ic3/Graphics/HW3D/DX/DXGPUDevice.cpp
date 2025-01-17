
#include "DXGPUDevice.h"
#include "DXGPUDriver.h"
#include "DXApiTranslationLayer.h"

namespace Ic3::Graphics::GCI
{

	DXGPUDevice::DXGPUDevice( DXGPUDriver & pDXGPUDriver, ComPtr<IDXGIFactory2> pDXGIFactory2 )
	: GPUDevice( pDXGPUDriver )
	, mDXGIFactory2( std::move( pDXGIFactory2 ) )
	, mDXGIDebug( ATL::QueryDXGIDebugInterface( pDXGPUDriver.GetConfigFlags() ) )
	, mDXGIInfoQueue( ATL::QueryDXGIDebugInfoQueue( pDXGPUDriver.GetConfigFlags() ) )
	{}

	DXGPUDevice::~DXGPUDevice() = default;

} // namespace Ic3::Graphics::GCI
