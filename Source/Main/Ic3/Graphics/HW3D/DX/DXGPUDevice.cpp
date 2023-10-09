
#include "DXGPUDevice.h"
#include "DXGPUDriver.h"
#include "DXAPITranslationLayer.h"

namespace Ic3::Graphics::GCI
{

	DXGPUDevice::DXGPUDevice( DXGPUDriver & pDXGPUDriver, ComPtr<IDXGIFactory2> pDXGIFactory2 )
	: GPUDevice( pDXGPUDriver )
	, mDXGIFactory2( std::move( pDXGIFactory2 ) )
	, mDXGIDebug( ATL::queryDXGIDebugInterface( pDXGPUDriver.getConfigFlags() ) )
	, mDXGIInfoQueue( ATL::queryDXGIDebugInfoQueue( pDXGPUDriver.getConfigFlags() ) )
	{}

	DXGPUDevice::~DXGPUDevice() = default;

} // namespace Ic3::Graphics::GCI
