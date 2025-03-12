
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMAND_LIST_GENERIC_RENDER_PASS_H__
#define __IC3_GRAPHICS_GCI_COMMAND_LIST_GENERIC_RENDER_PASS_H__

#include "CommandList.h"
#include "State/GraphicsPipelineStateImplRenderPassGeneric.h"

namespace Ic3::Graphics::GCI
{

	/**
		* A more specialized implementation of the CommandList class for drivers without support for RenderPasses (GL, DX11).
		* This CommandList works with PIM::RenderPassDescriptorGeneric and uses its stored RenderPassConfiguration to manually
		* execute load/store render pass actions whenever a render pass begins/ends.
		*/
	class IC3_GRAPHICS_GCI_CLASS CommandListRenderPassGeneric : public CommandList
	{
	public:
		CommandListRenderPassGeneric(
				CommandSystem & pCommandSystem,
				ECommandListType pListType,
				GraphicsPipelineStateController & pPipelineStateController );

		virtual ~CommandListRenderPassGeneric();

	protected:
		/**
		 * @brief 
		 * @param pRenderPassConfiguration 
		 * @param pDynamicConfig 
		 */
		virtual void ExecuteRenderPassLoadActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig ) = 0;

		/**
		 * @brief 
		 * @param pRenderPassConfiguration 
		 * @param pDynamicConfig 
		 */
		virtual void ExecuteRenderPassStoreActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig ) = 0;

		/**
		 * @copybrief CommandList::ValidateActiveRenderPass
		 */
		virtual bool ValidateActiveRenderPass() const noexcept override;

		/**
		 * @copybrief CommandList::ValidateActiveRenderPass
		 */
		virtual bool OnBeginRenderPass(
				const RenderPassDescriptor & pRenderPassDescriptor,
				cppx::bitmask<ECommandListActionFlags> pFlags ) override;

		/**
		 * @copybrief CommandList::ValidateActiveRenderPass
		 */
		virtual bool OnBeginRenderPassDynamic(
				RenderPassDescriptorDynamic & pRenderPassDescriptor,
				cppx::bitmask<ECommandListActionFlags> pFlags ) override;

		/**
		 * @copybrief CommandList::ValidateActiveRenderPass
		 */
		virtual void OnEndRenderPass() override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMAND_LIST_GENERIC_RENDER_PASS_H__
