
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_PREREQUISITES_H__

#include <Ic3/Graphics/GCI/Memory/CommonMemoryDefs.h>
#include <Ic3/System/gfxVulkan.h>

#if( IC3_BUILD_STATIC )
#  define ICFGX_VKCOMMON_API
#  define ICFGX_VKCOMMON_CLASS
#  define ICFGX_VKCOMMON_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_VKCOMMON_BUILD )
#    define ICFGX_VKCOMMON_API    IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_VKCOMMON_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_VKCOMMON_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define ICFGX_VKCOMMON_API    IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_VKCOMMON_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_VKCOMMON_OBJ    IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

#include "Prerequisites/VK_commonUtils.h"
#include "Prerequisites/VK_constantsMap.h"
#include "Prerequisites/VK_factoryUtils.h"

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( VKGPUDevice );
	ic3DeclareClassHandle( VKGPUDriver );
	ic3DeclareClassHandle( VKPresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_PREREQUISITES_H__
