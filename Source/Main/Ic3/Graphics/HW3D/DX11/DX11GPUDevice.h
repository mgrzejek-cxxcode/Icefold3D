
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_DX11_GPU_DEVICE_H__

#include "DX11Prerequisites.h"
#include "State/DX11pipelineImmutableStateFactory.h"
#include <Ic3/Graphics/HW3D/DX/DXgpuDevice.h>
#include <Ic3/Graphics/GCI/State/PipelineImmutableStateCache.h>

namespace Ic3::Graphics::GCI
{

	struct DX11GPUDeviceCreateInfo : public GPUDeviceCreateInfo
	{
	};

	/// @brief
	class IC3_GX_DX11_CLASS DX11GPUDevice : public DXGPUDevice
	{
		friend class DX11CommandList;
		friend class DX11GraphicsPipelineStateObject;

	public:
		ComPtr<ID3D11Device1> const mD3D11Device1;
		ComPtr<ID3D11Debug> const mD3D11DebugInterface;

		explicit DX11GPUDevice(
				DX11GPUDriver & pDriver,
				ComPtr<ID3D11Device1> pD3D11Device1,
				ComPtr<ID3D11Debug> pD3D11Debug );

		virtual ~DX11GPUDevice();

		void WaitForCommandSync( CommandSync & pCommandSync ) override final;

		static DX11GPUDeviceHandle Create( DX11GPUDriver & pDX11Driver, const DX11GPUDeviceCreateInfo & pCreateInfo );

	private:
		virtual void InitializeCommandSystem() override;

		virtual GPUBufferHandle _DrvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo ) override final;
		virtual SamplerHandle _DrvCreateSampler( const SamplerCreateInfo & pCreateInfo ) override final;
		virtual ShaderHandle _DrvCreateShader( const ShaderCreateInfo & pCreateInfo ) override final;
		virtual TextureHandle _DrvCreateTexture( const TextureCreateInfo & pCreateInfo ) override final;

		virtual RenderTargetTextureHandle _DrvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & pCreateInfo ) override final;

		virtual GraphicsPipelineStateObjectHandle _DrvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo ) override final;

	private:
		DX11PipelineImmutableStateFactory _immutableStateFactoryDX11;
		PipelineImmutableStateCache _immutableStateCache;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_GPU_DEVICE_H__
