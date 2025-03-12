
#include "DXPresentationLayer.h"
#include "DXGPUDevice.h"
#include <Ic3/System/Core/DisplaySystem.h>
#include <Ic3/System/Core/WindowSystem.h>
#include <Ic3/System/Core/WindowNative.h>

namespace Ic3::Graphics::GCI
{

	DXPresentationLayer::DXPresentationLayer( GPUDevice & pGPUDevice )
	: PresentationLayer( pGPUDevice )
	{}

	DXPresentationLayer::~DXPresentationLayer() = default;


	DXScreenPresentationLayer::DXScreenPresentationLayer( GPUDevice & pGPUDevice, System::WindowHandle pSysWindow, ComPtr<IDXGISwapChain1> pDXGISwapChain1 ) noexcept
	: DXPresentationLayer( pGPUDevice )
	, mSysWindow( pSysWindow )
	, mDXGISwapChain1( std::move( pDXGISwapChain1 ) )
	{}

	DXScreenPresentationLayer::~DXScreenPresentationLayer() noexcept = default;

	System::EventSource * DXScreenPresentationLayer::GetInternalSystemEventSource() const noexcept
	{
		return mSysWindow.get();
	}

	void DXScreenPresentationLayer::Resize( uint32 pWidth, uint32 pHeight )
	{
		mSysWindow->ResizeClientArea( { pWidth, pHeight } );
	}

	void DXScreenPresentationLayer::SetFullscreenMode( bool pEnable )
	{
		mSysWindow->SetFullscreenMode( true );
	}

	cxm::vec2u32 DXScreenPresentationLayer::QueryRenderTargetSize() const
	{
		return mSysWindow->GetClientAreaSize();
	}

	System::WindowHandle DXScreenPresentationLayer::CreateSysWindow(
			DXGPUDevice & pGPUDevice,
			const PresentationLayerCreateInfo & pCreateInfo )
	{
		try
		{
			auto sysWindowManager = pCreateInfo.sysWindowManager;
			if( !sysWindowManager )
			{
				auto sysDisplayManager = pCreateInfo.sysDisplayManager;
				if( !sysDisplayManager )
				{
					Ic3DebugAssert( pGPUDevice.mSysContext );
					sysDisplayManager = pGPUDevice.mSysContext->CreateDisplayManager();
				}
				Ic3DebugAssert( pGPUDevice.mSysContext );
				sysWindowManager = pGPUDevice.mSysContext->CreateWindowManager( sysDisplayManager );
			}

			System::WindowCreateInfo windowCreateInfo;
			windowCreateInfo.title = "DXWindow";

			if( pCreateInfo.displayConfigFlags.is_set( eDisplayConfigurationFlagFullscreenBit ) )
			{
				windowCreateInfo.frameGeometry.size = System::cxFrameSizeMax;
				windowCreateInfo.frameGeometry.style = System::EFrameStyle::Overlay;
			}
			else
			{
				windowCreateInfo.frameGeometry.position = pCreateInfo.screenRect.offset;
				windowCreateInfo.frameGeometry.size = pCreateInfo.screenRect.size;
				windowCreateInfo.frameGeometry.style = System::EFrameStyle::Fixed;
			}

			auto sysWindow = sysWindowManager->CreateWindow( windowCreateInfo );

		#if( PCL_TARGET_OS == PCL_TARGET_OS_WINDESKTOP )
			if( pCreateInfo.displayConfigFlags.is_set( eDisplayConfigurationFlagFullscreenBit ) )
			{
				auto * win32Window = sysWindow->QueryInterface<System::Win32Window>();
				::SetCapture( win32Window->mNativeData.hwnd );
				::ShowCursor( FALSE );
			}
		#endif

			return sysWindow;
		}
		catch ( ... )
		{
			Ic3DebugInterrupt();
		}

		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
