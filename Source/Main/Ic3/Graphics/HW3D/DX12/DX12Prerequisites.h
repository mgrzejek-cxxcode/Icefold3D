
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_DX12_PREREQUISITES_H__

#include <Ic3/Graphics/HW3D/DX/DXPrerequisites.h>

#include <dxgi1_3.h>
#include <dxgi1_4.h>

#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3d12shader.h>

#if( IC3_BUILD_STATIC )
#  define IC3_GX_DX12_API
#  define IC3_GX_DX12_CLASS
#  define IC3_GX_DX12_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_DX12_BUILD )
#    define IC3_GX_DX12_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GX_DX12_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GX_DX12_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_GX_DX12_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GX_DX12_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GX_DX12_OBJ    PCL_ATTR_DLL_IMPORT
#  endif
#endif

#include "Prerequisites/DX2coreDefs.h"

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( DX12CommandContext );
	Ic3DeclareClassHandle( DX12GpuCmdManager );
	Ic3DeclareClassHandle( DX12GpuDevice );
	Ic3DeclareClassHandle( DX12GpuDriver );
	Ic3DeclareClassHandle( DX12PresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_PREREQUISITES_H__
