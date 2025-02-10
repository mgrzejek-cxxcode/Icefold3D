
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_COMMON_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_COMMON_H__

#include "CommonGPUStateDefs.h"

namespace Ic3::Graphics::GCI
{

	/*
	 *
	 */
	class PipelineStateDescriptor : public GPUDeviceChildObject
	{
	public:
		pipeline_state_descriptor_id_t const mDescriptorID;

	public:
		PipelineStateDescriptor( GPUDevice & pGPUDevice, pipeline_state_descriptor_id_t pDescriptorID );
		virtual ~PipelineStateDescriptor();

		/**
		 * Queries the dynamic type of the descriptor, expressed as one of the possible values defined in
		 * EPipelineStateDescriptorType enumeration. Implemented by the child descriptor classes.
		 * @return The dynamic type of this descriptor instance.
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept;

		/**
		 * Returns true if this descriptor is a dynamic descriptor. Dynamic descriptor is a descriptor class
		 * that does not contain any API-specific properties (i.e. it is defined fully at the GCI abstraction level).
		 * The following classes are implementations of dynamic descriptors:
		 * - IAVertexStreamStateDescriptorDynamic
		 * - RenderPassConfigurationStateDescriptorDynamic
		 * - RenderTargetBindingStateDescriptorDynamic
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD virtual bool IsDynamicDescriptor() const noexcept;
	};

	/*
	 * @brief A monolithic state object, containing complete state information for the relative pipeline functionality.
	 * PipelineStateObject serves as a base class for all concrete PSO subtypes. There are two main types of PSOs:
	 * - Compute PSO for the async compute pipeline. Required for executing Dispatch*() calls.
	 * - Graphics PSO for the graphics/rendering pipeline. Required for executing all kind of Draw*() calls.
	 * Both have their dedicated classes: ComputePipelineStateObject and GraphicsPipelineStateObject, respectively.
	 */
	class IC3_GRAPHICS_GCI_CLASS PipelineStateObject : public GPUDeviceChildObject
	{
	public:
		pipeline_state_object_id_t const mStateObjectID;

	public:
		PipelineStateObject( GPUDevice & pGPUDevice, pipeline_state_object_id_t pStateObjectID );
		virtual ~PipelineStateObject();

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineType GetPipelineType() const noexcept = 0;
	};

	/**
	 *
	 */
	class PipelineStateDescriptorFactory
	{
	public:
		PipelineStateDescriptorFactory() = default;
		virtual ~PipelineStateDescriptorFactory() = default;

		virtual GPUDevice & GetGPUDevice() const noexcept = 0;

		virtual BlendStateDescriptorHandle CreateBlendStateDescriptor(
				const BlendStateDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				const RasterizerStateDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual GraphicsShaderLinkageStateDescriptorHandle CreateGraphicsShaderLinkageStateDescriptor(
				const GraphicsShaderLinkageStateDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual IAVertexAttributeLayoutStateDescriptorHandle CreateIAVertexAttributeLayoutStateDescriptor(
				const IAVertexAttributeLayoutStateDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual ShaderRootSignatureStateDescriptorHandle CreateShaderRootSignatureStateDescriptor(
				const ShaderRootSignatureStateDescriptorCreateInfo & pCreateInfo) = 0;

		virtual IAVertexStreamBindingStateDescriptorHandle CreateIAVertexStreamBindingStateDescriptor(
				const IAVertexStreamBindingStateDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual RenderPassConfigurationStateDescriptorHandle CreateRenderPassConfigurationStateDescriptor(
				const RenderPassConfigurationStateDescriptorCreateInfo & pCreateInfo ) = 0;
	};

	/**
	 *
	 */
	class PipelineStateDescriptorFactoryNull : public PipelineStateDescriptorFactory
	{
	public:
		PipelineStateDescriptorFactoryNull() = default;
		virtual ~PipelineStateDescriptorFactoryNull() = default;

		virtual GPUDevice & GetGPUDevice() const noexcept override final;

		virtual BlendStateDescriptorHandle CreateBlendStateDescriptor(
				const BlendStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				const RasterizerStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual GraphicsShaderLinkageStateDescriptorHandle CreateGraphicsShaderLinkageStateDescriptor(
				const GraphicsShaderLinkageStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual IAVertexAttributeLayoutStateDescriptorHandle CreateIAVertexAttributeLayoutStateDescriptor(
				const IAVertexAttributeLayoutStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual ShaderRootSignatureStateDescriptorHandle CreateShaderRootSignatureStateDescriptor(
				const ShaderRootSignatureStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual IAVertexStreamBindingStateDescriptorHandle CreateIAVertexStreamBindingStateDescriptor(
				const IAVertexStreamBindingStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RenderPassConfigurationStateDescriptorHandle CreateRenderPassConfigurationStateDescriptor(
				const RenderPassConfigurationStateDescriptorCreateInfo & pCreateInfo ) override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_COMMON_H__
