
#ifndef __IC3_SYSTEM_GFX_OPENGL_NATIVE_H__
#define __IC3_SYSTEM_GFX_OPENGL_NATIVE_H__

#include "OpenGLDriver.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
#  include "../Internal/Platform/OSAPI/Android/AndroidOpenGLDriver.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_OSX )
#  include "../Internal/Platform/OSAPI/OSX/OSXOpenGLDriver.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
#  include "../Internal/Platform/OSAPI/Win32/Win32OpenGLDriver.h"
#elif( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
#  include "../Internal/Platform/OSAPI/X11/X11OpenGLDriver.h"
#endif

namespace Ic3::System
{
} // namespace Ic3::System

#endif // __IC3_SYSTEM_GFX_OPENGL_NATIVE_H__
