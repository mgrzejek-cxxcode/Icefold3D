
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_API_TRANSLATION_LAYER_H__

#include "MTLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	namespace ATL
	{

		// GLenum TranslateGLBaseDataType( EBaseDataType pBaseDataType );
		// GLenum TranslateGLBlendFactor( EBlendFactor pBlendFactor );
		// GLenum TranslateGLBlendOp( EBlendOp pBlendOp );
		// GLenum TranslateGLBufferBindTarget( EGpuBufferTarget pBufferTarget );
		// GLenum TranslateGLBufferMapFlags( EGpuMemoryMapMode pMapMode, cppx::bitmask<EGpuMemoryFlags> pMemoryFlags );
		MTLCompareFunction translateMTLCompFunc( ECompFunc pCompFunc );
		// GLenum TranslateGLCullMode( ECullMode pCullMode );
		// GLenum TranslateGLIndexDataFormat( EIndexDataFormat pIndexDataFormat );
		// GLenum TranslateGLPrimitiveFillMode( EPrimitiveFillMode pFillMode );
		// GLenum TranslateGLPrimitiveTopology( EPrimitiveTopology pTopology );
		// GLenum translateShaderType( EShaderType pShaderType );
		MTLStencilOperation translateMTLStencilOp( EStencilOp pStencilOp );
		MTLSamplerAddressMode translateMTLTextureAddressMode( ETextureAddressMode pAddressMode );
		MTLSamplerBorderColor translateMTLTextureBorderPredefinedColor( ETextureBorderPredefinedColor pColor );
		MTLSamplerMinMagFilter translateMTLTextureFilter( ETextureFilter pFilter );
		MTLSamplerMipFilter translateMTLTextureMipMode( ETextureMipMode pMipMode );
		// GLenum TranslateGLTextureBindTarget( ETextureClass pTextureDimensionClass );
		// GLenum TranslateGLTextureInternalFormat( ETextureFormat pTextureFormat );
		// GLenum TranslateGLTexturePixelDataLayout( ETextureFormat pTextureFormat );
		// GLenum TranslateGLTriangleVerticesOrder( ETriangleVerticesOrder pVerticesOrder );

		MTLSamplerBorderColor selectMTLSamplerBorderColor(
				ETextureBorderPredefinedColor pPredefinedColor,
				const Math::RGBAColorR32Norm & pCustomColor );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_API_TRANSLATION_LAYER_H__
