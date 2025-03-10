
#ifndef __IC3_SYSTEM_FILE_MANAGER_NATIVE_H__
#define __IC3_SYSTEM_FILE_MANAGER_NATIVE_H__

#include "IOCommonDefs.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
#  include "../Internal/Platform/OSAPI/Android/AndroidFileSystem.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
#  include "../Internal/Platform/OSAPI/Win32/Win32FileSystem.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
#  include "../Internal/Platform/OSAPI/X11/X11FileSystem.h"
#endif

#endif //__IC3_SYSTEM_FILE_MANAGER_NATIVE_H__
