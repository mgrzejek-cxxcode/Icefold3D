
#include "DX12GpuDevice.h"
#include "DX12GpuDriver.h"
#include "DX12GpuCmdContext.h"
#include "DX12GpuCmdManager.h"
#include "DX12coreAPIProxy.h"

namespace Ic3::Graphics::GCI
{

	DX12GpuDevice::DX12GpuDevice( DX12GpuDriver & pDriver,
	                              ComPtr<IDXGIFactory3> pDXGIFactory3,
	                              ComPtr<ID3D12Device> pD3D12Device,
	                              ComPtr<ID3D12Debug> pD3D12DebugInterface )
	: DXGpuDevice( pDriver, pDXGIFactory3 )
	, mDXGIFactory3( std::move( pDXGIFactory3 ) )
	, mD3D12Device( std::move( pD3D12Device ) )
	, mD3D12DebugInterface( std::move( pD3D12DebugInterface ) )
	{ }

	DX12GpuDevice::~DX12GpuDevice() = default;

	DX12GpuDeviceHandle DX12GpuDevice::Create( DX12GpuDriver & pDX12Driver, const DX12GpuDeviceCreateInfo & pCreateInfo )
	{
		auto d3d12DebugInterface = DX12CoreAPIProxy::InitializeD3D12DebugInterface( pDX12Driver.GetConfigFlags() );

		auto d3d12Device = DX12CoreAPIProxy::CreateD3D12Device();
		Ic3DebugAssert( d3d12Device );

		auto dxgiFactory3 = DX12CoreAPIProxy::createDXGIFactoryForD3D12Device( d3d12Device, d3d12DebugInterface );
		Ic3DebugAssert( dxgiFactory3 );

		auto dx12GpuDevice = CreateGfxObject<DX12GpuDevice>( pDX12Driver, dxgiFactory3, d3d12Device, d3d12DebugInterface );
		Ic3DebugAssert( dx12GpuDevice );

		dx12GpuDevice->InitializeCommandSystem();

		if( pCreateInfo.flags.is_set( E_GPU_DEVICE_CREATE_FLAG_INIT_DEFAULT_PRESENT_QUEUE_BIT ) )
		{
			dx12GpuDevice->InitializeDefaultPresentDeviceQueue();
		}

		return dx12GpuDevice;
	}

	ID3D12CommandQueue * DX12GpuDevice::GetD3D12DeviceQueue( gpu_cmd_device_queue_id_t pQueueID ) const
	{
		return _cmdManager->QueryInterface<DX12GpuCmdManager>()->GetD3D12DeviceQueue( pQueueID );
	}

	void DX12GpuDevice::InitializeCommandSystem()
	{
		Ic3DebugAssert( !_cmdManager );
		auto cmdManager = CreateGfxObject<DX12GpuCmdManager>( *this );
		auto initResult = cmdManager->Initialize();
		if( initResult )
		{
			_cmdManager = std::move( cmdManager );
		}
	}

	void DX12GpuDevice::InitializeDefaultPresentDeviceQueue()
	{
		if( !_cmdManager->IsQueueAvailable( E_DEVICE_COMMAND_QUEUE_ID_PRESENT ) )
		{
			Ic3DebugAssert( _cmdManager->IsQueueAvailable( E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_GRAPHICS ) );
			_cmdManager->SetQueueAlias( E_DEVICE_COMMAND_QUEUE_ID_PRESENT, E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_GRAPHICS );
		}
	}

} // namespace Ic3::Graphics::GCI
