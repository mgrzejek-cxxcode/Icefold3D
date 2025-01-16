
#include "POSIXPerfCounter.h"

#if( PCL_TARGET_OS & PCL_TARGET_FLAG_OS_POSIX )
namespace Ic3::System
{

	perf_counter_value_t NativePerfCounterQueryValue()
	{
		timespec timeSpec {};
		clock_gettime( CLOCK_MONOTONIC, &timeSpec );
		return static_cast<uint64>( timeSpec.tv_sec ) * 1000000000u + static_cast<uint64>( timeSpec.tv_nsec );
	}

	perf_counter_res_t NativePerfCounterQueryResolution()
	{
		return 1000000000u;
	}

} // namespace Ic3::System
#endif
