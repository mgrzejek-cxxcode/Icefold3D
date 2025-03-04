
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMAND_LIST_GENERIC_RENDER_PASS_H__
#define __IC3_GRAPHICS_GCI_COMMAND_LIST_GENERIC_RENDER_PASS_H__

#include "CommandList.h"
#include "State/GraphicsPipelineStateImplGenericRenderPass.h"

namespace Ic3::Graphics::GCI
{

	namespace PIM
	{

		/**
		 * A more specialized implementation of the CommandList class for drivers without support for RenderPasses (GL, DX11).
		 * This CommandList works with PIM::RenderPassDescriptorGeneric and uses its stored RenderPassConfiguration to manually
		 * execute load/store render pass actions whenever a render pass begins/ends.
		 */
		class CommandListGenericRenderPass : public CommandList
		{
		public:
			CommandListGenericRenderPass(
					CommandSystem & pCommandSystem,
					ECommandListType pListType,
					GraphicsPipelineStateController & pPipelineStateController )
			: CommandList( pCommandSystem, pListType, pPipelineStateController )
			{}

			virtual ~CommandListGenericRenderPass() = default;

		protected:
			virtual void ExecuteRenderPassLoadActions(
					const RenderPassConfiguration & pRenderPassConfiguration,
					const GraphicsPipelineDynamicConfig & pDynamicConfig ) = 0;

			virtual void ExecuteRenderPassStoreActions(
					const RenderPassConfiguration & pRenderPassConfiguration,
					const GraphicsPipelineDynamicConfig & pDynamicConfig ) = 0;

			virtual bool ValidateActiveRenderPass() const noexcept override
			{
				return CommandList::ValidateActiveRenderPass() && _currentRenderPassConfiguration;
			}

			virtual bool OnBeginRenderPass(
					const RenderPassDescriptor & pRenderPassDescriptor,
					cppx::bitmask<ECommandListActionFlags> pFlags ) override
			{
				if( CommandList::OnBeginRenderPass( pRenderPassDescriptor, pFlags ) )
				{
					const auto * genericRenderPassDescriptor = pRenderPassDescriptor.QueryInterface<RenderPassDescriptorGeneric>();

					_currentRenderPassConfiguration = &( genericRenderPassDescriptor->mPassConfiguration );

					ExecuteRenderPassLoadActions(
							*_currentRenderPassConfiguration,
							_graphicsPipelineStateController->GetPipelineDynamicConfig() );

					return true;
				}

				return false;
			}

			virtual bool OnBeginRenderPassDynamic(
					RenderPassDescriptorDynamic & pRenderPassDescriptor,
					cppx::bitmask<ECommandListActionFlags> pFlags ) override
			{
				if( CommandList::OnBeginRenderPassDynamic( pRenderPassDescriptor, pFlags ) )
				{
					const auto * dynamicRenderPassDescriptor = pRenderPassDescriptor.QueryInterface<RenderPassDescriptorDynamic>();

					_currentRenderPassConfiguration = &( dynamicRenderPassDescriptor->GetRenderTargetArrayConfiguration() );

					ExecuteRenderPassLoadActions(
							*_currentRenderPassConfiguration,
							_graphicsPipelineStateController->GetPipelineDynamicConfig() );

					return true;
				}

				return false;
			}

			virtual void OnEndRenderPass() override
			{
				ValidateActiveRenderPassOrThrow();

				ExecuteRenderPassStoreActions(
						*_currentRenderPassConfiguration,
						_graphicsPipelineStateController->GetPipelineDynamicConfig() );

				_currentRenderPassConfiguration = nullptr;

				CommandList::OnEndRenderPass();
			}

			CPPX_ATTR_NO_DISCARD const RenderPassConfiguration & GetCurrentRenderPassGenericConfiguration() const noexcept
			{
				ValidateActiveRenderPassOrThrow();
				return *_currentRenderPassConfiguration;
			}

		private:
			const RenderPassConfiguration * _currentRenderPassConfiguration = nullptr;
		};

	} // namespace PIM

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMAND_LIST_GENERIC_RENDER_PASS_H__
