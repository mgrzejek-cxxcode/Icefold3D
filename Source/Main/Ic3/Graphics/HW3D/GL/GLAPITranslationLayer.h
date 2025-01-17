
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_API_TRANSLATION_LAYER_H__

#include "GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	// API Translation Layer
	namespace ATL
	{

		GLenum ChooseGLBufferStorageFlags( GLenum pBindTarget, cppx::bitmask<resource_flags_value_t> pBufferFlags, cppx::bitmask<EGpuMemoryFlags> pMemoryFlags );

		GLenum ChooseGLBufferUsagePolicy( GLenum pBindTarget, cppx::bitmask<resource_flags_value_t> pBufferFlags );

		GLenum ChooseGLTextureMagFilter( ETextureFilter pMagFilter, ETextureMipMode pMipMode );

		GLenum ChooseGLTextureMinFilter( ETextureFilter pMinFilter, ETextureMipMode pMipMode );

		GLsizei QueryGLTextureInternalFormatBPP( GLenum pGLInternalFormat );

		GLenum TranslateGLBaseDataType( EBaseDataType pBaseDataType );

		GLenum TranslateGLBlendFactor( EBlendFactor pBlendFactor );

		GLenum TranslateGLBlendOp( EBlendOp pBlendOp );

		GLenum TranslateGLBufferBindTarget( EGpuBufferTarget pBufferTarget );

		GLenum TranslateGLBufferMapFlags( EGpuMemoryMapMode pMapMode, cppx::bitmask<EGpuMemoryFlags> pMemoryFlags );

		GLenum TranslateGLCompFunc( ECompFunc pCompFunc );

		GLenum TranslateGLCullMode( ECullMode pCullMode );

		GLenum TranslateGLIndexDataFormat( EIndexDataFormat pIndexDataFormat );

		GLenum TranslateGLPrimitiveFillMode( EPrimitiveFillMode pFillMode );

		GLenum TranslateGLPrimitiveTopology( EPrimitiveTopology pTopology );

		GLenum translateShaderType( EShaderType pShaderType );

		GLenum TranslateGLStencilOp( EStencilOp pStencilOp );

		GLenum TranslateGLTextureAddressMode( ETextureAddressMode pAddressMode );

		GLenum TranslateGLTextureBindTarget( ETextureClass pTextureDimensionClass );

		GLenum TranslateGLTextureInternalFormat( ETextureFormat pTextureFormat );

		GLenum TranslateGLTexturePixelDataLayout( ETextureFormat pTextureFormat );

		GLenum TranslateGLTriangleVerticesOrder( ETriangleVerticesOrder pVerticesOrder );

		const char * TranslateGLDebugOutputExtensionName( GLDebugOutputVersion pDebugOutputVersion );

		const char * TranslateGLDebugEventCategoryStrAMD( GLenum pEventCategory );

		const char * TranslateGLDebugEventSeverityStr( GLenum pEventSeverity );

		const char * TranslateGLDebugEventSourceStr( GLenum pEventSource );

		const char * TranslateGLDebugEventTypeStr( GLenum pEventType );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_API_TRANSLATION_LAYER_H__
