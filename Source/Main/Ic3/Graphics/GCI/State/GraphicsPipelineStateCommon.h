
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_COMMON_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_COMMON_H__

#include "PipelineStateCommon.h"
#include "CommonGraphicsConfig.h"
#include "InputAssemblerCommon.h"
#include "GraphicsShaderDefs.h"
#include "RenderTargetCommon.h"
#include "ShaderRootSignature.h"

namespace Ic3::Graphics::GCI
{

	struct GraphicsPipelineStateObjectCreateInfo
	{
		BlendConfig blendConfig;
		pipeline_state_descriptor_id_t blendDescriptorID;
		mutable BlendStateDescriptorHandle blendState;

		DepthStencilConfig depthStencilConfig;
		mutable DepthStencilStateDescriptorHandle depthStencilState;

		RasterizerConfig rasterizerConfig;
		mutable RasterizerStateDescriptorHandle rasterizerState;

		GraphicsShaderSet shaderSet;
		mutable GraphicsShaderLinkageStateDescriptorHandle shaderLinkageState;

		IAInputLayoutDefinition inputLayoutDefinition;
		mutable IAInputLayoutStateDescriptorHandle inputLayoutState;

		ShaderRootSignatureDesc shaderRootSignatureDesc;
		mutable ShaderRootSignature shaderRootSignature;

		RenderTargetLayout renderTargetLayout;

	};

	struct GraphicsPipelineStateObjectDescriptorIDSet
	{
		pipeline_state_descriptor_id_t descIDIAInputLayout = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDGraphicsShaderLinkage = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDShaderRootSignature = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDBlend = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDDepthStencil = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDRasterizer = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDMultiSampling = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t _padding = 0;
	};

	/// @brief PSO for the graphics pipeline.
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObject : public PipelineStateObject
	{
	public:
		GraphicsPipelineStateObject( GPUDevice & pGPUDevice );
		virtual ~GraphicsPipelineStateObject();

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineType GetPipelineType() const noexcept override final;

		/**
		 *
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD const GraphicsPipelineStateObjectDescriptorIDSet & GetDescriptorIDSet() const noexcept;

	private:
		GraphicsPipelineStateObjectDescriptorIDSet _descriptorIDSet;
	};

	inline const GraphicsPipelineStateObjectDescriptorIDSet & GraphicsPipelineStateObject::GetDescriptorIDSet() const noexcept
	{
		return  _descriptorIDSet;
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_COMMON_H__
