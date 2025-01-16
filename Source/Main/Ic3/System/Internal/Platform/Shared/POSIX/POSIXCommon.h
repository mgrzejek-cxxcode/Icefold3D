
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_POSIX_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_POSIX_COMMON_H__

#include <Ic3/System/Prerequisites.h>

namespace Ic3::System
{

	namespace Platform
	{

		/// [POSIX API] Returns an immutable string literal with the description of a specified errno value.
		const char * PXAQueryErrnoStringByCode( int pErrno );

	}

}

#endif // __IC3_SYSTEM_PLATFORM_SHARED_POSIX_COMMON_H__
