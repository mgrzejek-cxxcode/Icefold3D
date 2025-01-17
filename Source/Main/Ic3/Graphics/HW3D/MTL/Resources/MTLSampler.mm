
#include "MTLsampler.h"
#include "../MTLUDevice.h"
#include "../MTL_ApiTranslationLayer.h"

namespace Ic3::Graphics::GCI
{

	MetalSampler::MetalSampler( MetalGPUDevice & pGPUDevice, id<MTLSamplerState> pMTLSamplerState )
	: Sampler( pGPUDevice )
	, mMTLSamplerState( pMTLSamplerState )
	{}

	MetalSampler::~MetalSampler() = default;

	MetalSamplerHandle MetalSampler::CreateSampler( MetalGPUDevice & pGPUDevice, const SamplerCreateInfo & pCreateInfo )
	{
		auto * mtlSamplerDescriptor = TranslateSamplerConfig( pCreateInfo.samplerConfig );
		auto * mtlSamplerState = [pGPUDevice.mMTLDevice newSamplerStateWithDescriptor:mtlSamplerDescriptor];

		[mtlSamplerDescriptor dealloc];

		auto metalSampler = CreateGfxObject<MetalSampler>( pGPUDevice, mtlSamplerState );

		return metalSampler;
	}

	MTLSamplerDescriptor * MetalSampler::TranslateSamplerConfig( const SamplerConfig & pSamplerConfig )
	{
		auto * mtlSamplerDescriptor = [[MTLSamplerDescriptor alloc] init];

		const auto borderColor = ATL::selectMTLSamplerBorderColor( pSamplerConfig.borderPredefinedColor, pSamplerConfig.borderColor );
		[mtlSamplerDescriptor setBorderColor:borderColor];

		const auto rAddressMode = ATL::translateMTLTextureAddressMode( pSamplerConfig.addressModeConfig.coordU );
		const auto sAddressMode = ATL::translateMTLTextureAddressMode( pSamplerConfig.addressModeConfig.coordV );
		const auto tAddressMode = ATL::translateMTLTextureAddressMode( pSamplerConfig.addressModeConfig.coordW );

		[mtlSamplerDescriptor setRAddressMode:rAddressMode];
		[mtlSamplerDescriptor setSAddressMode:sAddressMode];
		[mtlSamplerDescriptor setTAddressMode:tAddressMode];

		const auto minFilter = ATL::translateMTLTextureFilter( pSamplerConfig.filterConfig.minFilter );
		const auto magFilter = ATL::translateMTLTextureFilter( pSamplerConfig.filterConfig.magFilter );
		const auto mipMode = ATL::translateMTLTextureMipMode( pSamplerConfig.filterConfig.mipMode );

		[mtlSamplerDescriptor setMinFilter:minFilter];
		[mtlSamplerDescriptor setMagFilter:magFilter];
		[mtlSamplerDescriptor setMipFilter:mipMode];
		[mtlSamplerDescriptor setMaxAnisotropy:pSamplerConfig.filterConfig.anisotropyLevel];

		const auto compFunction = ATL::translateMTLCompFunc( pSamplerConfig.textureCompareFunc );
		[mtlSamplerDescriptor setCompareFunction:compFunction];

		[mtlSamplerDescriptor setLodMinClamp:pSamplerConfig.mipLODRange.begin];
		[mtlSamplerDescriptor setLodMaxClamp:pSamplerConfig.mipLODRange.end];

		[mtlSamplerDescriptor setNormalizedCoordinates:TRUE];

		return mtlSamplerDescriptor;
	}

}
