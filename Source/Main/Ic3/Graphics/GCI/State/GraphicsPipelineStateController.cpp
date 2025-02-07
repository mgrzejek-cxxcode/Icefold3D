
#include "GraphicsPipelineStateController.h"
#include "GraphicsPipelineStateCommon.h"
#include "GraphicsPipelineStateDescriptorIA.h"
#include "RenderTargetStateDescriptors.h"

#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <cppx/memory.h>

namespace Ic3::Graphics::GCI
{

	GraphicsPipelineStateController::GraphicsPipelineStateController() = default;

	GraphicsPipelineStateController::~GraphicsPipelineStateController() = default;

	bool GraphicsPipelineStateController::IAIsVertexStreamStateDynamic() const noexcept
	{
		return _currentCommonState.iaVertexStreamState && _currentCommonState.iaVertexStreamState->IsDynamicDescriptor();
	}

	bool GraphicsPipelineStateController::IsRenderTargetStateDynamic() const noexcept
	{
		return _currentCommonState.renderTargetBindingState && _currentCommonState.renderTargetBindingState->IsDynamicDescriptor();
	}

	const GraphicsPipelineDynamicState & GraphicsPipelineStateController::GetRenderPassDynamicState() const noexcept
	{
		return _currentRenderPassDynamicState;
	}

	const ShaderRootSignature & GraphicsPipelineStateController::GetShaderRootSignature() const noexcept
	{
		Ic3DebugAssert( _currentCommonState.graphicsPSO );
		return _currentCommonState.graphicsPSO->mShaderRootSignature;
	}

	void GraphicsPipelineStateController::SetRenderPassDynamicState( const GraphicsPipelineDynamicState & pDynamicState )
	{
		_currentRenderPassDynamicState = pDynamicState;
	}

	void GraphicsPipelineStateController::ResetRenderPassDynamicState()
	{
		_currentRenderPassDynamicState.activeStateMask.clear();
	}

	bool GraphicsPipelineStateController::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		if( _currentCommonState.graphicsPSO != &pGraphicsPSO )
		{
			_currentCommonState.graphicsPSO = &pGraphicsPSO;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonPSOBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPSOBit );
	}

	bool GraphicsPipelineStateController::ResetGraphicsPipelineStateObject()
	{
		if( _currentCommonState.graphicsPSO )
		{
			_currentCommonState.graphicsPSO = nullptr;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonPSOBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPSOBit );
	}

	bool GraphicsPipelineStateController::SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState )
	{
		_currentCommonState.iaVertexStreamState = &( IAVertexStreamStateDescriptor::GetDynamicOverrideState() );
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexStreamBit );
		return true;
	}

	bool GraphicsPipelineStateController::SetIAVertexStreamState( const IAVertexStreamStateDescriptor & pIAVertexStreamState )
	{
		if( _currentCommonState.iaVertexStreamState != &pIAVertexStreamState )
		{
			_currentCommonState.iaVertexStreamState = &pIAVertexStreamState;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexStreamBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonVertexStreamBit );
	}

	bool GraphicsPipelineStateController::ResetIAVertexStreamState()
	{
		_currentCommonState.iaVertexStreamState = nullptr;
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexStreamBit );
		return true;
	}

	bool GraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState )
	{
		_currentCommonState.renderTargetBindingState = &( RenderTargetBindingStateDescriptor::GetDynamicOverrideState() );
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
		return true;
	}

	bool GraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingStateDescriptor & pRenderTargetBindingState )
	{
		if( _currentCommonState.renderTargetBindingState != &pRenderTargetBindingState )
		{
			_currentCommonState.renderTargetBindingState = &pRenderTargetBindingState;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
	}

	bool GraphicsPipelineStateController::ResetRenderTargetBindingState()
	{
		_currentCommonState.renderTargetBindingState = nullptr;
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
		return true;
	}

	bool GraphicsPipelineStateController::SetViewport( const ViewportDesc & pViewportDesc )
	{
		return true;
	}

	bool GraphicsPipelineStateController::SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		return true;
	}

	bool GraphicsPipelineStateController::SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		return true;
	}

	bool GraphicsPipelineStateController::SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		return true;
	}

	bool GraphicsPipelineStateController::SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		return true;
	}

} // namespace Ic3::Graphics::GCI
