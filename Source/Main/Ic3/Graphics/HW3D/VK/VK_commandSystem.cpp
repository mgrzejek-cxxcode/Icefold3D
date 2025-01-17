
#include "VK_commandSystem.h"
#include "VK_commandList.h"
#include "VKUDevice.h"
#include <Ic3/Graphics/GCI/CommandContext.h>

namespace Ic3::Graphics::GCI
{

	VKCommandSystem::VKCommandSystem( VKGpuDevice & pVKGpuDevice )
	: CommandSystem( pVKGpuDevice )
	{ }

	VKCommandSystem::~VKCommandSystem() = default;

	std::unique_ptr<CommandContext> VKCommandSystem::AcquireCommandContext( ECommandContextType pContextType )
	{
		std::unique_ptr<CommandContext> commandContext;

		auto contextExecutionMode = ecGetCommandContextExecutionMode( pContextType );
		// if( auto * commandList = AcquireCommandList( contextExecutionMode ) )
		{
			if( contextExecutionMode == ECommandExecutionMode::Direct )
			{
				// commandContext.reset( new CommandContextDirectGraphics( *this, *commandList ) );
			}
			else
			{
				// commandContext.reset( new CommandContextDeferredGraphics( *this, *commandList ) );
			}
		}

		return commandContext;
	}

	CommandSync VKCommandSystem::SubmitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo )
	{
		CommandSync cmdSyncObject;

		if( pSubmitInfo.syncMode == ECommandSubmitSyncMode::Default )
		{
		}

		return cmdSyncObject;
	}

} // namespace Ic3::Graphics::GCI
