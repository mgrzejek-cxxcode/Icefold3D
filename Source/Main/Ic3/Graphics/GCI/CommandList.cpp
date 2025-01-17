
#include "CommandList.h"
#include "CommandSystem.h"
#include "GpuDevice.h"
#include "Resources/GpuBuffer.h"
#include "State/GraphicsPipelineStateController.h"
#include "State/RenderTargetDynamicStates.h"
#include "State/RenderTargetImmutableStates.h"

namespace Ic3::Graphics::GCI
{

	enum ECommandListInternalStateFlags : uint32
	{
		E_COMMAND_LIST_INTERNAL_STATE_FLAG_ACTIVE_RENDER_PASS_BIT = 0x10000
	};

	CommandList::CommandList(
			CommandSystem & pCommandSystem,
			ECommandListType pListType,
			GraphicsPipelineStateController & pPipelineStateController )
	: GpuDeviceChildObject( pCommandSystem.mGpuDevice )
	, mCommandSystem( &pCommandSystem )
	, mListType( pListType )
	, _graphicsPipelineStateController( &pPipelineStateController )
	{}

	CommandList::~CommandList() = default;

	bool CommandList::CheckCommandClassSupport( ECommandQueueClass pQueueClass ) const noexcept
	{
		return CheckFeatureSupport( static_cast<ECommandObjectPropertyFlags>( pQueueClass ) );
	}

	bool CommandList::CheckFeatureSupport( cppx::bitmask<ECommandObjectPropertyFlags> pCommandListFlags ) const noexcept
	{
		// Command list type (its value) is basically a bitwise OR of all supported bits.
		cppx::bitmask<ECommandObjectPropertyFlags> commandListPropertyFlags = static_cast<ECommandObjectPropertyFlags>( mListType );

		// Check if the specified command classes and/or execution type matches those supported by the list.
		return commandListPropertyFlags.is_set( pCommandListFlags & ECommandObjectPropertyMaskAll );
	}

	bool CommandList::IsRenderPassActive() const noexcept
	{
		return _internalStateMask.is_set( E_COMMAND_LIST_INTERNAL_STATE_FLAG_ACTIVE_RENDER_PASS_BIT );
	}

	bool CommandList::HasPendingGraphicsPipelineStateChanges() const noexcept
	{
		return _graphicsPipelineStateController->HasPendingStateChanges();
	}

	bool CommandList::AcquireList()
	{
		auto listLockStatus = false;
		auto acquireSuccessful = _listLockStatus.compare_exchange_strong(
			listLockStatus,
			true,
			std::memory_order_acq_rel,
			std::memory_order_relaxed );

		return acquireSuccessful;
	}

	void CommandList::ReleaseList()
	{
		_listLockStatus.store( false, std::memory_order_release );
	}

	bool CommandList::ApplyGraphicsPipelineStateChanges()
	{
		return _graphicsPipelineStateController->ApplyStateChanges();
	}

	void CommandList::BeginCommandSequence()
	{}

	void CommandList::EndCommandSequence()
	{}

	bool CommandList::MapBuffer( GpuBuffer & pBuffer, EGpuMemoryMapMode pMapMode )
	{
		return MapBufferRegion( pBuffer, GpuMemoryRegion{ 0, pBuffer.mBufferProperties.byteSize }, pMapMode );
	}

	bool CommandList::MapBufferRegion( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode )
	{
		if( !pBuffer.ValidateMapRequest( pRegion, pMapMode ) )
		{
			Ic3DebugInterrupt();
			return false;
		}

		return pBuffer.MapRegion( this, pRegion, pMapMode );
	}

	bool CommandList::UnmapBuffer( GpuBuffer & pBuffer )
	{
		if( !pBuffer.IsMapped() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		pBuffer.Unmap( this );

		return true;
	}

	bool CommandList::FlushMappedBuffer( GpuBuffer & pBuffer )
	{
		GpuMemoryRegion flushRegion;
		flushRegion.offset = 0;
		flushRegion.size = pBuffer.mBufferProperties.byteSize;

		return FlushMappedBufferRegion( pBuffer, flushRegion );
	}

	bool CommandList::FlushMappedBufferRegion( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion )
	{
		if( !pBuffer.IsMapped() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		pBuffer.FlushMappedRegion( this, pRegion );

		return true;
	}

	bool CommandList::InvalidateBuffer( GpuBuffer & pBuffer )
	{
		GpuMemoryRegion InvalidateRegion;
		InvalidateRegion.offset = 0;
		InvalidateRegion.size = pBuffer.mBufferProperties.byteSize;

		return InvalidateBufferRegion( pBuffer, InvalidateRegion );
	}

	bool CommandList::InvalidateBufferRegion( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion )
	{
		pBuffer.InvalidateRegion( this, pRegion );
		return true;
	}

	bool CommandList::UpdateBufferDataCopy( GpuBuffer & pBuffer, GpuBuffer & pSourceBuffer, const GpuBufferDataCopyDesc & pCopyDesc )
	{
		GpuBufferSubDataCopyDesc subDataCopyDesc;
		subDataCopyDesc.flags = pCopyDesc.flags;
		subDataCopyDesc.flags.set( eGpuBufferDataCopyFlagModeInvalidateBit );
		subDataCopyDesc.flags.unset( eGpuBufferDataCopyFlagModeAppendBit );
		subDataCopyDesc.sourceBufferRegion.offset = 0;
		subDataCopyDesc.sourceBufferRegion.size = pSourceBuffer.mBufferProperties.byteSize;
		subDataCopyDesc.targetBufferOffset = 0;

		return UpdateBufferSubDataCopy( pBuffer, pSourceBuffer, subDataCopyDesc );
	}

	bool CommandList::UpdateBufferSubDataCopy( GpuBuffer & pBuffer, GpuBuffer & pSourceBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc )
	{
		if( pBuffer.IsMapped() || pSourceBuffer.IsMapped() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		if( pBuffer.mBufferProperties.byteSize != pSourceBuffer.mBufferProperties.byteSize )
		{
			Ic3DebugInterrupt();
			return false;
		}

		pBuffer.UpdateSubDataCopy( this, pSourceBuffer, pCopyDesc );

		return true;
	}

	bool CommandList::UpdateBufferDataUpload( GpuBuffer & pBuffer, const GpuBufferDataUploadDesc & pUploadDesc )
	{
		GpuBufferSubDataUploadDesc subDataUploadDesc;
		subDataUploadDesc.flags = pUploadDesc.flags;
		subDataUploadDesc.flags.set( eGpuBufferDataCopyFlagModeInvalidateBit );
		subDataUploadDesc.flags.unset( eGpuBufferDataCopyFlagModeAppendBit );
		subDataUploadDesc.bufferRegion.offset = 0;
		subDataUploadDesc.bufferRegion.size = cppx::get_min_of( pUploadDesc.inputDataDesc.size, pBuffer.mBufferProperties.byteSize );
		subDataUploadDesc.inputDataDesc = pUploadDesc.inputDataDesc;

		return UpdateBufferSubDataUpload( pBuffer, subDataUploadDesc );
	}

	bool CommandList::UpdateBufferSubDataUpload( GpuBuffer & pBuffer, const GpuBufferSubDataUploadDesc & pUploadDesc )
	{
		if( !pUploadDesc.inputDataDesc )
		{
			Ic3DebugInterrupt();
			return false;
		}

		if( pBuffer.IsMapped() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		pBuffer.UpdateSubDataUpload( this, pUploadDesc );

		return true;
	}

	bool CommandList::BeginRenderPass(
			const RenderPassConfigurationImmutableState & pRenderPassState,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		return OnBeginRenderPass( pFlags );
	}
	
	bool CommandList::BeginRenderPass(
			const RenderPassConfigurationDynamicState & pRenderPassState,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		return OnBeginRenderPass( pFlags );
	}

	void CommandList::EndRenderPass()
	{
		OnEndRenderPass();
	}

	void CommandList::SetRenderPassDynamicState( const GraphicsPipelineDynamicState & pDynamicState )
	{
		_graphicsPipelineStateController->SetRenderPassDynamicState( pDynamicState );
	}

	void CommandList::ResetRenderPassDynamicState()
	{
		_graphicsPipelineStateController->ResetRenderPassDynamicState();
	}

	bool CommandList::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		return _graphicsPipelineStateController->SetGraphicsPipelineStateObject( pGraphicsPSO );
	}

	bool CommandList::SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState )
	{
		return _graphicsPipelineStateController->SetIAVertexStreamState( pIAVertexStreamState );
	}

	bool CommandList::SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState )
	{
		return _graphicsPipelineStateController->SetIAVertexStreamState( pIAVertexStreamState );
	}

	bool CommandList::SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState )
	{
		return _graphicsPipelineStateController->SetRenderTargetBindingState( pRenderTargetBindingState );
	}

	bool CommandList::SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState )
	{
		return _graphicsPipelineStateController->SetRenderTargetBindingState( pRenderTargetBindingState );
	}

	bool CommandList::CmdSetViewport( const ViewportDesc & pViewportDesc )
	{
		if( !IsRenderPassActive() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		return _graphicsPipelineStateController->SetViewport( pViewportDesc );
	}

	bool CommandList::CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		if( !IsRenderPassActive() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		return _graphicsPipelineStateController->SetShaderConstant( pParamRefID, pData );
	}

	bool CommandList::CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer )
	{
		if( !IsRenderPassActive() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		return _graphicsPipelineStateController->SetShaderConstantBuffer( pParamRefID, pConstantBuffer );
	}

	bool CommandList::CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		if( !IsRenderPassActive() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		return _graphicsPipelineStateController->SetShaderTextureImage( pParamRefID, pTexture );
	}

	bool CommandList::CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		if( !IsRenderPassActive() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		return _graphicsPipelineStateController->SetShaderTextureSampler( pParamRefID, pSampler );
	}

	bool CommandList::OnBeginRenderPass( cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		if( IsRenderPassActive() )
		{
			return false;
		}

		_internalStateMask.set( E_COMMAND_LIST_INTERNAL_STATE_FLAG_ACTIVE_RENDER_PASS_BIT );

		_internalStateMask.set_or_unset(
				eCommandListActionFlagRenderPassPreserveDynamicStateBit,
				pFlags.is_set( eCommandListActionFlagRenderPassPreserveDynamicStateBit ));

		if( pFlags.is_set( eCommandListActionFlagRenderPassApplyPipelineStateBit ) )
		{
			_graphicsPipelineStateController->ApplyStateChanges();
		}

		return true;
	}

	void CommandList::OnEndRenderPass()
	{
		Ic3DebugAssert( IsRenderPassActive() );

		if( !_internalStateMask.is_set( eCommandListActionFlagRenderPassPreserveDynamicStateBit ) )
		{
			_graphicsPipelineStateController->ResetRenderPassDynamicState();
		}

		_internalStateMask.unset( E_COMMAND_LIST_INTERNAL_STATE_FLAG_ACTIVE_RENDER_PASS_BIT );
	}


	CommandListRenderPassDefault::CommandListRenderPassDefault(
			CommandSystem & pCommandSystem,
			ECommandListType pListType,
			GraphicsPipelineStateController & pPipelineStateController )
	: CommandList( pCommandSystem, pListType, pPipelineStateController )
	{}

	CommandListRenderPassDefault::~CommandListRenderPassDefault() = default;

	const RenderPassConfiguration & CommandListRenderPassDefault::GetRenderPassConfiguration() const noexcept
	{
		return _currentRenderPassConfiguration;
	}

	bool CommandListRenderPassDefault::BeginRenderPass(
			const RenderPassConfigurationImmutableState & pRenderPassState,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		if( CommandList::BeginRenderPass( pRenderPassState, pFlags ) )
		{
			const auto * defaultRenderPassState = pRenderPassState.QueryInterface<RenderPassConfigurationImmutableStateDefault>();
			_currentRenderPassConfiguration = defaultRenderPassState->mRenderPassConfiguration;

			ExecuteRenderPassLoadActions(
					_currentRenderPassConfiguration,
					_graphicsPipelineStateController->GetRenderPassDynamicState() );

			return true;
		}

		return false;
	}

	bool CommandListRenderPassDefault::BeginRenderPass(
			const RenderPassConfigurationDynamicState & pRenderPassState,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		if( CommandList::BeginRenderPass( pRenderPassState, pFlags ) )
		{
			_currentRenderPassConfiguration = pRenderPassState.GetRenderPassConfiguration();

			ExecuteRenderPassLoadActions(
					_currentRenderPassConfiguration,
					_graphicsPipelineStateController->GetRenderPassDynamicState() );

			return true;
		}

		return false;
	}

	void CommandListRenderPassDefault::EndRenderPass()
	{
		if( IsRenderPassActive() )
		{
			ExecuteRenderPassStoreActions(
					_currentRenderPassConfiguration,
					_graphicsPipelineStateController->GetRenderPassDynamicState() );

			CommandList::EndRenderPass();
		}
	}


} // namespace Ic3::Graphics::GCI
