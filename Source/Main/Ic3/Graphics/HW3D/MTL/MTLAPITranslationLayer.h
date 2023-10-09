
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_API_TRANSLATION_LAYER_H__

#include "MTLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	namespace ATL
	{

		// GLenum translateGLBaseDataType( EBaseDataType pBaseDataType );
		// GLenum translateGLBlendFactor( EBlendFactor pBlendFactor );
		// GLenum translateGLBlendOp( EBlendOp pBlendOp );
		// GLenum translateGLBufferBindTarget( EGPUBufferTarget pBufferTarget );
		// GLenum translateGLBufferMapFlags( EGPUMemoryMapMode pMapMode, Bitmask<EGPUMemoryFlags> pMemoryFlags );
		MTLCompareFunction translateMTLCompFunc( ECompFunc pCompFunc );
		// GLenum translateGLCullMode( ECullMode pCullMode );
		// GLenum translateGLIndexDataFormat( EIndexDataFormat pIndexDataFormat );
		// GLenum translateGLPrimitiveFillMode( EPrimitiveFillMode pFillMode );
		// GLenum translateGLPrimitiveTopology( EPrimitiveTopology pTopology );
		// GLenum translateShaderType( EShaderType pShaderType );
		MTLStencilOperation translateMTLStencilOp( EStencilOp pStencilOp );
		MTLSamplerAddressMode translateMTLTextureAddressMode( ETextureAddressMode pAddressMode );
		MTLSamplerBorderColor translateMTLTextureBorderPredefinedColor( ETextureBorderPredefinedColor pColor );
		MTLSamplerMinMagFilter translateMTLTextureFilter( ETextureFilter pFilter );
		MTLSamplerMipFilter translateMTLTextureMipMode( ETextureMipMode pMipMode );
		// GLenum translateGLTextureBindTarget( ETextureClass pTextureDimensionClass );
		// GLenum translateGLTextureInternalFormat( ETextureFormat pTextureFormat );
		// GLenum translateGLTexturePixelDataLayout( ETextureFormat pTextureFormat );
		// GLenum translateGLTriangleVerticesOrder( ETriangleVerticesOrder pVerticesOrder );

		MTLSamplerBorderColor selectMTLSamplerBorderColor(
				ETextureBorderPredefinedColor pPredefinedColor,
				const Math::RGBAColorR32Norm & pCustomColor );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_API_TRANSLATION_LAYER_H__
