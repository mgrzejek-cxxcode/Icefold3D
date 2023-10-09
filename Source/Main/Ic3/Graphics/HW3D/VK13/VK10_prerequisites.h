
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VK13_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_VK13_PREREQUISITES_H__

#include <ic3/GPUapiVK/VKPrerequisites.h>

#if( IC3_BUILD_STATIC )
#  define ICFGX_VK10_API
#  define ICFGX_VK10_CLASS
#  define ICFGX_VK10_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_VK13_BUILD )
#    define ICFGX_VK10_API    IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_VK10_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_VK10_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define ICFGX_VK10_API    IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_VK10_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_VK10_OBJ    IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

namespace Ic3::Graphics::GCI
{

	// ic3DeclareClassHandle( VK10CommandContext );
	// ic3DeclareClassHandle( VK10GPUCmdManager );
	// ic3DeclareClassHandle( VK10GPUDevice );
	// ic3DeclareClassHandle( VK10GPUDriver );
	// ic3DeclareClassHandle( VK10PresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VK13_PREREQUISITES_H__
