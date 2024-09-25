
#ifndef __IC3_CPPX_SHARED_STRING_H__
#define __IC3_CPPX_SHARED_STRING_H__

#include "RefCounter.h"

namespace Ic3::Cppx
{

	constexpr auto cxSharedStringDefaultSSOSize = 16;

	template <typename TPChar, size_t tpSSOSize, typename TPAllocator>
	class SharedCharBuffer
	{
	private:
		TPChar _storageSSO[tpSSOSize];
		TPChar * _storageDynamic;
		TPChar * _bufferPtr;
	};

	template <typename TPChar, size_t tpSSOSize, typename TPAllocator>
	class SharedString
	{
	private:
		struct CharStorage
		{
		};
	};

}

#endif /* __IC3_CPPX_SHARED_STRING_H__ */
