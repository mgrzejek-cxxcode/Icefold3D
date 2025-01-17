
#include "MTLGpuDevice.h"
#include "MTLGpuDriver.h"

namespace Ic3::Graphics::GCI
{

	MetalGpuDevice::MetalGpuDevice( MetalGpuDriver & pGpuDriver, System::MetalDeviceHandle pSysMetalDevice )
	: GpuDevice( pGpuDriver )
	, mSysMetalDevice( std::move( pSysMetalDevice ) )
	, mMTLDevice( mSysMetalDevice->mDeviceData->mtlDevice )
	, _immutableStateFactoryGL( *this )
	, _immutableStateCache( _immutableStateFactoryGL )
	{}

	MetalGpuDevice::~MetalGpuDevice() = default;

	void MetalGpuDevice::WaitForCommandSync( CommandSync & pCommandSync )
	{}

	void MetalGpuDevice::InitializeCommandSystem()
	{}

	bool MetalGpuDevice::_DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{}

	GpuBufferHandle MetalGpuDevice::_DrvCreateGpuBuffer( const GpuBufferCreateInfo & pCreateInfo )
	{}

	SamplerHandle MetalGpuDevice::_DrvCreateSampler( const SamplerCreateInfo & pCreateInfo )
	{}

	ShaderHandle MetalGpuDevice::_DrvCreateShader( const ShaderCreateInfo & pCreateInfo )
	{}

	TextureHandle MetalGpuDevice::_DrvCreateTexture( const TextureCreateInfo & pCreateInfo )
	{}

	RenderTargetTextureHandle MetalGpuDevice::_DrvCreateRenderTargetTexture(
			const RenderTargetTextureCreateInfo & pCreateInfo )
	{}

	GraphicsPipelineStateObjectHandle MetalGpuDevice::_DrvCreateGraphicsPipelineStateObject(
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{}
	
}
