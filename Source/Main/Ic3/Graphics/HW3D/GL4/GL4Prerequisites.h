
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GL4_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_GL4_PREREQUISITES_H__

#include <Ic3/Graphics/HW3D/GL/GLPrerequisites.h>

#if( IC3_BUILD_STATIC )
#  define ICFGX_GL4_API
#  define ICFGX_GL4_CLASS
#  define ICFGX_GL4_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_GL4_BUILD )
#    define ICFGX_GL4_API    IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_GL4_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_GL4_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define ICFGX_GL4_API    IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_GL4_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_GL4_OBJ    IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

#define ICFGX_GL4_USE_COMPAT_API 0

#if defined( ICFGX_GL4_USE_COMPAT_API )
#  if( IC3_PCL_TARGET_OS == IC3_PCL_TARGET_OS_MACOS )
#    undef ICFGX_GL4_USE_COMPAT_API
#    define ICFGX_GL4_USE_COMPAT_API 1
#    pragma message( "Core OpenGL API not available on macOS. Overwriting ICFGX_GL4_USE_COMPAT_API with 0." )
#  endif
#else
#  define ICFGX_GL4_USE_COMPAT_API 0
#endif

#if( !ICFGX_GL4_USE_COMPAT_API )
#  pragma message( "OpenGL 4 Driver will use <Core> API." )
#else
#  pragma message( "OpenGL 4 Driver will use <Compat> API." )
#endif

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( GL4GPUDevice );
	ic3DeclareClassHandle( GL4GPUDriver );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GL4_PREREQUISITES_H__
