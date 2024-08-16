
#ifndef  __IC3_PLATFORM_CONFIG_INTERNAL_COMPILER_GCC_H__
#define  __IC3_PLATFORM_CONFIG_INTERNAL_COMPILER_GCC_H__

#include <signal.h>
#include <unistd.h>

#define w64
#define gnoexcept noexcept

#define IC3_PCL_FILE  __FILE__
#define IC3_PCL_FUNC  __FUNCTION__
#define IC3_PCL_LINE  __LINE__

#define IC3_PCL_TODO( description )

#define IC3_PCL_ATTR_ALIGN( n )               alignas(n)
#define IC3_PCL_ATTR_DEPRECATED( msg, repl )  __attribute__(( deprecated( msg, repl ) ))
#define IC3_PCL_ATTR_DLL_EXPORT               __attribute__(( dllexport ))
#define IC3_PCL_ATTR_DLL_IMPORT               __attribute__(( dllimport ))
#define IC3_PCL_ATTR_NO_RETURN                __attribute__(( noreturn ))
#define IC3_PCL_ATTR_THREAD_LOCAL             __thread
#define IC3_PCL_ATTR_ALWAYS_INLINE            __attribute__(( always_inline )) inline

#define IC3_PCL_DEBUG_BREAK()         __builtin_trap() // raise( SIGINT )
#define IC3_PCL_RUNTIME_BREAK()       __builtin_trap() // raise( SIGABRT )
#define IC3_PCL_DEBUG_OUTPUT( text )  printf( "%s\n", text )
#define IC3_PCL_SLEEP( miliseconds )  usleep( miliseconds * 1000 )

#define IC3_PCL_BSWAP16	__builtin_bswap16
#define IC3_PCL_BSWAP32	__builtin_bswap32
#define IC3_PCL_BSWAP64	__builtin_bswap64

#define  IC3_PCL_POPCNT16	__builtin_popcount
#define  IC3_PCL_POPCNT32	__builtin_popcountl
#define  IC3_PCL_POPCNT64	__builtin_popcountll

#define IC3_PCL_ROTL16( x, n ) ( (x << n) | (x >> (16-n)) )
#define IC3_PCL_ROTL32( x, n ) ( (x << n) | (x >> (32-n)) )
#define IC3_PCL_ROTL64( x, n ) ( (x << n) | (x >> (64-n)) )
#define IC3_PCL_ROTR16( x, n ) ( (x >> n) | (x << (16-n)) )
#define IC3_PCL_ROTR32( x, n ) ( (x >> n) | (x << (32-n)) )
#define IC3_PCL_ROTR64( x, n ) ( (x >> n) | (x << (64-n)) )

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wmissing-braces"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#endif /*  __IC3_PLATFORM_CONFIG_INTERNAL_COMPILER_GCC_H__ */
