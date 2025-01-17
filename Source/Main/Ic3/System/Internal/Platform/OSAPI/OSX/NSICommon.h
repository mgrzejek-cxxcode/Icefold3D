
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_NS_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_NS_COMMON_H__

#include <Ic3/System/Prerequisites.h>
#include <objc/objc.h>

#import <AppKit/NSWindow.h>
#import <AppKit/NSApplication.h>

// NSApplication(NSScripting) provides [orderedWindows] property.
#import <AppKit/NSApplicationScripting.h>

namespace Ic3::System
{

    // Required to create the alias below.
    namespace Platform
    {}

    class OSXSysContext;

	Ic3SysDeclareHandle( OSXDisplayManager  );
	Ic3SysDeclareHandle( OSXDisplayDriver   );
	Ic3SysDeclareHandle( OSXEventController );
	Ic3SysDeclareHandle( OSXWindowManager   );

}

namespace Platform = Ic3::System::Platform;

using OSXSysContext = Ic3::System::OSXSysContext;
using OSXDisplayManager = Ic3::System::OSXDisplayManager;
using OSXDisplayDriver = Ic3::System::OSXDisplayDriver;
using OSXEventController = Ic3::System::OSXEventController;
using OSXWindowManager = Ic3::System::OSXWindowManager;

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_NS_COMMON_H__
