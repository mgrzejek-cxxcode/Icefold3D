
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GCI_TYPE_INFO_H__
#define __IC3_GRAPHICS_GCI_GCI_TYPE_INFO_H__

#include <Ic3/CoreLib/Reflection/EnumTypeInfo.h>

namespace Ic3::Graphics::GCI
{

	Ic3EnableCustomExceptionSupport();
	Ic3EnableEnumTypeInfoSupport();

    ///
	#define Ic3TypeInfoEnumDeclareGraphicsGCI( TPEnum ) Ic3TypeInfoEnumDeclareAPI( IC3_GRAPHICS_GCI_API, TPEnum )

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GCI_TYPE_INFO_H__
