
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SEPARABLE_PIPELINE_STATE_H__
#define __IC3_GRAPHICS_GCI_SEPARABLE_PIPELINE_STATE_H__

#include "GraphicsPipelineStateController.h"
#include "PipelineStateObject.h"

namespace Ic3::Graphics::GCI
{

	enum EGraphicsStateUpdateSeparableFlags : graphics_state_update_mask_value_t
	{
		eGraphicsStateUpdateFlagSeparableStateBlendBit = 0x0100,
		eGraphicsStateUpdateFlagSeparableStateDepthStencilBit = 0x0200,
		eGraphicsStateUpdateFlagSeparableStateRasterizerBit = 0x0400,
		eGraphicsStateUpdateFlagSeparableStateIAInputLayoutBit = 0x0800,
		eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit = 0x1000,

		eGraphicsStateUpdateMaskSeparableStatesAll = 0x1F00,
		
		eGraphicsStateUpdateFlagSeparableShaderVertexBit = 0x010000,
		eGraphicsStateUpdateFlagSeparableShaderHullBit = 0x020000,
		eGraphicsStateUpdateFlagSeparableShaderDomainBit = 0x040000,
		eGraphicsStateUpdateFlagSeparableShaderGeometryBit = 0x080000,
		eGraphicsStateUpdateFlagSeparableShaderPixelBit = 0x100000,

		eGraphicsStateUpdateMaskSeparableShadersAll = 0x1F0000,

		eGraphicsStateUpdateMaskSeparableAll =
			eGraphicsStateUpdateMaskSeparableStatesAll |
			eGraphicsStateUpdateMaskSeparableShadersAll,
		
		eGraphicsStateUpdateMaskCombinedInputAssembler =
			eGraphicsStateUpdateFlagCommonVertexStreamBit |
			eGraphicsStateUpdateFlagSeparableStateIAInputLayoutBit,
	};

	struct SeparablePSOStateSet
	{
		BlendImmutableStateHandle blendState;
		DepthStencilImmutableStateHandle depthStencilState;
		RasterizerImmutableStateHandle rasterizerState;
		IAInputLayoutImmutableStateHandle iaInputLayoutState;
		GraphicsShaderLinkageImmutableStateHandle shaderLinkageState;
	};

	struct SeparablePSOStateCache
	{
		BlendImmutableState * blendState = nullptr;
		DepthStencilImmutableState * depthStencilState = nullptr;
		RasterizerImmutableState * rasterizerState = nullptr;
		IAInputLayoutImmutableState * iaInputLayoutState = nullptr;
		GraphicsShaderLinkageImmutableState * shaderLinkageState = nullptr;

		void reset()
		{
			blendState = nullptr;
			depthStencilState = nullptr;
			rasterizerState = nullptr;
			iaInputLayoutState = nullptr;
			shaderLinkageState = nullptr;
		}
	};

	struct SeparableShaderCache
	{
		Shader * vertexShader = nullptr;
		Shader * hullShader = nullptr;
		Shader * domainShader = nullptr;
		Shader * geometryShader = nullptr;
		Shader * pixelShader = nullptr;

		void reset()
		{
			vertexShader = nullptr;
			hullShader = nullptr;
			domainShader = nullptr;
			geometryShader = nullptr;
			pixelShader = nullptr;
		}
	};

	/// @brief A more specific type of GraphicsPSO for drivers without monolithic state object support.
	///
	/// This class has been introduced for drivers which do not have the concept of monolithic PSOs, including:
	/// (1) DirectX 11 (with its ID3D11BlendState, ID3D11DepthStencilState, etc),
	/// (2) OpenGL Core/ES (with no concept of state objects at all).
	/// For those drivers, PSOs contain an explicit "state descriptor" for each: blend, depth/stencil, rasterizer and
	/// vertex input state. Those pdesc are, of course, driver-specific (for DX11 they will have ID3D11XXXState
	/// interfaces, for OpenGL - bunch of translated state stored as a group of GL constants), but their IDs are stored
	/// at this common level. This, combined with a neat GraphicsPipelineStateDescriptorCache class, enables writing
	/// single implementation for caching and general state handling which can be used by the mentioned drivers.
	/// Additionally, separable PSOs also contain an explicit per-stage shader binding (which is part of the combined
	/// state in monolithic PSOs). This is another thing we can handle here instead of doing it per-driver.
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObjectSeparable : public GraphicsPipelineStateObject
	{
	public:
		///
		SeparablePSOStateSet const mSeparableStates;

	public:
		GraphicsPipelineStateObjectSeparable(
				GpuDevice & pGpuDevice,
				RenderTargetLayout pRenderTargetLayout,
				ShaderInputSignature pShaderInputSignature,
				const SeparablePSOStateSet & pSeparableStates );

		virtual ~GraphicsPipelineStateObjectSeparable();
	};

	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObjectSeparableShader : public GraphicsPipelineStateObjectSeparable
	{
	public:
		///
		const GraphicsShaderSet & mSeparableShaders;

	public:
		GraphicsPipelineStateObjectSeparableShader(
				GpuDevice & pGpuDevice,
				RenderTargetLayout pRenderTargetLayout,
				ShaderInputSignature pShaderInputSignature,
				const SeparablePSOStateSet & pSeparableStates,
				const GraphicsShaderSet & pSeparableShaders );

		virtual ~GraphicsPipelineStateObjectSeparableShader();
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateControllerSeparable : public GraphicsPipelineStateController
	{
	public:
		GraphicsPipelineStateControllerSeparable();
		virtual ~GraphicsPipelineStateControllerSeparable();

		CPPX_ATTR_NO_DISCARD const SeparablePSOStateCache & GetCurrentSeparableStates() const noexcept;

		/// @see GraphicsPipelineStateController::setGraphicsPipelineStateObject
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO ) override;

		/// @see GraphicsPipelineStateController::resetGraphicsPipelineStateObject
		virtual bool ResetGraphicsPipelineStateObject() override;

	private:
		cppx::bitmask<uint64> SetSeparablePSOStates( const GraphicsPipelineStateObjectSeparable & pGraphicsPSOSeparable );

	private:
		SeparablePSOStateCache _currentSeparableStates;
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateControllerSeparableShader : public GraphicsPipelineStateControllerSeparable
	{
	public:
		GraphicsPipelineStateControllerSeparableShader();
		virtual ~GraphicsPipelineStateControllerSeparableShader();

		CPPX_ATTR_NO_DISCARD const SeparableShaderCache & GetCurrentSeparableShaders() const noexcept;

		/// @see GraphicsPipelineStateController::setGraphicsPipelineStateObject
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO ) override;

		/// @see GraphicsPipelineStateController::resetGraphicsPipelineStateObject
		virtual bool ResetGraphicsPipelineStateObject() override;

	private:
		cppx::bitmask<uint64> SetSeparableShaders( const GraphicsPipelineStateObjectSeparableShader & pGraphicsPSOSeparableShader );

	private:
		SeparableShaderCache _currentSeparableShaders;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SEPARABLE_PIPELINE_STATE_H__
