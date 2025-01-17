
#include "DX12GpuCmdContext.h"
#include "DX12GpuCmdManager.h"
#include "DX12GpuDevice.h"
#include "DX12coreAPIProxy.h"

namespace Ic3::Graphics::GCI
{

	DX12CommandContext::DX12CommandContext( DX12GpuCmdManager & pDX12GpuCmdManager,
	                                      ComPtr<ID3D12CommandAllocator> pD3D12CommandAllocator,
	                                      ComPtr<ID3D12GraphicsCommandList> pD3D12GraphicsCommandList )
	: CommandContext( pDX12GpuCmdManager )
	, mD3D12CommandAllocator( std::move( pD3D12CommandAllocator ) )
	, mD3D12GraphicsCommandList( std::move( pD3D12GraphicsCommandList ) )
	, mD3D12CommandListType( mD3D12GraphicsCommandList->GetType() )
	{ }

	DX12CommandContext::~DX12CommandContext() = default;

	void DX12CommandContext::BeginCommandSequence( const CommandContextCommandSequenceBeginInfo & pInfo )
	{
		ic3DX12CmdContextVerifyGraphics( this );
		mD3D12CommandAllocator->Reset();
		mD3D12GraphicsCommandList->Reset( mD3D12CommandAllocator.Get(), nullptr );
	}

	void DX12CommandContext::EndCommandSequence( const CommandContextCommandSequenceEndInfo & pInfo )
	{
		ic3DX12CmdContextVerifyGraphics( this );
		mD3D12GraphicsCommandList->Close();
	}

	void DX12CommandContext::ExecuteSecondaryContext( CommandContext & pSecondaryCmdContext )
	{
		auto * dx12CmdContext = pSecondaryCmdContext.QueryInterface<DX12CommandContext>();
		mD3D12GraphicsCommandList->ExecuteBundle( dx12CmdContext->mD3D12GraphicsCommandList.Get() );
	}

	void DX12CommandContext::ClearColorTarget( const Math::RGBAColorR32Norm & pColor )
	{
		ic3DX12CmdContextVerifyGraphics( this );

		const FLOAT clearColor[] = { pColor.fpRed, pColor.fpGreen, pColor.fpBlue, pColor.fpAlpha };
		for( uint32 rtIndex = 0; rtIndex < _renderTargetState.rtvDescriptorsNum; ++rtIndex )
		{
			auto & rtvDescriptorHandle = _renderTargetState.rtvDescriptorArray[rtIndex];
			mD3D12GraphicsCommandList->ClearRenderTargetView( rtvDescriptorHandle, clearColor, 0, nullptr );
		}
	}

	void DX12CommandContext::SetRenderTargetState( const DX12RenderTargetState & pRenderTargetState )
	{
		mem_copy_unchecked(
				&_renderTargetState,
				sizeof( DX12RenderTargetState ),
				&pRenderTargetState,
				sizeof( DX12RenderTargetState ) );

		const D3D12_CPU_DESCRIPTOR_HANDLE * rtvDescriptorArray = nullptr;
		const D3D12_CPU_DESCRIPTOR_HANDLE * dsvDescriptor = nullptr;

		if( _renderTargetState.rtvDescriptorsNum > 0 )
		{
			rtvDescriptorArray = _renderTargetState.rtvDescriptorArray.data();
		}
		if( !DX12CoreAPIProxy::CheckDescriptorEmpty( _renderTargetState.dsvDescriptor ) )
		{
			dsvDescriptor = &( _renderTargetState.dsvDescriptor );
		}

		mD3D12GraphicsCommandList->OMSetRenderTargets( _renderTargetState.rtvDescriptorsNum, rtvDescriptorArray, FALSE, dsvDescriptor );
	}

} // namespace Ic3::Graphics::GCI
