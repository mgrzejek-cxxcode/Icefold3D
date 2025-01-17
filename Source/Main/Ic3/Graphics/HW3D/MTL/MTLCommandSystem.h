
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_COMMAND_SYSTEM_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_COMMAND_SYSTEM_H__

#include "MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/CommandSystem.h>

namespace Ic3::Graphics::GCI
{

	/// @brief
	class MetalCommandSystem : public CommandSystem
	{
	public:
		explicit MetalCommandSystem( MetalGPUDevice & pMetalGPUDevice );
		virtual ~MetalCommandSystem();

		virtual std::unique_ptr<CommandContext> AcquireCommandContext( ECommandContextType pContextType ) override;

		virtual CommandSync SubmitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo ) override;

	private:
		MetalCommandList * AcquireCommandList( ECommandExecutionMode pCommandExecutionMode );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_COMMAND_SYSTEM_H__
