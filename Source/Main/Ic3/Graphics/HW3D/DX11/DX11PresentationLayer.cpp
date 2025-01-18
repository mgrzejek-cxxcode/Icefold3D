
#include "DX11presentationLayer.h"
#include "DX11CommandList.h"
#include "DX11APITranslationLayer.h"
#include "DX11GPUDevice.h"
#include "State/DX11RenderTarget.h"
#include <Ic3/Graphics/GCI/CommandContext.h>

namespace Ic3::Graphics::GCI
{

	DX11ScreenPresentationLayer::DX11ScreenPresentationLayer(
			DX11GPUDevice & pDevice,
			System::WindowHandle pSysWindow,
			ComPtr<IDXGISwapChain1> pDXGISwapChain1,
			RenderTargetBindingImmutableStateHandle pScreenRenderTargetBindingState )
	: DXScreenPresentationLayer( pDevice, pSysWindow, std::move( pDXGISwapChain1 ) )
	, mD3D11Device1( pDevice.mD3D11Device1 )
	, mScreenRenderTargetBindingState( pScreenRenderTargetBindingState )
	{}

	DX11ScreenPresentationLayer::~DX11ScreenPresentationLayer() = default;

	DX11ScreenPresentationLayerHandle DX11ScreenPresentationLayer::Create( DX11GPUDevice & pDevice, const DX11PresentationLayerCreateInfo & pCreateInfo )
	{
		auto sysWindow = createSysWindow( pDevice, pCreateInfo );
		Ic3DebugAssert( sysWindow );

		auto dxgiSwapChain = ATL::CreateD3D11SwapChainForSystemWindow( pDevice, sysWindow.get() );
		Ic3DebugAssert( dxgiSwapChain );

		ComPtr<ID3D11Texture2D> backBufferRTTexture;
		auto hResult = dxgiSwapChain->GetBuffer( 0, IID_PPV_ARGS( &backBufferRTTexture ) );
		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		D3D11_TEXTURE2D_DESC backBufferDSTextureDesc;
		backBufferRTTexture->GetDesc( &backBufferDSTextureDesc );
		backBufferDSTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		backBufferDSTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		backBufferDSTextureDesc.CpuAccessFlags = 0;

		ComPtr<ID3D11Texture2D> backBufferDSTexture;
		hResult = pDevice.mD3D11Device1->CreateTexture2D( &backBufferDSTextureDesc, nullptr, backBufferDSTexture.GetAddressOf() );
		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return false;
		}

		auto renderTargetState = DX11RenderTargetBindingImmutableState::CreateForScreen( pDevice, backBufferRTTexture, backBufferDSTexture );
		Ic3DebugAssert( renderTargetState );

		auto presentationLayer = CreateGfxObject<DX11ScreenPresentationLayer>( pDevice, sysWindow, std::move( dxgiSwapChain ), renderTargetState );

		return presentationLayer;
	}

	void DX11ScreenPresentationLayer::BindRenderTarget( CommandContext * pCmdContext )
	{
		auto * directGraphicsContext = pCmdContext->QueryInterface<CommandContextDirectGraphics>();
		directGraphicsContext->SetRenderTargetBindingState( *mScreenRenderTargetBindingState );

		// auto * dx11CommandList = pCmdContext->mCommandList->QueryInterface<DX11CommandList>();
		// auto * backBufferRTView = GetBackBufferRTView();
		// auto * backBufferDSView = GetBackBufferDSView();
		// dx11CommandList->mD3D11DeviceContext1->OMSetRenderTargets( 1, &backBufferRTView, backBufferDSView );
	}

	void DX11ScreenPresentationLayer::InvalidateRenderTarget( CommandContext * pCmdContext )
	{
	}

	void DX11ScreenPresentationLayer::Present()
	{
		mDXGISwapChain1->Present( 0, 0 );
	}

	ID3D11RenderTargetView * DX11ScreenPresentationLayer::GetBackBufferRTView()
	{
		if( !_backBufferRTView )
		{
			_CreateBackBufferResources();
		}
		return _backBufferRTView.Get();
	}

	ID3D11DepthStencilView * DX11ScreenPresentationLayer::GetBackBufferDSView()
	{
		if( !_backBufferRTView )
		{
			_CreateBackBufferResources();
		}
		return _backBufferDSView.Get();
	}

	bool DX11ScreenPresentationLayer::_CreateBackBufferResources()
	{
		ComPtr<ID3D11Texture2D> backBufferRTTexture;
		auto hResult = mDXGISwapChain1->GetBuffer( 0, IID_PPV_ARGS( &backBufferRTTexture ) );
		if( FAILED( hResult ) )
		{
			return false;
		}

		D3D11_TEXTURE2D_DESC backBufferDSTextureDesc;
		backBufferRTTexture->GetDesc( & backBufferDSTextureDesc );
		backBufferDSTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		backBufferDSTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		backBufferDSTextureDesc.CpuAccessFlags = 0;

		ComPtr<ID3D11Texture2D> backBufferDSTexture;
		hResult = mD3D11Device1->CreateTexture2D( &backBufferDSTextureDesc, nullptr, backBufferDSTexture.GetAddressOf() );
		if( FAILED( hResult ) )
		{
			return false;
		}

		ComPtr<ID3D11RenderTargetView> backBufferRTView;
		hResult = mD3D11Device1->CreateRenderTargetView( backBufferRTTexture.Get(), nullptr, backBufferRTView.GetAddressOf() );
		if( FAILED( hResult ) )
		{
			return false;
		}

		ComPtr<ID3D11DepthStencilView> backBufferDSView;
		hResult = mD3D11Device1->CreateDepthStencilView( backBufferDSTexture.Get(), nullptr, backBufferDSView.GetAddressOf() );
		if( FAILED( hResult ) )
		{
			return false;
		}

		_backBufferDSTexture = backBufferDSTexture;
		_backBufferDSView = backBufferDSView;
		_backBufferRTView = backBufferRTView;

		return true;
	}
	
} // namespace Ic3::Graphics::GCI
