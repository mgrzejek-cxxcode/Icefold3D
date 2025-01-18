
#include "AndroidSysContext.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	namespace Platform
	{

		ASessionData & AndroidGetASessionData( SysContext & pSysContext )
		{
			auto * androidSysContext = pSysContext.QueryInterface<AndroidSysContext>();
			return androidSysContext->mNativeData.aSessionData;
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_ANDROID
