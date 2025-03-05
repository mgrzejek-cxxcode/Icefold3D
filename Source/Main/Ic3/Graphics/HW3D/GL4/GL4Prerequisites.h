
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GL4_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_GL4_PREREQUISITES_H__

#include <Ic3/Graphics/HW3D/GL/GLPrerequisites.h>

#if( IC3_BUILD_STATIC )
#  define IC3_GX_GL4_API
#  define IC3_GX_GL4_CLASS
#  define IC3_GX_GL4_OBJ    extern
#else
#  if( IC3_GRAPHICS_HWAPI_GL4_BUILD )
#    define IC3_GX_GL4_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GX_GL4_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GX_GL4_OBJ    PCL_ATTR_DLL_EXPORT extern
#  else
#    define IC3_GX_GL4_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GX_GL4_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GX_GL4_OBJ    PCL_ATTR_DLL_IMPORT extern
#  endif
#endif

#define IC3_GX_GL4_USE_COMPAT_API 0

#if defined( IC3_GX_GL4_USE_COMPAT_API )
#  if( PCL_TARGET_OS == PCL_TARGET_OS_MACOS )
#    undef IC3_GX_GL4_USE_COMPAT_API
#    define IC3_GX_GL4_USE_COMPAT_API 1
#    pragma message( "Core OpenGL API not available on macOS. Overwriting IC3_GX_GL4_USE_COMPAT_API with 0." )
#  endif
#else
#  define IC3_GX_GL4_USE_COMPAT_API 0
#endif

#if( !IC3_GX_GL4_USE_COMPAT_API )
#  pragma message( "OpenGL 4 Driver will use <Core> API." )
#else
#  pragma message( "OpenGL 4 Driver will use <Compat> API." )
#endif

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( GL4GPUDevice );
	Ic3DeclareClassHandle( GL4GPUDriver );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GL4_PREREQUISITES_H__
