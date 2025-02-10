
#include "CommandList.h"
#include "CommandSystem.h"
#include "GPUDevice.h"
#include "Resources/GPUBuffer.h"
#include "State/GraphicsPipelineStateController.h"
#include "State/GraphicsPipelineStateDescriptorRTO.h"
#include "State/RenderTargetStateDescriptors.h"

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
	: GPUDeviceChildObject( pCommandSystem.mGPUDevice )
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
		return commandListPropertyFlags.is_set( pCommandListFlags & eCommandObjectPropertyMaskAll );
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

	bool CommandList::MapBuffer( GPUBuffer & pBuffer, EGPUMemoryMapMode pMapMode )
	{
		return MapBufferRegion( pBuffer, GPUMemoryRegion{ 0, pBuffer.mBufferProperties.byteSize }, pMapMode );
	}

	bool CommandList::MapBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode )
	{
		if( !pBuffer.ValidateMapRequest( pRegion, pMapMode ) )
		{
			Ic3DebugInterrupt();
			return false;
		}

		return pBuffer.MapRegion( this, pRegion, pMapMode );
	}

	bool CommandList::UnmapBuffer( GPUBuffer & pBuffer )
	{
		if( !pBuffer.IsMapped() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		pBuffer.Unmap( this );

		return true;
	}

	bool CommandList::FlushMappedBuffer( GPUBuffer & pBuffer )
	{
		GPUMemoryRegion flushRegion;
		flushRegion.offset = 0;
		flushRegion.size = pBuffer.mBufferProperties.byteSize;

		return FlushMappedBufferRegion( pBuffer, flushRegion );
	}

	bool CommandList::FlushMappedBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion )
	{
		if( !pBuffer.IsMapped() )
		{
			Ic3DebugInterrupt();
			return false;
		}

		pBuffer.FlushMappedRegion( this, pRegion );

		return true;
	}

	bool CommandList::InvalidateBuffer( GPUBuffer & pBuffer )
	{
		GPUMemoryRegion InvalidateRegion;
		InvalidateRegion.offset = 0;
		InvalidateRegion.size = pBuffer.mBufferProperties.byteSize;

		return InvalidateBufferRegion( pBuffer, InvalidateRegion );
	}

	bool CommandList::InvalidateBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion )
	{
		pBuffer.InvalidateRegion( this, pRegion );
		return true;
	}

	bool CommandList::UpdateBufferDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferDataCopyDesc & pCopyDesc )
	{
		GPUBufferSubDataCopyDesc subDataCopyDesc;
		subDataCopyDesc.flags = pCopyDesc.flags;
		subDataCopyDesc.flags.set( eGPUBufferDataCopyFlagModeInvalidateBit );
		subDataCopyDesc.flags.unset( eGPUBufferDataCopyFlagModeAppendBit );
		subDataCopyDesc.sourceBufferRegion.offset = 0;
		subDataCopyDesc.sourceBufferRegion.size = pSourceBuffer.mBufferProperties.byteSize;
		subDataCopyDesc.targetBufferOffset = 0;

		return UpdateBufferSubDataCopy( pBuffer, pSourceBuffer, subDataCopyDesc );
	}

	bool CommandList::UpdateBufferSubDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc )
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

	bool CommandList::UpdateBufferDataUpload( GPUBuffer & pBuffer, const GPUBufferDataUploadDesc & pUploadDesc )
	{
		GPUBufferSubDataUploadDesc subDataUploadDesc;
		subDataUploadDesc.flags = pUploadDesc.flags;
		subDataUploadDesc.flags.set( eGPUBufferDataCopyFlagModeInvalidateBit );
		subDataUploadDesc.flags.unset( eGPUBufferDataCopyFlagModeAppendBit );
		subDataUploadDesc.bufferRegion.offset = 0;
		subDataUploadDesc.bufferRegion.size = cppx::get_min_of( pUploadDesc.inputDataDesc.size, pBuffer.mBufferProperties.byteSize );
		subDataUploadDesc.inputDataDesc = pUploadDesc.inputDataDesc;

		return UpdateBufferSubDataUpload( pBuffer, subDataUploadDesc );
	}

	bool CommandList::UpdateBufferSubDataUpload( GPUBuffer & pBuffer, const GPUBufferSubDataUploadDesc & pUploadDesc )
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
			const RenderPassConfigurationStateDescriptor & pRenderPassState,
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

	bool CommandList::SetIAVertexStreamState( const IAVertexStreamStateDescriptor & pIAVertexStreamState )
	{
		return _graphicsPipelineStateController->SetIAVertexStreamState( pIAVertexStreamState );
	}

	bool CommandList::SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState )
	{
		return _graphicsPipelineStateController->SetIAVertexStreamState( pIAVertexStreamState );
	}

	bool CommandList::SetRenderTargetBindingState( const RenderTargetBindingStateDescriptor & pRenderTargetBindingState )
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

	bool CommandList::CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
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

		const auto preserveDynamicState = pFlags.is_set( eCommandListActionFlagRenderPassPreserveDynamicStateBit );
		_internalStateMask.set_or_unset( eCommandListActionFlagRenderPassPreserveDynamicStateBit, preserveDynamicState );

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
			const RenderPassConfigurationStateDescriptor & pRenderPassState,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		if( CommandList::BeginRenderPass( pRenderPassState, pFlags ) )
		{
			const auto * defaultRenderPassState = pRenderPassState.QueryInterface<RenderPassConfigurationStateDescriptorGeneric>();
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
