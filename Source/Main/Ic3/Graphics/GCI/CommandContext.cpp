
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
	, mCommandFlags( CXU::GetCommandObjectPropertyFlags( pContextType ) )
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

	bool CommandContext::MapBuffer( GPUBuffer & pBuffer, EGPUMemoryMapMode pMapMode )
	{
		return mCommandList->MapBuffer( pBuffer, pMapMode );
	}

	bool CommandContext::MapBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode )
	{
		return mCommandList->MapBufferRegion( pBuffer, pRegion, pMapMode );
	}

	bool CommandContext::UnmapBuffer( GPUBuffer & pBuffer )
	{
		return mCommandList->UnmapBuffer( pBuffer );
	}

	bool CommandContext::FlushMappedBuffer( GPUBuffer & pBuffer )
	{
		return mCommandList->FlushMappedBuffer( pBuffer );
	}

	bool CommandContext::FlushMappedBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion )
	{
		return mCommandList->FlushMappedBufferRegion( pBuffer, pRegion );
	}

	bool CommandContext::CheckCommandListSupport( cppx::bitmask<ECommandObjectPropertyFlags> pCmdListFlags )
	{
		return mCommandList->CheckFeatureSupport( pCmdListFlags );
	}


	void CommandContextDirect::Submit()
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirect ) );
		mCommandSystem->SubmitContext( *this, cxCommandContextSubmitDefault );
	}

	CommandSync CommandContextDirect::Submit( const CommandContextSubmitInfo & pSubmitInfo )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandSystem->SubmitContext( *this, pSubmitInfo );
	}

	void CommandContextDirect::CmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandList->CmdExecuteDeferredContext( pDeferredContext );
	}

	bool CommandContextDirect::InvalidateBuffer( GPUBuffer & pBuffer )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandList->InvalidateBuffer( pBuffer );
	}

	bool CommandContextDirect::InvalidateBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirect ) );
		return mCommandList->InvalidateBufferRegion( pBuffer, pRegion );
	}


	bool CommandContextDirectTransfer::UpdateBufferDataCopy(
			GPUBuffer & pBuffer,
			GPUBuffer & pSourceBuffer,
			const GPUBufferDataCopyDesc & pCopyDesc )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->UpdateBufferDataCopy( pBuffer, pSourceBuffer, pCopyDesc );
	}

	bool CommandContextDirectTransfer::UpdateBufferSubDataCopy(
			GPUBuffer & pBuffer,
			GPUBuffer & pSourceBuffer,
			const GPUBufferSubDataCopyDesc & pCopyDesc )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->UpdateBufferSubDataCopy( pBuffer, pSourceBuffer, pCopyDesc );
	}

	bool CommandContextDirectTransfer::UpdateBufferDataUpload( GPUBuffer & pBuffer, const GPUBufferDataUploadDesc & pUploadDesc )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->UpdateBufferDataUpload( pBuffer, pUploadDesc );
	}

	bool CommandContextDirectTransfer::UpdateBufferSubDataUpload( GPUBuffer & pBuffer, const GPUBufferSubDataUploadDesc & pUploadDesc )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectTransfer ) );
		return mCommandList->UpdateBufferSubDataUpload( pBuffer, pUploadDesc );
	}


	void CommandContextDirectCompute::CmdDispatchCompute( uint32 pThrGroupSizeX, uint32 pThrGroupSizeY, uint32 pThrGroupSizeZ )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectCompute ) );
		return mCommandList->CmdDispatchCompute( pThrGroupSizeX, pThrGroupSizeY, pThrGroupSizeZ );
	}

	void CommandContextDirectCompute::CmdDispatchComputeIndirect( uint32 pIndirectBufferOffset )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectCompute ) );
		return mCommandList->CmdDispatchComputeIndirect( pIndirectBufferOffset );
	}


	bool CommandContextDirectGraphics::BeginRenderPass(
			const RenderPassDescriptor & pRenderPassDescriptor,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->BeginRenderPass( pRenderPassDescriptor, pFlags );
	}

	bool CommandContextDirectGraphics::BeginRenderPassDynamic(
			RenderPassDescriptorDynamic & pRenderPassDescriptor,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->BeginRenderPassDynamic( pRenderPassDescriptor, pFlags );
	}

	void CommandContextDirectGraphics::EndRenderPass()
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->EndRenderPass();
	}

	bool CommandContextDirectGraphics::CmdSetViewport( const ViewportDesc & pViewportDesc )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetViewport( pViewportDesc );
	}

	bool CommandContextDirectGraphics::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPipelineStateObject )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetGraphicsPipelineStateObject( pGraphicsPipelineStateObject );
	}

	bool CommandContextDirectGraphics::SetRenderTargetDescriptor(
			const RenderTargetDescriptor & pRenderTargetDescriptor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetRenderTargetDescriptor( pRenderTargetDescriptor );
	}

	bool CommandContextDirectGraphics::SetRenderTargetDescriptorDynamic(
			RenderTargetDescriptorDynamic & pRenderTargetDescriptor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetRenderTargetDescriptorDynamic( pRenderTargetDescriptor );
	}

	bool CommandContextDirectGraphics::SetVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetVertexSourceBindingDescriptor( pVertexSourceBindingDescriptor );
	}

	bool CommandContextDirectGraphics::SetVertexSourceBindingDescriptorDynamic(
			VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->SetVertexSourceBindingDescriptorDynamic( pVertexSourceBindingDescriptor );
	}

	void CommandContextDirectGraphics::CmdSetDynamicBlendConstantColor( const cxm::rgba_color_r32_norm & pBlendConstantColor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetDynamicBlendConstantColor( pBlendConstantColor );
	}

	void CommandContextDirectGraphics::CmdSetDynamicRenderTargetClearConfig( const RenderTargetAttachmentClearConfig & pClearConfig )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetDynamicRenderTargetClearConfig( pClearConfig );
	}

	void CommandContextDirectGraphics::CmdSetDynamicStencilTestRefValue( uint8 pStencilRefValue )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetDynamicStencilTestRefValue( pStencilRefValue );
	}

	void CommandContextDirectGraphics::CmdResetDynamicPipelineConfig( cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> pConfigMask )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdResetDynamicPipelineConfig( pConfigMask );
	}

	bool CommandContextDirectGraphics::CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetShaderConstant( pParamRefID, pData );
	}

	bool CommandContextDirectGraphics::CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetShaderConstantBuffer( pParamRefID, pConstantBuffer );
	}

	bool CommandContextDirectGraphics::CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetShaderTextureImage( pParamRefID, pTexture );
	}

	bool CommandContextDirectGraphics::CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdSetShaderTextureSampler( pParamRefID, pSampler );
	}

	void CommandContextDirectGraphics::CmdDrawDirectIndexed(
			native_uint pIndicesNum,
			native_uint pIndicesOffset,
			native_uint pBaseVertexIndex )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdDrawDirectIndexed( pIndicesNum, pIndicesOffset, pBaseVertexIndex );
	}

	void CommandContextDirectGraphics::CmdDrawDirectIndexedInstanced(
			native_uint pIndicesNumPerInstance,
			native_uint pInstancesNum,
			native_uint pIndicesOffset )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdDrawDirectIndexedInstanced( pIndicesNumPerInstance, pInstancesNum, pIndicesOffset );
	}

	void CommandContextDirectGraphics::CmdDrawDirectNonIndexed(
			native_uint pVerticesNum,
			native_uint pVerticesOffset )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdDrawDirectNonIndexed( pVerticesNum, pVerticesOffset );
	}

	void CommandContextDirectGraphics::CmdDrawDirectNonIndexedInstanced(
			native_uint pVerticesNumPerInstance,
			native_uint pInstancesNum,
			native_uint pVerticesOffset )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		return mCommandList->CmdDrawDirectNonIndexedInstanced( pVerticesNumPerInstance, pInstancesNum, pVerticesOffset );
	}


	bool CommandContextDeferred::MapBufferDeferred( GPUBuffer & pBuffer )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferred ) );
		return mCommandList->MapBuffer( pBuffer, EGPUMemoryMapMode::WriteAppend );
	}

	bool CommandContextDeferred::MapBufferRegionDeferred( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferred ) );
		return mCommandList->MapBufferRegion( pBuffer, pRegion, EGPUMemoryMapMode::WriteAppend );
	}

	bool CommandContextDeferred::UnmapBufferDeferred( GPUBuffer & pBuffer )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferred ) );
		return mCommandList->UnmapBuffer( pBuffer );
	}


	bool CommandContextDeferredGraphics::BeginRenderPass(
			const RenderPassDescriptor & pRenderPassDescriptor,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->BeginRenderPass( pRenderPassDescriptor, pFlags );
	}


	bool CommandContextDeferredGraphics::BeginRenderPassDynamic(
			RenderPassDescriptorDynamic & pRenderPassDescriptor,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->BeginRenderPassDynamic( pRenderPassDescriptor, pFlags );
	}

	void CommandContextDeferredGraphics::EndRenderPass()
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->EndRenderPass();
	}

	bool CommandContextDeferredGraphics::CmdSetViewport( const ViewportDesc & pViewportDesc )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetViewport( pViewportDesc );
	}

	bool CommandContextDeferredGraphics::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPipelineStateObject )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->SetGraphicsPipelineStateObject( pGraphicsPipelineStateObject );
	}

	bool CommandContextDeferredGraphics::SetRenderTargetDescriptor(
			const RenderTargetDescriptor & pRenderTargetDescriptor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->SetRenderTargetDescriptor( pRenderTargetDescriptor );
	}

	bool CommandContextDeferredGraphics::SetRenderTargetDescriptorDynamic(
			RenderTargetDescriptorDynamic & pRenderTargetDescriptor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->SetRenderTargetDescriptorDynamic( pRenderTargetDescriptor );
	}

	bool CommandContextDeferredGraphics::SetVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->SetVertexSourceBindingDescriptor( pVertexSourceBindingDescriptor );
	}

	bool CommandContextDeferredGraphics::SetVertexSourceBindingDescriptorDynamic(
			VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->SetVertexSourceBindingDescriptorDynamic( pVertexSourceBindingDescriptor );
	}

	void CommandContextDeferredGraphics::CmdSetDynamicBlendConstantColor( const cxm::rgba_color_r32_norm & pBlendConstantColor )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetDynamicBlendConstantColor( pBlendConstantColor );
	}

	void CommandContextDeferredGraphics::CmdSetDynamicRenderTargetClearConfig( const RenderTargetAttachmentClearConfig & pClearConfig )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetDynamicRenderTargetClearConfig( pClearConfig );
	}

	void CommandContextDeferredGraphics::CmdSetDynamicStencilTestRefValue( uint8 pStencilRefValue )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetDynamicStencilTestRefValue( pStencilRefValue );
	}

	void CommandContextDeferredGraphics::CmdResetDynamicPipelineConfig( cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> pConfigMask )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdResetDynamicPipelineConfig( pConfigMask );
	}

	bool CommandContextDeferredGraphics::CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetShaderConstant( pParamRefID, pData );
	}

	bool CommandContextDeferredGraphics::CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetShaderConstantBuffer( pParamRefID, pConstantBuffer );
	}

	bool CommandContextDeferredGraphics::CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetShaderTextureImage( pParamRefID, pTexture );
	}

	bool CommandContextDeferredGraphics::CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdSetShaderTextureSampler( pParamRefID, pSampler );
	}

	void CommandContextDeferredGraphics::CmdDrawDirectIndexed(
			native_uint pIndicesNum,
			native_uint pIndicesOffset,
			native_uint pBaseVertexIndex )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdDrawDirectIndexed( pIndicesNum, pIndicesOffset, pBaseVertexIndex );
	}

	void CommandContextDeferredGraphics::CmdDrawDirectIndexedInstanced(
			native_uint pIndicesNumPerInstance,
			native_uint pInstancesNum,
			native_uint pIndicesOffset,
			EIndexDataFormat pIndexFormat )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdDrawDirectIndexedInstanced( pIndicesNumPerInstance, pInstancesNum, pIndicesOffset );
	}

	void CommandContextDeferredGraphics::CmdDrawDirectNonIndexed(
			native_uint pVerticesNum,
			native_uint pVerticesOffset )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdDrawDirectNonIndexed( pVerticesNum, pVerticesOffset );
	}

	void CommandContextDeferredGraphics::CmdDrawDirectNonIndexedInstanced(
			native_uint pVerticesNumPerInstance,
			native_uint pInstancesNum,
			native_uint pVerticesOffset )
	{
		Ic3DebugAssert( CheckCommandListSupport( eCommandObjectPropertyMaskContextFamilyDeferredGraphics ) );
		return mCommandList->CmdDrawDirectNonIndexedInstanced( pVerticesNumPerInstance, pInstancesNum, pVerticesOffset );
	}

} // namespace Ic3::Graphics::GCI
