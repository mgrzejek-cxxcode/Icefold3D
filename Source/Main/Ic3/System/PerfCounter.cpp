
#include "PerfCounter.h"

namespace Ic3::System
{

	perf_counter_value_t NativePerfCounterQueryValue();
	perf_counter_res_t NativePerfCounterQueryResolution();

	perf_counter_value_t PerfCounter::QueryCurrentStamp()
	{
		return NativePerfCounterQueryValue();
	}

	perf_counter_res_t PerfCounter::QueryResolution()
	{
		return NativePerfCounterQueryResolution();
	}

	long double PerfCounter::ConvertToDuration( perf_counter_value_t pStampDiff, const perf_counter_ratio_t & pUnitRatio )
	{
		// This is the resolution of the perf counter. It yields a value representing the number
		// of "ticks" per second. This is needed to compute the time from the stamp difference.
		auto perfCounterResolution = QueryResolution();

		// Now:
		// > NumberOfSeconds = pStampDiff / perfCounterResolution
		// The result unit is specified as pUnitRatio. ::first is the numerator and ::second - denominator
		// of a ratio representing the result time unit with seconds as a relative base unit. So we have:
		// > Duration = NumberOfSeconds / (pUnitRatio.first / pUnitRatio.second)
		// > Duration = (pStampDiff / perfCounterResolution) / (pUnitRatio.first / pUnitRatio.second)
		// To keep the maximum precision we obviously want to change middle division to multiplication by inversion:
		// > Duration = (pStampDiff / perfCounterResolution) * (pUnitRatio.second / pUnitRatio.first)
		// > Duration = (pStampDiff * pUnitRatio.second) / (perfCounterResolution * pUnitRatio.first)

		auto resultNum = static_cast<long double>( pStampDiff ) * pUnitRatio.second;
		auto resultDenom = static_cast<long double>( perfCounterResolution ) * pUnitRatio.first;

		return resultNum / resultDenom;
	}

	perf_counter_value_t PerfCounter::ConvertFromDuration( long double pDuration, const perf_counter_ratio_t & pUnitRatio )
	{
		auto perfCounterResolution = QueryResolution();

		// Looking at the last line of the above calculations:
		// > Duration = (pStampDiff * pUnitRatio.second) / (perfCounterResolution * pUnitRatio.first)
		// We can simply get the formula for computing pStampDiff (this is the perf counter stamp diff)
		// from Duration (this is the 'pDuration' input parameter to this very function). So we have:
		// > pStampDiff = (Duration * perfCounterResolution * pUnitRatio.first) / pUnitRatio.second

		auto resultNum = static_cast<long double>( pDuration ) * perfCounterResolution * pUnitRatio.first;
		auto resultDenom = static_cast<long double>( pUnitRatio.second );

		return static_cast<perf_counter_value_t>( resultNum / resultDenom );
	}

} // namespace Ic3::System
