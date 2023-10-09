
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_DEVICE_H__

#include "MTLPrerequisites.h"
#include "State/MTL_pipelineImmutableStateFactory.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <Ic3/Graphics/GCI/State/PipelineImmutableStateCache.h>

namespace Ic3::Graphics::GCI
{

	/// @brief
	class MetalGPUDevice : public GPUDevice
	{
	public:
		System::MetalDeviceHandle const mSysMetalDevice;

		id<MTLDevice> const mMTLDevice;

	public:
		explicit MetalGPUDevice( MetalGPUDriver & pGPUDriver, System::MetalDeviceHandle pSysMetalDevice );
		virtual ~MetalGPUDevice();

		virtual void waitForCommandSync( CommandSync & pCommandSync ) override;

	protected:
		virtual void initializeCommandSystem() override;

	private:
	    virtual bool _drvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer ) override;

	    virtual GPUBufferHandle _drvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo ) override final;
	    virtual SamplerHandle _drvCreateSampler( const SamplerCreateInfo & pCreateInfo ) override final;
	    virtual ShaderHandle _drvCreateShader( const ShaderCreateInfo & pCreateInfo ) override final;
	    virtual TextureHandle _drvCreateTexture( const TextureCreateInfo & pCreateInfo ) override final;

		virtual RenderTargetTextureHandle _drvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & pCreateInfo ) override final;

		virtual GraphicsPipelineStateObjectHandle _drvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo ) override final;

	private:
		MetalPipelineImmutableStateFactory _immutableStateFactoryGL;
		PipelineImmutableStateCache _immutableStateCache;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_DEVICE_H__
