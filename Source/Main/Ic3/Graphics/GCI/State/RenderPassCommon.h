
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__
#define __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__

#include "RenderTargetCommon.h"

namespace Ic3::Graphics::GCI
{
	/**
	 * Defines bit flags for individual actions performed on RTA attachments during render passes (both load and store).
	 * Each flag corresponds to a single action. ERenderPassAttachmentLoadAction/ERenderPassAttachmentStoreAction combine
	 * them into supported action types.
	 */
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

	/**
	 * Defines supported load actions for RTA attachments, i.e. what happens with attachment when a render pass begins.
	 */
	enum class ERenderPassAttachmentLoadAction : uint32
	{
		/// The contents of the attachment are to be cleared with a user-defined value, overwriting the existing ones.
		/// Details for the clear operation (like values to be used) are specified via RenderPassActionLoadParameters::ClearParams.
		/// @note
		/// Clear parameters are part of the dynamic pipeline state - they can be overwritten at runtime via a dedicated
		/// command, without the necessity of re-creating the RenderPassDescriptor.
		Clear = eRenderPassAttachmentActionFlagLoadClearBit,
		/// Indicates that the pass does not have any dependency on the previous contents of the resource.
		/// The resource may have the previous data or some uninitialized one. No guarantees are given.
		Discard = eRenderPassAttachmentActionFlagLoadDiscardBit,
		/// The contents of the attachment produced by a previous pass are to be loaded and made available for this pass.
		/// This usually means that the attachment will be used as a source.
		Fetch = eRenderPassAttachmentActionFlagLoadFetchBit,
		/// The attachment is not used by the pass and can be made unavailable, potentially saving bandwidth and reducing
		/// memory usage. Reading from or writing to an attachment with this action is prohibited.
		RestrictAccess = eRenderPassAttachmentActionFlagAccessRestrictBit,
		/// Load action is not specified. This implies a "do not care" scenario. No assumptions should be made about
		/// the contents of an attachment when this action is specified.
		Undefined = 0
	};

	/**
	 * Defines supported store actions for RTA attachments, i.e. what happens with attachment when a render pass ends.
	 */
	enum class ERenderPassAttachmentStoreAction : uint32
	{
		/// Indicates that the contents need not be preserved after the pass is finished. The contents MAY
		/// (but not necessarily have to) be discarded by the driver. There should be no assumptions about the
		/// contents of an attachment that specifies this action after a render pass ends.
		Discard = eRenderPassAttachmentActionFlagStoreDiscardBit,
		/// The contents of the resource is to be preserved and kept after the pass is done.
		/// This indicates there will be a dependency on the data in the future.
		Keep = eRenderPassAttachmentActionFlagStoreKeepBit,
		/// The contents of the resource is to be preserved and kept after the pass is done just like in case of Keep action.
		/// In addition, the contents are resolved as described for the Resolve action. This is basically a combination of the two.
		KeepResolve = eRenderPassAttachmentActionFlagStoreKeepBit | eRenderPassAttachmentActionFlagStoreResolveBit,
		/// The contents of the attachment are potentially multi-sampled and are to be resolved at the end of a pass.
		/// This requires that the RenderTargetBinding specifies an additional resolve buffer/texture that serves as
		/// a target for the resolve operation.
		Resolve = eRenderPassAttachmentActionFlagStoreResolveBit,
		/// Store action is not specified. This implies a "do not care" scenario. No assumptions should be made about
		/// the contents of an attachment when this action is specified.
		Undefined = 0
	};

	/**
	 * Definition of parameters used to control the execution of load actions for RTA attachments.
	 */
	union RenderPassActionLoadParameters
	{
		/**
		 * Definition of parameters for the clear op, used to clear the attachment when the loadAction contains
		 * eRenderPassAttachmentActionFlagLoadClearBit.
		 */
		struct ClearParams
		{
			/// Clear configuration with values for color/depth/stencil buffers to clear with.
			RenderTargetAttachmentClearConfig clearConfig;
			/// Clear mask which controls buffers to be cleared:
			/// - for color attachments the only valid bit is eRenderTargetBufferFlagColorBit,
			/// - for depth/stencil attachment valid bits are eRenderTargetBufferFlagDepthBit and eRenderTargetBufferFlagStencilBit.
			/// Bits that do not apply to a given attachment type will be silently ignored.
			cppx::bitmask<ERenderTargetBufferFlags> clearMask = eRenderTargetBufferMaskAll;
		};

		/// Parameters for Clear action.
		ClearParams opClear{};
	};

	/**
	 * Definition of parameters used to control the execution of store actions for RTA attachments.
	 */
	union RenderPassActionStoreParameters
	{
		struct ResolveParams
		{
		};

		ResolveParams opResolve{};
	};

	/**
	 * Stores a render pass configuration for a single RTA attachment - defines its usage and determines what actions
	 * the driver performs on that attachment when a render pass starts/ends.
	 */
	struct RenderPassAttachmentConfig
	{
		/// Load action for the attachment, executed when a render pass starts.
		ERenderPassAttachmentLoadAction loadAction = ERenderPassAttachmentLoadAction::Undefined;

		/// Combined parameters for the load action, used to perform that action.
		RenderPassActionLoadParameters loadParameters;

		/// Store action for the attachment, executed when a render pass ends.
		ERenderPassAttachmentStoreAction storeAction = ERenderPassAttachmentStoreAction::Undefined;

		/// Combined parameters for the store action, used to perform that action.
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

	/**
	 * Specialization of TRenderTargetArrayConfiguration for RenderPassAttachmentConfig, aliased as RenderPassConfiguration.
	 * Describes the configuration for active attachments in the RTA (Render Target Array) in the context of a render pass.
	 * Specifically, it defines what load (beginning of an RP) and store (end of an RP) actions are performed on the attachments.
	 */
	template <>
	struct TRenderTargetArrayConfiguration<RenderPassAttachmentConfig> : public TRenderTargetArrayConfigurationBase<RenderPassAttachmentConfig>
	{
		using CachedAttachmentsInfoMap = std::unordered_map<ERenderPassAttachmentActionFlags, ERTAttachmentFlags>;

		/// Additional map for caching information about which attachments have certain load/store actions
		/// defined as their render pass usage. This is filled and used by certain GPU drivers that know
		/// up-front which usages they will require most often. It eliminates the necessity of calling
		/// GetAttachmentsMaskWith{Load|Store} functions which may not be very effective (they iterate
		/// the whole attachment array separately for load and store config).
		CachedAttachmentsInfoMap cachedAttachmentsMap;

		/**
		 * Returns a bitmask with attachments bits that have the specified action flags set as their load and/or store
		 * action for a render pass. It uses the cached info, which can be bypassed by specifying pBypassCache=true.
		 * If no cached data is found for the specified flags combination, it calls GetAttachmentsMaskWith{Load|Store}Flags
		 * to get the data directly from the internal configuration.
		 * @param pActionFlags A bitmask with action flags requested for attachments to have.
		 * @param pBypassCache If true, the cache map (cachedAttachmentsMap) will not be searched.
		 * @note
		 * Bypassing the cache (pBypassCache=true) effectively means doing:
		 * - L = GetAttachmentsMaskWithLoadFlags( pActionFlags );
		 * - S = GetAttachmentsMaskWithStoreFlags( pActionFlags );
		 * - The result is a mask with attachments that appear in both L and S.
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetAttachmentsMaskWithFlags(
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags,
				bool pBypassCache = false ) const noexcept;

		/**
		 * Returns a bitmask with attachments bits that have the specified action flags set as their load action for an RP.
		 * @note
		 * This function iterates over the array of all attachments (GCM::kRTOMaxCombinedAttachmentsNum).
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetAttachmentsMaskWithLoadFlags(
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept;

		/**
		 * Returns a bitmask with attachments bits that have the specified action flags set as their store action for an RP.
		 * @note
		 * This function iterates over the array of all attachments (GCM::kRTOMaxCombinedAttachmentsNum).
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetAttachmentsMaskWithStoreFlags(
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept;

		/**
		 * Returns a bitmask with attachments bits that have the specified load action set for an RP.
		 * @note
		 * This function iterates over the array of all attachments (GCM::kRTOMaxCombinedAttachmentsNum).
		 */
		CPPX_ATTR_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetAttachmentsMaskWithLoadAction(
				ERenderPassAttachmentLoadAction pLoadAction ) const noexcept
		{
			return GetAttachmentsMaskWithLoadFlags( static_cast<ERenderPassAttachmentActionFlags>( pLoadAction ) );
		}

		/**
		 * Returns a bitmask with attachments bits that have the specified store action set for an RP.
		 * @note
		 * This function iterates over the array of all attachments (GCM::kRTOMaxCombinedAttachmentsNum).
		 */
		CPPX_ATTR_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetAttachmentsMaskWithStoreAction(
				ERenderPassAttachmentStoreAction pStoreAction ) const noexcept
		{
			return GetAttachmentsMaskWithStoreFlags( static_cast<ERenderPassAttachmentActionFlags>( pStoreAction ) );
		}

		/**
		 * Retrieves a bitmask with attachments bits that have the specified action flags set as their load and/or store
		 * action for a render pass and if that bitmask is not zero (i.e. there is at least one attachment that matches
		 * the request), it is saved in the internal cache for later retrieval via GetAttachmentsMaskWithFlags().
		 * This function is used by certain GPU drivers that support the known features of the render pass system,
		 * so the attachments can be retrieved faster at runtime.
		 */
		void CacheAttachmentsWithActionFlags( cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags );

		/**
		 * Resets the internal bitmasks cache. Using GetAttachmentsMaskWithFlags() after this function is called,
		 * causes it to retrieve the attachments via the non-cached GetAttachmentsMaskWith{Load|Store}Flags functions.
		 */
		void ResetCachedAttachments();
	};

	/**
	 * Defined render pass configuration for all RTA attachments. Used to create RenderPassDescriptor and internally.
	 */
	using RenderPassConfiguration = TRenderTargetArrayConfiguration<RenderPassAttachmentConfig>;

	/**
	 * CreateInfo struct for RenderPassDescriptor.
	 */
	struct RenderPassDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		/// Render pass configuration.
		RenderPassConfiguration passConfiguration;

		/**
		 * Returns the hash value computed from the configuration stored in this CreateInfo.
		 */
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

#endif // __IC3_GRAPHICS_GCI_RENDER_PASS_COMMON_H__
