
#ifndef  __IC3_PLATFORM_CONFIG_INTERNAL_COMPILER_ICC_H__
#define  __IC3_PLATFORM_CONFIG_INTERNAL_COMPILER_ICC_H__

#define w64 __w64
#define gnoexcept

#define IC3_PCL_FILE  __FILE__
#define IC3_PCL_FUNC  __FUNCSIG__
#define IC3_PCL_LINE  __LINE__

#define IC3_PCL_TODO( description ) __pragma( message( description ) )

#define IC3_PCL_ATTR_ALIGN( n )               __declspec( align( n ) )
#define IC3_PCL_ATTR_DEPRECATED( msg, repl )  __declspec( deprecated )
#define IC3_PCL_ATTR_DLL_EXPORT               __declspec( dllexport )
#define IC3_PCL_ATTR_DLL_IMPORT               __declspec( dllimport )
#define IC3_PCL_ATTR_NO_RETURN                __declspec( noreturn )
#define IC3_PCL_ATTR_THREAD_LOCAL             __declspec( thread )
#define IC3_PCL_ATTR_ALWAYS_INLINE            __forceinline

#define IC3_PCL_DEBUG_BREAK()         __debugbreak()
#define IC3_PCL_DEBUG_OUTPUT( text )  utputDebugStringA("%s\n", text)
#define IC3_PCL_SLEEP( miliseconds )  leep(miliseconds)

#define IC3_PCL_BYTESWAP16( n )  ( ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8) )
#define IC3_PCL_BYTESWAP32       _bswap
#define IC3_PCL_BYTESWAP64       _bswap64

#define IC3_PCL_ROTL16( x, n )  ( (x << n) | (x >> (16-n)) )
#define IC3_PCL_ROTL32          _rotl
#define IC3_PCL_ROTL64          _lrotl

#define IC3_PCL_ROTR16( x, n )  ( (x >> n) | (x << (16-n)) )
#define IC3_PCL_ROTR32          _rotr
#define IC3_PCL_ROTR64          _lrotr

#endif /*  __IC3_PLATFORM_CONFIG_INTERNAL_COMPILER_ICC_H__ */
