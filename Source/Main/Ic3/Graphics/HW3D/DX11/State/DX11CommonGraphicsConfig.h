
#ifndef __IC3_GRAPHICS_HW3D_DX11_COMMON_GRAPHICS_CONFIG_H__
#define __IC3_GRAPHICS_HW3D_DX11_COMMON_GRAPHICS_CONFIG_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorCommon.h>

namespace Ic3::Graphics::GCI
{

	struct DX11BlendSettings : public D3D11_BLEND_DESC
	{
		Math::RGBAColorR32Norm ConstantFactor;
	};

	struct DX11DepthStencilSettings : public D3D11_DEPTH_STENCIL_DESC
	{};

	struct DX11RasterizerSettings : public D3D11_RASTERIZER_DESC
	{};


	///
	class DX11BlendStateDescriptor : public BlendStateDescriptor
	{
	public:
		ComPtr<ID3D11BlendState> const mD3D11BlendState;

	public:
		DX11BlendStateDescriptor(
				DX11GPUDevice & pGPUDevice,
				cppx::bitmask<EBlendConfigFlags> pBlendFlags,
				ComPtr<ID3D11BlendState> pD3D11BlendState );

		virtual ~DX11BlendStateDescriptor();

		static GpaHandle<DX11BlendStateDescriptor> CreateInstance( DX11GPUDevice & pGPUDevice, const BlendSettings & pBlendSettings );
	};

	///
	class DX11DepthStencilStateDescriptor : public DepthStencilStateDescriptor
	{
	public:
		ComPtr<ID3D11DepthStencilState> const mD3D11DepthStencilState;

	public:
		DX11DepthStencilStateDescriptor(
				DX11GPUDevice & pGPUDevice,
				cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
				ComPtr<ID3D11DepthStencilState> pD3D11DepthStencilState );

		virtual ~DX11DepthStencilStateDescriptor();

		static GpaHandle<DX11DepthStencilStateDescriptor> CreateInstance( DX11GPUDevice & pGPUDevice, const DepthStencilSettings & pDepthStencilSettings );
	};

	///
	class DX11RasterizerStateDescriptor : public RasterizerStateDescriptor
	{
	public:
		ComPtr<ID3D11RasterizerState> const mD3D11RasterizerState;

	public:
		DX11RasterizerStateDescriptor(
				DX11GPUDevice & pGPUDevice,
				cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags,
				ComPtr<ID3D11RasterizerState> pD3D11RasterizerState );

		virtual ~DX11RasterizerStateDescriptor();

		static GpaHandle<DX11RasterizerStateDescriptor> CreateInstance( DX11GPUDevice & pGPUDevice, const RasterizerSettings & pRasterizerSettings );
	};


	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD DX11BlendSettings TranslateDX11BlendSettings( const BlendSettings & pSettings );

		CPPX_ATTR_NO_DISCARD DX11DepthStencilSettings TranslateDX11DepthStencilSettings( const DepthStencilSettings & pSettings );

		CPPX_ATTR_NO_DISCARD DX11RasterizerSettings TranslateDX11RasterizerSettings( const RasterizerSettings & pSettings );

		CPPX_ATTR_NO_DISCARD D3D11_RENDER_TARGET_BLEND_DESC TranslateDX11RenderTargetColorAttachmentBlendSettings(
			const RenderTargetColorAttachmentBlendSettings & pSettings );

	}

}

#endif // __IC3_GRAPHICS_HW3D_DX11_COMMON_GRAPHICS_CONFIG_H__
