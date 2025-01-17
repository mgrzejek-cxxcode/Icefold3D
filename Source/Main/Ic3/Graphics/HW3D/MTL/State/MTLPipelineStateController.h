
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_STATE_CONTROLLER_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/SeparablePipelineState.h>

namespace Ic3::Graphics::GCI
{

	/// @brief
	class MTLGraphicsPipelineStateController : public GraphicsPipelineStateController
	{
	public:
		MetalCommandList * const mMetalCommandList;

	public:
		MTLGraphicsPipelineStateController( MetalCommandList & pMetalCommandList );
		~MTLGraphicsPipelineStateController();

		virtual bool ApplyStateChanges() override final;

		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO ) override;
		virtual bool ResetGraphicsPipelineStateObject() override;

		virtual bool SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState ) override;
		virtual bool SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState ) override;
		virtual bool ResetIAVertexStreamState() override;

		virtual bool SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState ) override;
		virtual bool SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState ) override;
		virtual bool ResetRenderTargetBindingState() override;

		virtual bool SetViewport( const ViewportDesc & pViewportDesc ) override;
		virtual bool SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData ) override;
		virtual bool SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer ) override;
		virtual bool SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture ) override;
		virtual bool SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_STATE_CONTROLLER_H__
