
#ifndef __IC3_CORELIB_LINEAR_MEMORY_ALLOCATOR_H__
#define __IC3_CORELIB_LINEAR_MEMORY_ALLOCATOR_H__

#include "../Exception.h"

namespace Ic3
{

	struct MemoryBlockMovementInfo
	{
		uintptr_t mCurrentAddress;
		uintptr_t mNewAddress;
		size_t mRegionSize;
	};

	class LinearMemoryAllocator
	{
	private:
		uintptr_t _baseAddress;
		uintptr_t _currentAllocPtr;
		size_t _freeSpace;
	};

} // namespace Ic3

#endif // __IC3_CORELIB_LINEAR_MEMORY_ALLOCATOR_H__
