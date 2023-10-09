
#ifndef __IC3_SYSTEM_ASSET_COMMON_H__
#define __IC3_SYSTEM_ASSET_COMMON_H__

#include "Prerequisites.h"

#if( IC3_PCL_TARGET_OS == IC3_PCL_TARGET_OS_ANDROID )
#  define IC3_SYSTEM_USE_ASSET_SYSTEM_DEFAULT 0
#else
#  define IC3_SYSTEM_USE_ASSET_SYSTEM_DEFAULT 1
#endif

namespace Ic3::System
{

	ic3SysDeclareHandle( Asset );
	ic3SysDeclareHandle( AssetDirectory );
	ic3SysDeclareHandle( AssetLoader );

} // namespace Ic3::System

#endif // __IC3_SYSTEM_ASSET_COMMON_H__
