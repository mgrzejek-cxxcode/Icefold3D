
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
#  if( IC3_GRAPHICS_HW3D_DX_BUILD )
#    define IC3_GX_DX11_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GX_DX11_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GX_DX11_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_GX_DX11_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GX_DX11_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GX_DX11_OBJ    PCL_ATTR_DLL_IMPORT
#  endif
#endif

#define Ic3DebugOutputDX11( ... )                 Ic3DebugOutput(                 "Ic3.Graphics.HW3D.DX11", __VA_ARGS__ )
#define Ic3DebugOutputDX11Ex( pMessageType, ... ) Ic3DebugOutputEx( pMessageType, "Ic3.Graphics.HW3D.DX11", __VA_ARGS__ )

namespace Ic3::Graphics::GCI
{

	class GraphicsShaderLinkageDescriptorGenericSeparable;
	class RootSignatureDescriptorGeneric;

	class DX11BlendStateDescriptor;
	class DX11DepthStencilStateDescriptor;
	class DX11RasterizerStateDescriptor;
	class DX11VertexAttributeLayoutDescriptor;
	class DX11VertexSourceBindingDescriptor;
	class DX11RenderTargetDescriptor;

	Ic3GCIDeclareClassHandle( DX11CommandList );
	Ic3GCIDeclareClassHandle( DX11CommandSystem );
	Ic3GCIDeclareClassHandle( DX11GPUDevice );
	Ic3GCIDeclareClassHandle( DX11GPUDriver );
	Ic3GCIDeclareClassHandle( DX11PresentationLayer );

	Ic3GCIDeclareClassHandle( DX11RenderTargetDescriptor );

	Ic3GCIDeclareTypedefHandle( DX11GraphicsShaderLinkageDescriptor, GraphicsShaderLinkageDescriptorGenericSeparable );
	Ic3GCIDeclareTypedefHandle( DX11RootSignatureDescriptor, RootSignatureDescriptorGeneric );


	namespace Utilities
	{

		CPPX_ATTR_NO_DISCARD ID3D11DeviceContext1 * DX11GetD3D11DeviceContextFromCommandList( void * pDX11CommandList );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PREREQUISITES_H__
