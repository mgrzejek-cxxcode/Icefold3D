
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_COMMON_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_COMMON_H__

#include "CommonGPUStateDefs.h"

namespace Ic3::Graphics::GCI
{

	Ic3GCIDeclareClassHandle( BlendStateDescriptor );
	Ic3GCIDeclareClassHandle( DepthStencilStateDescriptor );
	Ic3GCIDeclareClassHandle( RasterizerStateDescriptor );
	Ic3GCIDeclareClassHandle( GraphicsShaderLinkageDescriptor );
	Ic3GCIDeclareClassHandle( VertexAttributeLayoutDescriptor );
	Ic3GCIDeclareClassHandle( RootSignatureDescriptor );

	Ic3GCIDeclareClassHandle( RenderPassDescriptor );
	Ic3GCIDeclareClassHandle( RenderPassDescriptorDynamic );

	Ic3GCIDeclareClassHandle( RenderTargetDescriptor );
	Ic3GCIDeclareClassHandle( RenderTargetDescriptorDynamic );

	Ic3GCIDeclareClassHandle( VertexSourceBindingDescriptor );
	Ic3GCIDeclareClassHandle( VertexSourceBindingDescriptorDynamic );

	inline constexpr auto kSDIRenderTargetDescriptorScreenDefaultUID = CXU::MakePipelineStateDescriptorIDUserComponent( 0u );

	inline constexpr auto kSDIRenderTargetDescriptorScreenDefault =
			CXU::DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTRenderTarget, kSDIRenderTargetDescriptorScreenDefaultUID );

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

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_COMMON_H__
