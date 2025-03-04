
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_COMMAND_DEFS_H__
#define __IC3_GRAPHICS_GCI_COMMON_COMMAND_DEFS_H__

#include "State/CommonGPUStateDefs.h"
#include "State/RenderTargetCommon.h"
#include <atomic>

namespace Ic3::Graphics::GCI
{

	class CommandContext;
	class CommandContextDirect;
	class CommandContextDirectTransfer;
	class CommandContextDirectCompute;
	class CommandContextDirectGraphics;
	class CommandContextDeferred;
	class CommandContextDeferredGraphics;
	class CommandList;
	class CommandSystem;

	class ComputePipelineStateObject;
	class GraphicsPipelineStateObject;

	using gpu_cmd_device_queue_id_t = native_uint;

    enum : gpu_cmd_device_queue_id_t
    {
        eDeviceCommandQueueIdDefaultGraphics = 0u,
        eDeviceCommandQueueIdDefaultCompute = 1u,
        eDeviceCommandQueueIdDefaultTransfer = 2u,
        eDeviceCommandQueueIdPresent = 3u,
        eDeviceCommandQueueIdUnknown = cppx::meta::limits<gpu_cmd_device_queue_id_t>::max_value,
    };

	enum ECommandListActionFlags : uint32
	{
		eCommandListActionFlagRenderPassApplyPipelineStateBit = 0x01,

		eCommandListActionFlagRenderPassPreserveDynamicStateBit = 0x02,

		eCommandListActionFlagsDefault =
			eCommandListActionFlagRenderPassApplyPipelineStateBit,
	};

	enum ECommandObjectPropertyFlags : uint32
	{
		eCommandObjectPropertyFlagCommandClassCommonBit = 0x01,
		eCommandObjectPropertyFlagCommandClassTransferBit = 0x02,
		eCommandObjectPropertyFlagCommandClassComputeBit = 0x04,
		eCommandObjectPropertyFlagCommandClassGraphicsBit = 0x08,
		eCommandObjectPropertyMaskCommandClassAll = 0x0F,

		eCommandObjectPropertyFlagExecutionModeDirectBit = 0x10,
			eCommandObjectPropertyFlagExecutionModeDeferredBit = 0x20,
			eCommandObjectPropertyMaskExecutionModeAll = 0x30,

		eCommandObjectPropertyMaskContextFamilyDirect =
			eCommandObjectPropertyFlagCommandClassCommonBit |
			eCommandObjectPropertyFlagExecutionModeDirectBit,

		eCommandObjectPropertyMaskContextFamilyDeferred =
			eCommandObjectPropertyFlagCommandClassCommonBit |
			eCommandObjectPropertyFlagExecutionModeDeferredBit,

		eCommandObjectPropertyMaskContextFamilyDirectTransfer =
			eCommandObjectPropertyMaskContextFamilyDirect |
			eCommandObjectPropertyFlagCommandClassTransferBit,

		eCommandObjectPropertyMaskContextFamilyDirectCompute =
			eCommandObjectPropertyMaskContextFamilyDirectTransfer |
			eCommandObjectPropertyFlagCommandClassComputeBit,

		eCommandObjectPropertyMaskContextFamilyDirectGraphics =
			eCommandObjectPropertyMaskContextFamilyDirectCompute |
			eCommandObjectPropertyFlagCommandClassGraphicsBit,

		eCommandObjectPropertyMaskContextFamilyDeferredGraphics =
			eCommandObjectPropertyMaskContextFamilyDeferred |
			eCommandObjectPropertyFlagCommandClassGraphicsBit,

		eCommandObjectPropertyMaskAll =
			eCommandObjectPropertyMaskCommandClassAll |
			eCommandObjectPropertyMaskExecutionModeAll,
	};

	enum class ECommandExecutionMode : uint32
	{
		Direct = eCommandObjectPropertyFlagExecutionModeDirectBit,
		Deferred = eCommandObjectPropertyFlagExecutionModeDeferredBit
	};

	enum class ECommandQueueClass : uint32
	{
		Transfer = eCommandObjectPropertyFlagCommandClassCommonBit | eCommandObjectPropertyFlagCommandClassTransferBit,
		Compute = static_cast<uint32>( Transfer ) | eCommandObjectPropertyFlagCommandClassComputeBit,
		Graphics = static_cast<uint32>( Compute ) | eCommandObjectPropertyFlagCommandClassGraphicsBit,
		Default = 0,
	};

	enum class ECommandObjectType : uint32
	{
		DirectTransfer = static_cast<uint32>( ECommandQueueClass::Transfer ) | static_cast<uint32>( ECommandExecutionMode::Direct ),
		DirectCompute = static_cast<uint32>( ECommandQueueClass::Compute ) | static_cast<uint32>( ECommandExecutionMode::Direct ),
		DirectGraphics = static_cast<uint32>( ECommandQueueClass::Graphics ) | static_cast<uint32>( ECommandExecutionMode::Direct ),
		DeferredGraphics = static_cast<uint32>( ECommandQueueClass::Graphics ) | static_cast<uint32>( ECommandExecutionMode::Deferred ),
		Undefined = 0
	};

	using ECommandContextType = ECommandObjectType;
	using ECommandListType = ECommandObjectType;

	namespace CXU
	{

		inline constexpr ECommandExecutionMode GetCommandObjectExecutionMode( ECommandObjectType pType ) noexcept
		{
			return static_cast<ECommandExecutionMode>( static_cast<uint32>( pType ) & eCommandObjectPropertyMaskExecutionModeAll );
		}

		inline constexpr cppx::bitmask<ECommandObjectPropertyFlags> GetCommandObjectPropertyFlags( ECommandObjectType pType ) noexcept
		{
			return cppx::make_bitmask<uint32>( static_cast<uint32>( pType ) & eCommandObjectPropertyMaskAll );
		}

	}

	enum class ECommandSubmitStateOp : enum_default_value_t
	{
		// Context is a temporary, "fire-and-forget" context. After its submission,
		// its internal state is invalidated, and it may no longer be used for command
		// submission. User must acquire a new context to do so.
		Discard,

		// Context is used multiple times or stored in a permanent manner. After the
		// submission, its internal state is updated with a new data and becomes ready
		// for further command recording/submission.
		// This flag is useful if it is required for a context object to stay alive
		// (and useful) between multiple submissions (e.g. captured within a lambda).
		Reset
	};

	enum class ECommandSubmitSyncMode : enum_default_value_t
	{
		// Enables the sync of CmdContext execution status. It causes two things to happen:
		// 1) Additional command is issued when submitting the command context, which will
		//    signal a sync object (fence or semaphore) used for the synchronization process.
		// 2) Returns a non-empty CommandSync object (its operator bool() evaluates to true),
		//    which can be used to wait for completion of all commands issued before.
		Default,

		// No sync option is provided. Commands are executed in a driver-specific manner.
		// Only commands executed on the same device queue are guaranteed to be executed
		// in the order of recording/submission.
		None
	};

	struct CommandContextSubmitInfo
	{
		ECommandQueueClass queuePreference = ECommandQueueClass::Default;
		ECommandSubmitStateOp stateOp = ECommandSubmitStateOp::Discard;
		ECommandSubmitSyncMode syncMode = ECommandSubmitSyncMode::None;
	};

	inline constexpr CommandContextSubmitInfo cxCommandContextSubmitDefault {};

	struct CommandSync
	{
	public:
		using SyncDataReleaseFuncPtr = void ( * )( void * );

		void * syncData = nullptr;

		SyncDataReleaseFuncPtr syncDataReleaseFunc = nullptr;

	public:
		CommandSync() = default;

		CommandSync( CommandSync && pSrcObject )
		: syncData( pSrcObject.syncData )
		, syncDataReleaseFunc( pSrcObject.syncDataReleaseFunc )
		{
			pSrcObject.syncData = nullptr;
			pSrcObject.syncDataReleaseFunc = nullptr;
		}

		~CommandSync()
		{
			if( syncData && syncDataReleaseFunc )
			{
				syncDataReleaseFunc( syncData );
				syncData = nullptr;
			}
		}

		CommandSync & operator=( CommandSync && pRhs )
		{
			CommandSync( std::move( pRhs ) ).Swap( *this );
			return *this;
		}

		explicit operator bool() const
		{
			return syncData != nullptr;
		}

		void Swap( CommandSync & pOther )
		{
			std::swap( syncData, pOther.syncData );
			std::swap( syncDataReleaseFunc, pOther.syncDataReleaseFunc );
		}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_COMMAND_DEFS_H__
