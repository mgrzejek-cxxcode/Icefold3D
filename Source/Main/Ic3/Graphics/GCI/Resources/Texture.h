
#pragma once

#ifndef __IC3_GRAPHICS_GCI_TEXTURE_H__
#define __IC3_GRAPHICS_GCI_TEXTURE_H__

#include "GPUResource.h"
#include "TextureReference.h"

namespace Ic3::Graphics::GCI
{

	struct TextureProperties : public GPUResourceProperties
	{
	};

	class Texture : public GPUResource
	{
		friend class CommandList;
		friend class TextureReference;

	public:
        TextureProperties const mTextureProperties;
		TextureLayout const mTextureLayout;

		Texture(
			GPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const TextureProperties & pTextureProperties,
			const TextureLayout & pTextureLayout );

		virtual ~Texture();

		IC3_ATTR_NO_DISCARD virtual const GPUResourceProperties & getProperties() const override final;

		IC3_ATTR_NO_DISCARD TextureSubResource getAllSubResourcesRef() const;

		IC3_ATTR_NO_DISCARD TextureSubResource getDefaultSubResourceRef() const;

		static RenderTargetTextureHandle createDefaultRenderTargetTextureView(
				GPUDevice & pGPUDevice,
				const RenderTargetTextureCreateInfo & pCreateInfo );
	};

	namespace RCU
	{

		IC3_ATTR_NO_DISCARD TextureDimensions getValidTextureDimensions( ETextureClass pTexClass, const TextureDimensions & pDimensions );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_TEXTURE_H__
