
#ifndef __IC3_SYSTEM_FILE_MANAGER_NATIVE_H__
#define __IC3_SYSTEM_FILE_MANAGER_NATIVE_H__

#include "FileCommon.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
#  include "../Internal/Platform/OSAPI/Android/AndroidFileManager.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
#  include "../Internal/Platform/OSAPI/Win32/Win32FileManager.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
#  include "../Internal/Platform/OSAPI/X11/X11FileManager.h"
#endif

#endif //__IC3_SYSTEM_FILE_MANAGER_NATIVE_H__
