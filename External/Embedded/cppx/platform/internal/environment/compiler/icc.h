
#ifndef  __CPPX_PLATFORM_CONFIG_INTERNAL_COMPILER_ICC_H__
#define  __CPPX_PLATFORM_CONFIG_INTERNAL_COMPILER_ICC_H__

#define w64 __w64
#define gnoexcept

#define PCL_FILE  __FILE__
#define PCL_FUNC  __FUNCSIG__
#define PCL_LINE  __LINE__

#define PCL_TODO( description ) __pragma( message( description ) )

#define PCL_ATTR_ALIGN( n )               __declspec( align( n ) )
#define PCL_ATTR_DEPRECATED( msg, repl )  __declspec( deprecated )
#define PCL_ATTR_DLL_EXPORT               __declspec( dllexport )
#define PCL_ATTR_DLL_IMPORT               __declspec( dllimport )
#define PCL_ATTR_NO_RETURN                __declspec( noreturn )
#define PCL_ATTR_THREAD_LOCAL             __declspec( thread )
#define PCL_ATTR_ALWAYS_INLINE            __forceinline

#define PCL_DEBUG_BREAK()         __debugbreak()
#define PCL_DEBUG_OUTPUT( text )  utputDebugStringA("%s\n", text)
#define PCL_SLEEP( miliseconds )  leep(miliseconds)

#define PCL_BSWAP16( n )  ( ( ( n & 0xFF00 ) >> 8 ) | ( ( n & 0x00FF ) << 8 ) )
#define PCL_BSWAP32       _bswap
#define PCL_BSWAP64       _bswap64

#define PCL_ROTL16( x, n )  ( ( x << n ) | ( x >> ( 16 - n ) ) )
#define PCL_ROTL32          _rotl
#define PCL_ROTL64          _lrotl

#define PCL_ROTR16( x, n )  ( ( x >> n ) | ( x << ( 16 - n ) ) )
#define PCL_ROTR32          _rotr
#define PCL_ROTR64          _lrotr

#endif /*  __CPPX_PLATFORM_CONFIG_INTERNAL_COMPILER_ICC_H__ */
