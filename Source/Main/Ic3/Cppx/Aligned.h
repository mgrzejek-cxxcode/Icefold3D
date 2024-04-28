
#ifndef __IC3_CPPX_ALIGNED_H__
#define __IC3_CPPX_ALIGNED_H__

#include "Utilities.h"

namespace Ic3::Cppx
{

	template <typename T, size_t tAlignment = alignof( T )>
	struct AlignedStorage
	{
		static constexpr auto sAlignment = static_cast<uint32>( getMaxOf( tAlignment, alignof( T ) ) );
		
		static constexpr auto sSize = static_cast<uint32>( sizeof( T ) );
		
		alignas( sAlignment ) byte data[sSize];

		IC3_ATTR_NO_DISCARD byte * ptr() noexcept
		{
			return &( data[0] );
		}

		IC3_ATTR_NO_DISCARD const byte * ptr() const noexcept
		{
			return &( data[0] );
		}
	};

}

#endif // __IC3_CPPX_ALIGNED_H__
