
#ifndef __IC3_CORELIB_LINEAR_MEMORY_ALLOCATOR_H__
#define __IC3_CORELIB_LINEAR_MEMORY_ALLOCATOR_H__

#include "../Exception.h"

namespace Ic3
{

	struct SMemoryBlockMovementInfo
	{
		uintptr_t currentAddress;
		uintptr_t newAddress;
		size_t regionSize;
	};

	class CLinearMemoryAllocator
	{
	private:
		uintptr_t _baseAddress;
		uintptr_t _currentAllocPtr;
		size_t _freeSpace;
	};

} // namespace Ic3

#endif // __IC3_CORELIB_LINEAR_MEMORY_ALLOCATOR_H__
