
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__
#define __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__

#include "RenderTargetCommon.h"

namespace Ic3::Graphics::GCI
{

	/// @brief
	enum ERenderPassAttachmentActionFlags : uint32
	{
		E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_ACCESS_RESTRICTBit = 0x001,
		E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_CLEARBit      = 0x010,
		E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_DISCARDBit    = 0x020,
		E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_FETCHBit      = 0x040,
		E_RENDER_PASS_ATTACHMENT_ACTION_MASK_LOAD_ALL            = 0x070,
		E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_DISCARDBit   = 0x100,
		E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_KEEPBit      = 0x200,
		E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_RESOLVEBit   = 0x400,
		E_RENDER_PASS_ATTACHMENT_ACTION_MASK_STORE_ALL           = 0x700,
		E_RENDER_PASS_ATTACHMENT_ACTION_MASK_ALL                 = 0x771,
	};

	/// @brief Specifies how the contents of render pass attachments are treated at the beginning of a render pass.
	enum class ERenderPassAttachmentLoadAction : uint32
	{
		Clear = E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_CLEARBit,
		/// Indicates that the pass does not have any dependency on the previous contents of the resource.
		/// The resource may have the previous data or some uninitialized one. No guarantees are given.
		Discard = E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_DISCARDBit,
		Fetch = E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_LOAD_FETCHBit,
		RestrictAccess = E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_ACCESS_RESTRICTBit,
		Undefined = 0
	};

	/// @brief Specifies how the contents of render pass attachments are treated at the end of a render pass.
	enum class ERenderPassAttachmentStoreAction : uint32
	{
		/// Indicates that the contents need not to be preserved after the pass is finished. The contents MAY
		/// (but not necessarily have to) be discarded by the driver. There should be no assumptions about it.
		Discard = E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_DISCARDBit,
		/// The contents of the resource is to be preserved and kept after the pass is done.
		/// This indicates there will be a dependency on the data in the future.
		Keep = E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_KEEPBit,
		KeepResolve = E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_KEEPBit | E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_RESOLVEBit,
		Resolve = E_RENDER_PASS_ATTACHMENT_ACTION_FLAG_STORE_RESOLVEBit,
		Undefined = 0
	};

	struct RenderPassAttachmentConfig
	{
		RenderTargetAttachmentClearConfig clearConfig;
		TBitmask<ERenderTargetBufferFlags> clearMask = E_RENDER_TARGET_BUFFER_MASK_ALL;
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
		TBitmask<ERTAttachmentFlags> attachmentsAccessRestrictMask = 0;

		TBitmask<ERTAttachmentFlags> attachmentsActionClearMask = 0;

		IC3_ATTR_NO_DISCARD native_uint countAttachmentsAccessRestrict() const noexcept
		{
			return popCount( attachmentsAccessRestrictMask & eRTAttachmentMaskAll );
		}

		IC3_ATTR_NO_DISCARD native_uint countAttachmentsActionClear() const noexcept
		{
			return popCount( attachmentsActionClearMask & eRTAttachmentMaskAll );
		}

		IC3_ATTR_NO_DISCARD RenderPassConfiguration getValidated() const noexcept
		{
			RenderPassConfiguration validatedConfiguration = *this;
			validatedConfiguration.resetAttachmentsFlags();
			return validatedConfiguration;
		}

		IC3_GRAPHICS_GCI_API void resetAttachmentsFlags() noexcept;
	};

	namespace SMU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD TBitmask<ERenderPassAttachmentActionFlags> getRenderPassAttachmentActionMask(
				const RenderPassAttachmentConfig & pAttachmentConfig );

		IC3_GRAPHICS_GCI_API_NO_DISCARD TBitmask<ERTAttachmentFlags> getRenderPassAttachmentArbitraryActionMask(
				const RenderPassColorAttachmentConfigArray & pColorAttachments,
				const RenderPassAttachmentConfig pDepthStencilAttachment,
				TBitmask<ERTAttachmentFlags> pActiveAttachmentsMask,
				TBitmask<ERenderPassAttachmentActionFlags> pActionMask );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__
