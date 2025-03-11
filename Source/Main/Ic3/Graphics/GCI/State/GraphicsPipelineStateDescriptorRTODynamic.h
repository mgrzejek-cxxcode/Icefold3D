
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
	class IC3_GRAPHICS_GCI_CLASS RenderPassDescriptorDynamic : public RTArrayDescriptorDynamicProxy<RenderPassDescriptor, RenderPassAttachmentConfig>
	{
	public:
		Ic3DeclareNonCopyable( RenderPassDescriptorDynamic );

		RenderPassDescriptorDynamic( GPUDevice & pGPUDevice );
		virtual ~RenderPassDescriptorDynamic();

		virtual ERenderPassAttachmentLoadAction GetAttachmentLoadAction( native_uint pAttachmentIndex ) const noexcept override final;

		virtual ERenderPassAttachmentStoreAction GetAttachmentStoreAction( native_uint pAttachmentIndex ) const noexcept override final;

		static TGfxHandle<RenderPassDescriptorDynamic> CreateNew( GPUDevice & pGPUDevice );
	};

	/**
	 * Represents a dynamic render pass configuration state (API-agnostic and modifiable)
	 * @see RenderPassDescriptor
	 */
	class IC3_GRAPHICS_GCI_CLASS RenderTargetDescriptorDynamic : public RTArrayDescriptorDynamicProxy<RenderTargetDescriptor, RenderTargetAttachmentBinding>
	{
	public:
		Ic3DeclareNonCopyable( RenderTargetDescriptorDynamic );

		RenderTargetDescriptorDynamic( GPUDevice & pGPUDevice );
		virtual ~RenderTargetDescriptorDynamic();

		static TGfxHandle<RenderTargetDescriptorDynamic> CreateNew( GPUDevice & pGPUDevice );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_DYNAMIC_H__
