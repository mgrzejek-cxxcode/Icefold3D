
#include "CommandContext.h"
#include "CommandList.h"
#include "CommandSystem.h"
#include "GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	CommandContext::CommandContext( CommandList & pCommandList, ECommandContextType pContextType )
	: GPUDeviceChildObject( pCommandList.mGPUDevice )
	, mCommandList( &pCommandList )
	, mCommandSystem( mCommandList->mCommandSystem )
	, mContextType( pContextType )
	, mCommandFlags( CxDef::getCommandObjectPropertyFlags( pContextType ) )
	{}

	CommandContext::~CommandContext() = default;

	bool CommandContext::checkCommandClassSupport( ECommandQueueClass pQueueClass ) const
	{
		return mCommandList->checkCommandClassSupport( pQueueClass );
	}

	bool CommandContext::checkFeatureSupport( TBitmask<ECommandObjectPropertyFlags> pCommandContextFlags ) const
	{
		return mCommandList->checkFeatureSupport( pCommandContextFlags );
	}

	void CommandContext::beginCommandSequence()
	{
		return mCommandList->beginCommandSequence();
	}

	void CommandContext::endCommandSequence()
	{
		return mCommandList->endCommandSequence();
	}

	bool CommandContext::mapBuffer( GPUBuffer & pBuffer, EGPUMemoryMapMode pMapMode )
	{
		return mCommandList->mapBuffer( pBuffer, pMapMode );
	}

	bool CommandContext::mapBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode )
	{
		return mCommandList->mapBufferRegion( pBuffer, pRegion, pMapMode );
	}

	bool CommandContext::unmapBuffer( GPUBuffer & pBuffer )
	{
		return mCommandList->unmapBuffer( pBuffer );
	}

	bool CommandContext::flushMappedBuffer( GPUBuffer & pBuffer )
	{
		return mCommandList->flushMappedBuffer( pBuffer );
	}

	bool CommandContext::flushMappedBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion )
	{
		return mCommandList->flushMappedBufferRegion( pBuffer, pRegion );
	}

	bool CommandContext::checkCommandListSupport( TBitmask<ECommandObjectPropertyFlags> pCmdListFlags )
	{
		return mCommandList->checkFeatureSupport( pCmdListFlags );
	}


	void CommandContextDirect::submit()
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		mCommandSystem->submitContext( *this, cxCommandContextSubmitDefault );
	}

	CommandSync CommandContextDirect::submit( const CommandContextSubmitInfo & pSubmitInfo )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandSystem->submitContext( *this, pSubmitInfo );
	}

	void CommandContextDirect::cmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandList->cmdExecuteDeferredContext( pDeferredContext );
	}

	bool CommandContextDirect::invalidateBuffer( GPUBuffer & pBuffer )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandList->invalidateBuffer( pBuffer );
	}

	bool CommandContextDirect::invalidateBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandList->invalidateBufferRegion( pBuffer, pRegion );
	}


	bool CommandContextDirectTransfer::updateBufferDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferDataCopyDesc & pCopyDesc )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->updateBufferDataCopy( pBuffer, pSourceBuffer, pCopyDesc );
	}

	bool CommandContextDirectTransfer::updateBufferSubDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->updateBufferSubDataCopy( pBuffer, pSourceBuffer, pCopyDesc );
	}

	bool CommandContextDirectTransfer::updateBufferDataUpload( GPUBuffer & pBuffer, const GPUBufferDataUploadDesc & pUploadDesc )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->updateBufferDataUpload( pBuffer, pUploadDesc );
	}

	bool CommandContextDirectTransfer::updateBufferSubDataUpload( GPUBuffer & pBuffer, const GPUBufferSubDataUploadDesc & pUploadDesc )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->updateBufferSubDataUpload( pBuffer, pUploadDesc );
	}


	void CommandContextDirectCompute::cmdDispatchCompute( uint32 pThrGroupSizeX, uint32 pThrGroupSizeY, uint32 pThrGroupSizeZ )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectCompute ) );
		return mCommandList->cmdDispatchCompute( pThrGroupSizeX, pThrGroupSizeY, pThrGroupSizeZ );
	}

	void CommandContextDirectCompute::cmdDispatchComputeIndirect( uint32 pIndirectBufferOffset )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectCompute ) );
		return mCommandList->cmdDispatchComputeIndirect( pIndirectBufferOffset );
	}


	bool CommandContextDirectGraphics::beginRenderPass(
		const RenderPassConfigurationImmutableState & pRenderPassState,
		TBitmask<ECommandListActionFlags> pFlags )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->beginRenderPass( pRenderPassState, pFlags );
	}

	bool CommandContextDirectGraphics::beginRenderPass(
		const RenderPassConfigurationDynamicState & pRenderPassState,
		TBitmask<ECommandListActionFlags> pFlags )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->beginRenderPass( pRenderPassState, pFlags );
	}

	void CommandContextDirectGraphics::endRenderPass()
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->endRenderPass();
	}

	bool CommandContextDirectGraphics::cmdSetViewport( const ViewportDesc & pViewportDesc )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->cmdSetViewport( pViewportDesc );
	}

	bool CommandContextDirectGraphics::setGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->setGraphicsPipelineStateObject( pGraphicsPSO );
	}

	bool CommandContextDirectGraphics::setIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->setIAVertexStreamState( pIAVertexStreamState );
	}

	bool CommandContextDirectGraphics::setIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->setIAVertexStreamState( pIAVertexStreamState );
	}

	bool CommandContextDirectGraphics::setRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->setRenderTargetBindingState( pRenderTargetBindingState );
	}

	bool CommandContextDirectGraphics::setRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->setRenderTargetBindingState( pRenderTargetBindingState );
	}

	bool CommandContextDirectGraphics::cmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->cmdSetShaderConstant( pParamRefID, pData );
	}

	bool CommandContextDirectGraphics::cmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->cmdSetShaderConstantBuffer( pParamRefID, pConstantBuffer );
	}

	bool CommandContextDirectGraphics::cmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->cmdSetShaderTextureImage( pParamRefID, pTexture );
	}

	bool CommandContextDirectGraphics::cmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->cmdSetShaderTextureSampler( pParamRefID, pSampler );
	}

	void CommandContextDirectGraphics::cmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->cmdDrawDirectIndexed( pIndicesNum, pIndicesOffset, pBaseVertexIndex );
	}

	void CommandContextDirectGraphics::cmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->cmdDrawDirectIndexedInstanced( pIndicesNumPerInstance, pInstancesNum, pIndicesOffset );
	}

	void CommandContextDirectGraphics::cmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->cmdDrawDirectNonIndexed( pVerticesNum, pVerticesOffset );
	}

	void CommandContextDirectGraphics::cmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->cmdDrawDirectNonIndexedInstanced( pVerticesNumPerInstance, pInstancesNum, pVerticesOffset );
	}


	bool CommandContextDeferred::mapBufferDeferred( GPUBuffer & pBuffer )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferred ) );
		return mCommandList->mapBuffer( pBuffer, EGPUMemoryMapMode::WriteAppend );
	}

	bool CommandContextDeferred::mapBufferRegionDeferred( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferred ) );
		return mCommandList->mapBufferRegion( pBuffer, pRegion, EGPUMemoryMapMode::WriteAppend );
	}

	bool CommandContextDeferred::unmapBufferDeferred( GPUBuffer & pBuffer )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferred ) );
		return mCommandList->unmapBuffer( pBuffer );
	}


	bool CommandContextDeferredGraphics::beginRenderPass(
		const RenderPassConfigurationImmutableState & pRenderPassState,
		TBitmask<ECommandListActionFlags> pFlags )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->beginRenderPass( pRenderPassState, pFlags );
	}

	bool CommandContextDeferredGraphics::beginRenderPass(
		const RenderPassConfigurationDynamicState & pRenderPassState,
		TBitmask<ECommandListActionFlags> pFlags )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->beginRenderPass( pRenderPassState, pFlags );
	}

	void CommandContextDeferredGraphics::endRenderPass()
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->endRenderPass();
	}

	bool CommandContextDeferredGraphics::cmdSetViewport( const ViewportDesc & pViewportDesc )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->cmdSetViewport( pViewportDesc );
	}

	bool CommandContextDeferredGraphics::setGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->setGraphicsPipelineStateObject( pGraphicsPSO );
	}

	bool CommandContextDeferredGraphics::cmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->cmdSetShaderConstant( pParamRefID, pData );
	}

	bool CommandContextDeferredGraphics::cmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->cmdSetShaderConstantBuffer( pParamRefID, pConstantBuffer );
	}

	bool CommandContextDeferredGraphics::cmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->cmdSetShaderTextureImage( pParamRefID, pTexture );
	}

	bool CommandContextDeferredGraphics::cmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->cmdSetShaderTextureSampler( pParamRefID, pSampler );
	}

	void CommandContextDeferredGraphics::cmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->cmdDrawDirectIndexed( pIndicesNum, pIndicesOffset, pBaseVertexIndex );
	}

	void CommandContextDeferredGraphics::cmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset, EIndexDataFormat pIndexFormat )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->cmdDrawDirectIndexedInstanced( pIndicesNumPerInstance, pInstancesNum, pIndicesOffset );
	}

	void CommandContextDeferredGraphics::cmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->cmdDrawDirectNonIndexed( pVerticesNum, pVerticesOffset );
	}

	void CommandContextDeferredGraphics::cmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset )
	{
		ic3DebugAssert( checkCommandListSupport( ECommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->cmdDrawDirectNonIndexedInstanced( pVerticesNumPerInstance, pInstancesNum, pVerticesOffset );
	}

} // namespace Ic3::Graphics::GCI
