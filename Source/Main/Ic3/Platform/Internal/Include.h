
#ifndef __IC3_PLATFORM_CONFIG_INCLUDE_H__
#define __IC3_PLATFORM_CONFIG_INCLUDE_H__

// C Library
#include <cstdlib>
#include <cstddef>
#include <cstdint>

// POSIX
#include <sys/types.h>
#include <sys/stat.h>

//
#include <algorithm>
#include <array>
#include <utility>
#include <vector>

#if( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_NEON )
#  include <arm_neon.h>
#elif( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_AVX )
#  include <immintrin.h>
#elif( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_SSE42 )
#  include <smmintrin.h>
#elif( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_SSE41 )
#  include <nmmintrin.h>
#elif( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_SSE4A )
#  include <ammintrin.h>
#elif( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_SSE3 )
#  include <pmmintrin.h>
#elif( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_SSE2 )
#  include <emmintrin.h>
#elif( IC3_PCL_EIS_SUPPORT_LEVEL & IC3_PCL_EIS_FEATURE_SSE )
#  include <xmmintrin.h>
#endif

#endif /* __IC3_PLATFORM_CONFIG_INCLUDE_H__ */
