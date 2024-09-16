
#ifndef __IC3_CPPX_STRING_EXT_H__
#define __IC3_CPPX_STRING_EXT_H__

#include "ArrayView.h"

namespace Ic3::Cppx
{

	template <typename TPValue, size_t tpMaxCapacity>
	class StaticVector
	{
	public:
		StaticVector() = default;

		StaticVector( StaticVector && pSrcObject )
		{}

		StaticVector( const StaticVector & pSrcObject )
		{}

		StaticVector & operator=( StaticVector && pRhs )
		{}

		StaticVector & operator=( const StaticVector & pRhs )
		{}

	private:
		using Storage = typename std::aligned_storage<sizeof( TPValue ), alignof( TPValue )>::type;
		Storage _data[tpMaxCapacity];
		size_t _beginIndex = tpMaxCapacity;
		size_t _endIndex = tpMaxCapacity;
	};

}

#endif /* __IC3_CPPX_STRING_EXT_H__ */
