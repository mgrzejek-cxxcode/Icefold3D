
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_API_TRANSLATION_LAYER_H__

#include "GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	// API Translation Layer
	namespace ATL
	{

		GLenum chooseGLBufferStorageFlags( GLenum pBindTarget, Bitmask<resource_flags_value_t> pBufferFlags, Bitmask<EGPUMemoryFlags> pMemoryFlags );

		GLenum chooseGLBufferUsagePolicy( GLenum pBindTarget, Bitmask<resource_flags_value_t> pBufferFlags );

		GLenum chooseGLTextureMagFilter( ETextureFilter pMagFilter, ETextureMipMode pMipMode );

		GLenum chooseGLTextureMinFilter( ETextureFilter pMinFilter, ETextureMipMode pMipMode );

		GLsizei queryGLTextureInternalFormatBPP( GLenum pGLInternalFormat );

		GLenum translateGLBaseDataType( EBaseDataType pBaseDataType );

		GLenum translateGLBlendFactor( EBlendFactor pBlendFactor );

		GLenum translateGLBlendOp( EBlendOp pBlendOp );

		GLenum translateGLBufferBindTarget( EGPUBufferTarget pBufferTarget );

		GLenum translateGLBufferMapFlags( EGPUMemoryMapMode pMapMode, Bitmask<EGPUMemoryFlags> pMemoryFlags );

		GLenum translateGLCompFunc( ECompFunc pCompFunc );

		GLenum translateGLCullMode( ECullMode pCullMode );

		GLenum translateGLIndexDataFormat( EIndexDataFormat pIndexDataFormat );

		GLenum translateGLPrimitiveFillMode( EPrimitiveFillMode pFillMode );

		GLenum translateGLPrimitiveTopology( EPrimitiveTopology pTopology );

		GLenum translateShaderType( EShaderType pShaderType );

		GLenum translateGLStencilOp( EStencilOp pStencilOp );

		GLenum translateGLTextureAddressMode( ETextureAddressMode pAddressMode );

		GLenum translateGLTextureBindTarget( ETextureClass pTextureDimensionClass );

		GLenum translateGLTextureInternalFormat( ETextureFormat pTextureFormat );

		GLenum translateGLTexturePixelDataLayout( ETextureFormat pTextureFormat );

		GLenum translateGLTriangleVerticesOrder( ETriangleVerticesOrder pVerticesOrder );

		const char * translateGLDebugOutputExtensionName( GLDebugOutputVersion pDebugOutputVersion );

		const char * translateGLDebugEventCategoryStrAMD( GLenum pEventCategory );

		const char * translateGLDebugEventSeverityStr( GLenum pEventSeverity );

		const char * translateGLDebugEventSourceStr( GLenum pEventSource );

		const char * translateGLDebugEventTypeStr( GLenum pEventType );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_API_TRANSLATION_LAYER_H__
