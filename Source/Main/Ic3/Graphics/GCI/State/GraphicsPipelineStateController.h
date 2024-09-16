
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__

#include "CommonGPUStateDefs.h"
#include <Ic3/CoreLib/ExceptionUtils.h>

namespace Ic3::Graphics::GCI
{

	class PipelineImmutableStateFactory;

	using graphics_state_update_mask_value_t = uint64;

	/// @brief Defines bit flags describing what kind of changes are pending for
	enum EGraphicsStateUpdateCommonFlags : graphics_state_update_mask_value_t
	{
		eGraphicsStateUpdateFlagCommonPSOBit = 0x01,
		eGraphicsStateUpdateFlagCommonVertexStreamBit = 0x02,
		eGraphicsStateUpdateFlagCommonRenderTargetBindingBit = 0x04,

		eGraphicsStateUpdateMaskCommonAll = 0x07
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateController
	{
	public:
		GraphicsPipelineStateController();
		virtual ~GraphicsPipelineStateController();

		virtual bool applyStateChanges() = 0;

		IC3_ATTR_NO_DISCARD bool isIAVertexStreamStateDynamic() const noexcept;

		IC3_ATTR_NO_DISCARD bool isRenderTargetStateDynamic() const noexcept;

		IC3_ATTR_NO_DISCARD const GraphicsPipelineDynamicState & getRenderPassDynamicState() const noexcept;

		IC3_ATTR_NO_DISCARD const ShaderInputSignature & getShaderInputSignature() const noexcept;

		void setRenderPassDynamicState( const GraphicsPipelineDynamicState & pDynamicState );

		void resetRenderPassDynamicState();

		/// @brief Binds the specified state object to the pipeline. Returns true if any change has been made.
		/// @return True if anything has been changed or false otherwise.
		/// @note Sub-classes should always call the base method first and check the result before doing the actual update.
		virtual bool setGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO );

		/// @brief Resets the current SO to the default one. Returns true if any change has been made.
		/// @return True if anything has been changed or false otherwise.
		/// @note Sub-classes should always call the base method first and check the result before doing the actual update.
		virtual bool resetGraphicsPipelineStateObject();

		virtual bool setIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState );
		virtual bool setIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState );
		virtual bool resetIAVertexStreamState();

		virtual bool setRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState );
		virtual bool setRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState );
		virtual bool resetRenderTargetBindingState();

		virtual bool setViewport( const ViewportDesc & pViewportDesc );
		virtual bool setShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData );
		virtual bool setShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer );
		virtual bool setShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture );
		virtual bool setShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler );

		/// @brief
		IC3_ATTR_NO_DISCARD bool hasPendingStateChanges() const noexcept
		{
			return !_stateUpdateMask.empty();
		}

		template <typename TGraphicsPipelineStateObject = GraphicsPipelineStateObject>
		inline const TGraphicsPipelineStateObject * getCurrentGraphicsPSO() const noexcept
		{
			if( _currentCommonState.graphicsPSO )
			{
				const auto * baseInterface = reinterpret_cast<const GPUAPIObject *>( _currentCommonState.graphicsPSO );
				return baseInterface->queryInterface<TGraphicsPipelineStateObject>();
			}
			return nullptr;
		}

		template <typename TGraphicsPipelineStateObject = GraphicsPipelineStateObject>
		inline const TGraphicsPipelineStateObject & getCurrentGraphicsPSORef() const
		{
			if( const auto * stateObject = getCurrentGraphicsPSO<TGraphicsPipelineStateObject>() )
			{
				return *stateObject;
			}
			ic3Throw( 0 );
		}

		template <typename TIAVertexStreamState = IAVertexStreamImmutableState>
		inline const TIAVertexStreamState * getCurrentIAVertexStreamState() const noexcept
		{
			if( _currentCommonState.iaVertexStreamState )
			{
				const auto * baseInterface = reinterpret_cast<const GPUAPIObject *>( _currentCommonState.iaVertexStreamState );
				return baseInterface->queryInterface<TIAVertexStreamState>();
			}
			return nullptr;
		}

		template <typename TIAVertexStreamState = IAVertexStreamImmutableState>
		inline const TIAVertexStreamState & getCurrentIAVertexStreamStateRef() const
		{
			if( const auto * stateObject = getCurrentIAVertexStreamState<TIAVertexStreamState>() )
			{
				return *stateObject;
			}
			ic3Throw( 0 );
		}

		template <typename TRenderTargetBindingState = RenderTargetBindingImmutableState>
		inline const TRenderTargetBindingState * getCurrentRenderTargetBindingState() const noexcept
		{
			if( _currentCommonState.renderTargetBindingState )
			{
				const auto * baseInterface = reinterpret_cast<const GPUAPIObject *>( _currentCommonState.renderTargetBindingState );
				return baseInterface->queryInterface<TRenderTargetBindingState>();
			}
			return nullptr;
		}

		template <typename TRenderTargetBindingState = RenderTargetBindingImmutableState>
		inline const TRenderTargetBindingState & getCurrentRenderTargetBindingStateRef() const
		{
			if( const auto * stateObject = getCurrentRenderTargetBindingState<TRenderTargetBindingState>() )
			{
				return *stateObject;
			}
			ic3Throw( 0 );
		}

	protected:
		void resetStateUpdateMask();

	protected:
		struct CurrentCommonState
		{
			const GraphicsPipelineStateObject * graphicsPSO = nullptr;
			const IAVertexStreamImmutableState * iaVertexStreamState = nullptr;
			const RenderTargetBindingImmutableState * renderTargetBindingState = nullptr;
		};

		CurrentCommonState _currentCommonState;

		GraphicsPipelineDynamicState _currentRenderPassDynamicState;

		TBitmask<graphics_state_update_mask_value_t> _stateUpdateMask = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__
