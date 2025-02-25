
#include "MTLpipelineStateController.h"

namespace Ic3::Graphics::GCI
{

	MTLGraphicsPipelineStateController::MTLGraphicsPipelineStateController( MetalCommandList & pMetalCommandList )
	: mMetalCommandList( &pMetalCommandList )
	{}

	MTLGraphicsPipelineStateController::~MTLGraphicsPipelineStateController() = default;

	bool MTLGraphicsPipelineStateController::ApplyStateChanges()
	{}

	bool MTLGraphicsPipelineStateController::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject )
	{}

	bool MTLGraphicsPipelineStateController::ResetGraphicsPipelineStateObject()
	{}

	bool MTLGraphicsPipelineStateController::SetVertexSourceBindingDescriptor( const IAVertexStreamDynamicState & pVertexSourceBinding )
	{}

	bool MTLGraphicsPipelineStateController::SetVertexSourceBindingDescriptor( const VertexSourceBindingDescriptor & pVertexSourceBinding )
	{}

	bool MTLGraphicsPipelineStateController::ResetVertexSourceBindingDescriptor()
	{}

	bool MTLGraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState )
	{}

	bool MTLGraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingCompiledState & pRenderTargetBindingState )
	{}

	bool MTLGraphicsPipelineStateController::ResetRenderTargetBindingState()
	{}

	bool MTLGraphicsPipelineStateController::SetViewport( const ViewportDesc & pViewportDesc )
	{}

	bool MTLGraphicsPipelineStateController::SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{}

	bool MTLGraphicsPipelineStateController::SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{}

	bool MTLGraphicsPipelineStateController::SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{}

	bool MTLGraphicsPipelineStateController::SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{}
	
}
