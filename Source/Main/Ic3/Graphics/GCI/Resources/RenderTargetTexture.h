
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

		cppx::bitmask<ETextureBindFlags> bindFlags;
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS RenderTargetTexture : public GPUResourceView
	{
		friend class GPUDevice;
		friend class Texture;

	public:
		ERenderTargetTextureType const mRTTextureType;

		cppx::bitmask<ERenderTargetBufferFlags> const mRTBufferMask;

		RenderTargetTextureLayout const mRTTextureLayout;

		/// Internal texture used by this RTT. Can be null, if this is a write-only depth-stencil
		/// RTT (created solely for the purpose of an off-screen depth/stencil testing).
		/// In particular, this is null always when IsDepthStencilRenderBuffer() returns true.
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
			TGfxHandle<GPUDeviceChildObject> pInternalRenderBuffer,
			cppx::bitmask<resource_flags_value_t> pRenderBufferFlags );

		virtual ~RenderTargetTexture();

		///
		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		///
		CPPX_ATTR_NO_DISCARD bool IsDepthStencilTexture() const noexcept;

		///
		CPPX_ATTR_NO_DISCARD bool IsDepthStencilRenderBuffer() const noexcept;

		///
		template <typename TRenderBufferType>
		CPPX_ATTR_NO_DISCARD TRenderBufferType * GetInternalRenderBuffer() const noexcept
		{
			return _internalRenderBuffer ? _internalRenderBuffer->QueryInterface<TRenderBufferType>() : nullptr;
		}

	private:
		///
		TGfxHandle<GPUDeviceChildObject> _internalRenderBuffer;
	};

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD ERenderTargetTextureType QueryRenderTargetTextureType( ETextureFormat pFormat );

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetTextureLayout QueryRenderTargetTextureLayout( const TextureLayout & pTextureLayout );

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool ValidateRenderTargetTextureLayout(
				TextureHandle pTargetTexture,
				const RenderTargetTextureLayout & pRTTextureLayout );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_TARGET_TEXTURE_H__
