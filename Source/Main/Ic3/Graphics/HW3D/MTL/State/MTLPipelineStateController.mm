
#include "MTLpipelineStateController.h"

namespace Ic3::Graphics::GCI
{

	MTLGraphicsPipelineStateController::MTLGraphicsPipelineStateController( MetalCommandList & pMetalCommandList )
	: mMetalCommandList( &pMetalCommandList )
	{}

	MTLGraphicsPipelineStateController::~MTLGraphicsPipelineStateController() = default;

	bool MTLGraphicsPipelineStateController::ApplyStateChanges()
	{}

	bool MTLGraphicsPipelineStateController::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{}

	bool MTLGraphicsPipelineStateController::ResetGraphicsPipelineStateObject()
	{}

	bool MTLGraphicsPipelineStateController::SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState )
	{}

	bool MTLGraphicsPipelineStateController::SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState )
	{}

	bool MTLGraphicsPipelineStateController::ResetIAVertexStreamState()
	{}

	bool MTLGraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState )
	{}

	bool MTLGraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState )
	{}

	bool MTLGraphicsPipelineStateController::ResetRenderTargetBindingState()
	{}

	bool MTLGraphicsPipelineStateController::SetViewport( const ViewportDesc & pViewportDesc )
	{}

	bool MTLGraphicsPipelineStateController::SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{}

	bool MTLGraphicsPipelineStateController::SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer )
	{}

	bool MTLGraphicsPipelineStateController::SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{}

	bool MTLGraphicsPipelineStateController::SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{}
	
}
