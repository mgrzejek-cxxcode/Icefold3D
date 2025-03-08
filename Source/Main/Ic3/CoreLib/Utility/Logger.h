
#ifndef __IC3_CORELIB_LOGGER_H__
#define __IC3_CORELIB_LOGGER_H__

#include "../Prerequisites.h"

namespace Ic3
{

	enum ELogSeverity
	{
		Undefined,
		Low,
		Normal,
		High,
		Critical
	};

	enum class ELogMessageType
	{
		Undefined,
		Debug,
		Notification,
		Warning,
		Error
	};

	struct ELogMessageSource
	{

	};

	class Logger
	{
	public:

	};

} // namespace Ic3

#endif // __IC3_CORELIB_LOGGER_H__
