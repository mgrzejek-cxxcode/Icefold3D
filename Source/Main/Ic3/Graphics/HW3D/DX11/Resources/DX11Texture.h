
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_TEXTURE_H__
#define __IC3_GRAPHICS_HW3D_DX11_TEXTURE_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/Texture.h>
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( DX11Texture );

	class DX11RenderBuffer;

	struct DX11TextureUsageDesc
	{
		Bitmask<D3D11_BIND_FLAG> bindFlags;
		Bitmask<D3D11_CPU_ACCESS_FLAG> cpuAccessFlags;
		Bitmask<D3D11_RESOURCE_MISC_FLAG> resourceMiscFlags;
		D3D11_USAGE usage;
	};

	struct DX11TextureInitDataDesc
	{
		std::vector<D3D11_SUBRESOURCE_DATA> d3d11SubResourceDataArray;
		uint32 textureSubResourcesNum;
		uint32 pixelByteSize;

		explicit operator bool() const
		{
			return !d3d11SubResourceDataArray.empty();
		}
	};

	struct DX11TextureCreateInfo
	{
		ETextureClass texClass;
		TextureDimensions dimensions;
		DXGI_FORMAT dxgiTextureFormat;
		UINT msaaLevel;
		DX11TextureUsageDesc dx11UsageDesc;
		DX11TextureInitDataDesc dx11InitDataDesc;
		Bitmask<resource_flags_value_t> resourceFlags;
	};

	class DX11Texture : public Texture
	{
		friend class DX11RenderBuffer;

	public:
		DXGI_FORMAT const mDXGIInternalFormat;
		ComPtr<ID3D11Device1> const mD3D11Device1;
		ComPtr<ID3D11Texture2D> const mD3D11Texture2D;
		ComPtr<ID3D11Texture3D> const mD3D11Texture3D;
		ComPtr<ID3D11ShaderResourceView> const mD3D11DefaultSRV;

	public:
		DX11Texture(
				DX11GPUDevice & pDX11GPUDevice,
				const ResourceMemoryInfo & pResourceMemory,
				const TextureProperties & pTextureProperties,
				const TextureLayout & pTextureLayout,
				DXGI_FORMAT pDXGIInternalFormat,
				ComPtr<ID3D11Texture2D> pD3D11Texture2D,
				ComPtr<ID3D11ShaderResourceView> pD3D11DefaultSRV );

		DX11Texture(
				DX11GPUDevice & pDX11GPUDevice,
				const ResourceMemoryInfo & pResourceMemory,
				const TextureProperties & pTextureProperties,
				const TextureLayout & pTextureLayout,
				DXGI_FORMAT pDXGIInternalFormat,
				ComPtr<ID3D11Texture3D> pD3D11Texture3D,
				ComPtr<ID3D11ShaderResourceView> pD3D11DefaultSRV );

		virtual ~DX11Texture();

		static DX11TextureHandle createDefault(
				DX11GPUDevice & pDX11GPUDevice,
				const TextureCreateInfo & pCreateInfo );

		static DX11TextureHandle createForRenderTarget(
				DX11GPUDevice & pDX11GPUDevice,
				const RenderTargetTextureCreateInfo & pCreateInfo );

		static RenderTargetTextureHandle createRenderTargetTextureView(
				DX11GPUDevice & pDX11GPUDevice,
				const RenderTargetTextureCreateInfo & pCreateInfo );
	};


	namespace rcutil
	{

		struct DX11TextureData
		{
			ComPtr<ID3D11Texture2D> d3d11Texture2D;
			ComPtr<ID3D11Texture3D> d3d11Texture3D;

			DX11TextureData( const ComPtr<ID3D11Texture2D> & pD3D11Texture2D )
			: d3d11Texture2D( pD3D11Texture2D )
			{}

			DX11TextureData( const ComPtr<ID3D11Texture3D> & pD3D11Texture3D )
			: d3d11Texture3D( pD3D11Texture3D )
			{}

			DX11TextureData( std::nullptr_t )
			{}

			explicit operator bool() const
			{
				return d3d11Texture2D || d3d11Texture3D;
			}
		};

		IC3_ATTR_NO_DISCARD DX11TextureData createTextureResourceDX11(
				DX11GPUDevice & pDX11GPUDevice,
				const DX11TextureCreateInfo & pCreateInfo );

		IC3_ATTR_NO_DISCARD DX11TextureData create2DTextureResourceDX11(
				DX11GPUDevice & pDX11GPUDevice,
				const DX11TextureCreateInfo & pCreateInfo );

		IC3_ATTR_NO_DISCARD DX11TextureData create2DMSTextureResourceDX11(
				DX11GPUDevice & pDX11GPUDevice,
				const DX11TextureCreateInfo & pCreateInfo );

		IC3_ATTR_NO_DISCARD DX11TextureData create3DTextureResourceDX11(
				DX11GPUDevice & pDX11GPUDevice,
				const DX11TextureCreateInfo & pCreateInfo );

		IC3_ATTR_NO_DISCARD ComPtr<ID3D11ShaderResourceView> create2DTextureDefaultShaderResourceViewDX11(
				ETextureClass pTexClass,
				const ComPtr<ID3D11Texture2D> & pD3D11Texture2D );

		IC3_ATTR_NO_DISCARD ComPtr<ID3D11ShaderResourceView> create3DTextureDefaultShaderResourceViewDX11(
				const ComPtr<ID3D11Texture3D> & pD3D11Texture3D );

		IC3_ATTR_NO_DISCARD bool checkIsDXGIFormatDepthStencilDX11( DXGI_FORMAT pDXGIFormat );

		IC3_ATTR_NO_DISCARD bool checkIsDXGIFormatTypelessDX11( DXGI_FORMAT pDXGIFormat );

		IC3_ATTR_NO_DISCARD DXGI_FORMAT getTypelessFormatForDpethStencilFormatDX11( DXGI_FORMAT pDXGIFormat );

		DXGI_FORMAT getDSVFormatForTypelessFormatDX11( DXGI_FORMAT pDXGIFormat );

		DXGI_FORMAT getSRVFormatForTypelessFormatDX11( DXGI_FORMAT pDXGIFormat, Bitmask<ERenderTargetBufferFlags> pRTBufferMask );

		IC3_ATTR_NO_DISCARD DX11TextureInitDataDesc translateTextureInitDataDescDX11( const TextureCreateInfo & pCreateInfo );

		IC3_ATTR_NO_DISCARD DX11TextureUsageDesc translateTextureUsageDescDX11( const TextureCreateInfo & pCreateInfo );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_TEXTURE_H__
