
#ifndef __IC3_SYSTEM_CORE_SYS_CONTEXT_NATIVE_H__
#define __IC3_SYSTEM_CORE_SYS_CONTEXT_NATIVE_H__

#include "Prerequisites.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
#  include "Internal/Platform/OSAPI/Android/AndroidSysContext.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_OSX )
#  include "Internal/Platform/OSAPI/OSX/OSXSysContext.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
#  include "Internal/Platform/OSAPI/Win32/Win32SysContext.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
#  include "Internal/Platform/OSAPI/X11/X11SysContext.h"
#endif

#endif // __IC3_SYSTEM_CORE_SYS_CONTEXT_NATIVE_H__
