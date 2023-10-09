
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_PREREQUISITES_H__

#include <Ic3/Graphics/GCI/Memory/CommonGPUMemoryDefs.h>

#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgidebug.h>

#include <d3dcompiler.h>

#if( IC3_DRIVER_GRAPHICS_HW3D_DX12_BUILD )
#  include <dxgi1_3.h>
#  include <dxgi1_4.h>
#endif

#if( IC3_BUILD_STATIC )
#  define ICFGX_DXCOMMON_API
#  define ICFGX_DXCOMMON_CLASS
#  define ICFGX_DXCOMMON_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_DXCOMMON_BUILD )
#    define ICFGX_DXCOMMON_API    IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_DXCOMMON_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_DXCOMMON_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define ICFGX_DXCOMMON_API    IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_DXCOMMON_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_DXCOMMON_OBJ    IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

#include "Prerequisites/DXCommonShaderDefs.h"
#include "Prerequisites/DXErrorUtils.h"

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( DXGPUDevice );
	ic3DeclareClassHandle( DXGPUDriver );
	ic3DeclareClassHandle( DXPresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_PREREQUISITES_H__
