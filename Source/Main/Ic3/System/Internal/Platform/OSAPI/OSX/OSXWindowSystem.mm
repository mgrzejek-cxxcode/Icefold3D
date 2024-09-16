
#include "OSXWindowSystem.h"
#include "OSXDisplaySystem.h"
#include "NSIWindow.h"
#include "NSIEventListener.h"

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_OSX )
namespace Ic3::System
{

	namespace Platform
	{

		OSXFrameGeometry _osxCheckFrameGeometry( NSScreen * pNSScreen, const FrameGeometry & pFrameGeometry );

		OSXFrameGeometry _osxCheckFrameGeometryUpdate( NSWindow * pNSWindow,
		                                               const FrameGeometry & pFrameGeometry,
		                                               TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags );

	}

	OSXWindowManager::OSXWindowManager( OSXDisplayManagerHandle pDisplayManager )
	: OSXNativeObject( std::move( pDisplayManager ) )
	{}
	
	OSXWindowManager::~OSXWindowManager() noexcept
	{}
	
	WindowHandle OSXWindowManager::_nativeCreateWindow( WindowCreateInfo pCreateInfo )
	{
		auto osxDisplayManager = mDisplayManager->getHandle<OSXDisplayManager>();
		auto windowObject = createSysObject<OSXWindow>( getHandle<OSXWindowManager>() );

		Platform::osxCreateWindow( windowObject->mNativeData, nullptr, pCreateInfo );
		Platform::osxCreateWindowDefaultView( windowObject->mNativeData );
		Platform::osxCreateEventListener( windowObject->mNativeData );
		Platform::osxSetInputWindow( windowObject->mNativeData );

		return windowObject;
	}

	void OSXWindowManager::_nativeDestroyWindow( Window & pWindow )
	{
	}


	OSXWindow::OSXWindow( OSXWindowManagerHandle pWindowManager )
	: OSXNativeObject( std::move( pWindowManager ), &mNativeData )
	{}
	
	OSXWindow::~OSXWindow() noexcept = default;
	
	void OSXWindow::_nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}
	
	void OSXWindow::_nativeSetFullscreenMode( bool pEnable )
	{}
	
	void OSXWindow::_nativeSetTitle( const std::string & pTitle )
	{
        Platform::osxSetFrameTitle( mNativeData.mNSWindow, pTitle );
    }
	
	void OSXWindow::_nativeUpdateGeometry( const FrameGeometry & pFrameGeometry, TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{}

	FrameSize OSXWindow::_nativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return {};
	}
	

	namespace Platform
	{

		void osxCreateWindow( OSXWindowNativeData & pWindowNativeData, NSScreen * pTargetScreen, const WindowCreateInfo & pCreateInfo )
		{
		@autoreleasepool
		{
			if( !pTargetScreen )
			{
				NSArray * nsScreenArray = [NSScreen screens];
				pTargetScreen = [nsScreenArray objectAtIndex:0];
			}

			NSOSXWindow * nsWindow = nil;

			@try
			{
				const auto frameGeometry = _osxCheckFrameGeometry( pTargetScreen, pCreateInfo.mFrameGeometry );

				nsWindow = [[NSOSXWindow alloc] initWithContentRect:frameGeometry.mFrameRect
				                                styleMask:static_cast<NSWindowStyleMask>( frameGeometry.mStyle )
				                                backing:NSBackingStoreBuffered
				                                defer:NO
				                                screen:pTargetScreen];

				if( [nsWindow respondsToSelector:@selector( setTabbingMode: )] )
				{
					[nsWindow setTabbingMode:NSWindowTabbingModeDisallowed];
				}

				pWindowNativeData.mNSWindow = nsWindow;
				pWindowNativeData.mNSTargetScreen = pTargetScreen;
			}
			@catch( NSException * pException )
			{
				const auto message = [[pException reason] UTF8String];
				ic3DebugInterrupt();
			}
		}
		}

		void osxCreateWindowDefaultView( OSXWindowNativeData & pWindowNativeData )
		{
		@autoreleasepool
		{
			if( ![( id )pWindowNativeData.mNSWindow isKindOfClass:[NSOSXWindow class]] )
			{
				return;
			}

			auto * nsWindow = static_cast<NSOSXWindow *>( pWindowNativeData.mNSWindow );

			@try
			{
				NSOSXWindowView * nsWindowView = [[NSOSXWindowView alloc] initForWindow:nsWindow];
				pWindowNativeData.mNSView = nsWindowView;
			}
			@catch( NSException * pException )
			{
				const auto message = [[pException reason] UTF8String];
				ic3DebugInterrupt();
			}
		}
		}

		void osxSetInputWindow( OSXWindowNativeData & pWindowNativeData )
		{
			if( ![pWindowNativeData.mNSWindow isMiniaturized] )
			{
				[NSApp activateIgnoringOtherApps:YES];

				[pWindowNativeData.mNSWindow makeKeyAndOrderFront:nil];
				[pWindowNativeData.mNSWindow orderFrontRegardless];
			}
		}

		void osxSetFrameTitle( NSWindow * pNSWindow, const std::string & pTitle )
		{
		@autoreleasepool
		{
			const char * newTitle = "";
			if( !pTitle.empty() )
			{
				newTitle = pTitle.c_str();
			}

			NSString * nsTitle = [[NSString alloc] initWithUTF8String:newTitle];
			[pNSWindow setTitle:nsTitle];
		}
		}

		void osxUpdateFrameGeometry( NSWindow * pNSWindow, const FrameGeometry & pFrameGeometry, TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
		{
		@autoreleasepool
		{
			const auto frameGeometry = _osxCheckFrameGeometryUpdate( pNSWindow, pFrameGeometry, pUpdateFlags );

			if( pUpdateFlags.isSet( eFrameGeometryUpdateFlagStyleBit ) )
			{
				[pNSWindow setStyleMask:static_cast<NSWindowStyleMask>( frameGeometry.mStyle )];
			}

			if( pUpdateFlags.isSet( eFrameGeometryUpdateFlagPositionBit ) )
			{
				[pNSWindow setFrameOrigin:frameGeometry.mFrameRect.origin];
			}

			if( pUpdateFlags.isSet( eFrameGeometryUpdateFlagSizeBit ) )
			{
				[pNSWindow setFrame:[pNSWindow frameRectForContentRect:frameGeometry.mFrameRect] display:YES];
			}
		}
		}

		FrameSize osxGetFrameSize( NSWindow * pNSWindow, EFrameSizeMode pSizeMode )
		{
		@autoreleasepool
		{
			NSRect resultRect;
			if( pSizeMode == EFrameSizeMode::OuterRect )
			{
				resultRect = [pNSWindow frame];
			}
			else
			{
				const auto frameRect = [pNSWindow frame];
				resultRect = [pNSWindow contentRectForFrameRect:frameRect];
			}

			FrameSize frameSize{};
			frameSize.x = static_cast<uint32>( resultRect.size.width );
			frameSize.y = static_cast<uint32>( resultRect.size.height );

			return frameSize;
		}
		}

		NSUInteger osxTranslateFrameStyle( EFrameStyle pStyle )
		{
			//
			constexpr NSUInteger cvCaptionFrameStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable;
			//
			constexpr NSUInteger cvFixedFrameStyle = cvCaptionFrameStyle | NSWindowStyleMaskMiniaturizable;
			//
			constexpr NSUInteger cvOverlayFrameStyle = NSWindowStyleMaskBorderless;
			//
			constexpr NSUInteger cvResizeableFrameStyle = cvFixedFrameStyle | NSWindowStyleMaskResizable;

			NSUInteger resultStyle = Cppx::QLimits<NSUInteger>::sMaxValue;

			switch ( pStyle )
			{
				case EFrameStyle::Caption:
				{
					resultStyle = cvCaptionFrameStyle;
					break;
				}
				case EFrameStyle::Fixed:
				{
					resultStyle = cvFixedFrameStyle;
					break;
				}
				case EFrameStyle::Overlay:
				{
					resultStyle = cvOverlayFrameStyle;
					break;
				}
				case EFrameStyle::Resizeable:
				{
					resultStyle = cvResizeableFrameStyle;
					break;
				}
				default:
				{
					resultStyle = 0u;
					break;
				}
			}

			return resultStyle;
		}

		OSXFrameGeometry _osxCheckFrameGeometry( NSScreen * pNSScreen, const FrameGeometry & pFrameGeometry )
		{
			// Alternatively: CGDisplayPixelsHigh( kCGDirectMainDisplay ) ??
			const auto screenRect = [pNSScreen frame];

			auto xOrigin = static_cast<CGFloat>( pFrameGeometry.mPosition.x );

			NSRect frameRect{};
			frameRect.origin.x = static_cast<CGFloat>( pFrameGeometry.mPosition.x );
			frameRect.origin.y = static_cast<CGFloat>( pFrameGeometry.mPosition.y );
			frameRect.size.width = static_cast<CGFloat>( pFrameGeometry.mSize.x );
			frameRect.size.height = static_cast<CGFloat>( pFrameGeometry.mSize.y );

			frameRect.origin.x = Cppx::getMaxOf( frameRect.origin.x, 0.0f );
			frameRect.origin.y = screenRect.size.height - frameRect.origin.y - frameRect.size.height;

			OSXFrameGeometry osxGeometry{};
			osxGeometry.mFrameRect = frameRect;
			osxGeometry.mStyle = osxTranslateFrameStyle( pFrameGeometry.mStyle );

			return osxGeometry;
		}

		OSXFrameGeometry _osxCheckFrameGeometryUpdate( NSWindow * pNSWindow,
		                                               const FrameGeometry & pFrameGeometry,
		                                               TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
		{
			OSXFrameGeometry osxGeometry{};

			if( pUpdateFlags.isSet( eFrameGeometryUpdateFlagStyleBit ) )
			{
				osxGeometry.mStyle = osxTranslateFrameStyle( pFrameGeometry.mStyle );
			}
			else
			{
				osxGeometry.mStyle = [pNSWindow styleMask];
			}

			if( pUpdateFlags.isSetAnyOf( eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeBit ) )
			{
				const auto screenRect = [[pNSWindow screen] frame];

				NSRect frameRect{};
				frameRect.origin.x = static_cast<CGFloat>( pFrameGeometry.mPosition.x );
				frameRect.origin.y = static_cast<CGFloat>( pFrameGeometry.mPosition.y );
				frameRect.size.width = static_cast<CGFloat>( pFrameGeometry.mSize.x );
				frameRect.size.height = static_cast<CGFloat>( pFrameGeometry.mSize.y );

				frameRect.origin.x = Cppx::getMaxOf( frameRect.origin.x, 0.0f );
				frameRect.origin.y = screenRect.size.height - frameRect.origin.y - frameRect.size.height;

				osxGeometry.mFrameRect = frameRect;
			}
			else
			{
				osxGeometry.mFrameRect = [pNSWindow frame];
			}

			return osxGeometry;
		}

	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_OSX
