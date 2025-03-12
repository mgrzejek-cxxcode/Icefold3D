
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_SEPARABLE_STATE_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_SEPARABLE_STATE_H__

#include "GraphicsPipelineStateCommon.h"
#include "GraphicsPipelineStateController.h"
#include "GraphicsPipelineStateDescriptorCommon.h"

namespace Ic3::Graphics::GCI
{

	enum EGraphicsStateUpdateSeparableFlags : graphics_state_update_mask_value_t
	{
		eGraphicsStateUpdateFlagSeparableStateBlendBit                 = 0x0100,
		eGraphicsStateUpdateFlagSeparableStateDepthStencilBit          = 0x0200,
		eGraphicsStateUpdateFlagSeparableStateRasterizerBit            = 0x0400,
		eGraphicsStateUpdateFlagSeparableStateRootSignatureBit         = 0x0800,
		eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit         = 0x1000,
		eGraphicsStateUpdateFlagSeparableStateVertexAttributeLayoutBit = 0x2000,
		eGraphicsStateUpdateMaskSeparableStatesAll                     = 0x3F00,

		eGraphicsStateUpdateFlagSeparableShaderVertexBit   = 0x010000,
		eGraphicsStateUpdateFlagSeparableShaderHullBit     = 0x020000,
		eGraphicsStateUpdateFlagSeparableShaderDomainBit   = 0x040000,
		eGraphicsStateUpdateFlagSeparableShaderGeometryBit = 0x080000,
		eGraphicsStateUpdateFlagSeparableShaderPixelBit    = 0x100000,
		eGraphicsStateUpdateMaskSeparableShadersAll        = 0x1F0000,

		eGraphicsStateUpdateMaskSeparableAll =
			eGraphicsStateUpdateMaskSeparableStatesAll |
			eGraphicsStateUpdateMaskSeparableShadersAll,
		
		eGraphicsStateUpdateMaskCombinedInputAssembler =
			eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit |
			eGraphicsStateUpdateFlagSeparableStateVertexAttributeLayoutBit,
	};

	struct SeparableGraphicsPSDSet
	{
		BlendStateDescriptorHandle blendStateDescriptor;
		DepthStencilStateDescriptorHandle depthStencilStateDescriptor;
		RasterizerStateDescriptorHandle rasterizerStateDescriptor;
		RootSignatureDescriptorHandle rootSignatureDescriptor;
		GraphicsShaderLinkageDescriptorHandle shaderLinkageStateDescriptor;
		VertexAttributeLayoutDescriptorHandle vertexAttributeLayoutDescriptor;
	};

	struct SeparableGraphicsPSDCache
	{
		BlendStateDescriptor * blendStateDescriptor = nullptr;
		DepthStencilStateDescriptor * depthStencilStateDescriptor = nullptr;
		RasterizerStateDescriptor * rasterizerStateDescriptor = nullptr;
		RootSignatureDescriptor * rootSignatureDescriptor = nullptr;
		GraphicsShaderLinkageDescriptor * shaderLinkageStateDescriptor = nullptr;
		VertexAttributeLayoutDescriptor * vertexAttributeLayoutDescriptor = nullptr;

		void Reset()
		{
			blendStateDescriptor = nullptr;
			depthStencilStateDescriptor = nullptr;
			rasterizerStateDescriptor = nullptr;
			rootSignatureDescriptor = nullptr;
			shaderLinkageStateDescriptor = nullptr;
			vertexAttributeLayoutDescriptor = nullptr;
		}
	};

	struct SeparableShaderCache
	{
		Shader * vertexShader = nullptr;
		Shader * tessHullShader = nullptr;
		Shader * tessDomainShader = nullptr;
		Shader * geometryShader = nullptr;
		Shader * pixelShader = nullptr;

		void Reset()
		{
			vertexShader = nullptr;
			tessHullShader = nullptr;
			tessDomainShader = nullptr;
			geometryShader = nullptr;
			pixelShader = nullptr;
		}
	};

	/**
	 * @brief A more specific type of GraphicsPipelineStateObject for drivers without monolithic PSO support.
	 *
	 * This class has been introduced for drivers which do not have the concept of monolithic PSOs, including:
	 * (1) DirectX 11 (with its ID3D11BlendState, ID3D11DepthStencilState, etc),
	 * (2) OpenGL Core/ES (with no concept of state objects at all).
	 * For those drivers, PSOs contain an explicit "state descriptor" for: blend, depth/stencil, rasterizer,
	 * vertex attribute layout, shader linkage and root signature. Those objects are, of course, driver-specific
	 * (for DX11 they will have ID3D11XXXState interfaces, for OpenGL - bunch of translated state stored as a group
	 * of GL constants), but their IDs are stored at this common level. This, combined with a neat
	 * GraphicsPipelineStateDescriptorCache class, enables writing single implementation for caching and general
	 * state handling which can be used by the mentioned drivers.
	 */
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObjectSeparable : public GraphicsPipelineStateObject
	{
	public:
		///
		SeparableGraphicsPSDSet const mSeparableDescriptors;

	public:
		GraphicsPipelineStateObjectSeparable(
				GPUDevice & pGPUDevice,
				pipeline_state_object_id_t pStateObjectID,
				const RenderTargetLayout & pRenderTargetLayout,
				const SeparableGraphicsPSDSet & pSeparableDescriptors );

		virtual ~GraphicsPipelineStateObjectSeparable();
	};

	/**
	 * @brief Even more specific type of GraphicsPipelineStateObject for drivers without monolithic shader linkage.
	 *
	 * GraphicsPipelineStateObjectSeparableShader is designed for APIs that do not incorporate the flow of linking
	 * multiple shader stages into a single binary object (either as a part of monolithic PSOs (VK/DX12) or an API-specific
	 * programs (OpenGL). This type of PSO simply contains a set of shaders to be bound to their pipeline stages.
	 */
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObjectSeparableShader : public GraphicsPipelineStateObjectSeparable
	{
	public:
		///
		const GraphicsShaderBinding & mShaderBinding;

	public:
		GraphicsPipelineStateObjectSeparableShader(
				GPUDevice & pGPUDevice,
				pipeline_state_object_id_t pStateObjectID,
				const RenderTargetLayout & pRenderTargetLayout,
				const SeparableGraphicsPSDSet & pSeparableDescriptors,
				const GraphicsShaderBinding & pGraphicsShaderBinding );

		virtual ~GraphicsPipelineStateObjectSeparableShader();
	};

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateControllerSeparable : public GraphicsPipelineStateController
	{
	public:
		GraphicsPipelineStateControllerSeparable();
		virtual ~GraphicsPipelineStateControllerSeparable();

		/// @see GraphicsPipelineStateController::SetGraphicsPipelineStateObject
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject ) override;

		/// @see GraphicsPipelineStateController::ResetGraphicsPipelineStateObject
		virtual bool ResetGraphicsPipelineStateObject() override;

		CPPX_ATTR_NO_DISCARD const SeparableGraphicsPSDCache & GetSeparableDescriptorsCache() const noexcept;

		template <typename TPBlendStateDescriptor = BlendStateDescriptor>
		CPPX_ATTR_NO_DISCARD const TPBlendStateDescriptor & GetCurrentPSOBlendStateDescriptor() const noexcept;

		template <typename TPDepthStencilStateDescriptor = DepthStencilStateDescriptor>
		CPPX_ATTR_NO_DISCARD const TPDepthStencilStateDescriptor & GetCurrentPSODepthStencilStateDescriptor() const noexcept;

		template <typename TPRasterizerStateDescriptor = RasterizerStateDescriptor>
		CPPX_ATTR_NO_DISCARD const TPRasterizerStateDescriptor & GetCurrentPSORasterizerStateDescriptor() const noexcept;

		template <typename TPRootSignatureDescriptor = RootSignatureDescriptor>
		CPPX_ATTR_NO_DISCARD const TPRootSignatureDescriptor & GetCurrentPSORootSignatureDescriptor() const noexcept;

		template <typename TPShaderLinkageDescriptor = GraphicsShaderLinkageDescriptor>
		CPPX_ATTR_NO_DISCARD const TPShaderLinkageDescriptor & GetCurrentPSOShaderLinkageDescriptor() const noexcept;

		template <typename TPVertexAttributeLayoutDescriptor = VertexAttributeLayoutDescriptor>
		CPPX_ATTR_NO_DISCARD const TPVertexAttributeLayoutDescriptor & GetCurrentPSOVertexAttributeLayoutDescriptor() const noexcept;

	private:
		cppx::bitmask<uint64> SetSeparableDescriptors( const GraphicsPipelineStateObjectSeparable & pSeparableGraphicsPSO );

	private:
		SeparableGraphicsPSDCache _separableDescriptorsCache;
	};

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateControllerSeparableShader : public GraphicsPipelineStateControllerSeparable
	{
	public:
		GraphicsPipelineStateControllerSeparableShader();
		virtual ~GraphicsPipelineStateControllerSeparableShader();

		/// @see GraphicsPipelineStateController::SetGraphicsPipelineStateObject
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject ) override;

		/// @see GraphicsPipelineStateController::ResetGraphicsPipelineStateObject
		virtual bool ResetGraphicsPipelineStateObject() override;

		CPPX_ATTR_NO_DISCARD const SeparableShaderCache & GetSeparableShaders() const noexcept;

	private:
		cppx::bitmask<uint64> SetSeparableShaders( const GraphicsPipelineStateObjectSeparableShader & pSeparableShaderGraphicsPSO );

	private:
		SeparableShaderCache _separableShadersCache;
	};

	namespace GCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD GraphicsPipelineStateObjectDescriptorIDSet GetDescriptorsIDSetForSeparablePSO(
			const SeparableGraphicsPSDSet & pSeparableGraphicsDescriptors ) noexcept;

	} // namespace GCU

} // namespace Ic3::Graphics::GCI

#include "GraphicsPipelineStateImplSeparableState.inl"

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_SEPARABLE_STATE_H__
