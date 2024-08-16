
#include "AndroidSysContext.h"

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	namespace Platform
	{

		ASessionData & androidGetASessionData( SysContext & pSysContext )
		{
			auto * androidSysContext = pSysContext.queryInterface<AndroidSysContext>();
			return androidSysContext->mNativeData.aSessionData;
		}

	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_ANDROID
