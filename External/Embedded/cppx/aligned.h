
#ifndef __CPPX_ALIGNED_H__
#define __CPPX_ALIGNED_H__

#include "utilities.h"

namespace cppx
{

	template <typename TPValue, size_t tpAlignment = alignof( TPValue )>
	struct aligned_storage
	{
		static constexpr auto alignment = static_cast<uint32>( get_max_of( tpAlignment, alignof( TPValue ) ) );
		
		static constexpr auto size = static_cast<uint32>( sizeof( TPValue ) );
		
		alignas( alignment ) byte data[size];

		CPPX_ATTR_NO_DISCARD byte * ptr() noexcept
		{
			return &( data[0] );
		}

		CPPX_ATTR_NO_DISCARD const byte * ptr() const noexcept
		{
			return &( data[0] );
		}
	};

}

#endif // __CPPX_ALIGNED_H__
