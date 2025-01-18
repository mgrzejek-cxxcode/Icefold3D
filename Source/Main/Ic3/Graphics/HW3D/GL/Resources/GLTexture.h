
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_TEXTURE_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_TEXTURE_H__

#include "../Objects/GLTextureObject.h"
#include <Ic3/Graphics/GCI/Resources/Texture.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( GLTexture );

	class GLTexture : public Texture
	{
		friend class GLCommandContext;

	public:
		GLTextureObjectHandle const mGLTextureObject = nullptr;

	public:
		GLTexture( GLGPUDevice & pGPUDevice,
		           const ResourceMemoryInfo & pResourceMemory,
		           const TextureProperties & pTextureProperties,
		           const TextureLayout & pTextureLayout,
		           GLTextureObjectHandle pGLTextureObject );

		virtual ~GLTexture();

		static GLTextureHandle CreateDefault( GLGPUDevice & pGPUDevice, const TextureCreateInfo & pCreateInfo );

		static GLTextureHandle CreateForRenderTarget( GLGPUDevice & pGPUDevice, const RenderTargetTextureCreateInfo & pCreateInfo );

		static RenderTargetTextureHandle CreateRenderTargetTextureView( GLGPUDevice & pGPUDevice, const RenderTargetTextureCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_TEXTURE_H__
