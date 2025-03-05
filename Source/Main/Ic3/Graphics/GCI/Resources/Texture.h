
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

	class IC3_GRAPHICS_GCI_CLASS Texture : public GPUResource
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

		CPPX_ATTR_NO_DISCARD virtual const GPUResourceProperties & GetProperties() const override final;

		CPPX_ATTR_NO_DISCARD TextureSubResource GetAllSubResourcesRef() const;

		CPPX_ATTR_NO_DISCARD TextureSubResource GetDefaultSubResourceRef() const;

		static RenderTargetTextureHandle CreateDefaultRenderTargetTextureView(
				GPUDevice & pGPUDevice,
				const RenderTargetTextureCreateInfo & pCreateInfo );
	};

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD TextureDimensions GetValidTextureDimensions( ETextureClass pTexClass, const TextureDimensions & pDimensions );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_TEXTURE_H__
