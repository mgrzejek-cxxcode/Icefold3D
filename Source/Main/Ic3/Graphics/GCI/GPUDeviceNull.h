
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

		CPPX_ATTR_NO_DISCARD virtual bool IsNullDevice() const noexcept override final
		{
			return true;
		}

		virtual void WaitForCommandSync( CommandSync & pCommandSync ) override final
		{}

	protected:
		virtual bool OnGPUResourceActiveRefsZero( GPUResource & pGPUResource ) override final
		{
			return false;
		}

	private:
		virtual void InitializeCommandSystem() override final
		{}

		virtual bool _DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer ) override final
		{
			return false;
		}

		virtual GPUBufferHandle _DrvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual SamplerHandle _DrvCreateSampler( const SamplerCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual ShaderHandle _DrvCreateShader( const ShaderCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual TextureHandle _DrvCreateTexture( const TextureCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual RenderTargetTextureHandle _DrvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}

		virtual GraphicsPipelineStateObjectHandle _DrvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__
