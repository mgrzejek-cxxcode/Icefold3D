
#include "DX11GraphicsPipelineStateCommon.h"
#include "../DX11APITranslationLayer.h"
#include "../DX11gpuDevice.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	DX11BlendStateDescriptor::DX11BlendStateDescriptor(
			DX11GPUDevice & pGPUDevice,
			cppx::bitmask<EBlendConfigFlags> pBlendFlags,
			ComPtr<ID3D11BlendState> pD3D11BlendState,
			const cxm::rgba_color_r32_norm & pConstantFactor )
	: HW3DPipelineStateDescriptor( pGPUDevice, pBlendFlags )
	, mD3D11BlendState( pD3D11BlendState )
	, mConstantFactor( pConstantFactor )
	{}

	DX11BlendStateDescriptor::~DX11BlendStateDescriptor() = default;

	TGfxHandle<DX11BlendStateDescriptor> DX11BlendStateDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		const auto dx11BlendSettings = GCU::TranslateBlendSettingsDX11( pCreateInfo.blendSettings );

		ComPtr<ID3D11BlendState> d3d11BlendState;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateBlendState(
			&dx11BlendSettings,
			d3d11BlendState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto stateDescriptor = CreateGfxObject<DX11BlendStateDescriptor>(
				pGPUDevice,
				pCreateInfo.blendSettings.flags,
				d3d11BlendState,
				dx11BlendSettings.ConstantFactor );

		return stateDescriptor;
	}


	DX11DepthStencilStateDescriptor::DX11DepthStencilStateDescriptor(
			DX11GPUDevice & pGPUDevice,
			cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
			ComPtr<ID3D11DepthStencilState> pD3D11DepthStencilState )
	: HW3DPipelineStateDescriptor( pGPUDevice, pDepthStencilFlags )
	, mD3D11DepthStencilState( pD3D11DepthStencilState )
	{}

	DX11DepthStencilStateDescriptor::~DX11DepthStencilStateDescriptor() = default;

	TGfxHandle<DX11DepthStencilStateDescriptor> DX11DepthStencilStateDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		const auto dx11DepthStencilSettings = GCU::TranslateDepthStencilSettingsDX11( pCreateInfo.depthStencilSettings );

		ComPtr<ID3D11DepthStencilState> d3d11DepthStencilState;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateDepthStencilState(
			&dx11DepthStencilSettings,
			d3d11DepthStencilState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto stateDescriptor = CreateGfxObject<DX11DepthStencilStateDescriptor>(
				pGPUDevice,
				pCreateInfo.depthStencilSettings.commonFlags,
				d3d11DepthStencilState );

		return stateDescriptor;
	}


	DX11RasterizerStateDescriptor::DX11RasterizerStateDescriptor(
			DX11GPUDevice & pGPUDevice,
			cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags,
			ComPtr<ID3D11RasterizerState> pD3D11RasterizerState )
	: HW3DPipelineStateDescriptor( pGPUDevice, pRasterizerFlags )
	, mD3D11RasterizerState( pD3D11RasterizerState )
	{}

	DX11RasterizerStateDescriptor::~DX11RasterizerStateDescriptor() = default;

	TGfxHandle<DX11RasterizerStateDescriptor> DX11RasterizerStateDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{

		const auto dx11RasterizerSettings = GCU::TranslateRasterizerSettingsDX11( pCreateInfo.rasterizerSettings );

		ComPtr<ID3D11RasterizerState> d3d11RasterizerState;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateRasterizerState(
			&dx11RasterizerSettings,
			d3d11RasterizerState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto stateDescriptor = CreateGfxObject<DX11RasterizerStateDescriptor>(
				pGPUDevice,
				pCreateInfo.rasterizerSettings.flags,
				d3d11RasterizerState );

		return stateDescriptor;
	}

	namespace GCU
	{

		DX11BlendSettings TranslateBlendSettingsDX11( const BlendSettings & pBlendSettings )
		{
			DX11BlendSettings dx11BlendSettings;

			///
			cppx::mem_set_zero( dx11BlendSettings.RenderTarget );

			dx11BlendSettings.AlphaToCoverageEnable = pBlendSettings.flags.is_set( eBlendConfigFlagEnableAlphaToCoverageBit );
			dx11BlendSettings.IndependentBlendEnable = pBlendSettings.flags.is_set( eBlendConfigFlagEnableMRTIndependentBlendingBit );

			if( pBlendSettings.attachmentsMask != 0 )
			{
				if( !dx11BlendSettings.IndependentBlendEnable )
				{
					dx11BlendSettings.RenderTarget[0] = GCU::TranslateRenderTargetColorAttachmentBlendSettingsDX11( pBlendSettings.attachments[0] );
					dx11BlendSettings.IndependentBlendEnable = FALSE;
				}
				else
				{
					ForEachRTAttachmentIndex( pBlendSettings.attachmentsMask,
						[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
						{
							auto & dx11AttachmentSettings = dx11BlendSettings.RenderTarget[pIndex];
							dx11AttachmentSettings = GCU::TranslateRenderTargetColorAttachmentBlendSettingsDX11( pBlendSettings.attachments[pIndex] );
							dx11AttachmentSettings.BlendEnable = TRUE;
							return true;
						} );
				}

				dx11BlendSettings.ConstantFactor = pBlendSettings.constantColor;
			}

			return dx11BlendSettings;
		}

		DX11DepthStencilSettings TranslateDepthStencilSettingsDX11( const DepthStencilSettings & pDepthStencilSettings )
		{
			DX11DepthStencilSettings dx11DepthStencilSettings;

			auto & depthSettings = pDepthStencilSettings.depthTest;
			dx11DepthStencilSettings.DepthEnable = pDepthStencilSettings.commonFlags.is_set( eDepthStencilConfigFlagEnableDepthTestBit );
			dx11DepthStencilSettings.DepthFunc = ATL::TranslateDX11CompFunc( depthSettings.depthCompFunc );
			dx11DepthStencilSettings.DepthWriteMask = ATL::TranslateDX11DepthWriteMask( depthSettings.depthWriteMask );

			dx11DepthStencilSettings.StencilEnable = pDepthStencilSettings.commonFlags.is_set( eDepthStencilConfigFlagEnableStencilTestBit );
			dx11DepthStencilSettings.StencilReadMask = pDepthStencilSettings.stencilTest.readMask;
			dx11DepthStencilSettings.StencilWriteMask = pDepthStencilSettings.stencilTest.writeMask;

			auto & stencilBackFaceDesc = pDepthStencilSettings.stencilTest.backFace;
			dx11DepthStencilSettings.BackFace.StencilFunc = ATL::TranslateDX11CompFunc( stencilBackFaceDesc.compFunc );
			dx11DepthStencilSettings.BackFace.StencilFailOp = ATL::TranslateDX11StencilOp( stencilBackFaceDesc.opFail );
			dx11DepthStencilSettings.BackFace.StencilDepthFailOp = ATL::TranslateDX11StencilOp( stencilBackFaceDesc.opPassDepthFail );
			dx11DepthStencilSettings.BackFace.StencilPassOp = ATL::TranslateDX11StencilOp( stencilBackFaceDesc.opPassDepthPass );

			auto & stencilFrontFaceDesc = pDepthStencilSettings.stencilTest.frontFace;
			dx11DepthStencilSettings.FrontFace.StencilFunc = ATL::TranslateDX11CompFunc( stencilFrontFaceDesc.compFunc );
			dx11DepthStencilSettings.FrontFace.StencilFailOp = ATL::TranslateDX11StencilOp( stencilFrontFaceDesc.opFail );
			dx11DepthStencilSettings.FrontFace.StencilDepthFailOp = ATL::TranslateDX11StencilOp( stencilFrontFaceDesc.opPassDepthFail );
			dx11DepthStencilSettings.FrontFace.StencilPassOp = ATL::TranslateDX11StencilOp( stencilFrontFaceDesc.opPassDepthPass );

			return dx11DepthStencilSettings;
		}

		DX11RasterizerSettings TranslateRasterizerSettingsDX11( const RasterizerSettings & pRasterizerSettings )
		{
			DX11RasterizerSettings dx11RasterizerDescriptor;

			dx11RasterizerDescriptor.ScissorEnable = pRasterizerSettings.flags.is_set( eRasterizerConfigFlagEnableScissorTestBit );
			dx11RasterizerDescriptor.FrontCounterClockwise = ( pRasterizerSettings.frontFaceVerticesOrder == ETriangleVerticesOrder::CounterClockwise );
			dx11RasterizerDescriptor.CullMode = ATL::TranslateDX11CullMode( pRasterizerSettings.cullMode );
			dx11RasterizerDescriptor.FillMode = ATL::TranslateDX11PrimitiveFillMode( pRasterizerSettings.primitiveFillMode );
			dx11RasterizerDescriptor.DepthClipEnable = TRUE;
			dx11RasterizerDescriptor.MultisampleEnable = FALSE;
			dx11RasterizerDescriptor.DepthBias = 0;
			dx11RasterizerDescriptor.DepthBiasClamp = 0.0f;
			dx11RasterizerDescriptor.SlopeScaledDepthBias = 0.0f;

			return dx11RasterizerDescriptor;
		}

		D3D11_RENDER_TARGET_BLEND_DESC TranslateRenderTargetColorAttachmentBlendSettingsDX11(
				const RenderTargetColorAttachmentBlendSettings & pColorAttachmentBlendSettings )
		{
			D3D11_RENDER_TARGET_BLEND_DESC dx11AttachmentBlendDesc;

			dx11AttachmentBlendDesc.SrcBlend = ATL::TranslateDX11BlendFactor( pColorAttachmentBlendSettings.factorSrcColor );
			dx11AttachmentBlendDesc.DestBlend = ATL::TranslateDX11BlendFactor( pColorAttachmentBlendSettings.factorDstColor );
			dx11AttachmentBlendDesc.BlendOp = ATL::TranslateDX11BlendOp( pColorAttachmentBlendSettings.opColor );
			dx11AttachmentBlendDesc.SrcBlendAlpha = ATL::TranslateDX11BlendFactor( pColorAttachmentBlendSettings.factorSrcAlpha );
			dx11AttachmentBlendDesc.DestBlendAlpha = ATL::TranslateDX11BlendFactor( pColorAttachmentBlendSettings.factorDstAlpha );
			dx11AttachmentBlendDesc.BlendOpAlpha = ATL::TranslateDX11BlendOp( pColorAttachmentBlendSettings.opAlpha );
			dx11AttachmentBlendDesc.RenderTargetWriteMask = ATL::TranslateDX11BlendRenderTargetWriteMask( pColorAttachmentBlendSettings.writeMask );

			return dx11AttachmentBlendDesc;

		}

	}

}
