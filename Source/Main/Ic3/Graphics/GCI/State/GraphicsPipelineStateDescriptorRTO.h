
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_H__

#include "PipelineStateDescriptor.h"
#include "RenderPassCommon.h"

namespace Ic3::Graphics::GCI
{

	class IC3_GRAPHICS_GCI_CLASS RenderTargetArrayConfigStateDescriptor : public PipelineStateDescriptor
	{
	public:
		RenderTargetArrayConfigStateDescriptor(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID );

		virtual ~RenderTargetArrayConfigStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual bool IsAttachmentActive( native_uint pAttachmentIndex ) const noexcept = 0;
	};

	/**
	 * Represents render pass configuration state that controls operations executed on RT attachments when an RP begins/ends.
	 * @see RenderPassDescriptorNative
	 * @see RenderPassDescriptorDynamic
	 */
	class IC3_GRAPHICS_GCI_CLASS RenderPassDescriptor : public RenderTargetArrayConfigStateDescriptor
	{
	public:
		RenderPassDescriptor(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID );

		virtual ~RenderPassDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual bool CheckAttachmentLoadActionFlags(
				uint32 pAttachmentIndex,
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual bool CheckAttachmentStoreActionFlags(
				uint32 pAttachmentIndex,
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept = 0;

		CPPX_ATTR_NO_DISCARD bool CheckAttachmentLoadAction(
				uint32 pAttachmentIndex,
				ERenderPassAttachmentLoadAction pAction ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool CheckAttachmentStoreAction(
				uint32 pAttachmentIndex,
				ERenderPassAttachmentLoadAction pAction ) const noexcept;
	};

	/**
	 */
	class IC3_GRAPHICS_GCI_CLASS RenderTargetDescriptor : public RenderTargetArrayConfigStateDescriptor
	{
	public:
		RenderTargetDescriptor(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID );

		virtual ~RenderTargetDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

	/// namespace PIM: Private Implementation
	namespace PIM
	{


		/**
		 * Represents a static render pass configuration state translated into the API-specific format.
		 * @note This descriptor type is created automatically by compatible APIs when an RP descriptor is requested.
		 * @see RenderPassDescriptor
		 */
		class IC3_GRAPHICS_GCI_CLASS RenderPassDescriptorNative : public RenderPassDescriptor
		{
			Ic3DeclareNonCopyable( RenderPassDescriptorNative );

		public:
			RenderPassDescriptorNative(
					GPUDevice & pGPUDevice,
					pipeline_state_descriptor_id_t pDescriptorID );

			virtual ~RenderPassDescriptorNative();
		};

		/**
		 */
		class IC3_GRAPHICS_GCI_CLASS RenderTargetDescriptorGeneric : public RenderTargetDescriptor
		{
		public:
			RenderTargetBinding const mRenderTargetBinding;

		public:
			RenderTargetDescriptorGeneric(
					GPUDevice & pGPUDevice,
					pipeline_state_descriptor_id_t pDescriptorID,
					const RenderTargetBinding & pTargetBinding );

			virtual ~RenderTargetDescriptorGeneric();

			CPPX_ATTR_NO_DISCARD virtual bool IsAttachmentActive( native_uint pAttachmentIndex ) const noexcept override final;

			static TGfxHandle<RenderTargetDescriptorGeneric> CreateFromRenderTargetBinding(
					GPUDevice & pGPUDevice,
					const RenderTargetBinding & pRenderTargetBinding,
					pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto );
		};

		/**
		 */
		class IC3_GRAPHICS_GCI_CLASS RenderTargetDescriptorNative : public RenderTargetDescriptor
		{
			Ic3DeclareNonCopyable( RenderTargetDescriptorNative );

		public:
			RenderTargetDescriptorNative(
					GPUDevice & pGPUDevice,
					pipeline_state_descriptor_id_t pDescriptorID );

			virtual ~RenderTargetDescriptorNative();
		};

	} // namespace PIM

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RTO_H__
