
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__

#include "CommonGpuStateDefs.h"
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

		virtual bool ApplyStateChanges() = 0;

		CPPX_ATTR_NO_DISCARD bool IsIAVertexStreamStateDynamic() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsRenderTargetStateDynamic() const noexcept;

		CPPX_ATTR_NO_DISCARD const GraphicsPipelineDynamicState & GetRenderPassDynamicState() const noexcept;

		CPPX_ATTR_NO_DISCARD const ShaderInputSignature & GetShaderInputSignature() const noexcept;

		void SetRenderPassDynamicState( const GraphicsPipelineDynamicState & pDynamicState );

		void ResetRenderPassDynamicState();

		/// @brief Binds the specified state object to the pipeline. Returns true if any change has been made.
		/// @return True if anything has been changed or false otherwise.
		/// @note Sub-classes should always call the base method first and check the result before doing the actual update.
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO );

		/// @brief Resets the current SO to the default one. Returns true if any change has been made.
		/// @return True if anything has been changed or false otherwise.
		/// @note Sub-classes should always call the base method first and check the result before doing the actual update.
		virtual bool ResetGraphicsPipelineStateObject();

		virtual bool SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState );
		virtual bool SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState );
		virtual bool ResetIAVertexStreamState();

		virtual bool SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState );
		virtual bool SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState );
		virtual bool ResetRenderTargetBindingState();

		virtual bool SetViewport( const ViewportDesc & pViewportDesc );
		virtual bool SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData );
		virtual bool SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer );
		virtual bool SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture );
		virtual bool SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler );

		/// @brief
		CPPX_ATTR_NO_DISCARD bool HasPendingStateChanges() const noexcept
		{
			return !_stateUpdateMask.empty();
		}

		template <typename TGraphicsPipelineStateObject = GraphicsPipelineStateObject>
		inline const TGraphicsPipelineStateObject * GetCurrentGraphicsPSO() const noexcept
		{
			if( _currentCommonState.graphicsPSO )
			{
				const auto * baseInterface = reinterpret_cast<const GfxObject *>( _currentCommonState.graphicsPSO );
				return baseInterface->QueryInterface<TGraphicsPipelineStateObject>();
			}
			return nullptr;
		}

		template <typename TGraphicsPipelineStateObject = GraphicsPipelineStateObject>
		inline const TGraphicsPipelineStateObject & GetCurrentGraphicsPSORef() const
		{
			if( const auto * stateObject = GetCurrentGraphicsPSO<TGraphicsPipelineStateObject>() )
			{
				return *stateObject;
			}
			ic3Throw( 0 );
		}

		template <typename TIAVertexStreamState = IAVertexStreamImmutableState>
		inline const TIAVertexStreamState * GetCurrentIAVertexStreamState() const noexcept
		{
			if( _currentCommonState.iaVertexStreamState )
			{
				const auto * baseInterface = reinterpret_cast<const GfxObject *>( _currentCommonState.iaVertexStreamState );
				return baseInterface->QueryInterface<TIAVertexStreamState>();
			}
			return nullptr;
		}

		template <typename TIAVertexStreamState = IAVertexStreamImmutableState>
		inline const TIAVertexStreamState & GetCurrentIAVertexStreamStateRef() const
		{
			if( const auto * stateObject = GetCurrentIAVertexStreamState<TIAVertexStreamState>() )
			{
				return *stateObject;
			}
			ic3Throw( 0 );
		}

		template <typename TRenderTargetBindingState = RenderTargetBindingImmutableState>
		inline const TRenderTargetBindingState * GetCurrentRenderTargetBindingState() const noexcept
		{
			if( _currentCommonState.renderTargetBindingState )
			{
				const auto * baseInterface = reinterpret_cast<const GfxObject *>( _currentCommonState.renderTargetBindingState );
				return baseInterface->QueryInterface<TRenderTargetBindingState>();
			}
			return nullptr;
		}

		template <typename TRenderTargetBindingState = RenderTargetBindingImmutableState>
		inline const TRenderTargetBindingState & GetCurrentRenderTargetBindingStateRef() const
		{
			if( const auto * stateObject = GetCurrentRenderTargetBindingState<TRenderTargetBindingState>() )
			{
				return *stateObject;
			}
			ic3Throw( 0 );
		}

	protected:
		void ResetStateUpdateMask();

	protected:
		struct CurrentCommonState
		{
			const GraphicsPipelineStateObject * graphicsPSO = nullptr;
			const IAVertexStreamImmutableState * iaVertexStreamState = nullptr;
			const RenderTargetBindingImmutableState * renderTargetBindingState = nullptr;
		};

		CurrentCommonState _currentCommonState;

		GraphicsPipelineDynamicState _currentRenderPassDynamicState;

		cppx::bitmask<graphics_state_update_mask_value_t> _stateUpdateMask = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__
