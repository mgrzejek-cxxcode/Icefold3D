
#include "CommandSystem.h"
#include "GpuDevice.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3::Graphics::GCI
{

	CommandSystem::CommandSystem( GpuDevice & pGpuDevice )
	: GpuDeviceChildObject( pGpuDevice )
	{}

	CommandSystem::~CommandSystem() = default;

	bool CommandSystem::SetQueueAlias( gpu_cmd_device_queue_id_t pAliasID, gpu_cmd_device_queue_id_t pMappedID )
	{
		if((pAliasID == eDeviceCommandQueueIdUnknown ) || (pMappedID == eDeviceCommandQueueIdUnknown ) )
		{
			return false;
		}
		auto aliasIter = _deviceQueueAliasMap.find( pAliasID );
		if( aliasIter == _deviceQueueAliasMap.end() )
		{
			_deviceQueueAliasMap[pAliasID] = pMappedID;
			return true;
		}
		return false;
	}

	bool CommandSystem::RemoveQueueAlias( gpu_cmd_device_queue_id_t pAliasID )
	{
		auto aliasIter = _deviceQueueAliasMap.find( pAliasID );
		if( aliasIter != _deviceQueueAliasMap.end() )
		{
			_deviceQueueAliasMap.erase( aliasIter );
			return true;
		}
		return false;
	}

	bool CommandSystem::CheckQueueAlias( gpu_cmd_device_queue_id_t pAliasID ) const
	{
		return ResolveQueueID( pAliasID ) != pAliasID;
	}

	gpu_cmd_device_queue_id_t CommandSystem::ResolveQueueID( gpu_cmd_device_queue_id_t pQueueID ) const
	{
		// Check if the specified ID is an alias. If so, the actual queue ID is the resolved name.
		auto resolvedID = cppx::get_map_value_ref_or_default( _deviceQueueAliasMap, pQueueID, eDeviceCommandQueueIdUnknown );
		if( resolvedID == eDeviceCommandQueueIdUnknown )
		{
			// If no such alias could be found, we assume this is a direct ID of a queue.
			resolvedID = pQueueID;
		}
		return resolvedID;
	}

} // namespace Ic3::Graphics::GCI
