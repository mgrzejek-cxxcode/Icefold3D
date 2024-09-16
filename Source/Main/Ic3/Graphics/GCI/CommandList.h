
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMAND_LIST_H__
#define __IC3_GRAPHICS_GCI_COMMAND_LIST_H__

#include "CommonCommandDefs.h"
#include "Resources/GPUBufferCommon.h"
#include "State/SamplerCommon.h"
#include "State/RenderPassCommon.h"

namespace Ic3::Graphics::GCI
{

	class GraphicsPipelineStateController;

	class IC3_GRAPHICS_GCI_CLASS CommandList : public GPUDeviceChildObject
	{
	public:
		CommandSystem * const mCommandSystem;

		ECommandListType const mListType;

		TBitmask<ECommandObjectPropertyFlags> const mCommandFlags;

		CommandList(
			CommandSystem & pCommandSystem,
			ECommandListType pListType,
			GraphicsPipelineStateController & pPipelineStateController );

		virtual ~CommandList();

		IC3_ATTR_NO_DISCARD bool checkCommandClassSupport( ECommandQueueClass pQueueClass ) const noexcept;

		IC3_ATTR_NO_DISCARD bool checkFeatureSupport( TBitmask<ECommandObjectPropertyFlags> pCommandListFlags ) const noexcept;

		IC3_ATTR_NO_DISCARD bool isRenderPassActive() const noexcept;

		IC3_ATTR_NO_DISCARD bool hasPendingGraphicsPipelineStateChanges() const noexcept;

		bool acquireList();
		void releaseList();

		bool applyGraphicsPipelineStateChanges();

		virtual void beginCommandSequence();
		virtual void endCommandSequence();

		bool mapBuffer( GPUBuffer & pBuffer, EGPUMemoryMapMode pMapMode );
		bool mapBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode );
		bool unmapBuffer( GPUBuffer & pBuffer );
		bool flushMappedBuffer( GPUBuffer & pBuffer );
		bool flushMappedBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );
		bool invalidateBuffer( GPUBuffer & pBuffer );
		bool invalidateBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );
		bool updateBufferDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferDataCopyDesc & pCopyDesc );
		bool updateBufferSubDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc );
		bool updateBufferDataUpload( GPUBuffer & pBuffer, const GPUBufferDataUploadDesc & pUploadDesc );
		bool updateBufferSubDataUpload( GPUBuffer & pBuffer, const GPUBufferSubDataUploadDesc & pUploadDesc );

		virtual bool beginRenderPass(
				const RenderPassConfigurationImmutableState & pRenderPassState,
				TBitmask<ECommandListActionFlags> pFlags );

		virtual bool beginRenderPass(
				const RenderPassConfigurationDynamicState & pRenderPassState,
				TBitmask<ECommandListActionFlags> pFlags );

		virtual void endRenderPass();

		void setRenderPassDynamicState( const GraphicsPipelineDynamicState & pDynamicState );
		void resetRenderPassDynamicState();

		bool setGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO );
		bool setIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState );
		bool setIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState );
		bool setRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState );
		bool setRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState );

		bool cmdSetViewport( const ViewportDesc & pViewportDesc );
		bool cmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData );
		bool cmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer );
		bool cmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture );
		bool cmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler );

		virtual void cmdDispatchCompute( uint32 pThrGroupSizeX, uint32 pThrGroupSizeY, uint32 pThrGroupSizeZ ) {} // = 0;
		virtual void cmdDispatchComputeIndirect( uint32 pIndirectBufferOffset ) {} // = 0;

		virtual void cmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex ) = 0;
		virtual void cmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset ) = 0;
		virtual void cmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset ) = 0;
		virtual void cmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset ) = 0;

		virtual void cmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext ) = 0;

	private:
		bool onBeginRenderPass( TBitmask<ECommandListActionFlags> pFlags );
		void onEndRenderPass();

	protected:
		std::atomic<bool> _listLockStatus = ATOMIC_VAR_INIT( false );

		TBitmask<uint32> _internalStateMask;

		GraphicsPipelineStateController * _graphicsPipelineStateController = nullptr;
	};

	class IC3_GRAPHICS_GCI_CLASS CommandListRenderPassDefault : public CommandList
	{
	public:
		CommandListRenderPassDefault(
				CommandSystem & pCommandSystem,
		ECommandListType pListType,
				GraphicsPipelineStateController & pPipelineStateController );

		virtual ~CommandListRenderPassDefault();

		IC3_ATTR_NO_DISCARD const RenderPassConfiguration & getRenderPassConfiguration() const noexcept;

		virtual bool beginRenderPass(
				const RenderPassConfigurationImmutableState & pRenderPassState,
				TBitmask<ECommandListActionFlags> pFlags ) override;

		virtual bool beginRenderPass(
				const RenderPassConfigurationDynamicState & pRenderPassState,
				TBitmask<ECommandListActionFlags> pFlags ) override;

		virtual void endRenderPass() override;

	protected:
		virtual void executeRenderPassLoadActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState ) = 0;

		virtual void executeRenderPassStoreActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState ) = 0;

	private:
		RenderPassConfiguration _currentRenderPassConfiguration;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMAND_LIST_H__
