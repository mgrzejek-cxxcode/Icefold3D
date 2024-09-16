
#pragma once

#ifndef __IC3_CORELIB_MUTEX_STD_DEFS_H__
#define __IC3_CORELIB_MUTEX_STD_DEFS_H__

#include "MutexCommon.h"
#include <mutex>
#include <shared_mutex>

namespace Ic3::Sync
{

	template <>
	struct QMutexInterfaceProxy<std::mutex>
	{
		using ProxyType = MutexInterfaceProxyStd;
	};

	template <>
	struct QMutexInterfaceProxy<std::shared_mutex>
	{
		using ProxyType = MutexInterfaceProxyStd;
	};

}

#endif // __IC3_CORELIB_MUTEX_STD_DEFS_H__
