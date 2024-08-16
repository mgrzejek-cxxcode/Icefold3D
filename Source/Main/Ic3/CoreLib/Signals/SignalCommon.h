
#pragma once

#ifndef __IC3_CORELIB_SIGNAL_COMMON_H__
#define __IC3_CORELIB_SIGNAL_COMMON_H__

#include "../Prerequisites.h"
#include <functional>

namespace Ic3
{

	using event_code_value_t = uint32;

	template <event_code_value_t tEventCode, typename... TEventArgs>
	struct SEvent
	{
		static constexpr event_code_value_t sEventCode = tEventCode;
	};

}

#endif // __IC3_CORELIB_SIGNAL_COMMON_H__
