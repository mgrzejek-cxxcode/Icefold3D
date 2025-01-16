
#include "syncCommon.h"
#include <thread>

namespace cppx::sync
{

	void yield_current_thread()
	{
		std::this_thread::yield();
	}

}
