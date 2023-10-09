
#ifndef __IC3_SYSTEM_EVENT_CORE_NATIVE_H__
#define __IC3_SYSTEM_EVENT_CORE_NATIVE_H__

#include "EventCommon.h"

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_ANDROID )
#  include "Internal/Platform/OSAPI/Android/AndroidEventCore.h"
#elif( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_WIN32 )
#  include "Internal/Platform/OSAPI/Win32/Win32EventCore.h"
#elif( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_X11 )
#  include "Internal/Platform/OSAPI/X11/X11EventCore.h"
#endif

#endif // __IC3_SYSTEM_EVENT_CORE_NATIVE_H__
