
#ifndef __IC3_CORELIB_PREREQUISITES_H__
#define __IC3_CORELIB_PREREQUISITES_H__

#include <cppx/arrayView.h>
#include <cppx/bitmask.h>
#include <cppx/bitUtils.h>
#include <cppx/range.h>
#include <cppx/refWrapper.h>
#include <cppx/typeLimits.h>
#include <cppx/stringView.h>

#if( IC3_BUILD_STATIC )
#  define IC3_CORELIB_API
#  define IC3_CORELIB_CLASS
#  define IC3_CORELIB_OBJ    extern
#else
#  if( IC3_CORELIB_BUILD )
#    define IC3_CORELIB_API    PCL_ATTR_DLL_EXPORT
#    define IC3_CORELIB_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_CORELIB_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_CORELIB_API    PCL_ATTR_DLL_IMPORT
#    define IC3_CORELIB_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_CORELIB_OBJ    PCL_ATTR_DLL_IMPORT
#  endif
#endif

#if( IC3_DEBUG )
#  define IC3_DEBUG_CODE( pCode ) pCode
#else
#  define IC3_DEBUG_CODE( pCode )
#endif

#if( IC3_DEBUG )
#  define IC3_DEBUG_INIT( ... ) { __VA_ARGS__ }
#else
#  define IC3_DEBUG_INIT( ... )
#endif

#define friendapi protected

#include "Prerequisites/CoreDefs.h"
#include "Prerequisites/CoreEnums.h"
#include "Prerequisites/DebugLayer.h"
#include "Prerequisites/ResultCode.h"
#include "Prerequisites/DynamicInterface.h"

namespace Ic3
{

	/// @brief Helper struct for triggering a "empty-init" ctor. Used whenever default initialization has a different meaning.
	struct InitEmptyTag
	{
	};

	/// @brief A dedicated instance of InitEmptyTag
	inline constexpr InitEmptyTag cxInitEmpty {};

} // namespace Ic3

#endif // __IC3_CORELIB_PREREQUISITES_H__
