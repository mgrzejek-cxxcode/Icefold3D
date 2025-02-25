
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__
#define __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__

#include "GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	class GPUDeviceFeatureQueryNull : public GPUDeviceFeatureQuery
	{
	public:
		virtual MultiSamplingSettingsList EnumSupportedMultisamplingConfigs( ETextureFormat ) const noexcept override final
		{
			return {};
		}
	};

	class GPUDeviceNull : public GPUDevice
	{
	public:
		explicit GPUDeviceNull( GPUDriver & pDriver )
		: GPUDevice( pDriver, _featureQueryImpl )
		{}

		virtual ~GPUDeviceNull() = default;

		CPPX_ATTR_NO_DISCARD virtual bool IsNullDevice() const noexcept override final
		{
			return true;
		}

		virtual void WaitForCommandSync( CommandSync & ) override final
		{}

	protected:
		virtual bool OnGPUResourceActiveRefsZero( GPUResource & ) override final
		{
			return false;
		}

	private:
		virtual void InitializeCommandSystem() override final
		{}

		virtual bool _DrvOnSetPresentationLayer( PresentationLayerHandle ) override final
		{
			return false;
		}

		virtual GPUBufferHandle _DrvCreateGPUBuffer( const GPUBufferCreateInfo & ) override final
		{
			return nullptr;
		}

		virtual SamplerHandle _DrvCreateSampler( const SamplerCreateInfo & ) override final
		{
			return nullptr;
		}

		virtual ShaderHandle _DrvCreateShader( const ShaderCreateInfo & ) override final
		{
			return nullptr;
		}

		virtual TextureHandle _DrvCreateTexture( const TextureCreateInfo & ) override final
		{
			return nullptr;
		}

		virtual RenderTargetTextureHandle _DrvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & ) override final
		{
			return nullptr;
		}

		virtual GraphicsPipelineStateObjectHandle _DrvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & ) override final
		{
			return nullptr;
		}

	private:
		GPUDeviceFeatureQueryNull _featureQueryImpl;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__
