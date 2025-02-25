
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_DYNAMIC_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_DYNAMIC_H__

#include "PipelineStateCommon.h"
#include "GraphicsPipelineStateDescriptorRTArrayCommon.h"
#include "GraphicsPipelineStateDescriptorRTO.h"
#include "RenderPassCommon.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * Represents a dynamic render pass configuration state (API-agnostic and modifiable)
	 * @see RenderPassDescriptor
	 */
	class RenderPassDescriptorDynamic : public RTArrayDescriptorDynamicProxy<RenderPassDescriptor, RenderPassAttachmentConfig>
	{
	public:
		Ic3DeclareNonCopyable( RenderPassDescriptorDynamic );

		RenderPassDescriptorDynamic(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto );

		virtual ~RenderPassDescriptorDynamic();

		static TGfxHandle<RenderPassDescriptorDynamic> CreateNew( GPUDevice & pGPUDevice );
	};

	/**
	 * Represents a dynamic render pass configuration state (API-agnostic and modifiable)
	 * @see RenderPassDescriptor
	 */
	class RenderTargetDescriptorDynamic : public RTArrayDescriptorDynamicProxy<RenderTargetDescriptor, RenderTargetAttachmentBinding>
	{
	public:
		Ic3DeclareNonCopyable( RenderTargetDescriptorDynamic );

		RenderTargetDescriptorDynamic(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto );

		virtual ~RenderTargetDescriptorDynamic();

		static TGfxHandle<RenderTargetDescriptorDynamic> CreateNew( GPUDevice & pGPUDevice );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_DYNAMIC_H__
