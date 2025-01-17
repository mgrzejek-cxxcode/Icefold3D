
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_PREREQUISITES_H__

#include <Ic3/Graphics/GCI/Memory/CommonMemoryDefs.h>
#include <Ic3/System/gfxVulkan.h>

#if( IC3_BUILD_STATIC )
#  define IC3_GX_VKCOMMON_API
#  define IC3_GX_VKCOMMON_CLASS
#  define IC3_GX_VKCOMMON_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_VKCOMMON_BUILD )
#    define IC3_GX_VKCOMMON_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GX_VKCOMMON_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GX_VKCOMMON_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_GX_VKCOMMON_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GX_VKCOMMON_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GX_VKCOMMON_OBJ    PCL_ATTR_DLL_IMPORT
#  endif
#endif

#include "Prerequisites/VK_commonUtils.h"
#include "Prerequisites/VK_constantsMap.h"
#include "Prerequisites/VK_factoryUtils.h"

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( VKGpuDevice );
	ic3DeclareClassHandle( VKGpuDriver );
	ic3DeclareClassHandle( VKPresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_PREREQUISITES_H__
