
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLES3_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_GLES3_PREREQUISITES_H__

#include <Ic3/Graphics/HW3D/GL/GLPrerequisites.h>

#if( IC3_BUILD_STATIC )
#  define ICFGX_GLES3_API
#  define ICFGX_GLES3_CLASS
#  define ICFGX_GLES3_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_GLES3_BUILD )
#    define ICFGX_GLES3_API    IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_GLES3_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_GLES3_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define ICFGX_GLES3_API    IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_GLES3_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_GLES3_OBJ    IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( GLES3CommandContext );
	ic3DeclareClassHandle( GLES3CommandSystem );
	ic3DeclareClassHandle( GLES3GPUDevice );
	ic3DeclareClassHandle( GLES3GPUDriver );
	ic3DeclareClassHandle( GLPresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLES3_PREREQUISITES_H__
