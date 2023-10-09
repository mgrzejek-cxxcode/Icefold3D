
#ifndef __IC3_MATH_PREREQUISITES_H__
#define __IC3_MATH_PREREQUISITES_H__

#include <Ic3/Cppx/Prerequisites.h>
#include <cmath>

#if( IC3_PCL_FEATURE_EIS )
#  define IC3_MATH_SIMD_ENABLE 1
#  if( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_SSE ) // SSE -> float[4] (__m128)
#    define IC3_MATH_SIMD_USE_VX128F 1
#  else
#    define IC3_MATH_SIMD_USE_VX128F 0
#  endif
#  if( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_SSE2 ) // SSE2 -> int32[4] (__m128i)
#    define IC3_MATH_SIMD_USE_VX128I 1
#  else
#    define IC3_MATH_SIMD_USE_VX128I 0
#  endif
#  if( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_AVX ) // AVX -> double[4] (__m256d)
#    define IC3_MATH_SIMD_USE_VX256D 1
#  else
#    define IC3_MATH_SIMD_USE_VX256D 0
#  endif
#  if( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_AVX2 ) // AVX2 -> int64[4] (__m256i)
#    define IC3_MATH_SIMD_USE_VX256I 1
#  else
#    define IC3_MATH_SIMD_USE_VX256I 0
#  endif
#endif

namespace Ic3::Math
{

	template <typename TValue, size_t tSize>
	using Array = TValue[tSize];

	template <typename TValue>
	using Array2 = Array<TValue, 2>;

	template <typename TValue>
	using Array3 = Array<TValue, 3>;

	template <typename TValue>
	using Array4 = Array<TValue, 4>;

	template <typename TScalar>
	using enable_if_scalar_t = std::enable_if_t<std::is_arithmetic<TScalar>::value, bool>;

	namespace Constants
	{

		constexpr float FLT_PI           = 3.1415926536f;
		constexpr float FLT_PI_DIV2      = 1.5707963268f;
		constexpr float FLT_PI_DIV4      = 0.7853981634f;
		constexpr float FLT_E            = 2.7182818285f;
		constexpr float FLT_LOG2E        = 1.4426950409f;
		constexpr float FLT_LOG10E       = 0.4342944819f;
		constexpr float FLT_LN2          = 0.6931471806f;
		constexpr float FLT_LN10         = 2.3025850930f;
		constexpr float FLT_1_OVER_PI    = 0.3183098862f;
		constexpr float FLT_2_OVER_PI    = 0.6366197724f;
		constexpr float FLT_SQRT_2       = 1.4142135624f;
		constexpr float FLT_SQRT_PI      = 1.7724538509f;
		constexpr float FLT_RAD_1DEG     = 0.0174532925f;   // 1 degree in radians
		constexpr float FLT_RAD_15DEG    = 0.2617993878f;   // 15 degrees in radians
		constexpr float FLT_RAD_30DEG    = 0.5235987756f;   // 30 degrees in radians
		constexpr float FLT_RAD_45DEG    = 0.7853981634f;   // 45 degrees in radians
		constexpr float FLT_RAD_60DEG    = 1.0471975512f;   // 60 degrees in radians
		constexpr float FLT_RAD_90DEG    = 1.5707963268f;   // 90 degrees in radians
		constexpr float FLT_RAD_180DEG   = 3.1415926536f;   // 180 degrees in radians
		constexpr float FLT_RAD_360DEG   = 6.2831853072f;   // 360 degrees in radians
		constexpr float FLT_DEG_1RAD     = 57.2957795131f;  // 1 radian in degrees
		constexpr float FLT_DEG_2RAD     = 114.5915590262f; // 2 radians in degrees
		constexpr float FLT_DEG_3RAD     = 171.8873385392f; // 3 radians in degrees

		constexpr double DBL_PI           = 3.14159265358979323846;
		constexpr double DBL_PI_DIV2      = 1.57079632679489661923;
		constexpr double DBL_PI_DIV4      = 0.78539816339744830961;
		constexpr double DBL_E            = 2.71828182845904523536;
		constexpr double DBL_LOG2E        = 1.44269504088896340736;
		constexpr double DBL_LOG10E       = 0.43429448190325182765;
		constexpr double DBL_LN2          = 0.69314718055994530941;
		constexpr double DBL_LN10         = 2.30258509299404568402;
		constexpr double DBL_1_OVER_PI    = 0.31830988618379067153;
		constexpr double DBL_2_OVER_PI    = 0.63661977236758134307;
		constexpr double DBL_SQRT_2       = 1.41421356237309504880;
		constexpr double DBL_SQRT_PI      = 1.77245385090551602729;
		constexpr double DBL_RAD_1DEG     = 0.01745329251994329576;   // 1 degree in radians
		constexpr double DBL_RAD_15DEG    = 0.26179938779914943653;   // 15 degrees in radians
		constexpr double DBL_RAD_30DEG    = 0.52359877559829887307;   // 30 degrees in radians
		constexpr double DBL_RAD_45DEG    = 0.78539816339744830961;   // 45 degrees in radians
		constexpr double DBL_RAD_60DEG    = 1.04719755119659774615;   // 60 degrees in radians
		constexpr double DBL_RAD_90DEG    = 1.57079632679489661923;   // 90 degrees in radians
		constexpr double DBL_RAD_180DEG   = 3.14159265358979323846;   // 180 degrees in radians
		constexpr double DBL_RAD_360DEG   = 6.28318530717958647692;   // 360 degrees in radians
		constexpr double DBL_DEG_1RAD     = 57.29577951308232087679;  // 1 radian in degrees
		constexpr double DBL_DEG_2RAD     = 114.59155902616464175359; // 2 radians in degrees
		constexpr double DBL_DEG_3RAD     = 171.88733853924696263039; // 3 radians in degrees

	}

}

#endif // __IC3_MATH_PREREQUISITES_H__
