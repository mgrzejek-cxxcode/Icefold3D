
#ifndef __IC3_MATH_RECT_H__
#define __IC3_MATH_RECT_H__

#include "Vector.h"

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

namespace Ic3::Math
{

	template <typename TPOffset, typename TPSize>
	struct Rect
	{
	public:
		Vector2<TPOffset> offset;
		Vector2<TPSize> size;

	public:
		constexpr Rect() = default;

		constexpr Rect( const Rect & ) = default;
		constexpr Rect & operator=( const Rect & ) = default;

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Rect( TPScalar pScalar ) noexcept
		: offset( pScalar, pScalar )
		, size( pScalar, pScalar )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr Rect( TPScalar pWidth, TPScalar pHeight ) noexcept
		: offset( 0, 0 )
		, size( pWidth, pHeight )
		{}

		template <typename TX, typename TY, typename TWidth, typename THeight>
		constexpr Rect( TX pX, TY pY, TWidth pWidth, THeight pHeight = 0 ) noexcept
		: offset( pX, pY )
		, size( pWidth, ( pHeight == 0 ) ? pWidth : pHeight )
		{}

		template <typename T1, typename T2>
		constexpr Rect( const Vector2<T1> & pOffset, const Vector2<T2> & pSize ) noexcept
		: offset( pOffset )
		, size( pSize )
		{}

		CPPX_ATTR_NO_DISCARD TPSize area() const
		{
			return size.x * size.y;
		}

		CPPX_ATTR_NO_DISCARD bool isNonZero() const
		{
			return ( size.x != 0 ) && ( size.y != 0 );
		}

		CPPX_ATTR_NO_DISCARD bool isZero() const
		{
			return ( size.x == 0 ) || ( size.y == 0 );
		}

		CPPX_ATTR_NO_DISCARD TPSize getX() const noexcept
		{
			return offset.x;
		}

		CPPX_ATTR_NO_DISCARD TPSize getY() const noexcept
		{
			return offset.y;
		}

		CPPX_ATTR_NO_DISCARD TPSize getWidth() const noexcept
		{
			return size.x;
		}

		CPPX_ATTR_NO_DISCARD TPSize getHeight() const noexcept
		{
			return size.y;
		}
	};

	using Recti32 = Rect<int32, uint32>;
	using Rectu32 = Rect<uint32, uint32>;
	using Recti64 = Rect<int64, uint64>;
	using Rectu64 = Rect<uint64, uint64>;
	using Rectf   = Rect<float, float>;

} // namespace Ic3::Math

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __IC3_MATH_RECT_H__
