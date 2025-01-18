
#ifndef __CPPX_SYNC_COMMON_H__
#define __CPPX_SYNC_COMMON_H__

#include "../prerequisites.h"

namespace cppx::sync
{

	/**
	 *
	 */
	void yield_current_thread();

	/**
	 *
	 */
	PCL_ATTR_ALWAYS_INLINE void hardware_pause()
	{
	#if( PCL_EIS_SUPPORT_HAS_MM_PAUSE )
		_mm_pause();
	#endif
	}

	/**
	 *
	 * @tparam tpBusySpinCount
	 * @tparam tpSpinCountBeforeYield
	 * @param pCounter
	 */
	template <size_t tpBusySpinCount = 8, size_t tpSpinCountBeforeYield = 32>
	PCL_ATTR_ALWAYS_INLINE void yield_current_thread_auto( uint64 pCounter = 0 )
	{
		if( pCounter < tpBusySpinCount )
		{
		}
		else if( pCounter < tpSpinCountBeforeYield )
		{
			hardware_pause();
		}
		else
		{
			yield_current_thread();
		}
	}

} // namespace Ic3::Sync

#endif /* __CPPX_SYNC_COMMON_H__ */
