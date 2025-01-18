
#include "DX11CommonGraphicsConfig.h"
#include "../DX11APITranslationLayer.h"
#include "../DX11gpuDevice.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	DX11BlendImmutableState::DX11BlendImmutableState(
			DX11GPUDevice & pGPUDevice,
			cppx::bitmask<EBlendConfigFlags> pBlendFlags,
			ComPtr<ID3D11BlendState> pD3D11BlendState )
	: BlendImmutableState( pGPUDevice, pBlendFlags )
	, mD3D11BlendState( pD3D11BlendState )
	{}

	DX11BlendImmutableState::~DX11BlendImmutableState() = default;

	GpaHandle<DX11BlendImmutableState> DX11BlendImmutableState::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const BlendConfig & pBlendConfig )
	{
		const auto dx11BlendConfig = SMU::TranslateDX11BlendConfig( pBlendConfig );

		ComPtr<ID3D11BlendState> d3d11BlendState;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateBlendState( &dx11BlendConfig, d3d11BlendState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto immutableState = CreateGfxObject<DX11BlendImmutableState>(
				pGPUDevice,
				pBlendConfig.flags,
				d3d11BlendState );

		return immutableState;
	}


	DX11DepthStencilImmutableState::DX11DepthStencilImmutableState(
			DX11GPUDevice & pGPUDevice,
			cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
			ComPtr<ID3D11DepthStencilState> pD3D11DepthStencilState )
	: DepthStencilImmutableState( pGPUDevice, pDepthStencilFlags )
	, mD3D11DepthStencilState( pD3D11DepthStencilState )
	{}

	DX11DepthStencilImmutableState::~DX11DepthStencilImmutableState() = default;

	GpaHandle<DX11DepthStencilImmutableState> DX11DepthStencilImmutableState::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const DepthStencilConfig & pDepthStencilConfig )
	{
		const auto dx11DepthStencilConfig = SMU::TranslateDX11DepthStencilConfig( pDepthStencilConfig );

		ComPtr<ID3D11DepthStencilState> d3d11DepthStencilState;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateDepthStencilState( &dx11DepthStencilConfig, d3d11DepthStencilState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto immutableState = CreateGfxObject<DX11DepthStencilImmutableState>(
				pGPUDevice,
				pDepthStencilConfig.commonFlags,
				d3d11DepthStencilState );

		return immutableState;
	}


	DX11RasterizerImmutableState::DX11RasterizerImmutableState(
			DX11GPUDevice & pGPUDevice,
			cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags,
			ComPtr<ID3D11RasterizerState> pD3D11RasterizerState )
	: RasterizerImmutableState( pGPUDevice, pRasterizerFlags )
	, mD3D11RasterizerState( pD3D11RasterizerState )
	{}

	DX11RasterizerImmutableState::~DX11RasterizerImmutableState() = default;

	GpaHandle<DX11RasterizerImmutableState> DX11RasterizerImmutableState::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const RasterizerConfig & pRasterizerConfig )
	{

		const auto dx11RasterizerConfig = SMU::TranslateDX11RasterizerConfig( pRasterizerConfig );

		ComPtr<ID3D11RasterizerState> d3d11RasterizerState;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateRasterizerState( &dx11RasterizerConfig, d3d11RasterizerState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto immutableState = CreateGfxObject<DX11RasterizerImmutableState>(
				pGPUDevice,
				pRasterizerConfig.flags,
				d3d11RasterizerState );

		return immutableState;
	}

	namespace SMU
	{

		DX11BlendConfig TranslateDX11BlendConfig( const BlendConfig & pConfig )
		{
			DX11BlendConfig dx11BlendConfig;

			///
			mem_set_zero( dx11BlendConfig.RenderTarget );

			dx11BlendConfig.AlphaToCoverageEnable = pConfig.flags.is_set( E_BLEND_CONFIG_FLAG_ENABLE_ALPHA_TO_COVERAGE_BIT );
			dx11BlendConfig.IndependentBlendEnable = pConfig.flags.is_set( E_BLEND_CONFIG_FLAG_ENABLE_MRT_INDEPENDENT_BLENDING_BIT );

			if( pConfig.attachmentsMask != 0 )
			{
				if( !dx11BlendConfig.IndependentBlendEnable )
				{
					dx11BlendConfig.RenderTarget[0] = SMU::TranslateDX11RTColorAttachmentBlendSettings( pConfig.attachments[0] );
					dx11BlendConfig.IndependentBlendEnable = FALSE;
				}
				else
				{
					ForEachRTAttachmentIndex( pConfig.attachmentsMask,
						[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
						{
							auto & dx11AttachmentSettings = dx11BlendConfig.RenderTarget[pIndex];
							dx11AttachmentSettings = SMU::TranslateDX11RTColorAttachmentBlendSettings( pConfig.attachments[pIndex] );
							dx11AttachmentSettings.BlendEnable = TRUE;
							return true;
						} );
				}

				dx11BlendConfig.ConstantFactor = pConfig.constantColor;
			}

			return dx11BlendConfig;
		}

		DX11DepthStencilConfig TranslateDX11DepthStencilConfig( const DepthStencilConfig & pConfig )
		{
			DX11DepthStencilConfig dx11DepthStencilConfig;

			auto & depthSettings = pConfig.depthTestSettings;
			dx11DepthStencilConfig.DepthEnable = pConfig.commonFlags.is_set( E_DEPTH_STENCIL_CONFIG_FLAG_ENABLE_DEPTH_TEST_BIT );
			dx11DepthStencilConfig.DepthFunc = ATL::TranslateDX11CompFunc( depthSettings.depthCompFunc );
			dx11DepthStencilConfig.DepthWriteMask = ATL::TranslateDX11DepthWriteMask( depthSettings.depthWriteMask );

			dx11DepthStencilConfig.StencilEnable = pConfig.commonFlags.is_set( E_DEPTH_STENCIL_CONFIG_FLAG_ENABLE_STENCIL_TEST_BIT );
			dx11DepthStencilConfig.StencilReadMask = pConfig.stencilTestSettings.readMask;
			dx11DepthStencilConfig.StencilWriteMask = pConfig.stencilTestSettings.writeMask;

			auto & stencilBackFaceDesc = pConfig.stencilTestSettings.backFace;
			dx11DepthStencilConfig.BackFace.StencilFunc = ATL::TranslateDX11CompFunc( stencilBackFaceDesc.compFunc );
			dx11DepthStencilConfig.BackFace.StencilFailOp = ATL::TranslateDX11StencilOp( stencilBackFaceDesc.opFail );
			dx11DepthStencilConfig.BackFace.StencilDepthFailOp = ATL::TranslateDX11StencilOp( stencilBackFaceDesc.opPassDepthFail );
			dx11DepthStencilConfig.BackFace.StencilPassOp = ATL::TranslateDX11StencilOp( stencilBackFaceDesc.opPassDepthPass );

			auto & stencilFrontFaceDesc = pConfig.stencilTestSettings.frontFace;
			dx11DepthStencilConfig.FrontFace.StencilFunc = ATL::TranslateDX11CompFunc( stencilFrontFaceDesc.compFunc );
			dx11DepthStencilConfig.FrontFace.StencilFailOp = ATL::TranslateDX11StencilOp( stencilFrontFaceDesc.opFail );
			dx11DepthStencilConfig.FrontFace.StencilDepthFailOp = ATL::TranslateDX11StencilOp( stencilFrontFaceDesc.opPassDepthFail );
			dx11DepthStencilConfig.FrontFace.StencilPassOp = ATL::TranslateDX11StencilOp( stencilFrontFaceDesc.opPassDepthPass );

			return dx11DepthStencilConfig;
		}

		DX11RasterizerConfig TranslateDX11RasterizerConfig( const RasterizerConfig & pConfig )
		{
			DX11RasterizerConfig dx11RasterizerDescriptor;

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

		D3D11_RENDER_TARGET_BLEND_DESC TranslateDX11RTColorAttachmentBlendSettings(
				const RTColorAttachmentBlendSettings & pSettings )
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
