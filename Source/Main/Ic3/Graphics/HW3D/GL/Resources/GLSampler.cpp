
#include "GLSampler.h"
#include "../GLAPITranslationLayer.h"
#include "../GLGPUDevice.h"

namespace Ic3::Graphics::GCI
{

	GLSampler::GLSampler( GLGPUDevice & pGPUDevice, GLSamplerObjectHandle pGLSamplerObject )
	: Sampler( pGPUDevice )
	, mGLSamplerObject( std::move( pGLSamplerObject ) )
	{ }

	GLSampler::~GLSampler() = default;

	GLSamplerHandle GLSampler::createSampler( GLGPUDevice & pGPUDevice, const SamplerCreateInfo & pCreateInfo )
	{
		GLSamplerState openglSamplerState;
		if( !translateSamplerConfig( pCreateInfo.samplerConfig, openglSamplerState ) )
		{
			return nullptr;
		}

		auto openglSamplerObject = GLSamplerObject::create( openglSamplerState );
		if( !openglSamplerObject )
		{
			return nullptr;
		}

		auto sampler = createGPUAPIObject<GLSampler>( pGPUDevice, std::move( openglSamplerObject ) );

		return sampler;
	}

	bool GLSampler::translateSamplerConfig( const SamplerConfig & pSamplerConfig, GLSamplerState & pOutSamplerState )
	{
		pOutSamplerState.borderColor = pSamplerConfig.borderColor;
		pOutSamplerState.mipLODRange.first = pSamplerConfig.mipLODRange.begin;
		pOutSamplerState.mipLODRange.second = pSamplerConfig.mipLODRange.end;

		pOutSamplerState.addressModeS = ATL::translateGLTextureAddressMode( pSamplerConfig.addressModeConfig.coordU );
		pOutSamplerState.addressModeT = ATL::translateGLTextureAddressMode( pSamplerConfig.addressModeConfig.coordV );
		pOutSamplerState.addressModeR = ATL::translateGLTextureAddressMode( pSamplerConfig.addressModeConfig.coordW );

		pOutSamplerState.magFilter = ATL::chooseGLTextureMagFilter( pSamplerConfig.filterConfig.magFilter, pSamplerConfig.filterConfig.mipMode );
		if( pOutSamplerState.magFilter == GL_INVALID_VALUE )
		{
			return false;
		}

		pOutSamplerState.minFilter = ATL::chooseGLTextureMinFilter( pSamplerConfig.filterConfig.minFilter, pSamplerConfig.filterConfig.mipMode );
		if( pOutSamplerState.minFilter == GL_INVALID_VALUE )
		{
			return false;
		}

	#if( ICFGX_GL_FEATURE_SUPPORT_TEXTURE_ANISOTROPIC_FILTER )
		pOutSamplerState.anisotropyLevel = pSamplerConfig.filterConfig.anisotropyLevel;
		if( pOutSamplerState.anisotropyLevel > 0 )
		{
			GLint maxAnisotropy = 0;

			glGetIntegerv( GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy );
			ic3OpenGLCheckLastResult();

			if( pOutSamplerState.anisotropyLevel > maxAnisotropy )
			{
				pOutSamplerState.anisotropyLevel = maxAnisotropy;
			}
		}
	#endif

		if( pSamplerConfig.textureCompareMode == ETextureCompareMode::RefToTexture )
		{
			pOutSamplerState.textureCompareMode = GL_COMPARE_REF_TO_TEXTURE;
			pOutSamplerState.textureCompareFunc = ATL::translateGLCompFunc( pSamplerConfig.textureCompareFunc );
		}
		else
		{
			pOutSamplerState.textureCompareMode = GL_NONE;
			pOutSamplerState.textureCompareFunc = GL_INVALID_VALUE;
		}

		return true;
	}

} // namespace Ic3::Graphics::GCI
