
#pragma once

#ifndef __IC3_CORELIB_EXCEPTION_UTILS_H__
#define __IC3_CORELIB_EXCEPTION_UTILS_H__

#include "Exception.h"

#define Ic3ThrowIfNull( pPointer, pExceptionCode ) \
	if( !pPointer ) { Ic3Throw( pExceptionCode ); }

#define Ic3ThrowDescIfNull( pPointer, pExceptionCode, pDesc ) \
	if( !pPointer ) { Ic3ThrowDesc( pExceptionCode, pDesc ); }

namespace Ic3
{
} // namespace Ic3

#endif // __IC3_CORELIB_EXCEPTION_UTILS_H__
