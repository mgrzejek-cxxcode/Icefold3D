
#pragma once

#ifndef __IC3_CORELIB_SIGNAL_COMMON_H__
#define __IC3_CORELIB_SIGNAL_COMMON_H__

#include "../Prerequisites.h"
#include <functional>

namespace Ic3
{

	using event_code_value_t = uint32;

	template <event_code_value_t tpEventCode, typename... TPEventArgs>
	struct Event
	{
		static constexpr event_code_value_t EVENT_CODE = tpEventCode;
	};

}

#endif // __IC3_CORELIB_SIGNAL_COMMON_H__
