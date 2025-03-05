
#pragma once

#ifndef __IC3_SCRIPT_COMMON_DEFS_H__
#define __IC3_SCRIPT_COMMON_DEFS_H__

namespace Ic3::Script
{

	Ic3EnableCustomExceptionSupport();
	Ic3EnableEnumTypeInfoSupport();

	///
	#define Ic3TypeInfoEnumDeclareScript( TPEnum ) Ic3TypeInfoEnumDeclareAPI( TPEnum, IC3_SCRIPT_API )

} // namespace Ic3::Script

#endif // __IC3_SCRIPT_COMMON_DEFS_H__
