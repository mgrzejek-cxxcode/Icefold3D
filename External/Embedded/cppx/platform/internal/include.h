
#ifndef __CPPX_PLATFORM_CONFIG_INCLUDE_H__
#define __CPPX_PLATFORM_CONFIG_INCLUDE_H__

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

#if( PCL_EIS_SUPPORT_LEVEL & PCL_EIS_FEATURE_NEON )
#  include <arm_neon.h>
#elif( PCL_EIS_SUPPORT_LEVEL & PCL_EIS_FEATURE_AVX )
#  include <immintrin.h>
#elif( PCL_EIS_SUPPORT_LEVEL & PCL_EIS_FEATURE_SSE42 )
#  include <smmintrin.h>
#elif( PCL_EIS_SUPPORT_LEVEL & PCL_EIS_FEATURE_SSE41 )
#  include <nmmintrin.h>
#elif( PCL_EIS_SUPPORT_LEVEL & PCL_EIS_FEATURE_SSE4A )
#  include <ammintrin.h>
#elif( PCL_EIS_SUPPORT_LEVEL & PCL_EIS_FEATURE_SSE3 )
#  include <pmmintrin.h>
#elif( PCL_EIS_SUPPORT_LEVEL & PCL_EIS_FEATURE_SSE2 )
#  include <emmintrin.h>
#elif( PCL_EIS_SUPPORT_LEVEL & PCL_EIS_FEATURE_SSE )
#  include <xmmintrin.h>
#endif

#endif /* __CPPX_PLATFORM_CONFIG_INCLUDE_H__ */
