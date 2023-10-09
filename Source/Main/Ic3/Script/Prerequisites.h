
#pragma once

#include <Ic3/Platform/Debug.h>
#include <Ic3/Cppx/ArrayView.h>
#include <Ic3/Cppx/Bitmask.h>
#include <Ic3/Cppx/StaticAlgo.h>
#include <Ic3/Cppx/Variant.h>
#include <Ic3/CoreLib/Exception.h>
#include <Ic3/CoreLib/Result.h>

#if( IC3_BUILD_STATIC )
#  define IC3_SCRIPT_API
#  define IC3_SCRIPT_CLASS
#  define IC3_SCRIPT_OBJ    extern
#else
#  if( IC3_SCRIPT_BUILD )
#    define IC3_SCRIPT_API    IC3_PCL_ATTR_DLL_EXPORT
#    define IC3_SCRIPT_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define IC3_SCRIPT_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_SCRIPT_API    IC3_PCL_ATTR_DLL_IMPORT
#    define IC3_SCRIPT_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define IC3_SCRIPT_OBJ    IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

namespace Ic3::Script
{

	IC3_SCRIPT_API void init();

}
