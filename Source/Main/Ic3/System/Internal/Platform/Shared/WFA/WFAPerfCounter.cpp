
#include <Ic3/System/PerfCounter.h>
#include "WFAPerfCounter.h"

#if( PCL_TARGET_OS & PCL_TARGET_FLAG_OS_WFA )
namespace Ic3::System
{

	perf_counter_value_t NativePerfCounterQueryValue()
	{
		LARGE_INTEGER perfCounter;
		QueryPerformanceCounter( &perfCounter );

		return static_cast<perf_counter_value_t>( perfCounter.QuadPart );
	}

	perf_counter_res_t NativePerfCounterGetResolution()
	{
		// "The frequency of the performance counter is fixed at system boot and is consistent across all processors.
		// Therefore, the frequency need only be queried upon application initialization, and the result can be cached."
		// Source: https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
		static perf_counter_res_t perfCounterFrequency = 0;

		if( perfCounterFrequency == 0 )
		{
			LARGE_INTEGER counterFrequency;
			QueryPerformanceFrequency( &counterFrequency );
			perfCounterFrequency = static_cast<perf_counter_res_t>( counterFrequency.QuadPart );
		}

		return perfCounterFrequency;
	}

} // namespace Ic3::System
#endif
