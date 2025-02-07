
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_H__

#include "PipelineStateCommon.h"
#include "RenderPassCommon.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * Represents render pass configuration state that controls operations executed on RT attachments when an RP begins/ends.
	 * @see RenderPassConfigStateDescriptorNative
	 * @see RenderPassConfigStateDescriptorDynamic
	 */
	class RenderPassConfigStateDescriptor : public PipelineStateDescriptor
	{
	public:
		RenderPassConfigStateDescriptor(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID );

		virtual ~RenderPassConfigStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

	/**
	 * Represents a dynamic render pass configuration state (API-agnostic and modifiable)
	 * @see RenderPassConfigStateDescriptor
	 */
	class RenderPassConfigStateDescriptorDynamic : public RenderPassConfigStateDescriptor
	{
	public:
		Ic3DeclareNonCopyable( RenderPassConfigStateDescriptorDynamic );

		RenderPassConfigStateDescriptorDynamic(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto );

		~RenderPassConfigStateDescriptorDynamic();

		/**
		 * @see GraphicsPipelineStateDescriptor::IsDynamicDescriptor()
		 */
		CPPX_ATTR_NO_DISCARD virtual bool IsDynamicDescriptor() const noexcept override final;

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsDepthStencilAttachmentActive() const noexcept;

		CPPX_ATTR_NO_DISCARD native_uint CountActiveColorAttachments() const noexcept;

		CPPX_ATTR_NO_DISCARD const RenderPassConfiguration & GetRenderPassConfiguration() const noexcept;

		RenderPassAttachmentReference * UpdateActiveColorAttachment( native_uint pColorAttachmentIndex );

		RenderPassAttachmentReference * UpdateActiveDepthStencilAttachment();

		void SetRenderPassConfiguration( const RenderPassConfiguration & pRenderPassConfiguration ) noexcept;

		void SetColorAttachmentRef(
				native_uint pColorAttachmentIndex,
				const RenderPassAttachmentReference & pAttachmentRef );

		void SetColorAttachmentRefs(
				const cppx::array_view<const RenderPassAttachmentReference> & pAttachmentRefs,
				native_uint pColorAttachmentFirstIndex = 0,
				native_uint pColorAttachmentCount = cppx::cve::native_uint_max );

		void SetDepthStencilAttachmentRef( const RenderPassAttachmentReference & pAttachmentRef );

		void ResetAll();

		void ResetAllFlags();

		void ResetColorAttachmentRef( native_uint pColorAttachmentIndex );

		void ResetColorAttachmentRefs(
				native_uint pColorAttachmentFirstIndex = 0,
				native_uint pColorAttachmentCount = cppx::cve::native_uint_max );

		void ResetDepthStencilAttachmentRef();

		TGfxHandle<RenderPassConfigStateDescriptorDynamic> CreateFromPassConfiguration(
				const RenderPassConfiguration & pRenderPassConfiguration );

	private:
		RenderPassAttachmentReference * _SetColorAttachmentActive( native_uint pColorAttachmentIndex );

		void _SetColorAttachmentRefs(
				native_uint pColorAttachmentFirstIndex,
				native_uint pColorAttachmentCount,
				const RenderPassAttachmentReference * pAttachmentRefs );

		RenderPassAttachmentReference * _SetDepthStencilAttachmentActive();

		void _SetDepthStencilAttachmentRef( const RenderPassAttachmentReference & pAttachmentRef );

		void _ResetColorAttachmentRefs( native_uint pColorAttachmentFirstIndex, native_uint pColorAttachmentCount );

		void _ResetDepthStencilAttachmentRef();

	private:
		RenderPassConfiguration _renderPassConfiguration;
	};

	/// namespace PIM: Private Implementation
	namespace PIM
	{

		/**
		 * Represents a static render pass configuration state for APIs without explicit support for render passes functionality.
		 * This type of RP descriptor simply wraps RenderPassConfiguration object, used to execute required attachments actions.
		 * @note This descriptor type is created automatically by compatible APIs when an RP descriptor is requested.
		 * @see RenderPassConfigStateDescriptor
		 */
		class RenderPassConfigStateDescriptorGeneric : public RenderPassConfigStateDescriptor
		{
		public:
			RenderPassConfiguration const mPassConfiguration;

		public:
			RenderPassConfigStateDescriptorGeneric(
					GPUDevice & pGPUDevice,
					pipeline_state_descriptor_id_t pDescriptorID,
					const RenderPassConfiguration & pPassConfiguration );

			virtual ~RenderPassConfigStateDescriptorGeneric();

			static TGfxHandle<RenderPassConfigStateDescriptorGeneric> CreateFromPassConfiguration(
					GPUDevice & pGPUDevice,
					const RenderPassConfiguration & pPassConfiguration,
					pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto );
		};

		/**
		 * Represents a static render pass configuration state translated into the API-specific format.
		 * @note This descriptor type is created automatically by compatible APIs when an RP descriptor is requested.
		 * @see RenderPassConfigStateDescriptor
		 */
		class RenderPassConfigStateDescriptorNative : public RenderPassConfigStateDescriptor
		{
			Ic3DeclareNonCopyable( RenderPassConfigStateDescriptorNative );

		public:
			RenderPassConfigStateDescriptorNative(
					GPUDevice & pGPUDevice,
					pipeline_state_descriptor_id_t pDescriptorID );

			virtual ~RenderPassConfigStateDescriptorNative();
		};

	} // namespace PIM

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_H__
