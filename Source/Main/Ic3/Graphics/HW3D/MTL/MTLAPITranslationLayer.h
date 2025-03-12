
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_API_TRANSLATION_LAYER_H__

#include "MTLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	namespace ATL
	{

		// GLenum GLTranslateBaseDataType( EBaseDataType pBaseDataType );
		// GLenum GLTranslateBlendFactor( EBlendFactor pBlendFactor );
		// GLenum GLTranslateBlendOp( EBlendOp pBlendOp );
		// GLenum GLTranslateBufferBindTarget( EGPUBufferTarget pBufferTarget );
		// GLenum GLTranslateBufferMapFlags( EGPUMemoryMapMode pMapMode, cppx::bitmask<EGPUMemoryFlags> pMemoryFlags );
		MTLCompareFunction translateMTLCompFunc( ECompFunc pCompFunc );
		// GLenum GLTranslateCullMode( ECullMode pCullMode );
		// GLenum GLTranslateIndexDataFormat( EIndexDataFormat pIndexDataFormat );
		// GLenum GLTranslatePrimitiveFillMode( EPrimitiveFillMode pFillMode );
		// GLenum GLTranslatePrimitiveTopology( EPrimitiveTopology pTopology );
		// GLenum GLTranslateShaderType( EShaderType pShaderType );
		MTLStencilOperation translateMTLStencilOp( EStencilOp pStencilOp );
		MTLSamplerAddressMode translateMTLTextureAddressMode( ETextureAddressMode pAddressMode );
		MTLSamplerBorderColor translateMTLTextureBorderPredefinedColor( ETextureBorderPredefinedColor pColor );
		MTLSamplerMinMagFilter translateMTLTextureFilter( ETextureFilter pFilter );
		MTLSamplerMipFilter translateMTLTextureMipMode( ETextureMipMode pMipMode );
		// GLenum GLTranslateTextureBindTarget( ETextureClass pTextureDimensionClass );
		// GLenum GLTranslateTextureInternalFormat( ETextureFormat pTextureFormat );
		// GLenum GLTranslateTexturePixelDataLayout( ETextureFormat pTextureFormat );
		// GLenum GLTranslateTriangleVerticesOrder( ETriangleVerticesOrder pVerticesOrder );

		MTLSamplerBorderColor selectMTLSamplerBorderColor(
				ETextureBorderPredefinedColor pPredefinedColor,
				const cxm::rgba_color_r32_norm & pCustomColor );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_API_TRANSLATION_LAYER_H__
