
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__
#define __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__

#include "RenderTargetCommon.h"

namespace Ic3::Graphics::GCI
{

	/// @brief
	enum ERenderPassAttachmentActionFlags : uint32
	{
		eRenderPassAttachmentActionFlagAccessRestrictBit = 0x001,
		eRenderPassAttachmentActionFlagLoadClearBit      = 0x010,
		eRenderPassAttachmentActionFlagLoadDiscardBit    = 0x020,
		eRenderPassAttachmentActionFlagLoadFetchBit      = 0x040,
		eRenderPassAttachmentActionMaskLoadAll           = 0x070,
		eRenderPassAttachmentActionFlagStoreDiscardBit   = 0x100,
		eRenderPassAttachmentActionFlagStoreKeepBit      = 0x200,
		eRenderPassAttachmentActionFlagStoreResolveBit   = 0x400,
		eRenderPassAttachmentActionMaskStoreAll          = 0x700,
		eRenderPassAttachmentActionMaskAll               = 0x771,
	};

	/// @brief Specifies how the contents of render pass attachments are treated at the beginning of a render pass.
	enum class ERenderPassAttachmentLoadAction : uint32
	{
		Clear = eRenderPassAttachmentActionFlagLoadClearBit,
		/// Indicates that the pass does not have any dependency on the previous contents of the resource.
		/// The resource may have the previous data or some uninitialized one. No guarantees are given.
		Discard = eRenderPassAttachmentActionFlagLoadDiscardBit,
		Fetch = eRenderPassAttachmentActionFlagLoadFetchBit,
		RestrictAccess = eRenderPassAttachmentActionFlagAccessRestrictBit,
		Undefined = 0
	};

	/// @brief Specifies how the contents of render pass attachments are treated at the end of a render pass.
	enum class ERenderPassAttachmentStoreAction : uint32
	{
		/// Indicates that the contents need not to be preserved after the pass is finished. The contents MAY
		/// (but not necessarily have to) be discarded by the driver. There should be no assumptions about it.
		Discard = eRenderPassAttachmentActionFlagStoreDiscardBit,
		/// The contents of the resource is to be preserved and kept after the pass is done.
		/// This indicates there will be a dependency on the data in the future.
		Keep = eRenderPassAttachmentActionFlagStoreKeepBit,
		KeepResolve = eRenderPassAttachmentActionFlagStoreKeepBit | eRenderPassAttachmentActionFlagStoreResolveBit,
		Resolve = eRenderPassAttachmentActionFlagStoreResolveBit,
		Undefined = 0
	};

	struct RenderPassAttachmentConfig
	{
		RenderTargetAttachmentClearConfig clearConfig;
		cppx::bitmask<ERenderTargetBufferFlags> clearMask = E_RENDER_TARGET_BUFFER_MASK_ALL;
		ERenderPassAttachmentLoadAction renderPassLoadAction = ERenderPassAttachmentLoadAction::Undefined;
		ERenderPassAttachmentStoreAction renderPassStoreAction = ERenderPassAttachmentStoreAction::Undefined;

		void reset()
		{
			renderPassLoadAction = ERenderPassAttachmentLoadAction::Undefined;
			renderPassStoreAction = ERenderPassAttachmentStoreAction::Undefined;
		}

		bool empty() const noexcept
		{
			return renderPassLoadAction == ERenderPassAttachmentLoadAction::Undefined ||
			       renderPassStoreAction == ERenderPassAttachmentStoreAction::Undefined;
		}

		explicit operator bool() const noexcept
		{
			return !empty();
		}
	};

	using RenderPassColorAttachmentConfigArray = RenderTargetColorAttachmentPropertyArray<RenderPassAttachmentConfig>;

	struct RenderPassConfiguration : public RenderTargetAttachmentConfigurationSet<RenderPassAttachmentConfig>
	{
		cppx::bitmask<ERTAttachmentFlags> attachmentsAccessRestrictMask = 0;

		cppx::bitmask<ERTAttachmentFlags> attachmentsActionClearMask = 0;

		CPPX_ATTR_NO_DISCARD native_uint CountAttachmentsAccessRestrict() const noexcept
		{
			return pop_count( attachmentsAccessRestrictMask & eRTAttachmentMaskAll );
		}

		CPPX_ATTR_NO_DISCARD native_uint CountAttachmentsActionClear() const noexcept
		{
			return pop_count( attachmentsActionClearMask & eRTAttachmentMaskAll );
		}

		CPPX_ATTR_NO_DISCARD RenderPassConfiguration GetValidated() const noexcept
		{
			RenderPassConfiguration validatedConfiguration = *this;
			validatedConfiguration.ResetAttachmentsFlags();
			return validatedConfiguration;
		}

		IC3_GRAPHICS_GCI_API void ResetAttachmentsFlags() noexcept;
	};

	namespace SMU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERenderPassAttachmentActionFlags> GetRenderPassAttachmentActionMask(
				const RenderPassAttachmentConfig & pAttachmentConfig );

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetRenderPassAttachmentArbitraryActionMask(
				const RenderPassColorAttachmentConfigArray & pColorAttachments,
				const RenderPassAttachmentConfig pDepthStencilAttachment,
				cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask,
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionMask );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__
