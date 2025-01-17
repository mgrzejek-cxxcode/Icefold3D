
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMAND_CONTEXT_H__
#define __IC3_GRAPHICS_GCI_COMMAND_CONTEXT_H__

#include "CommonCommandDefs.h"
#include "Resources/GpuBufferCommon.h"

namespace Ic3::Graphics::GCI
{

	class RenderPassConfigurationDynamicState;
	class RenderPassConfigurationImmutableState;

	enum ECommandListActionFlags : uint32;

	class CommandContext : public GpuDeviceChildObject
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

		bool MapBuffer( GpuBuffer & pBuffer, EGpuMemoryMapMode pMapMode );
		bool MapBufferRegion( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode );
		bool UnmapBuffer( GpuBuffer & pBuffer );
		bool FlushMappedBuffer( GpuBuffer & pBuffer );
		bool FlushMappedBufferRegion( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion );

	protected:
		CommandContext( CommandList & pCommandList, ECommandContextType pContextType );

		bool CheckCommandListSupport( cppx::bitmask<ECommandObjectPropertyFlags> pCmdListFlags );
	};

	class CommandContextDirect : public CommandContext
	{
	public:
		virtual ~CommandContextDirect() = default;

		void Submit();

		CommandSync Submit( const CommandContextSubmitInfo & pSubmitInfo );

		void CmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext );

		bool InvalidateBuffer( GpuBuffer & pBuffer );
		bool InvalidateBufferRegion( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion );

	protected:
		CommandContextDirect( CommandList & pCommandList, ECommandContextType pContextType )
		: CommandContext( pCommandList, pContextType )
		{}
	};

	class CommandContextDirectTransfer : public CommandContextDirect
	{
	public:
		static ECommandContextType const sContextType = ECommandContextType::DirectTransfer;

		CommandContextDirectTransfer( CommandList & pCommandList )
		: CommandContextDirect( pCommandList, ECommandContextType::DirectTransfer )
		{}

		virtual ~CommandContextDirectTransfer() = default;

		bool UpdateBufferDataCopy( GpuBuffer & pBuffer, GpuBuffer & pSourceBuffer, const GpuBufferDataCopyDesc & pCopyDesc );
		bool UpdateBufferSubDataCopy( GpuBuffer & pBuffer, GpuBuffer & pSourceBuffer, const GpuBufferSubDataCopyDesc & pCopyDesc );
		bool UpdateBufferDataUpload( GpuBuffer & pBuffer, const GpuBufferDataUploadDesc & pUploadDesc );
		bool UpdateBufferSubDataUpload( GpuBuffer & pBuffer, const GpuBufferSubDataUploadDesc & pUploadDesc );

		template <typename TData>
		void UpdateBufferDataUpload( GpuBuffer & pBuffer, const TData & pData )
		{
			GpuBufferDataUploadDesc dataUploadDesc;
			dataUploadDesc.flags = eGpuBufferDataCopyFlagModeInvalidateBit;
			dataUploadDesc.inputDataDesc.pointer = &pData;
			dataUploadDesc.inputDataDesc.size = sizeof( TData );

			UpdateBufferDataUpload( pBuffer, dataUploadDesc );
		}

		template <typename TData>
		void UpdateBufferSubDataUpload( GpuBuffer & pBuffer, const TData & pData, gpu_memory_size_t pOffset )
		{
			GpuBufferSubDataUploadDesc subDataUploadDesc;
			subDataUploadDesc.flags = eGpuBufferDataCopyFlagModeInvalidateBit;
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

	class CommandContextDirectCompute : public CommandContextDirectTransfer
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

	class CommandContextDirectGraphics : public CommandContextDirectCompute
	{
	public:
		static ECommandContextType const sContextType = ECommandContextType::DirectGraphics;

		CommandContextDirectGraphics( CommandList & pCommandList )
		: CommandContextDirectCompute( pCommandList, ECommandContextType::DeferredGraphics )
		{}

		~CommandContextDirectGraphics() = default;
		
		bool BeginRenderPass(
				const RenderPassConfigurationImmutableState & pRenderPassState,
				cppx::bitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		bool BeginRenderPass(
				const RenderPassConfigurationDynamicState & pRenderPassState,
				cppx::bitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		void EndRenderPass();

		bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO );
		bool SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState );
		bool SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState );
		bool SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState );
		bool SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState );

		bool CmdSetViewport( const ViewportDesc & pViewportDesc );
		bool CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData );
		bool CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer );
		bool CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture );
		bool CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler );

		void CmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex = 0 );
		void CmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset );
		void CmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset );
		void CmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset );
	};

	class CommandContextDeferred : public CommandContext
	{
	public:
		virtual ~CommandContextDeferred() = default;

		bool MapBufferDeferred( GpuBuffer & pBuffer );
		bool MapBufferRegionDeferred( GpuBuffer & pBuffer, const GpuMemoryRegion & pRegion );
		bool UnmapBufferDeferred( GpuBuffer & pBuffer );

	protected:
		CommandContextDeferred( CommandList & pCommandList, ECommandContextType pContextType )
		: CommandContext( pCommandList, pContextType )
		{}

	};

	class CommandContextDeferredGraphics : public CommandContextDeferred
	{
	public:
		static ECommandContextType const sContextType = ECommandContextType::DirectTransfer;

		CommandContextDeferredGraphics( CommandList & pCommandList )
		: CommandContextDeferred( pCommandList, ECommandContextType::DeferredGraphics )
		{}

		virtual ~CommandContextDeferredGraphics() = default;

		bool BeginRenderPass(
				const RenderPassConfigurationImmutableState & pRenderPassState,
				cppx::bitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		bool BeginRenderPass(
				const RenderPassConfigurationDynamicState & pRenderPassState,
				cppx::bitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		void EndRenderPass();

		bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO );
		bool SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState );
		bool SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState );
		bool SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState );
		bool SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState );

		bool CmdSetViewport( const ViewportDesc & pViewportDesc );
		bool CmdSetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData );
		bool CmdSetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer );
		bool CmdSetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture );
		bool CmdSetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler );

		void CmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex = 0 );
		void CmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset, EIndexDataFormat pIndexFormat );
		void CmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset );
		void CmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMAND_CONTEXT_H__
