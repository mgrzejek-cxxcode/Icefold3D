
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
		///
		Fetch = eRenderPassAttachmentActionFlagLoadFetchBit,
		///
		RestrictAccess = eRenderPassAttachmentActionFlagAccessRestrictBit,
		///
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
		///
		KeepResolve = eRenderPassAttachmentActionFlagStoreKeepBit | eRenderPassAttachmentActionFlagStoreResolveBit,
		///
		Resolve = eRenderPassAttachmentActionFlagStoreResolveBit,
		///
		Undefined = 0
	};

	union RenderPassActionLoadParameters
	{
		struct ClearParams
		{
			RenderTargetAttachmentClearConfig clearConfig;
			cppx::bitmask<ERenderTargetBufferFlags> clearMask = eRenderTargetBufferMaskAll;
		};

		ClearParams opClear{};
	};

	union RenderPassActionStoreParameters
	{
		struct ResolveParams
		{
		};

		ResolveParams opResolve{};
	};

	struct RenderPassAttachmentConfig
	{
		ERenderPassAttachmentLoadAction loadAction = ERenderPassAttachmentLoadAction::Undefined;

		RenderPassActionLoadParameters loadParameters;

		ERenderPassAttachmentStoreAction storeAction = ERenderPassAttachmentStoreAction::Undefined;

		RenderPassActionStoreParameters storeParameters;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return ( loadAction == ERenderPassAttachmentLoadAction::Undefined ) && ( storeAction == ERenderPassAttachmentStoreAction::Undefined );
		}

		CPPX_ATTR_NO_DISCARD bool IsLoadActionDefined() const noexcept
		{
			return cppx::make_bitmask( loadAction ).is_set_any_of( eRenderPassAttachmentActionMaskLoadAll );
		}

		CPPX_ATTR_NO_DISCARD bool IsStoreActionDefined() const noexcept
		{
			return cppx::make_bitmask( storeAction ).is_set_any_of( eRenderPassAttachmentActionMaskStoreAll );
		}

		CPPX_ATTR_NO_DISCARD bool HasLoadActionFlags( ERenderPassAttachmentActionFlags pLoadFlags ) const noexcept
		{
			return cppx::make_bitmask( loadAction ).is_set( pLoadFlags );
		}

		CPPX_ATTR_NO_DISCARD bool HasStoreActionFlags( ERenderPassAttachmentActionFlags pLoadFlags ) const noexcept
		{
			return cppx::make_bitmask( storeAction ).is_set( pLoadFlags );
		}

		void Reset()
		{
			loadAction = ERenderPassAttachmentLoadAction::Undefined;
			storeAction = ERenderPassAttachmentStoreAction::Undefined;
		}
	};

	template <>
	struct TRenderTargetArrayConfiguration<RenderPassAttachmentConfig> : public TRenderTargetArrayConfigurationBase<RenderPassAttachmentConfig>
	{
		using CachedAttachmentsInfoMap = std::unordered_map<ERenderPassAttachmentActionFlags, ERTAttachmentFlags>;

		CachedAttachmentsInfoMap cachedAttachmentsMap;

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetCachedAttachmentsWithFlags(
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetAttachmentsMaskWithLoadFlags(
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetAttachmentsMaskWithStoreFlags(
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept;

		CPPX_ATTR_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetAttachmentsMaskWithLoadAction(
				ERenderPassAttachmentLoadAction pLoadAction ) const noexcept
		{
			return GetAttachmentsMaskWithLoadFlags( static_cast<ERenderPassAttachmentActionFlags>( pLoadAction ) );
		}

		CPPX_ATTR_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetAttachmentsMaskWithStoreAction(
				ERenderPassAttachmentStoreAction pStoreAction ) const noexcept
		{
			return GetAttachmentsMaskWithStoreFlags( static_cast<ERenderPassAttachmentActionFlags>( pStoreAction ) );
		}

		void SaveCachedAttachmentsInfo( cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags );
	};

	/**
	 *
	 */
	using RenderPassConfiguration = TRenderTargetArrayConfiguration<RenderPassAttachmentConfig>;

	struct RenderPassDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		RenderPassConfiguration passConfiguration;

		CPPX_ATTR_NO_DISCARD pipeline_config_hash_t GetConfigHash() const noexcept
		{
			return cppx::hash_compute<pipeline_config_hash_t::hash_algo>(
					passConfiguration.attachments,
					passConfiguration.activeAttachmentsMask,
					passConfiguration.activeAttachmentsNum );
		}
	};

	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD bool RTOValidateRenderPassConfiguration(
				const RenderPassConfiguration & pRenderPassConfiguration );

	}

} // namespace Ic3::Graphics::GCI

#include "RenderPassCommon.inl"

#endif // __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__
