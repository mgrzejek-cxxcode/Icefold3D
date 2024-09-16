
#include "GraphicsPipelineStateController.h"
#include "PipelineStateObject.h"
#include "InputAssemblerImmutableStates.h"
#include "RenderTargetImmutableStates.h"

#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <Ic3/Cppx/Memory.h>

namespace Ic3::Graphics::GCI
{

	GraphicsPipelineStateController::GraphicsPipelineStateController() = default;

	GraphicsPipelineStateController::~GraphicsPipelineStateController() = default;

	bool GraphicsPipelineStateController::isIAVertexStreamStateDynamic() const noexcept
	{
		return _currentCommonState.iaVertexStreamState && _currentCommonState.iaVertexStreamState->isDynamicOverrideState();
	}

	bool GraphicsPipelineStateController::isRenderTargetStateDynamic() const noexcept
	{
		return _currentCommonState.renderTargetBindingState && _currentCommonState.renderTargetBindingState->isDynamicOverrideState();
	}

	const GraphicsPipelineDynamicState & GraphicsPipelineStateController::getRenderPassDynamicState() const noexcept
	{
		return _currentRenderPassDynamicState;
	}

	const ShaderInputSignature & GraphicsPipelineStateController::getShaderInputSignature() const noexcept
	{
		ic3DebugAssert( _currentCommonState.graphicsPSO );
		return _currentCommonState.graphicsPSO->mShaderInputSignature;
	}

	void GraphicsPipelineStateController::setRenderPassDynamicState( const GraphicsPipelineDynamicState & pDynamicState )
	{
		_currentRenderPassDynamicState = pDynamicState;
	}

	void GraphicsPipelineStateController::resetRenderPassDynamicState()
	{
		_currentRenderPassDynamicState.mActiveStateMask.clear();
	}

	bool GraphicsPipelineStateController::setGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		if( _currentCommonState.graphicsPSO != &pGraphicsPSO )
		{
			_currentCommonState.graphicsPSO = &pGraphicsPSO;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonPSOBit );
		}

		return _stateUpdateMask.isSet( eGraphicsStateUpdateFlagCommonPSOBit );
	}

	bool GraphicsPipelineStateController::resetGraphicsPipelineStateObject()
	{
		if( _currentCommonState.graphicsPSO )
		{
			_currentCommonState.graphicsPSO = nullptr;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonPSOBit );
		}

		return _stateUpdateMask.isSet( eGraphicsStateUpdateFlagCommonPSOBit );
	}

	bool GraphicsPipelineStateController::setIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState )
	{
		_currentCommonState.iaVertexStreamState = &( IAVertexStreamImmutableState::getDynamicOverrideState() );
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexStreamBit );
		return true;
	}

	bool GraphicsPipelineStateController::setIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState )
	{
		if( _currentCommonState.iaVertexStreamState != &pIAVertexStreamState )
		{
			_currentCommonState.iaVertexStreamState = &pIAVertexStreamState;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexStreamBit );
		}

		return _stateUpdateMask.isSet( eGraphicsStateUpdateFlagCommonVertexStreamBit );
	}

	bool GraphicsPipelineStateController::resetIAVertexStreamState()
	{
		_currentCommonState.iaVertexStreamState = nullptr;
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexStreamBit );
		return true;
	}

	bool GraphicsPipelineStateController::setRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState )
	{
		_currentCommonState.renderTargetBindingState = &( RenderTargetBindingImmutableState::getDynamicOverrideState() );
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
		return true;
	}

	bool GraphicsPipelineStateController::setRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState )
	{
		if( _currentCommonState.renderTargetBindingState != &pRenderTargetBindingState )
		{
			_currentCommonState.renderTargetBindingState = &pRenderTargetBindingState;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
		}

		return _stateUpdateMask.isSet( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
	}

	bool GraphicsPipelineStateController::resetRenderTargetBindingState()
	{
		_currentCommonState.renderTargetBindingState = nullptr;
		_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
		return true;
	}

	bool GraphicsPipelineStateController::setViewport( const ViewportDesc & pViewportDesc )
	{
		return true;
	}

	bool GraphicsPipelineStateController::setShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		return true;
	}

	bool GraphicsPipelineStateController::setShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		return true;
	}

	bool GraphicsPipelineStateController::setShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		return true;
	}

	bool GraphicsPipelineStateController::setShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		return true;
	}

} // namespace Ic3::Graphics::GCI
