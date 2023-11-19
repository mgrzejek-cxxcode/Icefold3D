
#ifndef  __IC3_PLATFORM_CONFIG_INTERNAL_COMPILER_CLANG_H__
#define  __IC3_PLATFORM_CONFIG_INTERNAL_COMPILER_CLANG_H__

#include <signal.h>
#include <unistd.h>

#define __cdecl
#define w64
#define gnoexcept noexcept

#define IC3_PCL_FILE  __FILE__
#define IC3_PCL_FUNC  __FUNCTION__
#define IC3_PCL_LINE  __LINE__

#define IC3_PCL_ATTR_ALIGN( n )               alignas( n )
#define IC3_PCL_ATTR_DEPRECATED( msg, repl )  __attribute__(( deprecated( msg,repl ) ))
#define IC3_PCL_ATTR_NO_RETURN                __attribute__(( noreturn ))
#define IC3_PCL_ATTR_THREAD_LOCAL             thread_local

#if( defined( __has_attribute ) && __has_attribute( always_inline ) )
#  define IC3_PCL_ATTR_ALWAYS_INLINE __attribute__(( always_inline )) inline
#else
#  define IC3_PCL_ATTR_ALWAYS_INLINE inline
#endif

#if defined( __has_declspec_attribute )
#  if __has_declspec_attribute( dllexport ) && __has_declspec_attribute( dllimport )
#    define IC3_PCL_ATTR_DLL_EXPORT __declspec( dllexport )
#    define IC3_PCL_ATTR_DLL_IMPORT __declspec( dllimport )
#  elif __has_declspec_attribute( __dllexport__ ) && __has_declspec_attribute( __dllimport__ )
#    define IC3_PCL_ATTR_DLL_EXPORT __declspec( __dllexport__ )
#    define IC3_PCL_ATTR_DLL_IMPORT __declspec( __dllimport__ )
#  else
#    define IC3_PCL_ATTR_DLL_EXPORT
#    define IC3_PCL_ATTR_DLL_IMPORT
#  endif
#else
#  define IC3_PCL_ATTR_DLL_EXPORT
#  define IC3_PCL_ATTR_DLL_IMPORT
#endif

#define IC3_PCL_DEBUG_BREAK()         raise( SIGINT )
#define IC3_PCL_RUNTIME_BREAK()       raise( SIGINT )
#define IC3_PCL_DEBUG_OUTPUT( text )  printf( "%s\n", text )
#define IC3_PCL_SLEEP( miliseconds )  usleep( miliseconds * 1000 )

#define IC3_PCL_BSWAP16 __builtin_bswap16
#define IC3_PCL_BSWAP32 __builtin_bswap32
#define IC3_PCL_BSWAP64 __builtin_bswap64

#define  IC3_PCL_POPCNT16 __builtin_popcount
#define  IC3_PCL_POPCNT32 __builtin_popcountl
#define  IC3_PCL_POPCNT64 __builtin_popcountll

//#define  IC3_PCL_BSCAN_LSB16( x ) __builtin_ffsl( ( long )x )
//#define  IC3_PCL_BSCAN_LSB32( x ) __builtin_ffsl( x )
//#define  IC3_PCL_BSCAN_LSB64( x ) __builtin_ffsll
//
//#define  IC3_PCL_BSCAN_MSB16( x ) __builtin_popcount
//#define  IC3_PCL_BSCAN_MSB32( x ) __builtin_popcountl
//#define  IC3_PCL_BSCAN_MSB64( x ) __builtin_popcountll

#define IC3_PCL_ROTL16( x, n ) ( (x << n) | (x >> (16-n)) )
#define IC3_PCL_ROTL32( x, n ) ( (x << n) | (x >> (32-n)) )
#define IC3_PCL_ROTL64( x, n ) ( (x << n) | (x >> (64-n)) )
#define IC3_PCL_ROTR16( x, n ) ( (x >> n) | (x << (16-n)) )
#define IC3_PCL_ROTR32( x, n ) ( (x >> n) | (x << (32-n)) )
#define IC3_PCL_ROTR64( x, n ) ( (x >> n) | (x << (64-n)) )

#if( IC3_PCL_CPP_VERSION >= IC3_PCL_CPP_VERSION_14 )
#  pragma clang diagnostic ignored "-Wc++17-extensions"
#endif

#pragma clang diagnostic ignored "-W#pragma-messages"
#pragma clang diagnostic ignored "-Wbraced-scalar-init"
#pragma clang diagnostic ignored "-Wdynamic-class-memaccess"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wunused-private-field"
#pragma clang diagnostic ignored "-Wunused-value"
#pragma clang diagnostic ignored "-Wnull-character"

#endif /*  __IC3_PLATFORM_CONFIG_INTERNAL_COMPILER_CLANG_H__ */
