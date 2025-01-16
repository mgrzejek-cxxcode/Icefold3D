
#ifndef __IC3_SYSTEM_ASSET_SYSTEM_NATIVE_H__
#define __IC3_SYSTEM_ASSET_SYSTEM_NATIVE_H__

#include "AssetCommon.h"

#if( IC3_SYSTEM_USE_ASSET_SYSTEM_DEFAULT )
#  include "Internal/Platform/Common/FileBasedAssetSystem.h"
#else
#  if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
#    include "Internal/Platform/OSAPI/Android/AndroidAssetSystem.h"
#  endif
#endif

#endif // __IC3_SYSTEM_ASSET_SYSTEM_NATIVE_H__
