
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_DX11_API_TRANSLATION_LAYER_H__

#include "DX11Prerequisites.h"
#include <Ic3/Graphics/HW3D/DX/DXAPITranslationLayer.h>

namespace Ic3::Graphics::GCI
{

	namespace ATL
	{

		ComPtr<ID3D11Device1> createD3D11Device( D3D_DRIVER_TYPE pDeviceType, Bitmask<UINT> pCreateFlags );

		ComPtr<ID3D11Debug> queryD3D11DebugInterfaceForD3D11Device( const ComPtr<ID3D11Device1> & pD3D11Device1 );

		ComPtr<IDXGIFactory2> queryDXGIFactoryForD3D11Device( const ComPtr<ID3D11Device1> & pD3D11Device1 );

		ComPtr<IDXGISwapChain1> createD3D11SwapChainForSystemWindow( DX11GPUDevice & pDX11GPUDevice, void * pSysWindow /* It must be a Ic3::System::Window */ );

		UINT translateDX11GPUDeviceCreateFlags( Bitmask<EGPUDriverConfigFlags> pDriverConfigFlags );

		D3D11_BLEND translateDX11BlendFactor( EBlendFactor pBlendFactor );

		D3D11_BLEND_OP translateDX11BlendOp( EBlendOp pBlendOp );

		D3D11_BLEND_OP translateDX11BlendOp( EBlendOp pBlendOp );

		UINT8 translateDX11BlendRenderTargetWriteMask( Bitmask<EBlendWriteMaskFlags> pWriteMask );

		Bitmask<D3D11_CLEAR_FLAG> translateDX11RTClearDepthStencilFlags( Bitmask<ERenderTargetBufferFlags> pClearFlags );

		D3D11_COMPARISON_FUNC translateDX11CompFunc( ECompFunc pCompFunc );

		D3D11_CULL_MODE translateDX11CullMode( ECullMode pCullMode );

		D3D11_DEPTH_WRITE_MASK translateDX11DepthWriteMask( EDepthWriteMask pDepthWriteMask );

		UINT translateDX11BufferBindFlags( Bitmask<resource_flags_value_t> pBufferFlags );

		D3D11_MAP translateDX11BufferMapFlags( EGPUMemoryMapMode pMapMode, Bitmask<EGPUMemoryFlags> pMemoryFlags );

		D3D11_FILL_MODE translateDX11PrimitiveFillMode( EPrimitiveFillMode pFillMode );

		D3D11_PRIMITIVE_TOPOLOGY translateDX11PrimitiveTopology( EPrimitiveTopology pTopology );

		D3D11_SHADER_TYPE translateDX11EShaderType( EShaderType pShaderType );

		D3D11_STENCIL_OP translateDX11StencilOp( EStencilOp pStencilOp );

		D3D11_TEXTURE_ADDRESS_MODE translateDX11ETextureAddressMode( ETextureAddressMode pAddressMode );

		UINT translateDX11ETextureBindFlags( Bitmask<resource_flags_value_t> pTextureFlags );

		D3D11_FILTER translateDX11ETextureFilter( ETextureFilter magFilter, ETextureFilter minFilter, ETextureMipMode mipMode, uint32 anisotropyLevel );

	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_API_TRANSLATION_LAYER_H__
