
#ifndef __IC3_CORELIB_PREREQUISITES_H__
#define __IC3_CORELIB_PREREQUISITES_H__

#include <Ic3/Platform/Debug.h>
#include <Ic3/Cppx/ArrayView.h>
#include <Ic3/Cppx/Bitmask.h>
#include <Ic3/Cppx/BitUtils.h>
#include <Ic3/Cppx/Range.h>
#include <Ic3/Cppx/RefWrapper.h>
#include <Ic3/Cppx/StaticLimits.h>
#include <Ic3/Cppx/StringView.h>

#if( IC3_BUILD_STATIC )
#  define IC3_CORELIB_API
#  define IC3_CORELIB_CLASS
#  define IC3_CORELIB_OBJ    extern
#else
#  if( IC3_CORELIB_BUILD )
#    define IC3_CORELIB_API    IC3_PCL_ATTR_DLL_EXPORT
#    define IC3_CORELIB_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define IC3_CORELIB_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_CORELIB_API    IC3_PCL_ATTR_DLL_IMPORT
#    define IC3_CORELIB_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define IC3_CORELIB_OBJ    IC3_PCL_ATTR_DLL_IMPORT
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

namespace Ic3
{

	using Cppx::QLimits;
	using Cppx::TArrayView;
	using Cppx::TBitmask;
	using Cppx::TConstRef;
	using Cppx::TRange;
	using Cppx::TRegion;
	using Cppx::TRef;
	using Cppx::TRefWrapper;
	using Cppx::TStringView;

}

#include "Prerequisites/CoreDefs.h"
#include "Prerequisites/CoreEnums.h"
#include "Prerequisites/ResultCode.h"
#include "Prerequisites/DynamicInterface.h"

namespace Ic3
{

	// Naming Convention
	// E -> Enum
	// S -> Struct
	// C -> Class
	// I -> Interface
	// T -> Templates and template type parameters
	// Q -> Queries/Traits -> compile-time!
	// F -> Functors
	// P -> Proxies
	// R -> Aliases/Typedefs

	/// @brief Helper struct for triggering a "empty-init" ctor. Used whenever default initialization has a different meaning.
	struct InitEmptyTag
	{
	};

	/// @brief A dedicated instance of InitEmptyTag
	inline constexpr InitEmptyTag CX_INIT_EMPTY {};

} // namespace Ic3

#endif // __IC3_CORELIB_PREREQUISITES_H__
