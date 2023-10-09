
#ifndef  __IC3_PLATFORM_CONFIG_INTERNAL_OS_LINUX_H__
#define  __IC3_PLATFORM_CONFIG_INTERNAL_OS_LINUX_H__

#include <dlfcn.h>
#include <cinttypes>
#include <csignal>

#undef major
#undef minor

#define IC3_PCL_ENV_DEFAULT_PATH_DELIMITER          '/'
#define IC3_PCL_ENV_DEFAULT_PATH_DELIMITER_STR      "/"
#define IC3_PCL_ENV_NON_STANDARD_PATH_DELIMITER     '\\'
#define IC3_PCL_ENV_NON_STANDARD_PATH_DELIMITER_STR "\\"
#define IC3_PCL_ENV_DYNAMIC_LIBRARY_PREFIX          "lib"
#define IC3_PCL_ENV_DYNAMIC_LIBRARY_EXTENSION       ".so"

#endif /*  __IC3_PLATFORM_CONFIG_INTERNAL_OS_LINUX_H__ */
