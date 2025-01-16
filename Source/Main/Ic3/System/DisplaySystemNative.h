
#ifndef __IC3_SYSTEM_INTERNAL_DISPLAY_SYSTEM_NATIVE_H__
#define __IC3_SYSTEM_INTERNAL_DISPLAY_SYSTEM_NATIVE_H__

#include "DisplaySystem.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
#  include "Internal/Platform/OSAPI/Android/AndroidDisplaySystem.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_OSX )
#  include "Internal/Platform/OSAPI/OSX/OSXDisplaySystem.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
#  include "Internal/Platform/OSAPI/Win32/Win32DisplaySystem.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
#  include "Internal/Platform/OSAPI/X11/X11DisplaySystem.h"
#endif

#if( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_SDL )
#  include "Internal/Platform/Shared/SDL/SDLDisplaySystem.h"
#endif

#endif // __IC3_SYSTEM_INTERNAL_DISPLAY_SYSTEM_NATIVE_H__
