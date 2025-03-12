
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_GLC_API_TRANSLATION_LAYER_H__

#include "GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	// OpenGL API Translation Layer
	namespace ATL
	{

		GLenum GLChooseBufferStorageFlags( GLenum pBindTarget, cppx::bitmask<resource_flags_value_t> pBufferFlags, cppx::bitmask<EGPUMemoryFlags> pMemoryFlags );

		GLenum GLChooseBufferUsagePolicy( GLenum pBindTarget, cppx::bitmask<resource_flags_value_t> pBufferFlags );

		GLenum GLChooseTextureMagFilter( ETextureFilter pMagFilter, ETextureMipMode pMipMode );

		GLenum GLChooseTextureMinFilter( ETextureFilter pMinFilter, ETextureMipMode pMipMode );

		GLsizei GLQueryTextureInternalFormatBPP( GLenum pGLInternalFormat );

		GLenum GLTranslateBaseDataType( EBaseDataType pBaseDataType );

		GLenum GLTranslateBlendFactor( EBlendFactor pBlendFactor );

		GLenum GLTranslateBlendOp( EBlendOp pBlendOp );

		GLenum GLTranslateBufferBindTarget( EGPUBufferTarget pBufferTarget );

		GLenum GLTranslateBufferMapFlags( EGPUMemoryMapMode pMapMode, cppx::bitmask<EGPUMemoryFlags> pMemoryFlags );

		GLenum GLTranslateCompFunc( ECompFunc pCompFunc );

		GLenum GLTranslateCullMode( ECullMode pCullMode );

		GLenum GLTranslateIndexDataFormat( EIndexDataFormat pIndexDataFormat );

		GLenum GLTranslatePrimitiveFillMode( EPrimitiveFillMode pFillMode );

		GLenum GLTranslatePrimitiveTopology( EPrimitiveTopology pTopology );

		GLenum GLTranslateShaderType( EShaderType pShaderType );

		GLenum GLTranslateStencilOp( EStencilOp pStencilOp );

		GLenum GLTranslateTextureAddressMode( ETextureAddressMode pAddressMode );

		GLenum GLTranslateTextureBindTarget( ETextureClass pTextureDimensionClass );

		GLenum GLTranslateTextureInternalFormat( ETextureFormat pTextureFormat );

		GLenum GLTranslateTexturePixelDataLayout( ETextureFormat pTextureFormat );

		GLenum GLTranslateTriangleVerticesOrder( ETriangleVerticesOrder pVerticesOrder );

		const char * GLTranslateDebugOutputExtensionName( GLDebugOutputVersion pDebugOutputVersion );

		const char * GLTranslateDebugEventCategoryStrAMD( GLenum pEventCategory );

		const char * GLTranslateDebugEventSeverityStr( GLenum pEventSeverity );

		const char * GLTranslateDebugEventSourceStr( GLenum pEventSource );

		const char * GLTranslateDebugEventTypeStr( GLenum pEventType );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_API_TRANSLATION_LAYER_H__
