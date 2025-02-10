
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

		cppx::bitmask<ECommandObjectPropertyFlags> const mCommandFlags;

		CommandList(
			CommandSystem & pCommandSystem,
			ECommandListType pListType,
			GraphicsPipelineStateController & pPipelineStateController );

		virtual ~CommandList();

		CPPX_ATTR_NO_DISCARD bool CheckCommandClassSupport( ECommandQueueClass pQueueClass ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool CheckFeatureSupport( cppx::bitmask<ECommandObjectPropertyFlags> pCommandListFlags ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsRenderPassActive() const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasPendingGraphicsPipelineStateChanges() const noexcept;

		bool AcquireList();
		void ReleaseList();

		bool ApplyGraphicsPipelineStateChanges();

		virtual void BeginCommandSequence();
		virtual void EndCommandSequence();

		bool MapBuffer( GPUBuffer & pBuffer, EGPUMemoryMapMode pMapMode );
		bool MapBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode );
		bool UnmapBuffer( GPUBuffer & pBuffer );
		bool FlushMappedBuffer( GPUBuffer & pBuffer );
		bool FlushMappedBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );
		bool InvalidateBuffer( GPUBuffer & pBuffer );
		bool InvalidateBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );
		bool UpdateBufferDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferDataCopyDesc & pCopyDesc );
		bool UpdateBufferSubDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc );
		bool UpdateBufferDataUpload( GPUBuffer & pBuffer, const GPUBufferDataUploadDesc & pUploadDesc );
		bool UpdateBufferSubDataUpload( GPUBuffer & pBuffer, const GPUBufferSubDataUploadDesc & pUploadDesc );

		virtual bool BeginRenderPass(
				const RenderPassConfigurationStateDescriptor & pRenderPassState,
				cppx::bitmask<ECommandListActionFlags> pFlags );

		virtual bool BeginRenderPass(
				const RenderPassConfigurationDynamicState & pRenderPassState,
				cppx::bitmask<ECommandListActionFlags> pFlags );

		virtual void EndRenderPass();

		void SetRenderPassDynamicState( const GraphicsPipelineDynamicState & pDynamicState );
		void ResetRenderPassDynamicState();

		bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO );
		bool SetIAVertexStreamState( const IAVertexStreamStateDescriptor & pIAVertexStreamState );
		bool SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState );
		bool SetRenderTargetBindingState( const RenderTargetBindingStateDescriptor & pRenderTargetBindingState );
		bool SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState );

		bool CmdSetViewport( const ViewportDesc & pViewportDesc );
		bool CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData );
		bool CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer );
		bool CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture );
		bool CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler );

		virtual void CmdDispatchCompute( uint32 pThrGroupSizeX, uint32 pThrGroupSizeY, uint32 pThrGroupSizeZ ) {} // = 0;
		virtual void CmdDispatchComputeIndirect( uint32 pIndirectBufferOffset ) {} // = 0;

		virtual void CmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex ) = 0;
		virtual void CmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset ) = 0;
		virtual void CmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset ) = 0;
		virtual void CmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset ) = 0;

		virtual void CmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext ) = 0;

	private:
		bool OnBeginRenderPass( cppx::bitmask<ECommandListActionFlags> pFlags );
		void OnEndRenderPass();

	protected:
		std::atomic<bool> _listLockStatus = ATOMIC_VAR_INIT( false );

		cppx::bitmask<uint32> _internalStateMask;

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

		CPPX_ATTR_NO_DISCARD const RenderPassConfiguration & GetRenderPassConfiguration() const noexcept;

		virtual bool BeginRenderPass(
				const RenderPassConfigurationStateDescriptor & pRenderPassState,
				cppx::bitmask<ECommandListActionFlags> pFlags ) override;

		virtual bool BeginRenderPass(
				const RenderPassConfigurationDynamicState & pRenderPassState,
				cppx::bitmask<ECommandListActionFlags> pFlags ) override;

		virtual void EndRenderPass() override;

	protected:
		virtual void ExecuteRenderPassLoadActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState ) = 0;

		virtual void ExecuteRenderPassStoreActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState ) = 0;

	private:
		RenderPassConfiguration _currentRenderPassConfiguration;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMAND_LIST_H__
