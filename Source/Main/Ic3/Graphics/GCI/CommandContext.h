
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMAND_CONTEXT_H__
#define __IC3_GRAPHICS_GCI_COMMAND_CONTEXT_H__

#include "CommonCommandDefs.h"
#include "Resources/GPUBufferCommon.h"

namespace Ic3::Graphics::GCI
{

	enum ECommandListActionFlags : uint32;

	class IC3_GRAPHICS_GCI_CLASS CommandContext : public GPUDeviceChildObject
	{
	public:
		CommandList * const mCommandList;

		CommandSystem * const mCommandSystem;

		ECommandContextType const mContextType;

		cppx::bitmask<ECommandObjectPropertyFlags> const mCommandFlags;

	public:
		virtual ~CommandContext();

		CPPX_ATTR_NO_DISCARD bool CheckCommandClassSupport( ECommandQueueClass pQueueClass ) const;

		CPPX_ATTR_NO_DISCARD bool CheckFeatureSupport( cppx::bitmask<ECommandObjectPropertyFlags> pCommandContextFlags ) const;

		void BeginCommandSequence();
		void EndCommandSequence();

		bool MapBuffer( GPUBuffer & pBuffer, EGPUMemoryMapMode pMapMode );
		bool MapBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode );
		bool UnmapBuffer( GPUBuffer & pBuffer );
		bool FlushMappedBuffer( GPUBuffer & pBuffer );
		bool FlushMappedBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );

	protected:
		CommandContext( CommandList & pCommandList, ECommandContextType pContextType );

		bool CheckCommandListSupport( cppx::bitmask<ECommandObjectPropertyFlags> pCmdListFlags );
	};

	class IC3_GRAPHICS_GCI_CLASS CommandContextDirect : public CommandContext
	{
	public:
		virtual ~CommandContextDirect() = default;

		void Submit();

		CommandSync Submit( const CommandContextSubmitInfo & pSubmitInfo );

		void CmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext );

		bool InvalidateBuffer( GPUBuffer & pBuffer );
		bool InvalidateBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );

	protected:
		CommandContextDirect( CommandList & pCommandList, ECommandContextType pContextType )
		: CommandContext( pCommandList, pContextType )
		{}
	};

	class IC3_GRAPHICS_GCI_CLASS CommandContextDirectTransfer : public CommandContextDirect
	{
	public:
		static ECommandContextType const sContextType = ECommandContextType::DirectTransfer;

		CommandContextDirectTransfer( CommandList & pCommandList )
		: CommandContextDirect( pCommandList, ECommandContextType::DirectTransfer )
		{}

		virtual ~CommandContextDirectTransfer() = default;

		bool UpdateBufferDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferDataCopyDesc & pCopyDesc );
		bool UpdateBufferSubDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc );
		bool UpdateBufferDataUpload( GPUBuffer & pBuffer, const GPUBufferDataUploadDesc & pUploadDesc );
		bool UpdateBufferSubDataUpload( GPUBuffer & pBuffer, const GPUBufferSubDataUploadDesc & pUploadDesc );

		template <typename TData>
		void UpdateBufferDataUpload( GPUBuffer & pBuffer, const TData & pData )
		{
			GPUBufferDataUploadDesc dataUploadDesc;
			dataUploadDesc.flags = eGPUBufferDataCopyFlagModeInvalidateBit;
			dataUploadDesc.inputDataDesc.pointer = &pData;
			dataUploadDesc.inputDataDesc.size = sizeof( TData );

			UpdateBufferDataUpload( pBuffer, dataUploadDesc );
		}

		template <typename TData>
		void UpdateBufferSubDataUpload( GPUBuffer & pBuffer, const TData & pData, gpu_memory_size_t pOffset )
		{
			GPUBufferSubDataUploadDesc subDataUploadDesc;
			subDataUploadDesc.flags = eGPUBufferDataCopyFlagModeInvalidateBit;
			subDataUploadDesc.inputDataDesc.pointer = &pData;
			subDataUploadDesc.inputDataDesc.size = sizeof( pData );
			subDataUploadDesc.bufferRegion.offset = pOffset;
			subDataUploadDesc.bufferRegion.size = sizeof( TData );

			UpdateBufferSubDataUpload( pBuffer, subDataUploadDesc );
		}

	protected:
		CommandContextDirectTransfer( CommandList & pCommandList, ECommandContextType pContextType )
		: CommandContextDirect( pCommandList, pContextType )
		{}
	};

	class IC3_GRAPHICS_GCI_CLASS CommandContextDirectCompute : public CommandContextDirectTransfer
	{
	public:
		static ECommandContextType const sContextType = ECommandContextType::DirectCompute;

		CommandContextDirectCompute( CommandList & pCommandList )
		: CommandContextDirectTransfer( pCommandList, ECommandContextType::DirectCompute )
		{}

		virtual ~CommandContextDirectCompute() = default;

		void CmdDispatchCompute( uint32 pThrGroupSizeX, uint32 pThrGroupSizeY, uint32 pThrGroupSizeZ );
		void CmdDispatchComputeIndirect( uint32 pIndirectBufferOffset );

	protected:
		CommandContextDirectCompute( CommandList & pCommandList, ECommandContextType pContextType )
		: CommandContextDirectTransfer( pCommandList, pContextType )
		{}
	};

	class IC3_GRAPHICS_GCI_CLASS CommandContextDirectGraphics : public CommandContextDirectCompute
	{
	public:
		static ECommandContextType const sContextType = ECommandContextType::DirectGraphics;

		CommandContextDirectGraphics( CommandList & pCommandList )
		: CommandContextDirectCompute( pCommandList, ECommandContextType::DeferredGraphics )
		{}

		~CommandContextDirectGraphics() = default;
		
		bool BeginRenderPass(
				const RenderPassDescriptor & pRenderPassDescriptor,
				cppx::bitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		bool BeginRenderPassDynamic(
				RenderPassDescriptorDynamic & pRenderPassDescriptor,
				cppx::bitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		void EndRenderPass();

		bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject );

		bool SetRenderTargetDescriptor( const RenderTargetDescriptor & pRenderTargetDescriptor );
		bool SetRenderTargetDescriptorDynamic( RenderTargetDescriptorDynamic & pRenderTargetDescriptor );

		bool SetVertexSourceBindingDescriptor( const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor );
		bool SetVertexSourceBindingDescriptorDynamic( VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor );

		void CmdSetDynamicBlendConstantColor( const cxm::rgba_color_r32_norm & pBlendConstantColor );
		void CmdSetDynamicRenderTargetClearConfig( const RenderTargetAttachmentClearConfig & pClearConfig );
		void CmdSetDynamicStencilTestRefValue( uint8 pStencilRefValue );
		void CmdResetDynamicPipelineConfig( cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> pConfigMask = eGraphicsPipelineDynamicConfigMaskAll );

		bool CmdSetViewport( const ViewportDesc & pViewportDesc );
		bool CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData );
		bool CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer );
		bool CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture );
		bool CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler );

		void CmdDrawDirectIndexed(
				native_uint pIndicesNum,
				native_uint pIndicesOffset,
				native_uint pBaseVertexIndex = 0 );
		void CmdDrawDirectIndexedInstanced(
				native_uint pIndicesNumPerInstance,
				native_uint pInstancesNum,
				native_uint pIndicesOffset );
		void CmdDrawDirectNonIndexed(
				native_uint pVerticesNum,
				native_uint pVerticesOffset );
		void CmdDrawDirectNonIndexedInstanced(
				native_uint pVerticesNumPerInstance,
				native_uint pInstancesNum,
				native_uint pVerticesOffset );
	};

	class IC3_GRAPHICS_GCI_CLASS CommandContextDeferred : public CommandContext
	{
	public:
		virtual ~CommandContextDeferred() = default;

		bool MapBufferDeferred( GPUBuffer & pBuffer );
		bool MapBufferRegionDeferred( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );
		bool UnmapBufferDeferred( GPUBuffer & pBuffer );

	protected:
		CommandContextDeferred( CommandList & pCommandList, ECommandContextType pContextType )
		: CommandContext( pCommandList, pContextType )
		{}

	};

	class IC3_GRAPHICS_GCI_CLASS CommandContextDeferredGraphics : public CommandContextDeferred
	{
	public:
		static ECommandContextType const sContextType = ECommandContextType::DirectTransfer;

		CommandContextDeferredGraphics( CommandList & pCommandList )
		: CommandContextDeferred( pCommandList, ECommandContextType::DeferredGraphics )
		{}

		virtual ~CommandContextDeferredGraphics() = default;

		bool BeginRenderPass(
				const RenderPassDescriptor & pRenderPassDescriptor,
				cppx::bitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		bool BeginRenderPassDynamic(
				RenderPassDescriptorDynamic & pRenderPassDescriptor,
				cppx::bitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		void EndRenderPass();

		bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject );

		bool SetRenderTargetDescriptor( const RenderTargetDescriptor & pRenderTargetDescriptor );
		bool SetRenderTargetDescriptorDynamic( RenderTargetDescriptorDynamic & pRenderTargetDescriptor );

		bool SetVertexSourceBindingDescriptor( const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor );
		bool SetVertexSourceBindingDescriptorDynamic( VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor );

		void CmdSetDynamicBlendConstantColor( const cxm::rgba_color_r32_norm & pBlendConstantColor );
		void CmdSetDynamicRenderTargetClearConfig( const RenderTargetAttachmentClearConfig & pClearConfig );
		void CmdSetDynamicStencilTestRefValue( uint8 pStencilRefValue );
		void CmdResetDynamicPipelineConfig( cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> pConfigMask = eGraphicsPipelineDynamicConfigMaskAll );

		bool CmdSetViewport( const ViewportDesc & pViewportDesc );
		bool CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData );
		bool CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer );
		bool CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture );
		bool CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler );

		void CmdDrawDirectIndexed(
			native_uint pIndicesNum,
			native_uint pIndicesOffset,
			native_uint pBaseVertexIndex = 0 );
		void CmdDrawDirectIndexedInstanced(
			native_uint pIndicesNumPerInstance,
			native_uint pInstancesNum,
			native_uint pIndicesOffset,
			EIndexDataFormat pIndexFormat );
		void CmdDrawDirectNonIndexed(
			native_uint pVerticesNum,
			native_uint pVerticesOffset );
		void CmdDrawDirectNonIndexedInstanced(
			native_uint pVerticesNumPerInstance,
			native_uint pInstancesNum,
			native_uint pVerticesOffset );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMAND_CONTEXT_H__
