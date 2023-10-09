
#pragma once

#ifndef __IC3_NXMAIN_RES_RESOURCE_H__
#define __IC3_NXMAIN_RES_RESOURCE_H__

#include "../Prerequisites.h"

namespace Ic3
{

	class Resource : public DynamicInterface
	{
	public:
		Resource() = default;
		virtual ~Resource() = default;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_RES_RESOURCE_H__
