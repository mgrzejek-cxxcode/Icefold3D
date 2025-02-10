
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SEPARABLE_PIPELINE_STATE_H__
#define __IC3_GRAPHICS_GCI_SEPARABLE_PIPELINE_STATE_H__

#include "GraphicsPipelineStateController.h"
#include "GraphicsPipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{

	enum EGraphicsStateUpdateSeparableFlags : graphics_state_update_mask_value_t
	{
		eGraphicsStateUpdateFlagSeparableStateBlendBit = 0x0100,
		eGraphicsStateUpdateFlagSeparableStateDepthStencilBit = 0x0200,
		eGraphicsStateUpdateFlagSeparableStateRasterizerBit = 0x0400,
		eGraphicsStateUpdateFlagSeparableStateIAVertexAttributeLayoutBit = 0x0800,
		eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit = 0x1000,
		eGraphicsStateUpdateFlagSeparableStateShaderRootSignatureBit = 0x2000,

		eGraphicsStateUpdateMaskSeparableStatesAll = 0x3F00,
		
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
			eGraphicsStateUpdateFlagCommonIAVertexStreamBindingBit |
			eGraphicsStateUpdateFlagSeparableStateIAVertexAttributeLayoutBit,
	};

	struct SeparableGraphicsPSDSet
	{
		BlendStateDescriptorHandle blendStateDescriptor;
		DepthStencilStateDescriptorHandle depthStencilStateDescriptor;
		RasterizerStateDescriptorHandle rasterizerStateDescriptor;
		IAVertexAttributeLayoutStateDescriptorHandle iaVertexAttributeLayoutStateDescriptor;
		GraphicsShaderLinkageStateDescriptorHandle shaderLinkageStateDescriptor;
		ShaderRootSignatureStateDescriptorHandle shaderRootSignatureStateDescriptor;
	};

	struct SeparableGraphicsPSDCache
	{
		BlendStateDescriptor * blendStateDescriptor = nullptr;
		DepthStencilStateDescriptor * depthStencilStateDescriptor = nullptr;
		RasterizerStateDescriptor * rasterizerStateDescriptor = nullptr;
		IAVertexAttributeLayoutStateDescriptor * iaVertexAttributeLayoutStateDescriptor = nullptr;
		GraphicsShaderLinkageStateDescriptor * shaderLinkageStateDescriptor = nullptr;
		ShaderRootSignatureStateDescriptor * shaderRootSignatureStateDescriptor = nullptr;

		void Reset()
		{
			blendStateDescriptor = nullptr;
			depthStencilStateDescriptor = nullptr;
			rasterizerStateDescriptor = nullptr;
			iaVertexAttributeLayoutStateDescriptor = nullptr;
			shaderLinkageStateDescriptor = nullptr;
			shaderRootSignatureStateDescriptor = nullptr;
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

	/// @brief A more specific type of GraphicsPipelineStateObject for drivers without monolithic PSO support.
	///
	/// This class has been introduced for drivers which do not have the concept of monolithic PSOs, including:
	/// (1) DirectX 11 (with its ID3D11BlendState, ID3D11DepthStencilState, etc),
	/// (2) OpenGL Core/ES (with no concept of state objects at all).
	/// For those drivers, PSOs contain an explicit "state descriptor" for each: blend, depth/stencil, rasterizer and
	/// vertex attribute layout, shader linkage and root signature. Those objects are, of course, driver-specific
	/// (for DX11 they will have ID3D11XXXState interfaces, for OpenGL - bunch of translated state stored as a group
	/// of GL constants), but their IDs are stored at this common level. This, combined with a neat
	/// GraphicsPipelineStateDescriptorCache class, enables writing single implementation for caching and general
	/// state handling which can be used by the mentioned drivers.
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObjectSeparable : public GraphicsPipelineStateObject
	{
	public:
		///
		SeparableGraphicsPSDSet const mSeparableDescriptors;

	public:
		GraphicsPipelineStateObjectSeparable(
				GPUDevice & pGPUDevice,
				pipeline_state_object_id_t pStateObjectID,
				const SeparableGraphicsPSDSet & pSeparableDescriptors );

		virtual ~GraphicsPipelineStateObjectSeparable();
	};

	/// @brief Even more specific type of GraphicsPipelineStateObject for drivers without monolithic shader linkage.
	///
	/// GraphicsPipelineStateObjectSeparableShader is designed for APIs that do not incorporate the flow of linking
	/// multiple shader stages into a single binary object (either as a part of monolithic PSOs (VK/DX12) or an API-specific
	/// programs (OpenGL). This type of PSO simply contains a set of shaders bound to their pipeline stages.
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObjectSeparableShader : public GraphicsPipelineStateObjectSeparable
	{
	public:
		///
		const GraphicsShaderBinding & mSeparableShaders;

	public:
		GraphicsPipelineStateObjectSeparableShader(
				GPUDevice & pGPUDevice,
				pipeline_state_object_id_t pStateObjectID,
				const SeparableGraphicsPSDSet & pSeparableDescriptors,
				const GraphicsShaderBinding & pSeparableShaders );

		virtual ~GraphicsPipelineStateObjectSeparableShader();
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateControllerSeparable : public GraphicsPipelineStateController
	{
	public:
		GraphicsPipelineStateControllerSeparable();
		virtual ~GraphicsPipelineStateControllerSeparable();

		CPPX_ATTR_NO_DISCARD const SeparableGraphicsPSDCache & GetSeparableDescriptors() const noexcept;

		/// @see GraphicsPipelineStateController::SetGraphicsPipelineStateObject
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO ) override;

		/// @see GraphicsPipelineStateController::ResetGraphicsPipelineStateObject
		virtual bool ResetGraphicsPipelineStateObject() override;

	private:
		cppx::bitmask<uint64> SetSeparableDescriptors( const GraphicsPipelineStateObjectSeparable & pGraphicsPSOSeparable );

	private:
		SeparableGraphicsPSDCache _separableDescriptorsCache;
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateControllerSeparableShader : public GraphicsPipelineStateControllerSeparable
	{
	public:
		GraphicsPipelineStateControllerSeparableShader();
		virtual ~GraphicsPipelineStateControllerSeparableShader();

		CPPX_ATTR_NO_DISCARD const SeparableShaderCache & GetSeparableShaders() const noexcept;

		/// @see GraphicsPipelineStateController::SetGraphicsPipelineStateObject
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO ) override;

		/// @see GraphicsPipelineStateController::ResetGraphicsPipelineStateObject
		virtual bool ResetGraphicsPipelineStateObject() override;

	private:
		cppx::bitmask<uint64> SetSeparableShaders( const GraphicsPipelineStateObjectSeparableShader & pGraphicsPSOSeparableShader );

	private:
		SeparableShaderCache _separableShadersCache;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SEPARABLE_PIPELINE_STATE_H__
