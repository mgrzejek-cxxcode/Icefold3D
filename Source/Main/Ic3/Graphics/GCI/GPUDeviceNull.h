
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__
#define __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__

#include "GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	class GPUDeviceNull : public GPUDevice
	{
	public:
		explicit GPUDeviceNull( GPUDriver & pDriver )
		: GPUDevice( pDriver )
		{}

		virtual ~GPUDeviceNull() = default;

		IC3_ATTR_NO_DISCARD virtual bool isNullDevice() const noexcept override final
		{
			return true;
		}

		virtual void waitForCommandSync( CommandSync & pCommandSync ) override final
		{}

	protected:
		virtual bool onGPUResourceActiveRefsZero( GPUResource & pGPUResource ) override final
		{
			return false;
		}

	private:
		virtual void initializeCommandSystem() override final
		{}

		virtual bool _drvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer ) override final
		{
			return false;
		}

		virtual GPUBufferHandle _drvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual SamplerHandle _drvCreateSampler( const SamplerCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual ShaderHandle _drvCreateShader( const ShaderCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual TextureHandle _drvCreateTexture( const TextureCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual RenderTargetTextureHandle _drvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual GraphicsPipelineStateObjectHandle _drvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__
