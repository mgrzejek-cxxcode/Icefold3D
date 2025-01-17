
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_COMMAND_SYSTEM_H__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_COMMAND_SYSTEM_H__

#include "VKPrerequisites.h"
#include <Ic3/Graphics/GCI/CommandSystem.h>

namespace Ic3::Graphics::GCI
{

	/// @brief
	class IC3_GX_VKCOMMON_CLASS VKCommandSystem : public CommandSystem
	{
	public:
		explicit VKCommandSystem( VKGpuDevice & pVKGpuDevice );
		virtual ~VKCommandSystem();

		virtual std::unique_ptr<CommandContext> AcquireCommandContext( ECommandContextType pContextType ) override;
		virtual CommandSync SubmitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_COMMAND_SYSTEM_H__
