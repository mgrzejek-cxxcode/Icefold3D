
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_COMMAND_LIST_H__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_COMMAND_LIST_H__

#include "VKPrerequisites.h"
#include <Ic3/Graphics/GCI/CommandList.h>

namespace Ic3::Graphics::GCI
{

	/// @brief
	class  VKCommandList : public CommandList
	{
		friend class VKCommandSystem;

	public:
		VKCommandList( VKCommandSystem & pVKCommandSystem, ECommandListType pListType );
		virtual ~VKCommandList();

		virtual void BeginCommandSequence() override;
		virtual void EndCommandSequence() override;

		virtual void executeDeferredContext( CommandContextDeferred & pDeferredContext ) override;

		virtual void clearRenderTarget( cppx::bitmask<ERenderTargetAttachmentFlags> pAttachmentMask ) override;
		virtual void SetViewport( const ViewportDesc & pViewportDesc ) override;
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineSO ) override;
		virtual bool SetVertexStreamStateObject( const VertexStreamStateObject & pVertexStreamSO ) override;
		virtual bool SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData ) override;
		virtual bool SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer ) override;
		virtual bool SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture ) override;
		virtual bool SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler ) override;
		virtual void drawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset ) override;
		virtual void drawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset ) override;
		virtual void drawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset ) override;
		virtual void drawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_COMMAND_LIST_H__
