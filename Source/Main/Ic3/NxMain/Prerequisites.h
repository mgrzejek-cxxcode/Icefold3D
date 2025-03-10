
#ifndef __IC3_NXMAIN_PREREQUISITES_H__
#define __IC3_NXMAIN_PREREQUISITES_H__

#include <Ic3/CoreLib/Prerequisites.h>

#include <cxm/vector.h>
#include <cxm/vectorOps.h>
#include <cxm/rect.h>
#include <cxm/matrix.h>
#include <cxm/matrixOps.h>
#include <cxm/matrixUtils.h>

#include <cppx/arrayView.h>
#include <cppx/immutableString.h>

#include <map>
#include <set>

#if( IC3_BUILD_STATIC )
#  define IC3_NXMAIN_API
#  define IC3_NXMAIN_CLASS
#  define IC3_NXMAIN_OBJ    extern
#else
#  if( IC3_NXMAIN_BUILD )
#    define IC3_NXMAIN_API    PCL_ATTR_DLL_EXPORT
#    define IC3_NXMAIN_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_NXMAIN_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_NXMAIN_API    PCL_ATTR_DLL_IMPORT
#    define IC3_NXMAIN_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_NXMAIN_OBJ    PCL_ATTR_DLL_IMPORT
#  endif
#endif

#define IC3_NXMAIN_API_NO_DISCARD CPPX_ATTR_NO_DISCARD IC3_NXMAIN_API

#include "Prerequisites/GCICommon.h"
#include "Prerequisites/GeometryCommon.h"
#include "Prerequisites/CoreEngineState.h"

namespace Ic3
{

	template <typename TData>
	using GeometryVertexStreamGenericArray = std::array<TData, GCM::kIAMaxDataStreamVertexBuffersNum>;

	class IC3_NXMAIN_CLASS CoreEngineObject : public IDynamicObject
	{
	public:
		const CoreEngineState & mCES;

	public:
		explicit CoreEngineObject( const CoreEngineState & pCES )
		: mCES( pCES )
		{}

		virtual ~CoreEngineObject() = default;
	};

}

#endif // __IC3_NXMAIN_PREREQUISITES_H__
