
#include "DX12GPUCmdManager.h"
#include "DX12GPUCmdContext.h"
#include "DX12GPUDevice.h"
#include "DX12coreAPIProxy.h"

namespace Ic3::Graphics::GCI
{

	DX12GPUCmdManager::DX12GPUCmdManager( DX12GPUDevice & pGPUDevice )
	: GPUCmdManager( pGPUDevice )
	, mD3D12Device( pGPUDevice.mD3D12Device )
	{}

	DX12GPUCmdManager::~DX12GPUCmdManager() = default;

	bool DX12GPUCmdManager::Initialize()
	{
		// TODO: Vendor-/GPU Family-specific checks:
		//  if(INTEL) DEFAULT_TRANSFER = DEFAULT_GRAPHICS; (UMA architecture)
		//  if(!AMD) DEFAULT_COMPUTE = DEFAULT_GRAPHICS; (Independent, Async Compute Engine support)

		DeviceCommandQueueProperties queueProperties;

		queueProperties.commandClassFlags = GPU_CMD_COMMAND_CLASS_GRAPHICS_BIT;
		if( !InitializeDeviceQueue( E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_GRAPHICS, queueProperties ) )
		{
			return false;
		}

		queueProperties.commandClassFlags = GPU_CMD_COMMAND_CLASS_COMPUTE_BIT;
		if( !InitializeDeviceQueue( E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_COMPUTE, queueProperties ) )
		{
			return false;
		}

		queueProperties.commandClassFlags = GPU_CMD_COMMAND_CLASS_TRANSFER_BIT;
		if( !InitializeDeviceQueue( E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_TRANSFER, queueProperties ) )
		{
			return false;
		}

		return true;
	}

	CommandContext * DX12GPUCmdManager::AcquireContext( const CommandContextProperties & pContextProperties )
	{
		DX12CommandContext * dx12CmdContext = nullptr;

		auto commandListType = DX12CoreAPIProxy::TranslateD3D12CommandListType( pContextProperties.contextType, pContextProperties.commandClassFlags );

		auto & availableContextList = _availableContextMap[commandListType];
		if( !availableContextList.empty() )
		{
			dx12CmdContext = availableContextList.back();
			availableContextList.pop_back();
		}

		if( !dx12CmdContext )
		{
			dx12CmdContext = CreateCmdContext( commandListType, true );
		}

		if( dx12CmdContext )
		{
			dx12CmdContext->SetActiveState();
		}

		return dx12CmdContext;
	}

	bool DX12GPUCmdManager::ExecuteContext( gpu_cmd_device_queue_id_t pQueueID, CommandContext & pContext )
	{
		auto * deviceQueueData = Ic3::get_map_value_ptr_or_null( _deviceQueueMap, pQueueID );
		if( !deviceQueueData )
		{
			return false;
		}

		auto * dx12CmdContext = pContext.QueryInterface<DX12CommandContext>();
		ID3D12CommandList * commandListArray[] = { dx12CmdContext->mD3D12GraphicsCommandList.Get() };
		deviceQueueData->d3d12CommandQueue->ExecuteCommandLists( 1, commandListArray );

		return true;
	}

	bool DX12GPUCmdManager::InitializeDeviceQueue( gpu_cmd_device_queue_id_t pQueueID, const DeviceCommandQueueProperties & pQueueProperties )
	{
		if( CheckQueueAlias( pQueueID ) )
		{
			return true;
		}

		auto * existingQueue = Ic3::get_map_value_ptr_or_null( _deviceQueueMap, pQueueID );
		if( existingQueue )
		{
			return true;
		}

		auto commandListType = DX12CoreAPIProxy::TranslateD3D12CommandListType( ECommandContextType::Direct, pQueueProperties.commandClassFlags );
		auto d3d12CommandQueue = DX12CoreAPIProxy::CreateD3D12CommandQueue( mD3D12Device, commandListType );
		if( !d3d12CommandQueue )
		{
			return false;
		}

		auto & deviceQueueData = _deviceQueueMap[pQueueID];
		deviceQueueData.d3d12CommandQueue = std::move( d3d12CommandQueue );

		return true;
	}

	bool DX12GPUCmdManager::IsQueueAvailable( gpu_cmd_device_queue_id_t pQueueID ) const
	{
		pQueueID = resolveQueueAlias( pQueueID );
		auto * deviceQueueData = Ic3::get_map_value_ptr_or_null( _deviceQueueMap, pQueueID );
		return deviceQueueData != nullptr;
	}

	ID3D12CommandQueue * DX12GPUCmdManager::GetD3D12DeviceQueue( gpu_cmd_device_queue_id_t pQueueID ) const
	{
		pQueueID = resolveQueueAlias( pQueueID );
		auto * deviceQueueData = Ic3::get_map_value_ptr_or_null( _deviceQueueMap, pQueueID );
		return deviceQueueData ? deviceQueueData->d3d12CommandQueue.Get() : nullptr;
	}

	DX12CommandContext * DX12GPUCmdManager::CreateCmdContext( D3D12_COMMAND_LIST_TYPE pD3D12CommandListType, bool pAutoAcquire )
	{
		auto d3d12CommandListData = DX12CoreAPIProxy::CreateD3D12CommandList( mD3D12Device, pD3D12CommandListType );
		if( !d3d12CommandListData )
		{
			return nullptr;
		}

		auto cmdContext = CreateGfxObject<DX12CommandContext>( *this,
		                                                         std::move( d3d12CommandListData.commandAllocator ),
		                                                         std::move( d3d12CommandListData.graphicsCommandList ) );

		auto * cmdContextPtr = cmdContext.get();
		_cmdContextStorage[cmdContextPtr] = std::move( cmdContext );

		if( pAutoAcquire )
		{
			cmdContextPtr->SetActiveState();
		}
		else
		{
			// Every context supports a specific command category, represented with D3D12_COMMAND_LIST_TYPE.
			// Internal map has lists of all available (unused) contexts which support that specific class.
			auto & availableContextList = _availableContextMap[pD3D12CommandListType];
			availableContextList.push_back( cmdContextPtr );
		}

		return cmdContextPtr;
	}

} // namespace Ic3::Graphics::GCI
