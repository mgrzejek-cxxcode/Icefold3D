
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_DX11_API_TRANSLATION_LAYER_H__

#include "DX11Prerequisites.h"
#include <Ic3/Graphics/HW3D/DX/DXApiTranslationLayer.h>

namespace Ic3::Graphics::GCI
{

	namespace ATL
	{

		ComPtr<ID3D11Device1> CreateD3D11Device( D3D_DRIVER_TYPE pDeviceType, cppx::bitmask<UINT> pCreateFlags );

		ComPtr<ID3D11Debug> QueryD3D11DebugInterfaceForD3D11Device( const ComPtr<ID3D11Device1> & pD3D11Device1 );

		ComPtr<IDXGIFactory2> QueryDXGIFactoryForD3D11Device( const ComPtr<ID3D11Device1> & pD3D11Device1 );

		ComPtr<IDXGISwapChain1> CreateD3D11SwapChainForSystemWindow( DX11GPUDevice & pDX11GPUDevice, void * pSysWindow /* It must be a Ic3::System::Window */ );

		UINT TranslateDX11GPUDeviceCreateFlags( cppx::bitmask<EGPUDriverConfigFlags> pDriverConfigFlags );

		D3D11_BLEND TranslateDX11BlendFactor( EBlendFactor pBlendFactor );

		D3D11_BLEND_OP TranslateDX11BlendOp( EBlendOp pBlendOp );

		D3D11_BLEND_OP TranslateDX11BlendOp( EBlendOp pBlendOp );

		UINT8 TranslateDX11BlendRenderTargetWriteMask( cppx::bitmask<EBlendWriteMaskFlags> pWriteMask );

		cppx::bitmask<D3D11_CLEAR_FLAG> TranslateDX11RTClearDepthStencilFlags( cppx::bitmask<ERenderTargetBufferFlags> pClearFlags );

		D3D11_COMPARISON_FUNC TranslateDX11CompFunc( ECompFunc pCompFunc );

		D3D11_CULL_MODE TranslateDX11CullMode( ECullMode pCullMode );

		D3D11_DEPTH_WRITE_MASK TranslateDX11DepthWriteMask( EDepthWriteMask pDepthWriteMask );

		UINT TranslateDX11BufferBindFlags( cppx::bitmask<resource_flags_value_t> pBufferFlags );

		D3D11_MAP TranslateDX11BufferMapFlags( EGPUMemoryMapMode pMapMode, cppx::bitmask<EGPUMemoryFlags> pMemoryFlags );

		D3D11_FILL_MODE TranslateDX11PrimitiveFillMode( EPrimitiveFillMode pFillMode );

		D3D11_PRIMITIVE_TOPOLOGY TranslateDX11PrimitiveTopology( EPrimitiveTopology pTopology );

		D3D11_SHADER_TYPE TranslateDX11EShaderType( EShaderType pShaderType );

		D3D11_STENCIL_OP TranslateDX11StencilOp( EStencilOp pStencilOp );

		D3D11_TEXTURE_ADDRESS_MODE TranslateDX11ETextureAddressMode( ETextureAddressMode pAddressMode );

		UINT TranslateDX11ETextureBindFlags( cppx::bitmask<resource_flags_value_t> pTextureFlags );

		D3D11_FILTER TranslateDX11ETextureFilter( ETextureFilter magFilter, ETextureFilter minFilter, ETextureMipMode mipMode, uint32 anisotropyLevel );

	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_API_TRANSLATION_LAYER_H__
