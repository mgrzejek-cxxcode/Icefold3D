
#ifndef __IC3_SYSTEM_WINDOW_NATIVE_H__
#define __IC3_SYSTEM_WINDOW_NATIVE_H__

#include "WindowSystem.h"
#include <list>

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_ANDROID )
#  include "Internal/Platform/OSAPI/Android/AndroidWindow.h"
#elif( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_OSX )
#  include "Internal/Platform/OSAPI/OSX/OSXWindowSystem.h"
#elif( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_WIN32 )
#  include "Internal/Platform/OSAPI/Win32/Win32WindowSystem.h"
#elif( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_X11 )
#  include "Internal/Platform/OSAPI/X11/X11WindowSystem.h"
#endif

namespace Ic3::System
{



} // namespace Ic3::System

#endif // __IC3_SYSTEM_WINDOW_NATIVE_H__
