
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLES3_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_GLES3_PREREQUISITES_H__

#include <Ic3/Graphics/HW3D/GL/GLPrerequisites.h>

#if( IC3_BUILD_STATIC )
#  define IC3_GX_GLES3_API
#  define IC3_GX_GLES3_CLASS
#  define IC3_GX_GLES3_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_GLES3_BUILD )
#    define IC3_GX_GLES3_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GX_GLES3_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GX_GLES3_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_GX_GLES3_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GX_GLES3_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GX_GLES3_OBJ    PCL_ATTR_DLL_IMPORT
#  endif
#endif

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( GLES3CommandContext );
	ic3DeclareClassHandle( GLES3CommandSystem );
	ic3DeclareClassHandle( GLES3GpuDevice );
	ic3DeclareClassHandle( GLES3GpuDriver );
	ic3DeclareClassHandle( GLPresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLES3_PREREQUISITES_H__
