
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_STORAGE_GPU_H__
#define __IC3_NXMAIN_GEOMETRY_STORAGE_GPU_H__

#include "GeometryStorage.h"

namespace Ic3
{

	enum class EGeometryBufferAllocationMode : enum_default_value_t
	{
		AllocLocal,
		ShareExternal
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_STORAGE_GPU_H__
