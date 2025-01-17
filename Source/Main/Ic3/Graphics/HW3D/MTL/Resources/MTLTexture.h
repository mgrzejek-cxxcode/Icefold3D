
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_TEXTURE_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_TEXTURE_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/Resources/Texture.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( MetalTexture );

	class MetalTexture : public Texture
	{
		friend class MetalCommandContext;

	public:
		MetalTextureObjectHandle const mMetalTextureObject = nullptr;

	public:
		MetalTexture( MetalGPUDevice & pGPUDevice,
		           const ResourceMemoryInfo & pResourceMemory,
		           const TextureProperties & pTextureProperties,
		           const TextureLayout & pTextureLayout,
		           MetalTextureObjectHandle pMetalTextureObject );

		virtual ~MetalTexture();

		static MetalTextureHandle Create( MetalGPUDevice & pGPUDevice, const TextureCreateInfo & pCreateInfo );

		static RenderTargetTextureHandle CreateForRenderTarget( MetalGPUDevice & pGPUDevice, const RenderTargetTextureCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_TEXTURE_H__
