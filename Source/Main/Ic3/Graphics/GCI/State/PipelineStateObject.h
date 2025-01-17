
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_OBJECT_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_OBJECT_H__

#include "GraphicsPipelineImmutableState.h"
#include "InputAssemblerCommon.h"
#include "GraphicsShaderState.h"
#include "RenderTargetCommon.h"
#include "ShaderInputSignature.h"

namespace Ic3::Graphics::GCI
{

	struct ComputePipelineStateObjectCreateInfo
	{
	};

	struct GraphicsPipelineStateObjectCreateInfo
	{
		BlendConfig blendConfig;
		mutable BlendImmutableStateHandle blendState;

		DepthStencilConfig depthStencilConfig;
		mutable DepthStencilImmutableStateHandle depthStencilState;

		RasterizerConfig rasterizerConfig;
		mutable RasterizerImmutableStateHandle rasterizerState;

		GraphicsShaderSet shaderSet;
		mutable GraphicsShaderLinkageImmutableStateHandle shaderLinkageState;

		IAInputLayoutDefinition inputLayoutDefinition;
		mutable IAInputLayoutImmutableStateHandle inputLayoutState;

		ShaderInputSignatureDesc shaderInputSignatureDesc;
		mutable ShaderInputSignature shaderInputSignature;

		RenderTargetLayout renderTargetLayout;

	};

	/// @brief A monolithic state object, containing subset of a certain GPU pipeline state.
	/// PipelineStateObject serves as a base class for all concrete PSO subtypes. There are two main types of PSOs:
	/// - Compute PSO for the async compute pipeline. Required for executing Dispatch*() calls.
	/// - Graphics PSO for the graphics/Rendering pipeline. Required for executing all kind of Draw*() calls.
	/// Both have their dedicated classes: ComputePipelineStateObject and GraphicsPipelineStateObject, respectively.
	class IC3_GRAPHICS_GCI_CLASS PipelineStateObject : public GpuDeviceChildObject
	{
	public:
		PipelineStateObject( GpuDevice & pGpuDevice );
		virtual ~PipelineStateObject();
	};

	/// @brief PSO for the compute pipeline.
	class IC3_GRAPHICS_GCI_CLASS ComputePipelineStateObject : public PipelineStateObject
	{
	public:
		ComputePipelineStateObject( GpuDevice & pGpuDevice );
		virtual ~ComputePipelineStateObject();
	};

	/// @brief PSO for the graphics pipeline.
	/// Contains a set of state required by the rendering pipeline, stored in either generic or driver-specific format.
	/// Graphics PSO contains the following state (see also GraphicsPipelineStateObjectCreateInfo):
	/// 1) Generic state (stored in the common format):
	/// > render target configuration
	/// > shader binding state
	/// > shader input signature
	/// 2) Driver-specific state (created at the driver level, translated to its native form):
	/// > blend state
	/// > depth/stencil state
	/// > rasterizer state
	/// > vertex input format
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObject : public PipelineStateObject
	{
	public:
		RenderTargetLayout const mRenderTargetLayout;
		ShaderInputSignature const mShaderInputSignature;

		GraphicsPipelineStateObject(
			GpuDevice & pGpuDevice,
			RenderTargetLayout pRenderTargetLayout,
			ShaderInputSignature pShaderInputSignature );

		virtual ~GraphicsPipelineStateObject();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_OBJECT_H__
