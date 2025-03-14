
#ifndef  __CPPX_PLATFORM_CONFIG_INTERNAL_ARCH_X86_H__
#define  __CPPX_PLATFORM_CONFIG_INTERNAL_ARCH_X86_H__

#if !defined( PCL_MEMORY_BASE_ALIGNMENT )
#  define PCL_MEMORY_BASE_ALIGNMENT 4
#endif

#define PCL_ENDIANNESS_NATIVE     PCL_ENDIANNESS_LE
#define PCL_ENDIANNESS_NON_NATIVE PCL_ENDIANNESS_BE
#define PCL_TARGET_64             0

#endif /*  __CPPX_PLATFORM_CONFIG_INTERNAL_ARCH_X86_H__ */
