
#include "X11SysContext.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		X11SessionData & X11GetXSessionData( SysContext & pSysContext )
		{
			auto * x11SysContext = pSysContext.QueryInterface<X11SysContext>();
			return x11SysContext->mNativeData.mXSessionData;
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_X11
