
#include "syncCommon.h"
#include <thread>

namespace Ic3::Sync
{

	void hardwarePause()
	{
	#if( IC3_PCL_EIS_SUPPORT_HAS_MM_PAUSE )
		_mm_pause();
	#endif
	}

	void yieldCurrentThread()
	{
		std::this_thread::yield();
	}

	void yieldCurrentThreadAuto( uint64_t pCounter )
	{
		constexpr uint64_t sBusySpinCount = 12;
		constexpr uint64_t sSpinCountBeforeYield = 48;

		if( pCounter < sBusySpinCount )
		{
		}
		else if( pCounter < sSpinCountBeforeYield )
		{
			hardwarePause();
		}
		else
		{
			yieldCurrentThread();
		}
	}

}
