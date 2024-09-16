
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMAND_CONTEXT_H__
#define __IC3_GRAPHICS_GCI_COMMAND_CONTEXT_H__

#include "CommonCommandDefs.h"
#include "Resources/GPUBufferCommon.h"

namespace Ic3::Graphics::GCI
{

	class RenderPassConfigurationDynamicState;
	class RenderPassConfigurationImmutableState;

	enum ECommandListActionFlags : uint32;

	class CommandContext : public GPUDeviceChildObject
	{
	public:
		CommandList * const mCommandList;

		CommandSystem * const mCommandSystem;

		ECommandContextType const mContextType;

		TBitmask<ECommandObjectPropertyFlags> const mCommandFlags;

	public:
		virtual ~CommandContext();

		IC3_ATTR_NO_DISCARD bool checkCommandClassSupport( ECommandQueueClass pQueueClass ) const;

		IC3_ATTR_NO_DISCARD bool checkFeatureSupport( TBitmask<ECommandObjectPropertyFlags> pCommandContextFlags ) const;

		void beginCommandSequence();
		void endCommandSequence();

		bool mapBuffer( GPUBuffer & pBuffer, EGPUMemoryMapMode pMapMode );
		bool mapBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode );
		bool unmapBuffer( GPUBuffer & pBuffer );
		bool flushMappedBuffer( GPUBuffer & pBuffer );
		bool flushMappedBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );

	protected:
		CommandContext( CommandList & pCommandList, ECommandContextType pContextType );

		bool checkCommandListSupport( TBitmask<ECommandObjectPropertyFlags> pCmdListFlags );
	};

	class CommandContextDirect : public CommandContext
	{
	public:
		virtual ~CommandContextDirect() = default;

		void submit();

		CommandSync submit( const CommandContextSubmitInfo & pSubmitInfo );

		void cmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext );

		bool invalidateBuffer( GPUBuffer & pBuffer );
		bool invalidateBufferRegion( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );

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

		bool updateBufferDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferDataCopyDesc & pCopyDesc );
		bool updateBufferSubDataCopy( GPUBuffer & pBuffer, GPUBuffer & pSourceBuffer, const GPUBufferSubDataCopyDesc & pCopyDesc );
		bool updateBufferDataUpload( GPUBuffer & pBuffer, const GPUBufferDataUploadDesc & pUploadDesc );
		bool updateBufferSubDataUpload( GPUBuffer & pBuffer, const GPUBufferSubDataUploadDesc & pUploadDesc );

		template <typename TData>
		void updateBufferDataUpload( GPUBuffer & pBuffer, const TData & pData )
		{
			GPUBufferDataUploadDesc dataUploadDesc;
			dataUploadDesc.flags = eGPUBufferDataCopyFlagModeInvalidateBit;
			dataUploadDesc.inputDataDesc.mPointer = &pData;
			dataUploadDesc.inputDataDesc.size = sizeof( TData );

			updateBufferDataUpload( pBuffer, dataUploadDesc );
		}

		template <typename TData>
		void updateBufferSubDataUpload( GPUBuffer & pBuffer, const TData & pData, gpu_memory_size_t pOffset )
		{
			GPUBufferSubDataUploadDesc subDataUploadDesc;
			subDataUploadDesc.flags = eGPUBufferDataCopyFlagModeInvalidateBit;
			subDataUploadDesc.inputDataDesc.mPointer = &pData;
			subDataUploadDesc.inputDataDesc.size = sizeof( pData );
			subDataUploadDesc.bufferRegion.offset = pOffset;
			subDataUploadDesc.bufferRegion.size = sizeof( TData );

			updateBufferSubDataUpload( pBuffer, subDataUploadDesc );
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

		void cmdDispatchCompute( uint32 pThrGroupSizeX, uint32 pThrGroupSizeY, uint32 pThrGroupSizeZ );
		void cmdDispatchComputeIndirect( uint32 pIndirectBufferOffset );

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
		
		bool beginRenderPass(
				const RenderPassConfigurationImmutableState & pRenderPassState,
				TBitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		bool beginRenderPass(
				const RenderPassConfigurationDynamicState & pRenderPassState,
				TBitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		void endRenderPass();

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

		void cmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex = 0 );
		void cmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset );
		void cmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset );
		void cmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset );
	};

	class CommandContextDeferred : public CommandContext
	{
	public:
		virtual ~CommandContextDeferred() = default;

		bool mapBufferDeferred( GPUBuffer & pBuffer );
		bool mapBufferRegionDeferred( GPUBuffer & pBuffer, const GPUMemoryRegion & pRegion );
		bool unmapBufferDeferred( GPUBuffer & pBuffer );

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

		bool beginRenderPass(
				const RenderPassConfigurationImmutableState & pRenderPassState,
				TBitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		bool beginRenderPass(
				const RenderPassConfigurationDynamicState & pRenderPassState,
				TBitmask<ECommandListActionFlags> pFlags = eCommandListActionFlagsDefault );

		void endRenderPass();

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

		void cmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex = 0 );
		void cmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset, EIndexDataFormat pIndexFormat );
		void cmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset );
		void cmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMAND_CONTEXT_H__
