
#ifndef  __CPPX_PLATFORM_CONFIG_INTERNAL_COMPILER_CLANG_H__
#define  __CPPX_PLATFORM_CONFIG_INTERNAL_COMPILER_CLANG_H__

#include <signal.h>
#include <unistd.h>

#define __cdecl
#define w64
#define gnoexcept noexcept

#define PCL_FILE  __FILE__
#define PCL_FUNC  __FUNCTION__
#define PCL_LINE  __LINE__

#define PCL_ATTR_ALIGN( n )               alignas( n )
#define PCL_ATTR_DEPRECATED( msg, repl )  __attribute__(( deprecated( msg,repl ) ))
#define PCL_ATTR_NO_RETURN                __attribute__(( noreturn ))
#define PCL_ATTR_THREAD_LOCAL             thread_local

#if( defined( __has_attribute ) && __has_attribute( always_inline ) )
#  define PCL_ATTR_ALWAYS_INLINE __attribute__(( always_inline )) inline
#else
#  define PCL_ATTR_ALWAYS_INLINE inline
#endif

#if defined( __has_declspec_attribute )
#  if __has_declspec_attribute( dllexport ) && __has_declspec_attribute( dllimport )
#    define PCL_ATTR_DLL_EXPORT __declspec( dllexport )
#    define PCL_ATTR_DLL_IMPORT __declspec( dllimport )
#  elif __has_declspec_attribute( __dllexport__ ) && __has_declspec_attribute( __dllimport__ )
#    define PCL_ATTR_DLL_EXPORT __declspec( __dllexport__ )
#    define PCL_ATTR_DLL_IMPORT __declspec( __dllimport__ )
#  else
#    define PCL_ATTR_DLL_EXPORT
#    define PCL_ATTR_DLL_IMPORT
#  endif
#else
#  define PCL_ATTR_DLL_EXPORT
#  define PCL_ATTR_DLL_IMPORT
#endif

#define PCL_DEBUG_BREAK()         raise( SIGINT )
#define PCL_RUNTIME_BREAK()       raise( SIGINT )
#define PCL_DEBUG_OUTPUT( text )  printf( "%s\n", text )
#define PCL_SLEEP( miliseconds )  usleep( miliseconds * 1000 )

#define PCL_BSWAP16 __builtin_bswap16
#define PCL_BSWAP32 __builtin_bswap32
#define PCL_BSWAP64 __builtin_bswap64

#define  PCL_POPCNT16 __builtin_popcount
#define  PCL_POPCNT32 __builtin_popcountl
#define  PCL_POPCNT64 __builtin_popcountll

//#define  PCL_BSCAN_LSB16( x ) __builtin_ffsl( ( long )x )
//#define  PCL_BSCAN_LSB32( x ) __builtin_ffsl( x )
//#define  PCL_BSCAN_LSB64( x ) __builtin_ffsll
//
//#define  PCL_BSCAN_MSB16( x ) __builtin_popcount
//#define  PCL_BSCAN_MSB32( x ) __builtin_popcountl
//#define  PCL_BSCAN_MSB64( x ) __builtin_popcountll

#define PCL_ROTL16( x, n ) ( (x << n) | (x >> (16-n)) )
#define PCL_ROTL32( x, n ) ( (x << n) | (x >> (32-n)) )
#define PCL_ROTL64( x, n ) ( (x << n) | (x >> (64-n)) )
#define PCL_ROTR16( x, n ) ( (x >> n) | (x << (16-n)) )
#define PCL_ROTR32( x, n ) ( (x >> n) | (x << (32-n)) )
#define PCL_ROTR64( x, n ) ( (x >> n) | (x << (64-n)) )

#if( PCL_CPP_VERSION >= PCL_CPP_VERSION_14 )
#  pragma clang diagnostic ignored "-Wc++17-extensions"
#endif

#pragma clang diagnostic ignored "-W#pragma-messages"
#pragma clang diagnostic ignored "-Wbraced-scalar-init"
#pragma clang diagnostic ignored "-Wdynamic-class-memaccess"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wunused-private-field"
#pragma clang diagnostic ignored "-Wunused-value"
#pragma clang diagnostic ignored "-Wnull-character"

#endif /*  __CPPX_PLATFORM_CONFIG_INTERNAL_COMPILER_CLANG_H__ */
