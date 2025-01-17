
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_SAMPLER_H__
#define __IC3_GRAPHICS_HW3D_DX11_SAMPLER_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/State/Sampler.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( DX11Sampler );

	class DX11Sampler : public Sampler
	{
	public:
		ComPtr<ID3D11SamplerState> const mD3D11SamplerState;

		DX11Sampler( DX11GpuDevice & pGpuDevice, ComPtr<ID3D11SamplerState> pD3D11SamplerState );
		virtual ~DX11Sampler();

		static DX11SamplerHandle Create( DX11GpuDevice & pDX11GpuDevice, const SamplerCreateInfo & pCreateInfo );

	private:
		static bool TranslateSamplerConfig( const SamplerConfig & pSamplerConfig, D3D11_SAMPLER_DESC & pOutD3D11SamplerConfig );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_SAMPLER_H__
