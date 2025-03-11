
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_GENERIC_RENDER_PASS_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_GENERIC_RENDER_PASS_H__

#include "GraphicsPipelineStateController.h"
#include "GraphicsPipelineStateDescriptorRTODynamic.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * Represents a static render pass configuration state for APIs without explicit support for render passes functionality.
	 * This type of RP descriptor simply wraps RenderPassConfiguration object, used to execute required attachments actions.
	 * @note This descriptor type is created automatically by compatible APIs when an RP descriptor is requested.
	 * @see RenderPassDescriptor
	 */
	class IC3_GRAPHICS_GCI_CLASS RenderPassDescriptorGeneric : public RenderPassDescriptor
	{
	public:
		RenderPassConfiguration const mPassConfiguration;

	public:
		RenderPassDescriptorGeneric(
				GPUDevice & pGPUDevice,
				const RenderPassConfiguration & pRenderPassConfiguration );

		virtual ~RenderPassDescriptorGeneric();

		CPPX_ATTR_NO_DISCARD virtual bool IsAttachmentActive( native_uint pAttachmentIndex ) const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual bool CheckAttachmentLoadActionFlags(
				uint32 pAttachmentIndex,
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual bool CheckAttachmentStoreActionFlags(
				uint32 pAttachmentIndex,
				cppx::bitmask<ERenderPassAttachmentActionFlags> pActionFlags ) const noexcept override final;

		static TGfxHandle<RenderPassDescriptorGeneric> CreateInstance(
				GPUDevice & pGPUDevice,
				const RenderPassDescriptorCreateInfo & pCreateInfo,
				const std::initializer_list<ERenderPassAttachmentActionFlags> & pCachedAttachmentsWithFlags );

		static TGfxHandle<RenderPassDescriptorGeneric> CreateInstance(
				GPUDevice & pGPUDevice,
				const RenderPassDescriptorCreateInfo & pCreateInfo );
	};


	/**
	 *
	 * @tparam TPBaseStateController
	 */
	template<typename TPBaseStateController>
	class GraphicsPipelineStateControllerGenericRenderPass : public TPBaseStateController
	{
		friend class CommandListGenericRenderPass;

	public:
		template <typename... TPArgs>
		GraphicsPipelineStateControllerGenericRenderPass( TPArgs && ...pArgs )
		: TPBaseStateController( std::forward<TPArgs>( pArgs )... )
		{}

		virtual ~GraphicsPipelineStateControllerGenericRenderPass() = default;

		virtual bool SetRenderPassDescriptor( const RenderPassDescriptor & pRenderPassDescriptor ) override
		{
			const auto * genericRenderPassDescriptor = pRenderPassDescriptor.QueryInterface<RenderPassDescriptorGeneric>();

			const auto baseResult = GraphicsPipelineStateController::SetRenderPassDescriptor( *genericRenderPassDescriptor );

			if( baseResult )
			{
				_currentRenderPassConfig = &( genericRenderPassDescriptor->mPassConfiguration );
			}

			return baseResult;
		}

		virtual bool SetRenderPassDescriptorDynamic( RenderPassDescriptorDynamic & pRenderPassDescriptor ) override
		{
			const auto baseResult = GraphicsPipelineStateController::SetRenderPassDescriptorDynamic( pRenderPassDescriptor );

			if( baseResult )
			{
				_currentRenderPassConfig = &( pRenderPassDescriptor.GetRenderTargetArrayConfiguration() );
			}

			return baseResult;
		}

		virtual void ResetRenderPassDescriptor() override
		{
			GraphicsPipelineStateController::ResetRenderPassDescriptor();
			_currentRenderPassConfig = nullptr;
		}

		const RenderPassConfiguration * GetCurrentRenderPassConfiguration() const noexcept
		{
			return _currentRenderPassConfig;
		}

		const RenderPassConfiguration & GetCurrentRenderPassConfigurationRef() const
		{
			if( !_currentRenderPassConfig )
			{
				throw 0;
			}

			return *_currentRenderPassConfig;
		}

	private:
		const RenderPassConfiguration * _currentRenderPassConfig = nullptr;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_GENERIC_RENDER_PASS_H__

