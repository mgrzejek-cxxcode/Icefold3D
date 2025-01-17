
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VK13_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_VK13_PREREQUISITES_H__

#include <Ic3/GPUapiVK/VKPrerequisites.h>

#if( IC3_BUILD_STATIC )
#  define IC3_GX_VK10_API
#  define IC3_GX_VK10_CLASS
#  define IC3_GX_VK10_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_VK13_BUILD )
#    define IC3_GX_VK10_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GX_VK10_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GX_VK10_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_GX_VK10_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GX_VK10_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GX_VK10_OBJ    PCL_ATTR_DLL_IMPORT
#  endif
#endif

namespace Ic3::Graphics::GCI
{

	// Ic3DeclareClassHandle( VK10CommandContext );
	// Ic3DeclareClassHandle( VK10GPUCmdManager );
	// Ic3DeclareClassHandle( VK10GPUDevice );
	// Ic3DeclareClassHandle( VK10GPUDriver );
	// Ic3DeclareClassHandle( VK10PresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VK13_PREREQUISITES_H__
