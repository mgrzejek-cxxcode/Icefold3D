
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMAND_SYSTEM_H__
#define __IC3_GRAPHICS_GCI_COMMAND_SYSTEM_H__

#include "CommonCommandDefs.h"
#include <unordered_map>

namespace Ic3::Graphics::GCI
{

	class IC3_GRAPHICS_GCI_CLASS CommandSystem : public GpuDeviceChildObject
	{
		friend class CommandContextDirect;

	public:
		explicit CommandSystem( GpuDevice & pGpuDevice );
		virtual ~CommandSystem();

		virtual std::unique_ptr<CommandContext> AcquireCommandContext( ECommandContextType pContextType ) = 0;

		virtual CommandSync SubmitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo ) = 0;

		template <typename TContext>
		std::unique_ptr<TContext> AcquireCommandContext()
		{
			return MoveInterfaceUniquePtr<TContext>( AcquireCommandContext( TContext::sContextType ) );
		}
		
		bool SetQueueAlias( gpu_cmd_device_queue_id_t pAliasID, gpu_cmd_device_queue_id_t pMappedID );

		bool RemoveQueueAlias( gpu_cmd_device_queue_id_t pAliasID );

		bool CheckQueueAlias( gpu_cmd_device_queue_id_t pAliasID ) const;

		gpu_cmd_device_queue_id_t ResolveQueueID( gpu_cmd_device_queue_id_t pQueueID ) const;

	private:
		using DeviceQueueAliasMap = std::unordered_map<gpu_cmd_device_queue_id_t, gpu_cmd_device_queue_id_t>;
		DeviceQueueAliasMap _deviceQueueAliasMap;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMAND_SYSTEM_H__
