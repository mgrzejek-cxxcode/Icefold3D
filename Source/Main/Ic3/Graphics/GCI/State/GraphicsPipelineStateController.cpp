
#include "GraphicsPipelineStateController.h"
#include "PipelineStateObject.h"
#include "InputAssemblerImmutableStates.h"
#include "RenderTargetImmutableStates.h"

#include <Ic3/Graphics/GCI/GpuDevice.h>
#include <cppx/memory.h>

namespace Ic3::Graphics::GCI
{

	GraphicsPipelineStateController::GraphicsPipelineStateController() = default;

	GraphicsPipelineStateController::~GraphicsPipelineStateController() = default;

	bool GraphicsPipelineStateController::IsIAVertexStreamStateDynamic() const noexcept
	{
		return _currentCommonState.iaVertexStreamState && _currentCommonState.iaVertexStreamState->IsDynamicOverrideState();
	}

	bool GraphicsPipelineStateController::IsRenderTargetStateDynamic() const noexcept
	{
		return _currentCommonState.renderTargetBindingState && _currentCommonState.renderTargetBindingState->IsDynamicOverrideState();
	}

	const GraphicsPipelineDynamicState & GraphicsPipelineStateController::GetRenderPassDynamicState() const noexcept
	{
		return _currentRenderPassDynamicState;
	}

	const ShaderInputSignature & GraphicsPipelineStateController::GetShaderInputSignature() const noexcept
	{
		Ic3DebugAssert( _currentCommonState.graphicsPSO );
		return _currentCommonState.graphicsPSO->mShaderInputSignature;
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
		_currentCommonState.iaVertexStreamState = &( IAVertexStreamImmutableState::GetDynamicOverrideState() );
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexStreamBit );
		return true;
	}

	bool GraphicsPipelineStateController::SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState )
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
		_currentCommonState.renderTargetBindingState = &( RenderTargetBindingImmutableState::GetDynamicOverrideState() );
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
		return true;
	}

	bool GraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState )
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

	bool GraphicsPipelineStateController::SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer )
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
