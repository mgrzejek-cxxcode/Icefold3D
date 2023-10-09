
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

		virtual void beginCommandSequence() override;
		virtual void endCommandSequence() override;

		virtual void executeDeferredContext( CommandContextDeferred & pDeferredContext ) override;

		virtual void clearRenderTarget( Bitmask<ERenderTargetAttachmentFlags> pAttachmentMask ) override;
		virtual void setViewport( const ViewportDesc & pViewportDesc ) override;
		virtual bool setGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineSO ) override;
		virtual bool setVertexStreamStateObject( const VertexStreamStateObject & pVertexStreamSO ) override;
		virtual bool setShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData ) override;
		virtual bool setShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer ) override;
		virtual bool setShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture ) override;
		virtual bool setShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler ) override;
		virtual void drawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset ) override;
		virtual void drawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset ) override;
		virtual void drawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset ) override;
		virtual void drawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_COMMAND_LIST_H__
