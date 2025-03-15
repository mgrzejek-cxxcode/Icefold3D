
#pragma once

#ifndef __IC3_GRAPHICS_NXM_NXM_TYPE_INFO_H__
#define __IC3_GRAPHICS_NXM_NXM_TYPE_INFO_H__

#include <Ic3/CoreLib/ExceptionUtils.h>
#include <Ic3/CoreLib/Reflection/EnumTypeInfo.h>

namespace Ic3::Graphics::NXM
{

	Ic3EnableCustomExceptionSupport();
	Ic3EnableEnumTypeInfoSupport();

    ///
	#define Ic3TypeInfoEnumDeclareNXM( TPEnum ) Ic3TypeInfoEnumDeclareAPI( IC3_NXMAIN_API, TPEnum )

} // namespace Ic3::Graphics::NXM

#endif // __IC3_GRAPHICS_NXM_NXM_TYPE_INFO_H__
