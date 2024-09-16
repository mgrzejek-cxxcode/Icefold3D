
#ifndef __IC3_CPPX_ALIGNED_H__
#define __IC3_CPPX_ALIGNED_H__

#include "Utilities.h"

namespace Ic3::Cppx
{

	template <typename TPValue, size_t tpAlignment = alignof( TPValue )>
	struct TAlignedStorage
	{
		static constexpr auto sAlignment = static_cast<uint32>( getMaxOf( tpAlignment, alignof( TPValue ) ) );
		
		static constexpr auto sSize = static_cast<uint32>( sizeof( TPValue ) );
		
		alignas( sAlignment ) byte mData[sSize];

		IC3_ATTR_NO_DISCARD byte * ptr() noexcept
		{
			return &( mData[0] );
		}

		IC3_ATTR_NO_DISCARD const byte * ptr() const noexcept
		{
			return &( mData[0] );
		}
	};

}

#endif // __IC3_CPPX_ALIGNED_H__
