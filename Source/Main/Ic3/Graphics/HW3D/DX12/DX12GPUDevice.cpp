
#include "DX12GPUDevice.h"
#include "DX12GPUDriver.h"
#include "DX12GPUCmdContext.h"
#include "DX12GPUCmdManager.h"
#include "DX12coreAPIProxy.h"

namespace Ic3::Graphics::GCI
{

	DX12GPUDevice::DX12GPUDevice( DX12GPUDriver & pDriver,
	                              ComPtr<IDXGIFactory3> pDXGIFactory3,
	                              ComPtr<ID3D12Device> pD3D12Device,
	                              ComPtr<ID3D12Debug> pD3D12DebugInterface )
	: DXGPUDevice( pDriver, pDXGIFactory3 )
	, mDXGIFactory3( std::move( pDXGIFactory3 ) )
	, mD3D12Device( std::move( pD3D12Device ) )
	, mD3D12DebugInterface( std::move( pD3D12DebugInterface ) )
	{ }

	DX12GPUDevice::~DX12GPUDevice() = default;

	DX12GPUDeviceHandle DX12GPUDevice::Create( DX12GPUDriver & pDX12Driver, const DX12GPUDeviceCreateInfo & pCreateInfo )
	{
		auto d3d12DebugInterface = DX12CoreAPIProxy::InitializeD3D12DebugInterface( pDX12Driver.GetConfigFlags() );

		auto d3d12Device = DX12CoreAPIProxy::CreateD3D12Device();
		Ic3DebugAssert( d3d12Device );

		auto dxgiFactory3 = DX12CoreAPIProxy::createDXGIFactoryForD3D12Device( d3d12Device, d3d12DebugInterface );
		Ic3DebugAssert( dxgiFactory3 );

		auto dx12GPUDevice = CreateGfxObject<DX12GPUDevice>( pDX12Driver, dxgiFactory3, d3d12Device, d3d12DebugInterface );
		Ic3DebugAssert( dx12GPUDevice );

		dx12GPUDevice->InitializeCommandSystem();

		if( pCreateInfo.flags.is_set( E_GPU_DEVICE_CREATE_FLAG_INIT_DEFAULT_PRESENT_QUEUE_BIT ) )
		{
			dx12GPUDevice->InitializeDefaultPresentDeviceQueue();
		}

		return dx12GPUDevice;
	}

	ID3D12CommandQueue * DX12GPUDevice::GetD3D12DeviceQueue( gpu_cmd_device_queue_id_t pQueueID ) const
	{
		return _cmdManager->QueryInterface<DX12GPUCmdManager>()->GetD3D12DeviceQueue( pQueueID );
	}

	void DX12GPUDevice::InitializeCommandSystem()
	{
		Ic3DebugAssert( !_cmdManager );
		auto cmdManager = CreateGfxObject<DX12GPUCmdManager>( *this );
		auto initResult = cmdManager->Initialize();
		if( initResult )
		{
			_cmdManager = std::move( cmdManager );
		}
	}

	void DX12GPUDevice::InitializeDefaultPresentDeviceQueue()
	{
		if( !_cmdManager->IsQueueAvailable( E_DEVICE_COMMAND_QUEUE_ID_PRESENT ) )
		{
			Ic3DebugAssert( _cmdManager->IsQueueAvailable( E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_GRAPHICS ) );
			_cmdManager->SetQueueAlias( E_DEVICE_COMMAND_QUEUE_ID_PRESENT, E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_GRAPHICS );
		}
	}

} // namespace Ic3::Graphics::GCI
