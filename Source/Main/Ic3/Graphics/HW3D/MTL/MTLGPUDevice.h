
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_DEVICE_H__

#include "MTLPrerequisites.h"
#include "State/MTL_pipelineImmutableStateFactory.h"
#include <Ic3/Graphics/GCI/GpuDevice.h>
#include <Ic3/Graphics/GCI/State/PipelineImmutableStateCache.h>

namespace Ic3::Graphics::GCI
{

	/// @brief
	class MetalGpuDevice : public GpuDevice
	{
	public:
		System::MetalDeviceHandle const mSysMetalDevice;

		id<MTLDevice> const mMTLDevice;

	public:
		explicit MetalGpuDevice( MetalGpuDriver & pGpuDriver, System::MetalDeviceHandle pSysMetalDevice );
		virtual ~MetalGpuDevice();

		virtual void WaitForCommandSync( CommandSync & pCommandSync ) override;

	protected:
		virtual void InitializeCommandSystem() override;

	private:
	    virtual bool _DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer ) override;

	    virtual GpuBufferHandle _DrvCreateGpuBuffer( const GpuBufferCreateInfo & pCreateInfo ) override final;
	    virtual SamplerHandle _DrvCreateSampler( const SamplerCreateInfo & pCreateInfo ) override final;
	    virtual ShaderHandle _DrvCreateShader( const ShaderCreateInfo & pCreateInfo ) override final;
	    virtual TextureHandle _DrvCreateTexture( const TextureCreateInfo & pCreateInfo ) override final;

		virtual RenderTargetTextureHandle _DrvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & pCreateInfo ) override final;

		virtual GraphicsPipelineStateObjectHandle _DrvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo ) override final;

	private:
		MetalPipelineImmutableStateFactory _immutableStateFactoryGL;
		PipelineImmutableStateCache _immutableStateCache;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_DEVICE_H__
