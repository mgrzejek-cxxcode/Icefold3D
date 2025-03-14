
#include "PerfCounter.h"

namespace Ic3::System
{

	perf_counter_value_t NativePerfCounterQueryValue();
	perf_counter_res_t NativePerfCounterGetResolution();

	perf_counter_value_t PerfCounter::QueryCounter()
	{
		return NativePerfCounterQueryValue();
	}

	perf_counter_res_t PerfCounter::GetResolution()
	{
		return NativePerfCounterGetResolution();
	}

	double PerfCounter::ConvertToDuration( perf_counter_value_t pStampDiff, const perf_counter_ratio_t & pUnitRatio )
	{
		// This is the resolution of the perf counter. It yields a value representing the number
		// of "ticks" per second. This is needed to compute the time from the stamp difference.
		auto perfCounterResolution = GetResolution();

		// Now:
		// > NumberOfSeconds = pStampDiff / perfCounterResolution
		// The result unit is specified as pUnitRatio. ::first is the numerator and ::second - denominator
		// of a ratio representing the result time unit with seconds as a relative base unit. So we have:
		// > Duration = NumberOfSeconds / (pUnitRatio.first / pUnitRatio.second)
		// > Duration = (pStampDiff / perfCounterResolution) / (pUnitRatio.first / pUnitRatio.second)
		// To keep the maximum precision we obviously want to change middle division to multiplication by inversion:
		// > Duration = (pStampDiff / perfCounterResolution) * (pUnitRatio.second / pUnitRatio.first)
		// > Duration = (pStampDiff * pUnitRatio.second) / (perfCounterResolution * pUnitRatio.first)

		const auto resultNum = static_cast<double>( pStampDiff ) * pUnitRatio.second;
		const auto resultDenom = static_cast<double>( perfCounterResolution ) * pUnitRatio.first;

		return resultNum / resultDenom;
	}

	perf_counter_value_t PerfCounter::ConvertFromDuration( double pDuration, const perf_counter_ratio_t & pUnitRatio )
	{
		auto perfCounterResolution = GetResolution();

		// Looking at the last line of the above calculations:
		// > Duration = (pStampDiff * pUnitRatio.second) / (perfCounterResolution * pUnitRatio.first)
		// We can simply get the formula for computing pStampDiff (this is the perf counter stamp diff)
		// from Duration (this is the 'pDuration' input parameter to this very function). So we have:
		// > pStampDiff = (Duration * perfCounterResolution * pUnitRatio.first) / pUnitRatio.second

		const auto resultNum = static_cast<double>( pDuration ) * perfCounterResolution * pUnitRatio.first;
		const auto resultDenom = static_cast<double>( pUnitRatio.second );

		return static_cast<perf_counter_value_t>( resultNum / resultDenom );
	}

} // namespace Ic3::System
