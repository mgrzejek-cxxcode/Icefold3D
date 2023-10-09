
#include "DXPresentationLayer.h"
#include "DXGPUDevice.h"
#include <Ic3/System/DisplaySystem.h>
#include <Ic3/System/WindowSystem.h>
#include <Ic3/System/WindowNative.h>

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

	System::EventSource * DXScreenPresentationLayer::getInternalSystemEventSource() const noexcept
	{
		return mSysWindow.get();
	}

	void DXScreenPresentationLayer::resize( uint32 pWidth, uint32 pHeight )
	{
	}

	void DXScreenPresentationLayer::setFullscreenMode( bool pEnable )
	{
	}

	Ic3::Math::Vec2u32 DXScreenPresentationLayer::queryRenderTargetSize() const
	{
		return mSysWindow->getClientAreaSize();
	}

	System::WindowHandle DXScreenPresentationLayer::createSysWindow( DXGPUDevice & pGPUDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		try
		{
			auto sysWindowManager = pCreateInfo.sysWindowManager;
			if( !sysWindowManager )
			{
			    auto sysDisplayManager = pCreateInfo.sysDisplayManager;
			    if( !sysDisplayManager )
			    {
			        ic3DebugAssert( pGPUDevice.mSysContext );
			        sysDisplayManager = pGPUDevice.mSysContext->createDisplayManager();
			    }
			    ic3DebugAssert( pGPUDevice.mSysContext );
			    sysWindowManager = pGPUDevice.mSysContext->createWindowManager( sysDisplayManager );
			}

			System::WindowCreateInfo windowCreateInfo;
			windowCreateInfo.title = "DXWindow";

			if( pCreateInfo.displayConfigFlags.isSet( E_DISPLAY_CONFIGURATION_FLAG_FULLSCREEN_BIT ) )
			{
				windowCreateInfo.frameGeometry.size = System::CX_FRAME_SIZE_MAX;
				windowCreateInfo.frameGeometry.style = System::EFrameStyle::Overlay;
			}
			else
			{
				windowCreateInfo.frameGeometry.position = pCreateInfo.screenRect.offset;
				windowCreateInfo.frameGeometry.size = pCreateInfo.screenRect.size;
				windowCreateInfo.frameGeometry.style = System::EFrameStyle::Fixed;
			}

			auto sysWindow = sysWindowManager->createWindow( windowCreateInfo );

        #if( IC3_PCL_TARGET_OS == IC3_PCL_TARGET_OS_WINDESKTOP )
			if( pCreateInfo.displayConfigFlags.isSet( E_DISPLAY_CONFIGURATION_FLAG_FULLSCREEN_BIT ) )
			{
			    auto * win32Window = sysWindow->queryInterface<System::Win32Window>();
			    ::SetCapture( win32Window->mNativeData.hwnd );
				::ShowCursor( FALSE );
			}
        #endif

			return sysWindow;
		}
		catch ( ... )
		{
			ic3DebugInterrupt();
		}

		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
