
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_GPU_CMD_MANAGER_H__
#define __IC3_GRAPHICS_HW3D_DX12_GPU_CMD_MANAGER_H__

#include "DX12Prerequisites.h"
#include <Ic3/Graphics/GCI/cmds/CommandSystem.h>

namespace Ic3::Graphics::GCI
{

	/// @brief
	class IC3_GX_DX12_CLASS DX12GPUCmdManager final : public GPUCmdManager
	{
	public:
		ComPtr<ID3D12Device> const mD3D12Device = nullptr;

		explicit DX12GPUCmdManager( DX12GPUDevice & pGPUDevice );
		virtual ~DX12GPUCmdManager();

		virtual bool Initialize() override;

		virtual CommandContext * AcquireContext( const CommandContextProperties & pContextProperties ) override;

		virtual bool ExecuteContext( gpu_cmd_device_queue_id_t pQueueID, CommandContext & pContext ) override;

		virtual bool InitializeDeviceQueue( gpu_cmd_device_queue_id_t pQueueID, const DeviceCommandQueueProperties & pQueueProperties ) override;

		virtual bool IsQueueAvailable( gpu_cmd_device_queue_id_t pQueueID ) const override;

		ID3D12CommandQueue * GetD3D12DeviceQueue( gpu_cmd_device_queue_id_t pQueueID ) const;

	private:
		DX12CommandContext * CreateCmdContext( D3D12_COMMAND_LIST_TYPE pD3D12CommandListType, bool pAutoAcquire );

	private:
		struct DeviceQueueData
		{
			ComPtr<ID3D12CommandQueue> d3d12CommandQueue;
		};
		using DX12DeviceQueueMap = std::unordered_map<gpu_cmd_device_queue_id_t, DeviceQueueData>;
		DX12DeviceQueueMap _deviceQueueMap;
		using DX12CmdContextStorage = std::unordered_map<DX12CommandContext *, DX12CommandContextHandle>;
		DX12CmdContextStorage _cmdContextStorage;
		using DX12CmdContextDynamicList = std::list<DX12CommandContext *>;
		using DX12CmdContextInternalMap = std::unordered_map<D3D12_COMMAND_LIST_TYPE, DX12CmdContextDynamicList>;
		DX12CmdContextInternalMap _availableContextMap;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_GPU_CMD_MANAGER_H__
