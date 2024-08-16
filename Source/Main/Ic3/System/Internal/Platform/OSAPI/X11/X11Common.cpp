
#include "X11SysContext.h"

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		X11SessionData & x11GetXSessionData( SysContext & pSysContext )
		{
			auto * x11SysContext = pSysContext.queryInterface<X11SysContext>();
			return x11SysContext->mNativeData.xSessionData;
		}

	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_X11
