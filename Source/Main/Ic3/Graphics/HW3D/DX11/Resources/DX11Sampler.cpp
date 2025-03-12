
#include "DX11Sampler.h"
#include "../DX11APITranslationLayer.h"
#include "../DX11gpuDevice.h"

namespace Ic3::Graphics::GCI
{

	DX11Sampler::DX11Sampler( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11SamplerState> pD3D11SamplerState )
	: Sampler( pGPUDevice )
	, mD3D11SamplerState( std::move( pD3D11SamplerState ) )
	{ }

	DX11Sampler::~DX11Sampler() = default;

	DX11SamplerHandle DX11Sampler::Create( DX11GPUDevice & pDX11GPUDevice, const SamplerCreateInfo & pCreateInfo )
	{
		D3D11_SAMPLER_DESC d3d11SamplerConfig;
		if( !TranslateSamplerConfig( pCreateInfo.samplerConfig, d3d11SamplerConfig ) )
		{
			return nullptr;
		}

		ComPtr<ID3D11SamplerState> d3d11SamplerState;
		auto hResult = pDX11GPUDevice.mD3D11Device1->CreateSamplerState( &d3d11SamplerConfig, d3d11SamplerState.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		auto sampler = CreateGfxObject<DX11Sampler>( pDX11GPUDevice, d3d11SamplerState );

		return sampler;
	}

	bool DX11Sampler::TranslateSamplerConfig( const SamplerConfig & pSamplerConfig, D3D11_SAMPLER_DESC & pOutD3D11SamplerConfig )
	{
		pOutD3D11SamplerConfig.AddressU = ATL::TranslateDX11ETextureAddressMode( pSamplerConfig.addressModeConfig.coordU );
		pOutD3D11SamplerConfig.AddressV = ATL::TranslateDX11ETextureAddressMode( pSamplerConfig.addressModeConfig.coordV );
		pOutD3D11SamplerConfig.AddressW = ATL::TranslateDX11ETextureAddressMode( pSamplerConfig.addressModeConfig.coordW );
		pOutD3D11SamplerConfig.ComparisonFunc = ATL::TranslateDX11CompFunc( pSamplerConfig.textureCompareFunc );

		pOutD3D11SamplerConfig.MaxAnisotropy = pSamplerConfig.filterConfig.anisotropyLevel;
		pOutD3D11SamplerConfig.MinLOD = pSamplerConfig.mipLODRange.begin;
		pOutD3D11SamplerConfig.MaxLOD = pSamplerConfig.mipLODRange.end;
		pOutD3D11SamplerConfig.MipLODBias = pSamplerConfig.mipLODBias;

		pOutD3D11SamplerConfig.BorderColor[0] = pSamplerConfig.borderColor.uv_rgba[0];
		pOutD3D11SamplerConfig.BorderColor[1] = pSamplerConfig.borderColor.uv_rgba[1];
		pOutD3D11SamplerConfig.BorderColor[2] = pSamplerConfig.borderColor.uv_rgba[2];
		pOutD3D11SamplerConfig.BorderColor[3] = pSamplerConfig.borderColor.uv_rgba[3];

		pOutD3D11SamplerConfig.Filter = ATL::TranslateDX11ETextureFilter(
				pSamplerConfig.filterConfig.magFilter,
				pSamplerConfig.filterConfig.minFilter,
				pSamplerConfig.filterConfig.mipMode,
				pSamplerConfig.filterConfig.anisotropyLevel );

		return true;
	}

} // namespace Ic3::Graphics::GCI
