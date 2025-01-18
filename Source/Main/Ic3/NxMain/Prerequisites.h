
#ifndef __IC3_NXMAIN_PREREQUISITES_H__
#define __IC3_NXMAIN_PREREQUISITES_H__

#include <Ic3/CoreLib/Prerequisites.h>
#include <Ic3/Math/Vector.h>
#include <Ic3/Math/VectorOps.h>
#include <Ic3/Math/Rect.h>
#include <Ic3/Math/Matrix.h>
#include <Ic3/Math/MatrixOps.h>
#include <Ic3/Math/MatrixUtils.h>
#include <cppx/arrayView.h>

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

#define IC3_NXMAIN_API_NO_DISCARD IC3_NXMAIN_API CPPX_ATTR_NO_DISCARD

#include "Prerequisites/GCICommon.h"
#include "Prerequisites/GeometryCommon.h"
#include "Prerequisites/CoreEngineState.h"

namespace Ic3
{

	template <typename TData>
	using GeometryVertexStreamGenericArray = std::array<TData, GCM::cxIAMaxVertexStreamsNum>;

	class CoreEngineObject : public IDynamicObject
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
