
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_DX11_PREREQUISITES_H__

#include <Ic3/Graphics/HW3D/DX/DXPrerequisites.h>

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <d3d11sdklayers.h>
#include <d3d11shader.h>
#include <d3d11shadertracing.h>

#if( IC3_BUILD_STATIC )
#  define ICFGX_DX11_API
#  define ICFGX_DX11_CLASS
#  define ICFGX_DX11_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_DX11_BUILD )
#    define ICFGX_DX11_API    IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_DX11_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define ICFGX_DX11_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define ICFGX_DX11_API    IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_DX11_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define ICFGX_DX11_OBJ    IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

namespace Ic3::Graphics::GCI
{

	class GraphicsShaderLinkageImmutableStateSeparable;
	class RenderPassConfigurationImmutableStateDefault;

	class DX11BlendImmutableState;
	class DX11DepthStencilImmutableState;
	class DX11RasterizerImmutableState;
	class DX11IAInputLayoutImmutableState;
	class DX11IAVertexStreamImmutableState;
	class DX11RenderTargetBindingImmutableState;

	ic3GpaDeclareClassHandle( DX11CommandList );
	ic3GpaDeclareClassHandle( DX11CommandSystem );
	ic3GpaDeclareClassHandle( DX11GPUDevice );
	ic3GpaDeclareClassHandle( DX11GPUDriver );
	ic3GpaDeclareClassHandle( DX11PresentationLayer );

	ic3GpaDeclareTypedefHandle( DX11GraphicsShaderLinkageImmutableState, GraphicsShaderLinkageImmutableStateSeparable );
	ic3GpaDeclareTypedefHandle( DX11RenderPassConfigurationImmutableState, RenderPassConfigurationImmutableStateDefault );


	namespace smutil
	{

		IC3_ATTR_NO_DISCARD ID3D11DeviceContext1 * getD3D11DeviceContextFromCommandList( void * pDX11CommandList );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PREREQUISITES_H__
