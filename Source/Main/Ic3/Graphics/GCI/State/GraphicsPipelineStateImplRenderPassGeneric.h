
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_RENDER_PASS_GENERIC_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_RENDER_PASS_GENERIC_H__

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
	class IC3_GRAPHICS_GCI_CLASS RenderPassDescriptorGeneric : public GCIPipelineStateDescriptor<RenderPassDescriptor>
	{
	public:
		RenderPassConfiguration const mPassConfiguration;

	public:
		RenderPassDescriptorGeneric(
				GPUDevice & pGPUDevice,
				const RenderPassConfiguration & pRenderPassConfiguration );

		virtual ~RenderPassDescriptorGeneric();

		/**
		 * @copybrief RenderTargetArrayConfigStateDescriptor::IsAttachmentConfigured
		 */
		virtual bool IsAttachmentConfigured( native_uint pAttachmentIndex ) const noexcept override final;

		/**
		 * @copybrief RenderPassDescriptor::GetAttachmentLoadAction
		 */
		virtual ERenderPassAttachmentLoadAction GetAttachmentLoadAction( native_uint pAttachmentIndex ) const noexcept override final;

		/**
		 * @copybrief RenderPassDescriptor::GetAttachmentStoreAction
		 */
		virtual ERenderPassAttachmentStoreAction GetAttachmentStoreAction( native_uint pAttachmentIndex ) const noexcept override final;

		static TGfxHandle<RenderPassDescriptorGeneric> CreateInstance(
				GPUDevice & pGPUDevice,
				const RenderPassDescriptorCreateInfo & pCreateInfo,
				const std::initializer_list<ERenderPassAttachmentActionFlags> & pCachedAttachmentsWithFlags );

		static TGfxHandle<RenderPassDescriptorGeneric> CreateInstance(
				GPUDevice & pGPUDevice,
				const RenderPassDescriptorCreateInfo & pCreateInfo );
	};

	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateControllerRenderPassGenericBase
	{
	public:
		const RenderPassConfiguration * GetCurrentRenderPassConfiguration() const noexcept
		{
			return _currentRenderPassConfiguration;
		}

		const RenderPassConfiguration & GetCurrentRenderPassConfigurationRef() const
		{
			if( !_currentRenderPassConfiguration )
			{
				throw 0;
			}

			return *_currentRenderPassConfiguration;
		}

		bool IsCurrentRenderPassConfigurationSet() const noexcept
		{
			return _currentRenderPassConfiguration != nullptr;
		}

	protected:
		void SetCurrentRenderPassConfiguration( const RenderPassConfiguration & pRenderPassConfiguration )
		{
			_currentRenderPassConfiguration = &pRenderPassConfiguration;
		}
		
		void ResetCurrentRenderPassConfiguration()
		{
			_currentRenderPassConfiguration = nullptr;
		}

	private:
		const RenderPassConfiguration * _currentRenderPassConfiguration = nullptr;
	};

	/**
	 * @brief Implements GraphicsPipelineStateController behaviour for drivers without explicit API-level render passes support (GL4, DX11).
	 * 
	 * GraphicsPipelineStateControllerRenderPassGeneric can be injected into inheritance chain by HW3D descriptors that use generic render passes.
	 * It implements RP handling by simply setting a generic representation of RP configuration for later usage during rendering commands.
	 * 
	 * @tparam TPBaseStateController
	 */
	template<typename TPBaseStateController>
	class GraphicsPipelineStateControllerRenderPassGeneric : public GraphicsPipelineStateControllerRenderPassGenericBase, public TPBaseStateController
	{
		friend class CommandListRenderPassGeneric;

	public:
		template <typename... TPArgs>
		GraphicsPipelineStateControllerRenderPassGeneric( TPArgs && ...pArgs )
		: TPBaseStateController( std::forward<TPArgs>( pArgs )... )
		{}

		virtual ~GraphicsPipelineStateControllerRenderPassGeneric() = default;

		/**
		 * @copybrief GraphicsPipelineStateController::SetRenderPassDescriptor
		 */
		virtual bool SetRenderPassDescriptor( const RenderPassDescriptor & pRenderPassDescriptor ) override
		{
			// Query the RenderPassDescriptorGeneric pointer, so we can validate if a proper descriptor type is used.
			// This generally cannot be broken, because drivers control internally the creation process. If this triggers,
			// it menas the driver implementation is broken and it mixes generic RP state controller with a custom RP descriptor (or vice versa).
			const auto * genericRenderPassDescriptor = pRenderPassDescriptor.QueryInterface<RenderPassDescriptorGeneric>();

			const auto baseResult = TPBaseStateController::SetRenderPassDescriptor( *genericRenderPassDescriptor );

			if( baseResult )
			{
				// Dynamic RP descriptors expose the generic RP configuration as a public member.
				// Retrieve the config and save it.
				SetCurrentRenderPassConfiguration( genericRenderPassDescriptor->mPassConfiguration );
			}

			return baseResult;
		}

		/**
		 * @copybrief GraphicsPipelineStateController::SetRenderPassDescriptorDynamic
		 */
		virtual bool SetRenderPassDescriptorDynamic( RenderPassDescriptorDynamic & pRenderPassDescriptor ) override
		{
			const auto baseResult = TPBaseStateController::SetRenderPassDescriptorDynamic( pRenderPassDescriptor );

			if( baseResult )
			{
				// Dynamic RP descriptors are simple wrappers over the generic RP configuration.
				// Retrieve the config and save it.
				SetCurrentRenderPassConfiguration( pRenderPassDescriptor.GetRenderTargetArrayConfiguration() );
			}

			return baseResult;
		}

		virtual void ResetRenderPassDescriptor() override
		{
			ResetCurrentRenderPassConfiguration();

			GraphicsPipelineStateController::ResetRenderPassDescriptor();
		}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_RENDER_PASS_GENERIC_H__

