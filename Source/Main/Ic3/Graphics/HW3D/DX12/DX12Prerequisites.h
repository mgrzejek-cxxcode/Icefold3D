
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
#  define ICFGX_DX12_API
#  define ICFGX_DX12_CLASS
#  define ICFGX_DX12_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_DX12_BUILD )
#    define ICFGX_DX12_API    IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_DX12_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_DX12_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define ICFGX_DX12_API    IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_DX12_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_DX12_OBJ    IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

#include "Prerequisites/DX2coreDefs.h"

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( DX12CommandContext );
	ic3DeclareClassHandle( DX12GPUCmdManager );
	ic3DeclareClassHandle( DX12GPUDevice );
	ic3DeclareClassHandle( DX12GPUDriver );
	ic3DeclareClassHandle( DX12PresentationLayer );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_PREREQUISITES_H__
