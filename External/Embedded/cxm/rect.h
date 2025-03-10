
#ifndef __CXM_RECT_H__
#define __CXM_RECT_H__

#include "vector.h"

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#  pragma clang diagnostic ignored "-Wnested-anon-types"
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( push )
#  pragma warning( disable: 4201 )  // 'Nonstandard extension used: nameless struct/union'
#endif

namespace cxm
{

	template <typename TPOffset, typename TPSize>
	struct rect
	{
	public:
		vector2<TPOffset> offset;
		vector2<TPSize> size;

	public:
		constexpr rect() = default;

		constexpr rect( const rect & ) = default;
		constexpr rect & operator=( const rect & ) = default;

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit rect( TPScalar pScalar ) noexcept
		: offset( pScalar, pScalar )
		, size( pScalar, pScalar )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr rect( TPScalar pWidth, TPScalar pHeight ) noexcept
		: offset( 0, 0 )
		, size( pWidth, pHeight )
		{}

		template <typename TX, typename TY, typename TWidth, typename THeight>
		constexpr rect( TX pX, TY pY, TWidth pWidth, THeight pHeight = 0 ) noexcept
		: offset( pX, pY )
		, size( pWidth, ( pHeight == 0 ) ? pWidth : pHeight )
		{}

		template <typename T1, typename T2>
		constexpr rect( const vector2<T1> & pOffset, const vector2<T2> & pSize ) noexcept
		: offset( pOffset )
		, size( pSize )
		{}

		CPPX_ATTR_NO_DISCARD TPSize area() const
		{
			return size.x * size.y;
		}

		CPPX_ATTR_NO_DISCARD bool is_non_zero() const
		{
			return ( size.x != 0 ) && ( size.y != 0 );
		}

		CPPX_ATTR_NO_DISCARD bool is_zero() const
		{
			return ( size.x == 0 ) || ( size.y == 0 );
		}

		CPPX_ATTR_NO_DISCARD TPSize get_x() const noexcept
		{
			return offset.x;
		}

		CPPX_ATTR_NO_DISCARD TPSize get_y() const noexcept
		{
			return offset.y;
		}

		CPPX_ATTR_NO_DISCARD TPSize get_width() const noexcept
		{
			return size.x;
		}

		CPPX_ATTR_NO_DISCARD TPSize get_height() const noexcept
		{
			return size.y;
		}
	};

	using rect_i32 = rect<int32, uint32>;
	using rect_u32 = rect<uint32, uint32>;
	using rect_i64 = rect<int64, uint64>;
	using rect_u64 = rect<uint64, uint64>;
	using rect_f32 = rect<float, float>;

} // namespace cxm

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __CXM_RECT_H__
