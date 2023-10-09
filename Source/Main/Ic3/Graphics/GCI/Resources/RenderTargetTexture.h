
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_TARGET_TEXTURE_H__
#define __IC3_GRAPHICS_GCI_RENDER_TARGET_TEXTURE_H__

#include "GPUResource.h"
#include "TextureReference.h"
#include "../State/RenderTargetCommon.h"

namespace Ic3::Graphics::GCI
{

	/// @brief
	struct RenderTargetTextureCreateInfo
	{
		TextureReference targetTexture;
		RenderTargetTextureLayout rtTextureLayout;
		Bitmask<ETextureBindFlags> bindFlags;
	};

	/// @brief
	class RenderTargetTexture : public GPUResourceView
	{
		friend class GPUDevice;
		friend class Texture;

	public:
		ERenderTargetTextureType const mRTTextureType;

		Bitmask<ERenderTargetBufferFlags> const mRTBufferMask;

		RenderTargetTextureLayout const mRTTextureLayout;

		/// Internal texture used by this RTT. Can be null, if this is a write-only depth-stencil
		/// RTT (created solely for the purpose of an off-screen depth/stencil testing).
		/// In particular, this is null always when isDepthStencilRenderBuffer() returns true.
		TextureReference const mTargetTexture;

	public:
		RenderTargetTexture(
			GPUDevice & pGPUDevice,
			ERenderTargetTextureType pRTTextureType,
			const RenderTargetTextureLayout & pRTTextureLayout,
			TextureReference pTargetTexture );

		RenderTargetTexture(
			GPUDevice & pGPUDevice,
			ERenderTargetTextureType pRTTextureType,
			const RenderTargetTextureLayout & pRTTextureLayout,
			GpaHandle<GPUDeviceChildObject> pInternalRenderBuffer,
			Bitmask<resource_flags_value_t> pRenderBufferFlags );

		virtual ~RenderTargetTexture();

		///
		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		///
		IC3_ATTR_NO_DISCARD bool isDepthStencilTexture() const noexcept;

		///
		IC3_ATTR_NO_DISCARD bool isDepthStencilRenderBuffer() const noexcept;

		///
		template <typename TRenderBufferType>
		IC3_ATTR_NO_DISCARD TRenderBufferType * getInternalRenderBuffer() const noexcept
		{
			return _internalRenderBuffer ? _internalRenderBuffer->queryInterface<TRenderBufferType>() : nullptr;
		}

	private:
		///
		GpaHandle<GPUDeviceChildObject> _internalRenderBuffer;
	};

	namespace rcutil
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD ERenderTargetTextureType queryRenderTargetTextureType( ETextureFormat pFormat );

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetTextureLayout queryRenderTargetTextureLayout( const TextureLayout & pTextureLayout );

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool validateRenderTargetTextureLayout(
				TextureHandle pTargetTexture,
				const RenderTargetTextureLayout & pRTTextureLayout );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_TARGET_TEXTURE_H__
