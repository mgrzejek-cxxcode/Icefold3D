
#ifndef  __CPPX_PLATFORM_CONFIG_INTERNAL_COMPILER_GCC_H__
#define  __CPPX_PLATFORM_CONFIG_INTERNAL_COMPILER_GCC_H__

#include <signal.h>
#include <unistd.h>

#define w64
#define gnoexcept noexcept

#define PCL_FILE  __FILE__
#define PCL_FUNC  __FUNCTION__
#define PCL_LINE  __LINE__

#define PCL_TODO( description )

#define PCL_ATTR_ALIGN( n )               alignas(n)
#define PCL_ATTR_DEPRECATED( msg, repl )  __attribute__(( deprecated( msg, repl ) ))
#define PCL_ATTR_DLL_EXPORT               __attribute__(( dllexport ))
#define PCL_ATTR_DLL_IMPORT               __attribute__(( dllimport ))
#define PCL_ATTR_NO_RETURN                __attribute__(( noreturn ))
#define PCL_ATTR_THREAD_LOCAL             __thread
#define PCL_ATTR_ALWAYS_INLINE            __attribute__(( always_inline )) inline

#define PCL_DEBUG_BREAK()         __builtin_trap() // raise( SIGINT )
#define PCL_RUNTIME_BREAK()       __builtin_trap() // raise( SIGABRT )
#define PCL_DEBUG_OUTPUT( text )  printf( "%s\n", text )
#define PCL_SLEEP( miliseconds )  usleep( miliseconds * 1000 )

#define PCL_BSWAP16	__builtin_bswap16
#define PCL_BSWAP32	__builtin_bswap32
#define PCL_BSWAP64	__builtin_bswap64

#define  PCL_POPCNT16	__builtin_popcount
#define  PCL_POPCNT32	__builtin_popcountl
#define  PCL_POPCNT64	__builtin_popcountll

#define PCL_ROTL16( x, n ) ( (x << n) | (x >> (16-n)) )
#define PCL_ROTL32( x, n ) ( (x << n) | (x >> (32-n)) )
#define PCL_ROTL64( x, n ) ( (x << n) | (x >> (64-n)) )
#define PCL_ROTR16( x, n ) ( (x >> n) | (x << (16-n)) )
#define PCL_ROTR32( x, n ) ( (x >> n) | (x << (32-n)) )
#define PCL_ROTR64( x, n ) ( (x >> n) | (x << (64-n)) )

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wmissing-braces"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#endif /*  __CPPX_PLATFORM_CONFIG_INTERNAL_COMPILER_GCC_H__ */
