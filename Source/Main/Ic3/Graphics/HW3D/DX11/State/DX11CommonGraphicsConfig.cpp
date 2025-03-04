
#include "DX11CommonGraphicsConfig.h"
#include "../DX11APITranslationLayer.h"
#include "../DX11gpuDevice.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	DX11BlendStateDescriptor::DX11BlendStateDescriptor(
			DX11GPUDevice & pGPUDevice,
			cppx::bitmask<EBlendConfigFlags> pBlendFlags,
			ComPtr<ID3D11BlendState> pD3D11BlendState )
	: BlendStateDescriptor( pGPUDevice, pBlendFlags )
	, mD3D11BlendState( pD3D11BlendState )
	{}

	DX11BlendStateDescriptor::~DX11BlendStateDescriptor() = default;

	GpaHandle<DX11BlendStateDescriptor> DX11BlendStateDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const BlendSettings & pBlendSettings )
	{
		const auto dx11BlendSettings = GCU::TranslateDX11BlendSettings( pBlendSettings );

		ComPtr<ID3D11BlendState> d3d11BlendState;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateBlendState( &dx11BlendSettings, d3d11BlendState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto stateDescriptor = CreateGfxObject<DX11BlendStateDescriptor>(
				pGPUDevice,
				pBlendSettings.flags,
				d3d11BlendState );

		return stateDescriptor;
	}


	DX11DepthStencilStateDescriptor::DX11DepthStencilStateDescriptor(
			DX11GPUDevice & pGPUDevice,
			cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
			ComPtr<ID3D11DepthStencilState> pD3D11DepthStencilState )
	: DepthStencilStateDescriptor( pGPUDevice, pDepthStencilFlags )
	, mD3D11DepthStencilState( pD3D11DepthStencilState )
	{}

	DX11DepthStencilStateDescriptor::~DX11DepthStencilStateDescriptor() = default;

	GpaHandle<DX11DepthStencilStateDescriptor> DX11DepthStencilStateDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const DepthStencilSettings & pDepthStencilSettings )
	{
		const auto dx11DepthStencilSettings = GCU::TranslateDX11DepthStencilSettings( pDepthStencilSettings );

		ComPtr<ID3D11DepthStencilState> d3d11DepthStencilState;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateDepthStencilState( &dx11DepthStencilSettings, d3d11DepthStencilState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto stateDescriptor = CreateGfxObject<DX11DepthStencilStateDescriptor>(
				pGPUDevice,
				pDepthStencilSettings.commonFlags,
				d3d11DepthStencilState );

		return stateDescriptor;
	}


	DX11RasterizerStateDescriptor::DX11RasterizerStateDescriptor(
			DX11GPUDevice & pGPUDevice,
			cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags,
			ComPtr<ID3D11RasterizerState> pD3D11RasterizerState )
	: RasterizerStateDescriptor( pGPUDevice, pRasterizerFlags )
	, mD3D11RasterizerState( pD3D11RasterizerState )
	{}

	DX11RasterizerStateDescriptor::~DX11RasterizerStateDescriptor() = default;

	GpaHandle<DX11RasterizerStateDescriptor> DX11RasterizerStateDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const RasterizerSettings & pRasterizerSettings )
	{

		const auto dx11RasterizerSettings = GCU::TranslateDX11RasterizerSettings( pRasterizerSettings );

		ComPtr<ID3D11RasterizerState> d3d11RasterizerState;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateRasterizerState( &dx11RasterizerSettings, d3d11RasterizerState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto stateDescriptor = CreateGfxObject<DX11RasterizerStateDescriptor>(
				pGPUDevice,
				pRasterizerSettings.flags,
				d3d11RasterizerState );

		return stateDescriptor;
	}

	namespace GCU
	{

		DX11BlendSettings TranslateDX11BlendSettings( const BlendSettings & pSettings )
		{
			DX11BlendSettings dx11BlendSettings;

			///
			mem_set_zero( dx11BlendSettings.RenderTarget );

			dx11BlendSettings.AlphaToCoverageEnable = pConfig.flags.is_set( E_BLEND_CONFIG_FLAG_ENABLE_ALPHA_TO_COVERAGE_BIT );
			dx11BlendSettings.IndependentBlendEnable = pConfig.flags.is_set( E_BLEND_CONFIG_FLAG_ENABLE_MRT_INDEPENDENT_BLENDING_BIT );

			if( pConfig.attachmentsMask != 0 )
			{
				if( !dx11BlendSettings.IndependentBlendEnable )
				{
					dx11BlendSettings.RenderTarget[0] = GCU::TranslateDX11RenderTargetColorAttachmentBlendSettings( pConfig.attachments[0] );
					dx11BlendSettings.IndependentBlendEnable = FALSE;
				}
				else
				{
					ForEachRTAttachmentIndex( pConfig.attachmentsMask,
						[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
						{
							auto & dx11AttachmentSettings = dx11BlendSettings.RenderTarget[pIndex];
							dx11AttachmentSettings = GCU::TranslateDX11RenderTargetColorAttachmentBlendSettings( pConfig.attachments[pIndex] );
							dx11AttachmentSettings.BlendEnable = TRUE;
							return true;
						} );
				}

				dx11BlendSettings.ConstantFactor = pConfig.constantColor;
			}

			return dx11BlendSettings;
		}

		DX11DepthStencilSettings TranslateDX11DepthStencilSettings( const DepthStencilSettings & pSettings )
		{
			DX11DepthStencilSettings dx11DepthStencilSettings;

			auto & depthSettings = pConfig.depthTest;
			dx11DepthStencilSettings.DepthEnable = pConfig.commonFlags.is_set( E_DEPTH_STENCIL_CONFIG_FLAG_ENABLE_DEPTH_TEST_BIT );
			dx11DepthStencilSettings.DepthFunc = ATL::TranslateDX11CompFunc( depthSettings.depthCompFunc );
			dx11DepthStencilSettings.DepthWriteMask = ATL::TranslateDX11DepthWriteMask( depthSettings.depthWriteMask );

			dx11DepthStencilSettings.StencilEnable = pConfig.commonFlags.is_set( E_DEPTH_STENCIL_CONFIG_FLAG_ENABLE_STENCIL_TEST_BIT );
			dx11DepthStencilSettings.StencilReadMask = pConfig.stencilTest.readMask;
			dx11DepthStencilSettings.StencilWriteMask = pConfig.stencilTest.writeMask;

			auto & stencilBackFaceDesc = pConfig.stencilTest.backFace;
			dx11DepthStencilSettings.BackFace.StencilFunc = ATL::TranslateDX11CompFunc( stencilBackFaceDesc.compFunc );
			dx11DepthStencilSettings.BackFace.StencilFailOp = ATL::TranslateDX11StencilOp( stencilBackFaceDesc.opFail );
			dx11DepthStencilSettings.BackFace.StencilDepthFailOp = ATL::TranslateDX11StencilOp( stencilBackFaceDesc.opPassDepthFail );
			dx11DepthStencilSettings.BackFace.StencilPassOp = ATL::TranslateDX11StencilOp( stencilBackFaceDesc.opPassDepthPass );

			auto & stencilFrontFaceDesc = pConfig.stencilTest.frontFace;
			dx11DepthStencilSettings.FrontFace.StencilFunc = ATL::TranslateDX11CompFunc( stencilFrontFaceDesc.compFunc );
			dx11DepthStencilSettings.FrontFace.StencilFailOp = ATL::TranslateDX11StencilOp( stencilFrontFaceDesc.opFail );
			dx11DepthStencilSettings.FrontFace.StencilDepthFailOp = ATL::TranslateDX11StencilOp( stencilFrontFaceDesc.opPassDepthFail );
			dx11DepthStencilSettings.FrontFace.StencilPassOp = ATL::TranslateDX11StencilOp( stencilFrontFaceDesc.opPassDepthPass );

			return dx11DepthStencilSettings;
		}

		DX11RasterizerSettings TranslateDX11RasterizerSettings( const RasterizerSettings & pSettings )
		{
			DX11RasterizerSettings dx11RasterizerDescriptor;

			dx11RasterizerDescriptor.ScissorEnable = pConfig.flags.is_set( E_RASTERIZER_CONFIG_FLAG_ENABLE_SCISSOR_TEST_BIT );
			dx11RasterizerDescriptor.FrontCounterClockwise = ( pConfig.frontFaceVerticesOrder == ETriangleVerticesOrder::CounterClockwise );
			dx11RasterizerDescriptor.CullMode = ATL::TranslateDX11CullMode( pConfig.cullMode );
			dx11RasterizerDescriptor.FillMode = ATL::TranslateDX11PrimitiveFillMode( pConfig.primitiveFillMode );
			dx11RasterizerDescriptor.DepthClipEnable = TRUE;
			dx11RasterizerDescriptor.MultisampleEnable = FALSE;
			dx11RasterizerDescriptor.DepthBias = 0;
			dx11RasterizerDescriptor.DepthBiasClamp = 0.0f;
			dx11RasterizerDescriptor.SlopeScaledDepthBias = 0.0f;

			return dx11RasterizerDescriptor;
		}

		D3D11_RENDER_TARGET_BLEND_DESC TranslateDX11RenderTargetColorAttachmentBlendSettings(
				const RenderTargetColorAttachmentBlendSettings & pSettings )
		{
			D3D11_RENDER_TARGET_BLEND_DESC dx11AttachmentBlendDesc;

			dx11AttachmentBlendDesc.SrcBlend = ATL::TranslateDX11BlendFactor( pSettings.factorSrcColor );
			dx11AttachmentBlendDesc.DestBlend = ATL::TranslateDX11BlendFactor( pSettings.factorDstColor );
			dx11AttachmentBlendDesc.BlendOp = ATL::TranslateDX11BlendOp( pSettings.opColor );
			dx11AttachmentBlendDesc.SrcBlendAlpha = ATL::TranslateDX11BlendFactor( pSettings.factorSrcAlpha );
			dx11AttachmentBlendDesc.DestBlendAlpha = ATL::TranslateDX11BlendFactor( pSettings.factorDstAlpha );
			dx11AttachmentBlendDesc.BlendOpAlpha = ATL::TranslateDX11BlendOp( pSettings.opAlpha );
			dx11AttachmentBlendDesc.RenderTargetWriteMask = ATL::TranslateDX11BlendRenderTargetWriteMask( pSettings.writeMask );

			return dx11AttachmentBlendDesc;

		}

	}

}
