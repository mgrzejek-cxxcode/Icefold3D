
#include "OSXWindowSystem.h"
#include "OSXDisplaySystem.h"
#include "NSIWindow.h"
#include "NSIEventListener.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_OSX )
namespace Ic3::System
{

	namespace Platform
	{

		OSXFrameGeometry _OSXCheckFrameGeometry( NSScreen * pNSScreen, const FrameGeometry & pFrameGeometry );

		OSXFrameGeometry _OSXCheckFrameGeometryUpdate(
				NSWindow * pNSWindow,
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags );

	}

	OSXWindowManager::OSXWindowManager( OSXDisplayManagerHandle pDisplayManager )
	: OSXNativeObject( std::move( pDisplayManager ) )
	{}
	
	OSXWindowManager::~OSXWindowManager() noexcept
	{}
	
	WindowHandle OSXWindowManager::_NativeCreateWindow( WindowCreateInfo pCreateInfo )
	{
		auto osxDisplayManager = mDisplayManager->GetHandle<OSXDisplayManager>();
		auto windowObject = CreateSysObject<OSXWindow>( GetHandle<OSXWindowManager>() );

		Platform::OSXCreateWindow( windowObject->mNativeData, nullptr, pCreateInfo );
		Platform::OSXCreateWindowDefaultView( windowObject->mNativeData );
		Platform::OSXCreateEventListener( windowObject->mNativeData );
		Platform::OSXSetInputWindow( windowObject->mNativeData );

		return windowObject;
	}

	void OSXWindowManager::_NativeDestroyWindow( Window & pWindow )
	{
	}


	OSXWindow::OSXWindow( OSXWindowManagerHandle pWindowManager )
	: OSXNativeObject( std::move( pWindowManager ), &mNativeData )
	{}
	
	OSXWindow::~OSXWindow() noexcept = default;
	
	void OSXWindow::_NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}
	
	void OSXWindow::_NativeSetFullscreenMode( bool pEnable )
	{}
	
	void OSXWindow::_NativeSetTitle( const std::string & pTitle )
	{
        Platform::OSXSetFrameTitle( mNativeData.mNSWindow, pTitle );
    }
	
	void OSXWindow::_NativeUpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{}

	FrameSize OSXWindow::_NativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return {};
	}
	

	namespace Platform
	{

		void OSXCreateWindow(
				OSXWindowNativeData & pWindowNativeData,
				NSScreen * pTargetScreen,
				const WindowCreateInfo & pCreateInfo )
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
				const auto frameGeometry = _OSXCheckFrameGeometry( pTargetScreen, pCreateInfo.frameGeometry );

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

		void OSXCreateWindowDefaultView( OSXWindowNativeData & pWindowNativeData )
		{
		@autoreleasepool
		{
			if( ![( id )pWindowNativeData.mNSWindow IsKindOfClass:[NSOSXWindow class]] )
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

		void OSXSetInputWindow( OSXWindowNativeData & pWindowNativeData )
		{
			if( ![pWindowNativeData.mNSWindow IsMiniaturized] )
			{
				[NSApp activateIgnoringOtherApps:YES];

				[pWindowNativeData.mNSWindow makeKeyAndOrderFront:nil];
				[pWindowNativeData.mNSWindow orderFrontRegardless];
			}
		}

		void OSXSetFrameTitle( NSWindow * pNSWindow, const std::string & pTitle )
		{
		@autoreleasepool
		{
			const char * newTitle = "";
			if( !pTitle.empty() )
			{
				newTitle = pTitle.c_str();
			}

			NSString * nsTitle = [[NSString alloc] initWithUTF8String:newTitle];
			[pNSWindow SetTitle:nsTitle];
		}
		}

		void OSXUpdateFrameGeometry(
				NSWindow * pNSWindow,
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
		{
		@autoreleasepool
		{
			const auto frameGeometry = _OSXCheckFrameGeometryUpdate( pNSWindow, pFrameGeometry, pUpdateFlags );

			if( pUpdateFlags.is_set( eFrameGeometryUpdateFlagStyleBit ) )
			{
				[pNSWindow setStyleMask:static_cast<NSWindowStyleMask>( frameGeometry.mStyle )];
			}

			if( pUpdateFlags.is_set( eFrameGeometryUpdateFlagPositionBit ) )
			{
				[pNSWindow setFrameOrigin:frameGeometry.mFrameRect.origin];
			}

			if( pUpdateFlags.is_set( eFrameGeometryUpdateFlagSizeBit ) )
			{
				[pNSWindow setFrame:[pNSWindow frameRectForContentRect:frameGeometry.mFrameRect] display:YES];
			}
		}
		}

		FrameSize OSXGetFrameSize( NSWindow * pNSWindow, EFrameSizeMode pSizeMode )
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

		NSUInteger OSXTranslateFrameStyle( EFrameStyle pStyle )
		{
			//
			constexpr NSUInteger cvCaptionFrameStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable;
			//
			constexpr NSUInteger cvFixedFrameStyle = cvCaptionFrameStyle | NSWindowStyleMaskMiniaturizable;
			//
			constexpr NSUInteger cvOverlayFrameStyle = NSWindowStyleMaskBorderless;
			//
			constexpr NSUInteger cvResizeableFrameStyle = cvFixedFrameStyle | NSWindowStyleMaskResizable;

			NSUInteger resultStyle = cppx::meta::limits<NSUInteger>::max_value;

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

		OSXFrameGeometry _OSXCheckFrameGeometry( NSScreen * pNSScreen, const FrameGeometry & pFrameGeometry )
		{
			// Alternatively: CGDisplayPixelsHigh( kCGDirectMainDisplay ) ??
			const auto screenRect = [pNSScreen frame];

			auto xOrigin = static_cast<CGFloat>( pFrameGeometry.mPosition.x );

			NSRect frameRect{};
			frameRect.origin.x = static_cast<CGFloat>( pFrameGeometry.mPosition.x );
			frameRect.origin.y = static_cast<CGFloat>( pFrameGeometry.mPosition.y );
			frameRect.size.width = static_cast<CGFloat>( pFrameGeometry.size.x );
			frameRect.size.height = static_cast<CGFloat>( pFrameGeometry.size.y );

			frameRect.origin.x = cppx::get_max_of( frameRect.origin.x, 0.0f );
			frameRect.origin.y = screenRect.size.height - frameRect.origin.y - frameRect.size.height;

			OSXFrameGeometry osxGeometry{};
			osxGeometry.mFrameRect = frameRect;
			osxGeometry.mStyle = OSXTranslateFrameStyle( pFrameGeometry.mStyle );

			return osxGeometry;
		}

		OSXFrameGeometry _OSXCheckFrameGeometryUpdate(
				NSWindow * pNSWindow,
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
		{
			OSXFrameGeometry osxGeometry{};

			if( pUpdateFlags.is_set( eFrameGeometryUpdateFlagStyleBit ) )
			{
				osxGeometry.mStyle = OSXTranslateFrameStyle( pFrameGeometry.mStyle );
			}
			else
			{
				osxGeometry.mStyle = [pNSWindow styleMask];
			}

			if( pUpdateFlags.is_set_any_of( eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeBit ) )
			{
				const auto screenRect = [[pNSWindow screen] frame];

				NSRect frameRect{};
				frameRect.origin.x = static_cast<CGFloat>( pFrameGeometry.mPosition.x );
				frameRect.origin.y = static_cast<CGFloat>( pFrameGeometry.mPosition.y );
				frameRect.size.width = static_cast<CGFloat>( pFrameGeometry.size.x );
				frameRect.size.height = static_cast<CGFloat>( pFrameGeometry.size.y );

				frameRect.origin.x = cppx::get_max_of( frameRect.origin.x, 0.0f );
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
#endif // PCL_TARGET_SYSAPI_OSX
