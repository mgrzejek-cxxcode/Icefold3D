
#include "MTLGPUDevice.h"
#include "MTLGPUDriver.h"

namespace Ic3::Graphics::GCI
{

	MetalGPUDevice::MetalGPUDevice( MetalGPUDriver & pGPUDriver, System::MetalDeviceHandle pSysMetalDevice )
	: GPUDevice( pGPUDriver )
	, mSysMetalDevice( std::move( pSysMetalDevice ) )
	, mMTLDevice( mSysMetalDevice->mDeviceData->mtlDevice )
	, _glcPipelineStateDescriptorFactory( *this )
	, _stateDescriptorCache( _glcPipelineStateDescriptorFactory )
	{}

	MetalGPUDevice::~MetalGPUDevice() = default;

	void MetalGPUDevice::WaitForCommandSync( CommandSync & pCommandSync )
	{}

	void MetalGPUDevice::InitializeCommandSystem()
	{}

	bool MetalGPUDevice::_DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{}

	GPUBufferHandle MetalGPUDevice::_DrvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo )
	{}

	SamplerHandle MetalGPUDevice::_DrvCreateSampler( const SamplerCreateInfo & pCreateInfo )
	{}

	ShaderHandle MetalGPUDevice::_DrvCreateShader( const ShaderCreateInfo & pCreateInfo )
	{}

	TextureHandle MetalGPUDevice::_DrvCreateTexture( const TextureCreateInfo & pCreateInfo )
	{}

	RenderTargetTextureHandle MetalGPUDevice::_DrvCreateRenderTargetTexture(
			const RenderTargetTextureCreateInfo & pCreateInfo )
	{}

	GraphicsPipelineStateObjectHandle MetalGPUDevice::_DrvCreateGraphicsPipelineStateObject(
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{}
	
}
