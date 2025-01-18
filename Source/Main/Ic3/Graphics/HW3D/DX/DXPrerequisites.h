
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
#  define IC3_GX_DXCOMMON_API
#  define IC3_GX_DXCOMMON_CLASS
#  define IC3_GX_DXCOMMON_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_DXCOMMON_BUILD )
#    define IC3_GX_DXCOMMON_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GX_DXCOMMON_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GX_DXCOMMON_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_GX_DXCOMMON_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GX_DXCOMMON_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GX_DXCOMMON_OBJ    PCL_ATTR_DLL_IMPORT
#  endif
#endif

#include "Prerequisites/DXCommonShaderDefs.h"
#include "Prerequisites/DXErrorUtils.h"

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( DXGPUDevice );
	Ic3DeclareClassHandle( DXGPUDriver );
	Ic3DeclareClassHandle( DXPresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_PREREQUISITES_H__
