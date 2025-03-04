
#include "GraphicsPipelineStateController.h"
#include "GraphicsPipelineStateDescriptorIADynamic.h"
#include "GraphicsPipelineStateDescriptorRTODynamic.h"

#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <cppx/memory.h>

namespace Ic3::Graphics::GCI
{

	GraphicsPipelineStateController::GraphicsPipelineStateController() = default;

	GraphicsPipelineStateController::~GraphicsPipelineStateController() = default;

	bool GraphicsPipelineStateController::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPipelineStateObject )
	{
		if( _currentPipelineBindings.pipelineStateObject != &pGraphicsPipelineStateObject )
		{
			_currentPipelineBindings.pipelineStateObject = &pGraphicsPipelineStateObject;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonPipelineStateObjectBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPipelineStateObjectBit );
	}

	bool GraphicsPipelineStateController::ResetGraphicsPipelineStateObject()
	{
		if( _currentPipelineBindings.pipelineStateObject )
		{
			_currentPipelineBindings.pipelineStateObject = nullptr;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonPipelineStateObjectBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPipelineStateObjectBit );
	}

	bool GraphicsPipelineStateController::SetRenderTargetDescriptor(
			const RenderTargetDescriptor & pRenderTargetDescriptor )
	{
		if( pRenderTargetDescriptor.IsDynamicDescriptor() )
		{
			throw 0;
		}

		if( _currentPipelineBindings.renderTargetDescriptor != &pRenderTargetDescriptor )
		{
			_currentPipelineBindings.renderTargetDescriptor = &pRenderTargetDescriptor;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit );
	}

	bool GraphicsPipelineStateController::SetRenderTargetDescriptorDynamic(
			RenderTargetDescriptorDynamic & pRenderTargetDescriptor )
	{
		if( _currentPipelineBindings.renderTargetDescriptor != &pRenderTargetDescriptor )
		{
			_currentPipelineBindings.renderTargetDescriptor = &pRenderTargetDescriptor;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit );
	}

	bool GraphicsPipelineStateController::ResetRenderTargetDescriptor()
	{
		if( _currentPipelineBindings.renderTargetDescriptor )
		{
			_currentPipelineBindings.renderTargetDescriptor = nullptr;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit );
	}

	bool GraphicsPipelineStateController::SetVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor )
	{
		if( pVertexSourceBindingDescriptor.IsDynamicDescriptor() )
		{
			throw 0;
		}

		if( _currentPipelineBindings.vertexSourceBindingDescriptor != &pVertexSourceBindingDescriptor )
		{
			_currentPipelineBindings.vertexSourceBindingDescriptor = &pVertexSourceBindingDescriptor;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit );
	}

	bool GraphicsPipelineStateController::SetVertexSourceBindingDescriptorDynamic(
			VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor )
	{
		if( _currentPipelineBindings.vertexSourceBindingDescriptor != &pVertexSourceBindingDescriptor )
		{
			_currentPipelineBindings.vertexSourceBindingDescriptor = &pVertexSourceBindingDescriptor;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit );
	}

	bool GraphicsPipelineStateController::ResetVertexSourceBindingDescriptor()
	{
		if( _currentPipelineBindings.vertexSourceBindingDescriptor )
		{
			_currentPipelineBindings.vertexSourceBindingDescriptor = nullptr;
			_stateUpdateMask.set( eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit );
		}

		return _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit );
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

	cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> GraphicsPipelineStateController::SetDynamicBlendConstantColor(
			const Math::RGBAColorR32Norm & pBlendConstantColor )
	{
		_currentPipelineDynamicConfig.blendConstantColor = pBlendConstantColor;
		_currentPipelineDynamicConfig.activeStateMask.set( eGraphicsPipelineDynamicConfigFlagBlendConstantColorBit );
		return _currentPipelineDynamicConfig.activeStateMask;
	}

	cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> GraphicsPipelineStateController::SetDynamicRenderTargetClearConfig(
			const RenderTargetAttachmentClearConfig & pClearConfig )
	{
		_currentPipelineDynamicConfig.renderTargetClearConfig = pClearConfig;
		_currentPipelineDynamicConfig.activeStateMask.set( eGraphicsPipelineDynamicConfigFlagRenderTargetClearConfigBit );
		return _currentPipelineDynamicConfig.activeStateMask;
	}

	cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> GraphicsPipelineStateController::SetDynamicStencilTestRefValue(
			uint8 pStencilRefValue )
	{
		_currentPipelineDynamicConfig.stencilTestRefValue = pStencilRefValue;
		_currentPipelineDynamicConfig.activeStateMask.set( eGraphicsPipelineDynamicConfigFlagStencilRefValueBit );
		return _currentPipelineDynamicConfig.activeStateMask;
	}

	void GraphicsPipelineStateController::ResetDynamicPipelineConfig(
			cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> pConfigMask )
	{
		_currentPipelineDynamicConfig.activeStateMask.unset( pConfigMask & eGraphicsPipelineDynamicConfigMaskAll );
	}

	cppx::bitmask<graphics_state_update_mask_value_t> GraphicsPipelineStateController::ResetStateUpdateMask()
	{
		const auto stateUpdateMaskValue = _stateUpdateMask;
		_stateUpdateMask.clear();
		return stateUpdateMaskValue;
	}

	bool GraphicsPipelineStateController::SetRenderPassDescriptor(
			const RenderPassDescriptor & pRenderPassDescriptor )
	{
		if( pRenderPassDescriptor.IsDynamicDescriptor() )
		{
			throw 0;
		}

		if( _currentPipelineBindings.renderPassDescriptor != &pRenderPassDescriptor )
		{
			_currentPipelineBindings.renderPassDescriptor = &pRenderPassDescriptor;
		}

		return true;
	}

	bool GraphicsPipelineStateController::SetRenderPassDescriptorDynamic(
			RenderPassDescriptorDynamic & pRenderPassDescriptor )
	{
		if( _currentPipelineBindings.renderPassDescriptor != &pRenderPassDescriptor )
		{
			_currentPipelineBindings.renderPassDescriptor = &pRenderPassDescriptor;
		}

		return true;
	}

	void GraphicsPipelineStateController::ResetRenderPassDescriptor()
	{
		_currentPipelineBindings.renderPassDescriptor = nullptr;
	}

} // namespace Ic3::Graphics::GCI
