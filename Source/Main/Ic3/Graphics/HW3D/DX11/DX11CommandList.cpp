
#include "DX11CommandList.h"
#include "DX11CommandSystem.h"
#include "DX11APITranslationLayer.h"
#include "DX11GPUDevice.h"
#include "Resources/DX11GPUBuffer.h"
#include "Resources/DX11Sampler.h"
#include "Resources/DX11Shader.h"
#include "Resources/DX11Texture.h"
#include "State/DX11pipelineStateController.h"
#include "State/DX11pipelineStateObject.h"
#include <Ic3/Graphics/GCI/CommandContext.h>

namespace Ic3::Graphics::GCI
{

	DX11CommandList::DX11CommandList( DX11CommandSystem & pDX11CommandSystem, ECommandListType pListType, ComPtr<ID3D11DeviceContext1> pD3D11DeviceContext1 )
	: CommandListRenderPassDefault( pDX11CommandSystem, pListType, _graphicsPipelineStateControllerDX11 )
	, mD3D11Device1( pDX11CommandSystem.mD3D11Device1 )
	, mD3D11DeviceContext1( std::move( pD3D11DeviceContext1 ) )
	, _graphicsPipelineStateControllerDX11( *this )
	{}

	DX11CommandList::~DX11CommandList() = default;

	void DX11CommandList::beginCommandSequence()
	{
		CommandList::beginCommandSequence();

		if( _d3d11ExecutionSyncQuery )
		{
			mD3D11DeviceContext1->Begin( _d3d11ExecutionSyncQuery.Get() );
		}
	}

	void DX11CommandList::endCommandSequence()
	{
		if( _d3d11ExecutionSyncQuery )
		{
			mD3D11DeviceContext1->End( _d3d11ExecutionSyncQuery.Get() );
		}

		// _stateController.resetInternalState();

		CommandList::endCommandSequence();
	}

	void DX11CommandList::cmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex )
	{
		_graphicsPipelineStateControllerDX11.applyStateChanges();
		mD3D11DeviceContext1->DrawIndexed( pIndicesNum, pIndicesOffset, pBaseVertexIndex );
	}

	void DX11CommandList::cmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset )
	{
		_graphicsPipelineStateControllerDX11.applyStateChanges();
		mD3D11DeviceContext1->DrawIndexedInstanced( pIndicesNumPerInstance, pInstancesNum, pIndicesOffset, 0, 0 );
	}

	void DX11CommandList::cmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset )
	{
		_graphicsPipelineStateControllerDX11.applyStateChanges();
		mD3D11DeviceContext1->Draw( pVerticesNum, pVerticesOffset );
	}

	void DX11CommandList::cmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset )
	{
	}

	void DX11CommandList::cmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext )
	{
		auto * dx11CommandList = pDeferredContext.mCommandList->queryInterface<DX11CommandList>();

		ComPtr<ID3D11CommandList> d3d11RecorderCommandList;
		dx11CommandList->mD3D11DeviceContext1->FinishCommandList( FALSE, &d3d11RecorderCommandList );

		mD3D11DeviceContext1->ExecuteCommandList( d3d11RecorderCommandList.Get(), FALSE );
	}

	ID3D11Query * DX11CommandList::releaseExecutionSyncQuery()
	{
		auto * syncQuery = _d3d11ExecutionSyncQuery.Get();
		_d3d11ExecutionSyncQuery.Reset();
		return syncQuery;
	}


	void DX11CommandList::executeRenderPassLoadActions(
			const RenderPassConfiguration & pRenderPassConfiguration,
			const GraphicsPipelineDynamicState & pDynamicState )
	{
		if( pRenderPassConfiguration.attachmentsActionClearMask != 0 )
		{
			smutil::renderPassClearRenderTargetDX11(
					mD3D11DeviceContext1.Get(),
					_graphicsPipelineStateControllerDX11.getCurrentRenderTargetBinding(),
					pRenderPassConfiguration,
					pDynamicState );
		}
	}

	void DX11CommandList::executeRenderPassStoreActions(
			const RenderPassConfiguration & pRenderPassConfiguration,
			const GraphicsPipelineDynamicState & pDynamicState )
	{
		if( pRenderPassConfiguration.attachmentsActionResolveMask != 0 )
		{
			smutil::renderPassResolveRenderTargetDX11(
					mD3D11DeviceContext1.Get(),
					_graphicsPipelineStateControllerDX11.getCurrentRenderTargetBinding(),
					pRenderPassConfiguration,
					pDynamicState );
		}
	}

	ComPtr<ID3D11Query> DX11CommandList::_createExecutionSyncQuery( ComPtr<ID3D11Device1> pD3D11Device )
	{
		D3D11_QUERY_DESC syncQueryDesc;
		syncQueryDesc.Query = D3D11_QUERY_EVENT;
		syncQueryDesc.MiscFlags = 0;

		ComPtr<ID3D11Query> d3d11Query;
		auto hResult = pD3D11Device->CreateQuery( &syncQueryDesc, d3d11Query.GetAddressOf() );
		if( FAILED( hResult ) || !d3d11Query )
		{
			ic3DebugInterrupt();
		}

		return d3d11Query;
	}

} // namespace Ic3::Graphics::GCI
