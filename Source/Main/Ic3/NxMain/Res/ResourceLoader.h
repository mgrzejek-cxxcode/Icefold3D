
#pragma once

#ifndef __IC3_NXMAIN_RES_RESOURCE_LOADER_H__
#define __IC3_NXMAIN_RES_RESOURCE_LOADER_H__

#include "../Prerequisites.h"

namespace Ic3
{

	class ResourceLoader
	{
	protected:
		GCI::CommandContext & getGPUCommandContext() const
		{
			if( !_gpuCommandContext )
			{
				throw 0;
			}
			return *_gpuCommandContext;
		}

	private:
		GCI::CommandContext * _gpuCommandContext;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_RES_RESOURCE_LOADER_H__
