
#pragma once

#ifndef __IC3_GRAPHICS_GCI_TEXTURE_H__
#define __IC3_GRAPHICS_GCI_TEXTURE_H__

#include "GpuResource.h"
#include "TextureReference.h"

namespace Ic3::Graphics::GCI
{

	struct TextureProperties : public GpuResourceProperties
	{
	};

	class Texture : public GpuResource
	{
		friend class CommandList;
		friend class TextureReference;

	public:
        TextureProperties const mTextureProperties;
		TextureLayout const mTextureLayout;

		Texture(
			GpuDevice & pGpuDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const TextureProperties & pTextureProperties,
			const TextureLayout & pTextureLayout );

		virtual ~Texture();

		CPPX_ATTR_NO_DISCARD virtual const GpuResourceProperties & GetProperties() const override final;

		CPPX_ATTR_NO_DISCARD TextureSubResource GetAllSubResourcesRef() const;

		CPPX_ATTR_NO_DISCARD TextureSubResource GetDefaultSubResourceRef() const;

		static RenderTargetTextureHandle CreateDefaultRenderTargetTextureView(
				GpuDevice & pGpuDevice,
				const RenderTargetTextureCreateInfo & pCreateInfo );
	};

	namespace RCU
	{

		CPPX_ATTR_NO_DISCARD TextureDimensions GetValidTextureDimensions( ETextureClass pTexClass, const TextureDimensions & pDimensions );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_TEXTURE_H__
