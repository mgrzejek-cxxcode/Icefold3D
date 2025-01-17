
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
#  define IC3_GX_DX11_API
#  define IC3_GX_DX11_CLASS
#  define IC3_GX_DX11_OBJ    extern
#else
#  if( IC3_DRIVER_GRAPHICS_HW3D_DX11_BUILD )
#    define IC3_GX_DX11_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GX_DX11_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GX_DX11_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_GX_DX11_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GX_DX11_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GX_DX11_OBJ    PCL_ATTR_DLL_IMPORT
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

	Ic3GCIDeclareClassHandle( DX11CommandList );
	Ic3GCIDeclareClassHandle( DX11CommandSystem );
	Ic3GCIDeclareClassHandle( DX11GpuDevice );
	Ic3GCIDeclareClassHandle( DX11GpuDriver );
	Ic3GCIDeclareClassHandle( DX11PresentationLayer );

	Ic3GCIDeclareTypedefHandle( DX11GraphicsShaderLinkageImmutableState, GraphicsShaderLinkageImmutableStateSeparable );
	Ic3GCIDeclareTypedefHandle( DX11RenderPassConfigurationImmutableState, RenderPassConfigurationImmutableStateDefault );


	namespace SMU
	{

		CPPX_ATTR_NO_DISCARD ID3D11DeviceContext1 * GetD3D11DeviceContextFromCommandList( void * pDX11CommandList );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PREREQUISITES_H__
