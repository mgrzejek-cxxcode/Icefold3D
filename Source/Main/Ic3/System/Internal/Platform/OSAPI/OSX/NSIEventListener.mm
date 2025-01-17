
#include "NSIEventListener.h"
#include "OSXEventCore.h"
#include <Ic3/System/Internal/EventCorePrivate.h>

@implementation NSOSXEventListener

-( NSOSXEventListener * ) initForNSWindow:( NSWindow * ) pNSWindow
{
	[super init];
	mNSWindow = pNSWindow;
	mNSView = [mNSWindow contentView];
	return self;
}

-( void ) bind:( OSXEventController * ) pEventController
{
@autoreleasepool
{
	Ic3DebugAssert( mNSWindow != nil );

	mEventController = pEventController;

	if( [mNSWindow delegate] != nil )
	{
		NSNotificationCenter * nsNotificationCenter = [NSNotificationCenter defaultCenter];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidExpose:) name:NSWindowDidExposeNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidMove:) name:NSWindowDidMoveNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidResize:) name:NSWindowDidResizeNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidMiniaturize:) name:NSWindowDidMiniaturizeNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidDeminiaturize:) name:NSWindowDidDeminiaturizeNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidBecomeKey:) name:NSWindowDidBecomeKeyNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidResignKey:) name:NSWindowDidResignKeyNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidChangeBackingProperties:) name:NSWindowDidChangeBackingPropertiesNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidChangeScreenProfile:) name:NSWindowDidChangeScreenProfileNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowWillEnterFullScreen:) name:NSWindowWillEnterFullScreenNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidEnterFullScreen:) name:NSWindowDidEnterFullScreenNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowWillExitFullScreen:) name:NSWindowWillExitFullScreenNotification object:mNSWindow];
		[nsNotificationCenter addObserver:self selector:@selector(windowDidExitFullScreen:) name:NSWindowDidExitFullScreenNotification object:mNSWindow];
		//[nsNotificationCenter addObserver:self selector:@selector(windowDidFailToEnterFullScreen:) name:@"NSWindowDidFailToEnterFullScreenNotification" object:mNSWindow];
		//[nsNotificationCenter addObserver:self selector:@selector(windowDidFailToExitFullScreen:) name:@"NSWindowDidFailToExitFullScreenNotification" object:mNSWindow];
	}
	else
	{
		[mNSWindow setDelegate:self];
	}

	[mNSWindow addObserver:self forKeyPath:@"visible" options:NSKeyValueObservingOptionNew context:nullptr];

	[mNSWindow setNextResponder:self];
	[mNSWindow setAcceptsMouseMovedEvents:YES];
}
}

-( void ) unbind
{
@autoreleasepool
{
	if( [mNSWindow delegate] != self )
	{
		NSNotificationCenter * nsNotificationCenter = [NSNotificationCenter defaultCenter];
		[nsNotificationCenter removeObserver:self name:NSWindowDidExposeNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidMoveNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidResizeNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidMiniaturizeNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidDeminiaturizeNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidBecomeKeyNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidResignKeyNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidChangeBackingPropertiesNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidChangeScreenProfileNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowWillEnterFullScreenNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidEnterFullScreenNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowWillExitFullScreenNotification object:mNSWindow];
		[nsNotificationCenter removeObserver:self name:NSWindowDidExitFullScreenNotification object:mNSWindow];
		//[nsNotificationCenter removeObserver:self name:@"NSWindowDidFailToEnterFullScreenNotification" object:mNSWindow];
		//[nsNotificationCenter removeObserver:self name:@"NSWindowDidFailToExitFullScreenNotification" object:mNSWindow];
	}
	else
	{
		[mNSWindow setDelegate:nil];
	}

	[mNSWindow removeObserver:self forKeyPath:@"visible"];

	if( [mNSWindow nextResponder] == self )
	{
		[mNSWindow setNextResponder:nil];
	}

	if( [mNSView nextResponder] == self )
	{
		[mNSView setNextResponder:nil];
	}

	mEventController = nullptr;
}
}

-( void ) observeValueForKeyPath:( NSString * ) pKeyPath
          ofObject:( id ) pObject
          change:( NSDictionary * ) pChange
          context:( void * ) pContext
{
	if( (  pObject == mNSWindow ) && [pKeyPath IsEqualToString:@"visible"] )
	{
		int newVisibility = [[pChange objectForKey:@"new"] intValue];
		if( newVisibility != 0 )
		{
			// visible
		}
		else
		{
			// hidden
		}
	}
}

-( void ) pauseVisibleObservation
{
}

-( void ) resumeVisibleObservation
{
}

#define Ic3DbgEventLog(...) // Ic3DebugOutput(__VA_ARGS__)

-( void ) keyDown:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("keyDown");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) keyUp:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("keyUp");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) flagsChanged:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("flagsChanged");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) mouseEntered:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("mouseEntered");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) mouseExited:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("mouseExited");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) mouseMoved:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("mouseMoved");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) mouseDown:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("mouseDown");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) mouseUp:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("mouseUp");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) mouseDragged:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("mouseDragged");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) rightMouseDown:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("rightMouseDown");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) rightMouseUp:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("rightMouseUp");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) rightMouseDragged:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("rightMouseDragged");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) otherMouseDown:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("otherMouseDown");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) otherMouseUp:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("otherMouseUp");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) otherMouseDragged:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("otherMouseDragged");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) scrollWheel:( NSEvent * ) pEvent
{
	Ic3DbgEventLog("scrollWheel");
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) tabletPoint:( NSEvent * ) pEvent
{
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) tabletProximity:( NSEvent * ) pEvent
{
	nativeEventDispatch( *mEventController, Platform::NativeEventType( pEvent ) );
}

-( void ) touchesBeganWithEvent:( NSEvent * ) pEvent
{
}

-( void ) touchesMovedWithEvent:( NSEvent * ) pEvent
{
}

-( void ) touchesEndedWithEvent:( NSEvent * ) pEvent
{
}

-( void ) touchesCancelledWithEvent:( NSEvent * ) pEvent
{
}

-( void ) handleTouches:( NSTouchPhase ) pTouchPhase withEvent:( NSEvent * ) pEvent
{
}

-( void ) windowDidExpose:( NSNotification * ) pNotification
{
}

-( void ) windowDidMove:( NSNotification * ) pNotification
{
}

-( void ) windowDidResize:( NSNotification * ) pNotification
{
}

-( void ) windowDidMiniaturize:( NSNotification * ) pNotification
{
}

-( void ) windowDidDeminiaturize:( NSNotification * ) pNotification
{
}

-( void ) windowDidBecomeKey:( NSNotification * ) pNotification
{
}

-( void ) windowDidResignKey:( NSNotification * ) pNotification
{
}

-( void ) windowDidChangeBackingProperties:( NSNotification * ) pNotification
{
}

-( void ) windowDidChangeScreenProfile:( NSNotification * ) pNotification
{
}

-( void ) windowWillEnterFullScreen:( NSNotification * ) pNotification
{
}

-( void ) windowDidEnterFullScreen:( NSNotification * ) pNotification
{
}

-( void ) windowWillExitFullScreen:( NSNotification * ) pNotification
{
}

-( void ) windowDidExitFullScreen:( NSNotification * ) pNotification
{
}

-( void ) windowWillClose:( NSNotification * ) pNotification
{
	nativeEventDispatch( *mEventController, Platform::NativeEventType( Platform::eOSXEventIDWindowWillClose, pNotification ) );
}

-( BOOL ) windowShouldClose:( id ) pSender
{
	return YES;
}

-( NSApplicationPresentationOptions ) window:( NSWindow * ) pWindow
                                      willUseFullScreenPresentationOptions:( NSApplicationPresentationOptions ) pProposedOptions
{
	return pProposedOptions;
}

@end
