
#include "CommandContext.h"
#include "CommandList.h"
#include "CommandSystem.h"
#include "GpuDevice.h"

namespace Ic3::Graphics::GCI
{

	CommandContext::CommandContext( CommandList & pCommandList, ECommandContextType pContextType )
	: GpuDeviceChildObject( pCommandList.mGpuDevice )
	, mCommandList( &pCommandList )
	, mCommandSystem( mCommandList->mCommandSystem )
	, mContextType( pContextType )
	, mCommandFlags( CxDef::GetCommandObjectPropertyFlags( pContextType ) )
	{}

	CommandContext::~CommandContext() = default;

	bool CommandContext::CheckCommandClassSupport( ECommandQueueClass pQueueClass ) const
	{
		return mCommandList->CheckCommandClassSupport( pQueueClass );
	}

	bool CommandContext::CheckFeatureSupport( cppx::bitmask<ECommandObjectPropertyFlags> pCommandContextFlags ) const
	{
		return mCommandList->CheckFeatureSupport( pCommandContextFlags );
	}

	void CommandContext::BeginCommandSequence()
	{
		return mCommandList->BeginCommandSequence();
	}

	void CommandContext::EndCommandSequence()
	{
		return mCommandList->EndCommandSequence();
	}

	bool CommandContext::MapBuffer( GpuBuffer & pBuffer, EGpuMemoryMapMode pMapMode )
	{
		return mCommandList->MapBuffer( pBuffer, pMapMode );
	}

	bool CommandContext::MapBufferRegion( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode )
	{
		return mCommandList->MapBufferRegion( pBuffer, pRegion, pMapMode );
	}

	bool CommandContext::UnmapBuffer( GpuBuffer & pBuffer )
	{
		return mCommandList->UnmapBuffer( pBuffer );
	}

	bool CommandContext::FlushMappedBuffer( GpuBuffer & pBuffer )
	{
		return mCommandList->FlushMappedBuffer( pBuffer );
	}

	bool CommandContext::FlushMappedBufferRegion( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion )
	{
		return mCommandList->FlushMappedBufferRegion( pBuffer, pRegion );
	}

	bool CommandContext::CheckCommandListSupport( cppx::bitmask<ECommandObjectPropertyFlags> pCmdListFlags )
	{
		return mCommandList->CheckFeatureSupport( pCmdListFlags );
	}


	void CommandContextDirect::Submit()
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		mCommandSystem->SubmitContext( *this, cxCommandContextSubmitDefault );
	}

	CommandSync CommandContextDirect::Submit( const CommandContextSubmitInfo & pSubmitInfo )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandSystem->SubmitContext( *this, pSubmitInfo );
	}

	void CommandContextDirect::CmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandList->CmdExecuteDeferredContext( pDeferredContext );
	}

	bool CommandContextDirect::InvalidateBuffer( GpuBuffer & pBuffer )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandList->InvalidateBuffer( pBuffer );
	}

	bool CommandContextDirect::InvalidateBufferRegion( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandList->InvalidateBufferRegion( pBuffer, pRegion );
	}


	bool CommandContextDirectTransfer::UpdateBufferDataCopy( GpuBuffer & pBuffer, GpuBuffer & pSourceBuffer, const GpuBufferDataCopyDesc & pCopyDesc )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->UpdateBufferDataCopy( pBuffer, pSourceBuffer, pCopyDesc );
	}

	bool CommandContextDirectTransfer::UpdateBufferSubDataCopy( GpuBuffer & pBuffer, GpuBuffer & pSourceBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->UpdateBufferSubDataCopy( pBuffer, pSourceBuffer, pCopyDesc );
	}

	bool CommandContextDirectTransfer::UpdateBufferDataUpload( GpuBuffer & pBuffer, const GpuBufferDataUploadDesc & pUploadDesc )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->UpdateBufferDataUpload( pBuffer, pUploadDesc );
	}

	bool CommandContextDirectTransfer::UpdateBufferSubDataUpload( GpuBuffer & pBuffer, const GpuBufferSubDataUploadDesc & pUploadDesc )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->UpdateBufferSubDataUpload( pBuffer, pUploadDesc );
	}


	void CommandContextDirectCompute::CmdDispatchCompute( uint32 pThrGroupSizeX, uint32 pThrGroupSizeY, uint32 pThrGroupSizeZ )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectCompute ) );
		return mCommandList->CmdDispatchCompute( pThrGroupSizeX, pThrGroupSizeY, pThrGroupSizeZ );
	}

	void CommandContextDirectCompute::CmdDispatchComputeIndirect( uint32 pIndirectBufferOffset )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectCompute ) );
		return mCommandList->CmdDispatchComputeIndirect( pIndirectBufferOffset );
	}


	bool CommandContextDirectGraphics::BeginRenderPass(
		const RenderPassConfigurationImmutableState & pRenderPassState,
		cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->BeginRenderPass( pRenderPassState, pFlags );
	}

	bool CommandContextDirectGraphics::BeginRenderPass(
		const RenderPassConfigurationDynamicState & pRenderPassState,
		cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->BeginRenderPass( pRenderPassState, pFlags );
	}

	void CommandContextDirectGraphics::EndRenderPass()
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->EndRenderPass();
	}

	bool CommandContextDirectGraphics::CmdSetViewport( const ViewportDesc & pViewportDesc )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetViewport( pViewportDesc );
	}

	bool CommandContextDirectGraphics::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetGraphicsPipelineStateObject( pGraphicsPSO );
	}

	bool CommandContextDirectGraphics::SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetIAVertexStreamState( pIAVertexStreamState );
	}

	bool CommandContextDirectGraphics::SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetIAVertexStreamState( pIAVertexStreamState );
	}

	bool CommandContextDirectGraphics::SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetRenderTargetBindingState( pRenderTargetBindingState );
	}

	bool CommandContextDirectGraphics::SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetRenderTargetBindingState( pRenderTargetBindingState );
	}

	bool CommandContextDirectGraphics::CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetShaderConstant( pParamRefID, pData );
	}

	bool CommandContextDirectGraphics::CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetShaderConstantBuffer( pParamRefID, pConstantBuffer );
	}

	bool CommandContextDirectGraphics::CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetShaderTextureImage( pParamRefID, pTexture );
	}

	bool CommandContextDirectGraphics::CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetShaderTextureSampler( pParamRefID, pSampler );
	}

	void CommandContextDirectGraphics::CmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdDrawDirectIndexed( pIndicesNum, pIndicesOffset, pBaseVertexIndex );
	}

	void CommandContextDirectGraphics::CmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdDrawDirectIndexedInstanced( pIndicesNumPerInstance, pInstancesNum, pIndicesOffset );
	}

	void CommandContextDirectGraphics::CmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdDrawDirectNonIndexed( pVerticesNum, pVerticesOffset );
	}

	void CommandContextDirectGraphics::CmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdDrawDirectNonIndexedInstanced( pVerticesNumPerInstance, pInstancesNum, pVerticesOffset );
	}


	bool CommandContextDeferred::MapBufferDeferred( GpuBuffer & pBuffer )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferred ) );
		return mCommandList->MapBuffer( pBuffer, EGpuMemoryMapMode::WriteAppend );
	}

	bool CommandContextDeferred::MapBufferRegionDeferred( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferred ) );
		return mCommandList->MapBufferRegion( pBuffer, pRegion, EGpuMemoryMapMode::WriteAppend );
	}

	bool CommandContextDeferred::UnmapBufferDeferred( GpuBuffer & pBuffer )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferred ) );
		return mCommandList->UnmapBuffer( pBuffer );
	}


	bool CommandContextDeferredGraphics::BeginRenderPass(
		const RenderPassConfigurationImmutableState & pRenderPassState,
		cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->BeginRenderPass( pRenderPassState, pFlags );
	}

	bool CommandContextDeferredGraphics::BeginRenderPass(
		const RenderPassConfigurationDynamicState & pRenderPassState,
		cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->BeginRenderPass( pRenderPassState, pFlags );
	}

	void CommandContextDeferredGraphics::EndRenderPass()
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->EndRenderPass();
	}

	bool CommandContextDeferredGraphics::CmdSetViewport( const ViewportDesc & pViewportDesc )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetViewport( pViewportDesc );
	}

	bool CommandContextDeferredGraphics::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->SetGraphicsPipelineStateObject( pGraphicsPSO );
	}

	bool CommandContextDeferredGraphics::CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetShaderConstant( pParamRefID, pData );
	}

	bool CommandContextDeferredGraphics::CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetShaderConstantBuffer( pParamRefID, pConstantBuffer );
	}

	bool CommandContextDeferredGraphics::CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetShaderTextureImage( pParamRefID, pTexture );
	}

	bool CommandContextDeferredGraphics::CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetShaderTextureSampler( pParamRefID, pSampler );
	}

	void CommandContextDeferredGraphics::CmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdDrawDirectIndexed( pIndicesNum, pIndicesOffset, pBaseVertexIndex );
	}

	void CommandContextDeferredGraphics::CmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset, EIndexDataFormat pIndexFormat )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdDrawDirectIndexedInstanced( pIndicesNumPerInstance, pInstancesNum, pIndicesOffset );
	}

	void CommandContextDeferredGraphics::CmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdDrawDirectNonIndexed( pVerticesNum, pVerticesOffset );
	}

	void CommandContextDeferredGraphics::CmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset )
	{
		ic3DebugAssert( CheckCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdDrawDirectNonIndexedInstanced( pVerticesNumPerInstance, pInstancesNum, pVerticesOffset );
	}

} // namespace Ic3::Graphics::GCI
