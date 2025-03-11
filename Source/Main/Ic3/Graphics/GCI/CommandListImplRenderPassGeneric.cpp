
#include "CommandListImplRenderPassGeneric.h"
#include "State/GraphicsPipelineStateImplRenderPassGeneric.h"

namespace Ic3::Graphics::GCI
{

	CommandListRenderPassGeneric::CommandListRenderPassGeneric(
			CommandSystem & pCommandSystem,
			ECommandListType pListType,
			GraphicsPipelineStateController & pPipelineStateController )
	: CommandList( pCommandSystem, pListType, pPipelineStateController )
	{}

	CommandListRenderPassGeneric::~CommandListRenderPassGeneric() = default;

	bool CommandListRenderPassGeneric::ValidateActiveRenderPass() const noexcept
	{
		auto * stateControllerRenderPassGeneric = GetStateController()->QueryInterface<GraphicsPipelineStateControllerRenderPassGenericBase>();
		return CommandList::ValidateActiveRenderPass() && stateControllerRenderPassGeneric->IsCurrentRenderPassConfigurationSet();
	}

	bool CommandListRenderPassGeneric::OnBeginRenderPass(
			const RenderPassDescriptor & pRenderPassDescriptor,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		if( CommandList::OnBeginRenderPass( pRenderPassDescriptor, pFlags ) )
		{
			auto * stateControllerRenderPassGeneric = GetStateController()->QueryInterface<GraphicsPipelineStateControllerRenderPassGenericBase>();

			ExecuteRenderPassLoadActions(
				stateControllerRenderPassGeneric->GetCurrentRenderPassConfigurationRef(),
				_graphicsPipelineStateController->GetPipelineDynamicConfig() );

			return true;
		}

		return false;
	}

	bool CommandListRenderPassGeneric::OnBeginRenderPassDynamic(
			RenderPassDescriptorDynamic & pRenderPassDescriptor,
			cppx::bitmask<ECommandListActionFlags> pFlags )
	{
		if( CommandList::OnBeginRenderPassDynamic( pRenderPassDescriptor, pFlags ) )
		{
			auto * stateControllerRenderPassGeneric = GetStateController()->QueryInterface<GraphicsPipelineStateControllerRenderPassGenericBase>();

			ExecuteRenderPassLoadActions(
				stateControllerRenderPassGeneric->GetCurrentRenderPassConfigurationRef(),
				_graphicsPipelineStateController->GetPipelineDynamicConfig() );

			return true;
		}

		return false;
	}

	void CommandListRenderPassGeneric::OnEndRenderPass()
	{
		ValidateActiveRenderPassOrThrow();

		auto * stateControllerRenderPassGeneric = GetStateController()->QueryInterface<GraphicsPipelineStateControllerRenderPassGenericBase>();

		ExecuteRenderPassStoreActions(
			stateControllerRenderPassGeneric->GetCurrentRenderPassConfigurationRef(),
			_graphicsPipelineStateController->GetPipelineDynamicConfig() );

		CommandList::OnEndRenderPass();
	}

} // namespace Ic3::Graphics::GCI
