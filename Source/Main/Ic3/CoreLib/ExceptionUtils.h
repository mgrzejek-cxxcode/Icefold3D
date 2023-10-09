
#pragma once

#ifndef __IC3_CORELIB_EXCEPTION_UTILS_H__
#define __IC3_CORELIB_EXCEPTION_UTILS_H__

#include "Exception.h"

#define ic3ThrowIfNull( pPointer, pExceptionCode ) \
	if( !pPointer ) { ic3Throw( pExceptionCode ); }

#define ic3ThrowDescIfNull( pPointer, pExceptionCode, pDesc ) \
	if( !pPointer ) { ic3ThrowDesc( pExceptionCode, pDesc ); }

namespace Ic3
{
} // namespace Ic3

#endif // __IC3_CORELIB_EXCEPTION_UTILS_H__
