
#include "DXPresentationLayer.h"
#include "DXGpuDevice.h"
#include <Ic3/System/DisplaySystem.h>
#include <Ic3/System/WindowSystem.h>
#include <Ic3/System/WindowNative.h>

namespace Ic3::Graphics::GCI
{

	DXPresentationLayer::DXPresentationLayer( GpuDevice & pGpuDevice )
	: PresentationLayer( pGpuDevice )
	{}

	DXPresentationLayer::~DXPresentationLayer() = default;


	DXScreenPresentationLayer::DXScreenPresentationLayer( GpuDevice & pGpuDevice, System::WindowHandle pSysWindow, ComPtr<IDXGISwapChain1> pDXGISwapChain1 ) noexcept
	: DXPresentationLayer( pGpuDevice )
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
	}

	void DXScreenPresentationLayer::SetFullscreenMode( bool pEnable )
	{
	}

	Ic3::Math::Vec2u32 DXScreenPresentationLayer::QueryRenderTargetSize() const
	{
		return mSysWindow->GetClientAreaSize();
	}

	System::WindowHandle DXScreenPresentationLayer::createSysWindow( DXGpuDevice & pGpuDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		try
		{
			auto sysWindowManager = pCreateInfo.sysWindowManager;
			if( !sysWindowManager )
			{
			    auto sysDisplayManager = pCreateInfo.sysDisplayManager;
			    if( !sysDisplayManager )
			    {
			        Ic3DebugAssert( pGpuDevice.mSysContext );
			        sysDisplayManager = pGpuDevice.mSysContext->CreateDisplayManager();
			    }
			    Ic3DebugAssert( pGpuDevice.mSysContext );
			    sysWindowManager = pGpuDevice.mSysContext->CreateWindowManager( sysDisplayManager );
			}

			System::WindowCreateInfo windowCreateInfo;
			windowCreateInfo.mTitle = "DXWindow";

			if( pCreateInfo.mDisplayConfigFlags.is_set( E_DISPLAY_CONFIGURATION_FLAG_FULLSCREEN_BIT ) )
			{
				windowCreateInfo.mFrameGeometry.size = System::CX_FRAME_SIZE_MAX;
				windowCreateInfo.mFrameGeometry.style = System::EFrameStyle::OVERLAY;
			}
			else
			{
				windowCreateInfo.mFrameGeometry.position = pCreateInfo.screenRect.offset;
				windowCreateInfo.mFrameGeometry.size = pCreateInfo.screenRect.size;
				windowCreateInfo.mFrameGeometry.style = System::EFrameStyle::Fixed;
			}

			auto sysWindow = sysWindowManager->createWindow( windowCreateInfo );

        #if( PCL_TARGET_OS == PCL_TARGET_OS_WINDESKTOP )
			if( pCreateInfo.mDisplayConfigFlags.is_set( E_DISPLAY_CONFIGURATION_FLAG_FULLSCREEN_BIT ) )
			{
			    auto * win32Window = sysWindow->QueryInterface<System::Win32Window>();
			    ::SetCapture( win32Window->mNativeData.mHWND );
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
