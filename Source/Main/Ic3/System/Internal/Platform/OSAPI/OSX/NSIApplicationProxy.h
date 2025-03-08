
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_NS_OSX_APPLICATION_PROXY_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_NS_OSX_APPLICATION_PROXY_H__

#include "NSICommon.h"
#include <Ic3/System/Core/WindowCommon.h>
#import <AppKit/NSApplication.h>
#import <AppKit/NSWindow.h>

namespace Ic3::System
{

	class OSXSysContext;
	class OSXEventController;

}

@interface NSApplication( NSAppleMenu )
-( void ) setAppleMenu:( NSMenu * )pMenu;
@end

/// @brief
@interface NSOSXApplicationProxy : NSApplication

-( void ) terminate:( id )pSender;

-( void ) sendEvent:( NSEvent * )pEvent;

+( void ) registerUserDefaults;

@end


@interface NSOSXApplicationDelegate : NSObject<NSApplicationDelegate>
{
	Ic3::System::OSXSysContext * mOSXSysContext;
}

-( void ) setOSXSysContext: ( Ic3::System::OSXSysContext * )pOSXSysContext;

-( id ) init;

-( void ) dealloc;

-( void ) applicationDidBecomeActiveNotificationHandler:( NSNotification * )pNotification;

-( void ) currentLocaleDidChangeNotificationHandler:( NSNotification * )pNotification;

-( void ) windowWillCloseNotificationHandler:( NSNotification * )pNotification;

-( void ) applicationWillFinishLaunching:( NSNotification * )pNotification;

-( void ) applicationDidFinishLaunching:( NSNotification * )pNotification;

-( BOOL ) applicationShouldTerminateAfterLastWindowClosed:( NSApplication * )pApplication;

-( NSApplicationTerminateReply ) applicationShouldTerminate:( NSApplication * )pSender;

-( void ) applicationWillTerminate:( NSNotification * )pNotification;

@end

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_NS_OSX_APPLICATION_PROXY_H__
