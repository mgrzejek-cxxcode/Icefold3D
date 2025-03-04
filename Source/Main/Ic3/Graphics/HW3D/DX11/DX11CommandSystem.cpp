
#include "DX11CommandSystem.h"
#include "DX11CommandList.h"
#include "DX11GPUDevice.h"
#include <Ic3/Graphics/GCI/CommandContext.h>

namespace Ic3::Graphics::GCI
{

	DX11CommandSystem::DX11CommandSystem( DX11GPUDevice & pDX11GPUDevice )
	: CommandSystem( pDX11GPUDevice )
	, mD3D11Device1( pDX11GPUDevice.mD3D11Device1 )
	{ }

	DX11CommandSystem::~DX11CommandSystem() = default;

	std::unique_ptr<CommandContext> DX11CommandSystem::AcquireCommandContext( ECommandContextType pContextType )
	{
		std::unique_ptr<CommandContext> commandContext;

		const auto contextExecutionMode = CXU::GetCommandObjectExecutionMode( pContextType );
		if( auto * commandList = AcquireCommandList( contextExecutionMode ) )
		{
			if( contextExecutionMode == ECommandExecutionMode::Direct )
			{
				commandContext.reset( new CommandContextDirectGraphics( *commandList ) );
			}
			else
			{
				commandContext.reset( new CommandContextDeferredGraphics( *commandList ) );
			}
		}

		return commandContext;
	}

	CommandSync DX11CommandSystem::SubmitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo )
	{
		CommandSync cmdSyncObject;

		if( pSubmitInfo.syncMode == ECommandSubmitSyncMode::Default )
		{
			auto * dx11CommandSyncData = new DX11CommandSyncData();
			cmdSyncObject.syncData = dx11CommandSyncData;
			cmdSyncObject.syncDataReleaseFunc = releaseDX11CommandSyncData;

			auto * dx11CommandList = pContext.mCommandList->QueryInterface<DX11CommandList>();
			dx11CommandSyncData->d3d11SyncQuery = dx11CommandList->ReleaseExecutionSyncQuery();
			dx11CommandSyncData->d3d11DeviceContext1 = dx11CommandList->mD3D11DeviceContext1.Get();
		}

		return cmdSyncObject;
	}

	DX11CommandList * DX11CommandSystem::AcquireCommandList( ECommandExecutionMode pCommandExecutionMode  )
	{
		DX11CommandList * commandList = nullptr;

		if( pCommandExecutionMode == ECommandExecutionMode::Direct )
		{
			if( !_directCommandList )
			{
				InitializeDirectCommandList();
			}
			if( _directCommandList && _directCommandList->AcquireList() )
			{
				commandList = _directCommandList.get();
			}
		}
		else
		{
			std::unique_lock<std::mutex> listLockGuard{ _availableListLock };
			if( !_availableList.empty() || CreateDeferredCommandList() )
			{
				commandList = _availableList.back();
				_availableList.pop_back();
			}
		}

		return commandList;
	}

	bool DX11CommandSystem::InitializeDirectCommandList()
	{
		Ic3DebugAssert( !_directCommandList );

		ComPtr<ID3D11DeviceContext1> d3d11ImmediateContext;
		mD3D11Device1->GetImmediateContext1( &d3d11ImmediateContext );
		if( !d3d11ImmediateContext )
		{
			return false;
		}

		_directCommandList = CreateGfxObject<DX11CommandList>( *this, ECommandListType::DirectGraphics, d3d11ImmediateContext );

		return true;
	}

	bool DX11CommandSystem::CreateDeferredCommandList()
	{
		ComPtr<ID3D11DeviceContext1> d3d11DeferredContext;
		auto hResult = mD3D11Device1->CreateDeferredContext1( 0, &d3d11DeferredContext );
		if( FAILED( hResult ) )
		{
			return false;
		}

		auto deferredCommandList = CreateGfxObject<DX11CommandList>( *this, ECommandListType::DeferredGraphics, d3d11DeferredContext );
		auto * deferredCommandListPtr = deferredCommandList.get();

		_deferredCommandListStorage[deferredCommandListPtr] = std::move( deferredCommandList );
		_availableList.push_back( deferredCommandListPtr );

		return true;
	}


	void releaseDX11CommandSyncData( void * pSyncData )
	{
		if( pSyncData )
		{
			auto * dx11CommandSyncData = static_cast<DX11CommandSyncData *>( pSyncData );
			dx11CommandSyncData->d3d11SyncQuery->Release();
			dx11CommandSyncData->d3d11SyncQuery = nullptr;
			dx11CommandSyncData->d3d11DeviceContext1 = nullptr;
			delete dx11CommandSyncData;
		}
	}

} // namespace Ic3::Graphics::GCI
