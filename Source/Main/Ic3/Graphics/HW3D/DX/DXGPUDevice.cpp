
#include "DXGPUDevice.h"
#include "DXGPUDriver.h"
#include "DXAPITranslationLayer.h"

namespace Ic3::Graphics::GCI
{

	DXGPUDevice::DXGPUDevice(
		DXGPUDriver & pDXGPUDriver,
		GPUDeviceFeatureQuery * pFeatureQueryInterface,
		PipelineStateDescriptorManager * pPipelineStateDescriptorManager,
		ComPtr<IDXGIFactory2> pDXGIFactory2 )
	: GPUDevice( pDXGPUDriver, pFeatureQueryInterface, pPipelineStateDescriptorManager )
	, mDXGIFactory2( std::move( pDXGIFactory2 ) )
	, mDXGIDebug( ATL::DXGIQueryDebugInterface( pDXGPUDriver.GetConfigFlags() ) )
	, mDXGIInfoQueue( ATL::DXGIQueryDebugInfoQueue( pDXGPUDriver.GetConfigFlags() ) )
	{}

	DXGPUDevice::~DXGPUDevice() = default;

} // namespace Ic3::Graphics::GCI
