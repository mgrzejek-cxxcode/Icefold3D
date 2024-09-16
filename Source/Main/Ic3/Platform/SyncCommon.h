
#ifndef __IC3_PLATFORM_SYNC_COMMON_H__
#define __IC3_PLATFORM_SYNC_COMMON_H__

#include "Platform.h"

namespace Ic3::Sync
{

	/// @brief
	void hardwarePause();

	/// @brief
	void yieldCurrentThread();

	/// @brief
	/// @param pCounter
	void yieldCurrentThreadAuto( uint64_t pCounter = 0 );

} // namespace Ic3::Sync

#endif /* __IC3_PLATFORM_SYNC_COMMON_H__ */
