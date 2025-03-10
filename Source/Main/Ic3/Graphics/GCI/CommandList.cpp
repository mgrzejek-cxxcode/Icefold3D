
#include "CommandList.h"
#include "CommandSystem.h"
#include "GPUDevice.h"
#include "Resources/GPUBuffer.h"
#include "State/GraphicsPipelineStateController.h"
#include "State/GraphicsPipelineStateDescriptorRTODynamic.h"

namespace Ic3::Graphics::GCI
{

	enum ECommandListInternalStateFlags : uint32
	{
		eCommandListInternalStateFlagActiveRenderPassBit = 0x10000
	};

	CommandList::CommandList(
			CommandSystem & pCommandSystem,
			ECommandListType pListType,
			GraphicsPipelineStateController & pPipelineStateController )
	: GPUDeviceChildObject( pCommandSystem.mGPUDevice )
	, mCommandSystem( &pCommandSystem )
	, mListType( pListType )
	, mCommandFlags( static_cast<ECommandObjectPropertyFlags>( pListType ) & eCommandObjectPropertyMaskAll )
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
		return _internalStateMask.is_set( eCommandListInternalStateFlagActiveRenderPassBit );
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

	bool CommandList::BeginRenderPass( const RenderPassDescriptor & pRenderPassDescriptor, cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		Ic3DebugAssert( !IsRenderPassActive() );

		_graphicsPipelineStateController->SetRenderPassDescriptor( pRenderPassDescriptor );

		return OnBeginRenderPass( pRenderPassDescriptor, pFlags );
	}

	bool CommandList::BeginRenderPassDynamic( RenderPassDescriptorDynamic & pRenderPassDescriptor, cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		Ic3DebugAssert( !IsRenderPassActive() );

		_graphicsPipelineStateController->SetRenderPassDescriptorDynamic( pRenderPassDescriptor );

		return OnBeginRenderPassDynamic( pRenderPassDescriptor, pFlags );
	}

	void CommandList::EndRenderPass()
	{
		Ic3DebugAssert( IsRenderPassActive() );

		OnEndRenderPass();

		_graphicsPipelineStateController->ResetRenderPassDescriptor();
	}

	bool CommandList::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPipelineStateObject )
	{
		return _graphicsPipelineStateController->SetGraphicsPipelineStateObject( pGraphicsPipelineStateObject );
	}

	bool CommandList::SetRenderTargetDescriptor(
			const RenderTargetDescriptor & pRenderTargetDescriptor )
	{
		return _graphicsPipelineStateController->SetRenderTargetDescriptor( pRenderTargetDescriptor );
	}

	bool CommandList::SetRenderTargetDescriptorDynamic(
			RenderTargetDescriptorDynamic & pRenderTargetDescriptor )
	{
		return _graphicsPipelineStateController->SetRenderTargetDescriptorDynamic( pRenderTargetDescriptor );
	}

	bool CommandList::SetVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor )
	{
		return _graphicsPipelineStateController->SetVertexSourceBindingDescriptor( pVertexSourceBindingDescriptor );
	}

	bool CommandList::SetVertexSourceBindingDescriptorDynamic(
			VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor )
	{
		return _graphicsPipelineStateController->SetVertexSourceBindingDescriptorDynamic( pVertexSourceBindingDescriptor );
	}

	void CommandList::CmdSetDynamicBlendConstantColor( const cxm::rgba_color_r32_norm & pBlendConstantColor )
	{
	}

	void CommandList::CmdSetDynamicRenderTargetClearConfig( const RenderTargetAttachmentClearConfig & pClearConfig )
	{
	}

	void CommandList::CmdSetDynamicStencilTestRefValue( uint8 pStencilRefValue )
	{
	}

	void CommandList::CmdResetDynamicPipelineConfig( cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> pConfigMask )
	{
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

	void CommandList::ValidateActiveRenderPassOrThrow() const
	{
		if( !ValidateActiveRenderPass() )
		{
			throw 0;
		}
	}

	bool CommandList::ValidateActiveRenderPass() const noexcept
	{
		return IsRenderPassActive() && _graphicsPipelineStateController->IsRenderPassDescriptorSet();
	}

	bool CommandList::OnBeginRenderPass(
			const RenderPassDescriptor & pRenderPassDescriptor,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		OnBeginRenderPassUpdateInternal( pFlags );

		return IsRenderPassActive();
	}


	bool CommandList::OnBeginRenderPassDynamic(
			RenderPassDescriptorDynamic & pRenderPassDescriptor,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		OnBeginRenderPassUpdateInternal( pFlags );

		return IsRenderPassActive();
	}

	void CommandList::OnEndRenderPass()
	{
		if( !_internalStateMask.is_set( eCommandListActionFlagRenderPassPreserveDynamicStateBit ) )
		{
			_graphicsPipelineStateController->ResetDynamicPipelineConfig();
		}

		_internalStateMask.unset( eCommandListInternalStateFlagActiveRenderPassBit );
	}

	void CommandList::OnBeginRenderPassUpdateInternal( cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		_internalStateMask.set( eCommandListInternalStateFlagActiveRenderPassBit );

		_internalStateMask.set_or_unset(
				eCommandListActionFlagRenderPassPreserveDynamicStateBit,
				pFlags.is_set( eCommandListActionFlagRenderPassPreserveDynamicStateBit ) );

		if( pFlags.is_set( eCommandListActionFlagRenderPassApplyPipelineStateBit ) )
		{
			_graphicsPipelineStateController->ApplyStateChanges();
		}
	}

} // namespace Ic3::Graphics::GCI
