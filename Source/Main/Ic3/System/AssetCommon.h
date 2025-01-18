
#ifndef __IC3_SYSTEM_ASSET_COMMON_H__
#define __IC3_SYSTEM_ASSET_COMMON_H__

#include "Prerequisites.h"

#if( PCL_TARGET_OS == PCL_TARGET_OS_ANDROID )
#  define IC3_SYSTEM_USE_ASSET_SYSTEM_DEFAULT 0
#else
#  define IC3_SYSTEM_USE_ASSET_SYSTEM_DEFAULT 1
#endif

namespace Ic3::System
{

	Ic3SysDeclareHandle( Asset );
	Ic3SysDeclareHandle( AssetDirectory );
	Ic3SysDeclareHandle( AssetLoader );

} // namespace Ic3::System

#endif // __IC3_SYSTEM_ASSET_COMMON_H__
