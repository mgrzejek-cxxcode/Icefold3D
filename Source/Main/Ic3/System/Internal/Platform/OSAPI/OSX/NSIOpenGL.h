
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_NS_INTERNAL_OPENGL_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_NS_INTERNAL_OPENGL_H__

#include "OSXWindowSystem.h"
#include <Ic3/System/WindowCommon.h>

#define GL_SILENCE_DEPRECATION 1

#import <AppKit/NSOpenGL.h>
#import <AppKit/NSOpenGLView.h>
#import <stdatomic.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif // __clang__

/// @brief
@interface NSOSXOpenGLContext : NSOpenGLContext
{
	@private std::atomic_uint32_t _updateFlag;
}

- ( void ) setUpdateFlag;

- ( BOOL ) IsUpdateRequired;

- ( BOOL ) updateConditional;

- ( void ) updateExplicit;

@end

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_NS_INTERNAL_OPENGL_H__
