
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_H__

#include "PipelineStateDescriptor.h"
#include "RenderPassCommon.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * @brief Common base class for descriptors that represent configuration related to RTA (Render Target Array).
	 * 
	 * This class serves as a base type for descriptors that configure different aspects of the pipeline related
	 * to RT attachments. Its current implementations are:
	 * - RenderPassDescriptor: configures attachments actions during a single render pass
	 * - RenderTargetDescriptor: defines resources bound to the RTA binding points
	 */
	class IC3_GRAPHICS_GCI_CLASS RenderTargetArrayConfigStateDescriptor : public PipelineStateDescriptor
	{
	public:
		RenderTargetArrayConfigStateDescriptor( GPUDevice & pGPUDevice );
		virtual ~RenderTargetArrayConfigStateDescriptor();

		/**
		 * @brief Returns true if the resource at the specified index is configured, or false otherwise.
		 * 
		 * RenderTargetArrayConfigStateDescriptor-based implementations configure an array of RT attachments.
		 * This functions returns true if the current descriptor has a valid configuration set for the specified
		 * attachments. This can have different meanings for different descriptors, for example:
		 * - RenderPassDescriptor: an attachment is configured if either load or store action (or both) are set
		 *   (and enabled) for the given attachment.
		 * - RenderTargetDescriptor: an attachment is configured if an RT texture is bound to the RT array for
		 *   the specified attachment.
		 * 
		 * @param pAttachmentIndex 
		 * @return True if the resource at the specified index is configured, or false otherwise.
		 */
		CPPX_ATTR_NO_DISCARD virtual bool IsAttachmentConfigured( native_uint pAttachmentIndex ) const noexcept = 0;
	};

	/**
	 * @brief Represents render pass configuration state that controls operations executed on RT attachments when an RP begins/ends.
	 * 
	 * RenderPassDescriptor is a non-cached descriptor which means they are created directly by the descriptor factory and each
	 * one is managed solely by the owner (no additional references are stored). RenderPassDescriptor is also available in a dynamic
	 * form, as RenderPassDescriptorDynamic.
	 * @see RenderPassDescriptorDynamic
	 */
	class IC3_GRAPHICS_GCI_CLASS RenderPassDescriptor : public NonCachedPipelineStateDescriptor<RenderTargetArrayConfigStateDescriptor>
	{
	public:
		RenderPassDescriptor( GPUDevice & pGPUDevice );
		virtual ~RenderPassDescriptor();

		/**
		 * @brief Implements PipelineStateDescitpro::GetDescriptorType.
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;

		/**
		 * @brief 
		 * @param pAttachmentIndex 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD virtual ERenderPassAttachmentLoadAction GetAttachmentLoadAction( native_uint pAttachmentIndex ) const noexcept = 0;

		/**
		 * @brief 
		 * @param pAttachmentIndex 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD virtual ERenderPassAttachmentStoreAction GetAttachmentStoreAction( native_uint pAttachmentIndex ) const noexcept = 0;

		/**
		 * @brief 
		 * 
		 * @param pAttachmentIndex 
		 * @param pActionFlags 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD bool CheckAttachmentLoadActionFlags(
				native_uint pAttachmentIndex,
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept;

		/**
		 * @brief 
		 * @param pAttachmentIndex 
		 * @param pActionFlags 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD bool CheckAttachmentStoreActionFlags(
				native_uint pAttachmentIndex,
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept;

		/**
		 * @brief 
		 * 
		 * @param pAttachmentIndex 
		 * @param pLoadAction 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD bool CheckAttachmentLoadAction(
				native_uint pAttachmentIndex,
				ERenderPassAttachmentLoadAction pLoadAction ) const noexcept;

		/**
		 * @brief 
		 * @param pAttachmentIndex 
		 * @param pStoreAction 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD bool CheckAttachmentStoreAction(
				native_uint pAttachmentIndex,
				ERenderPassAttachmentStoreAction pStoreAction ) const noexcept;
	};

	/**
	 * @brief Represents render target array binding, i.e. set of RT-enabled textures/buffers used outputs for rendering.
	 * 
	 * RenderTargetDescriptor is a non-cached descriptor which means they are created directly by the descriptor factory and each
	 * one is managed solely by the owner (no additional references are stored). RenderTargetDescriptor is also available in a dynamic
	 * form, as RenderTargetDescriptorDynamic.
	 * @see RenderTargetDescriptorDynamic
	 */
	class IC3_GRAPHICS_GCI_CLASS RenderTargetDescriptor : public NonCachedPipelineStateDescriptor<RenderTargetArrayConfigStateDescriptor>
	{
	public:
		RenderTargetDescriptor( GPUDevice & pGPUDevice );
		virtual ~RenderTargetDescriptor();

		/**
		 * @brief Implements PipelineStateDescitpro::GetDescriptorType.
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_H__
