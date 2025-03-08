
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_NS_INTERNAL_NS_WINDOW_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_NS_INTERNAL_NS_WINDOW_H__

#include "NSICommon.h"
#include <Ic3/System/Core/WindowCommon.h>
#import <AppKit/NSWindow.h>

/// @brief
@interface NSOSXWindow : NSWindow

-( BOOL ) canBecomeKeyWindow;

-( BOOL ) canBecomeMainWindow;

-( void ) sendEvent:( NSEvent * )pEvent;

-( void ) doCommandBySelector:( SEL )pSelector;

-( void ) keyDown:( NSEvent * )pEvent;

-( void ) mouseDown:( NSEvent * )pEvent;

-( void ) rightMouseDown:( NSEvent * )pEvent;

@end


/// @brief
@interface NSOSXWindowView : NSView
{
	@public NSOSXWindow * mNSWindow;
}

-( NSOSXWindowView * ) initForWindow:( NSOSXWindow * )pWindow;

-( BOOL ) acceptsFirstResponder;

-( void ) keyDown:( NSEvent * )pEvent;

-( void ) mouseDown:( NSEvent * )pEvent;

-( void ) rightMouseDown:( NSEvent * )pEvent;

@end

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_NS_INTERNAL_NS_WINDOW_H__
