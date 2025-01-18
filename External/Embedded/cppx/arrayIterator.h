
#ifndef __CPPX_ARRAY_ITERATOR_H__
#define __CPPX_ARRAY_ITERATOR_H__

#include "prerequisites.h"

namespace cppx
{

	template <bool tpIsReverse>
	struct advance_offset;

	template <>
	struct advance_offset<false>
	{
		static intptr_t get( intptr_t pOffset ) noexcept
		{
			return pOffset;
		}
	};

	template <>
	struct advance_offset<true>
	{
		static intptr_t get( intptr_t pOffset ) noexcept
		{
			return -pOffset;
		}
	};

	template <typename TPValue, bool tpIsReverse>
	struct array_iterator
	{
	public:
		using MyType = array_iterator<TPValue, tpIsReverse>;

		array_iterator() = default;

		array_iterator( const array_iterator & ) = default;
		array_iterator & operator=( const array_iterator & ) = default;

		array_iterator( TPValue * pValuePtr )
		: _currentPtr( pValuePtr )
		{}

		CPPX_ATTR_NO_DISCARD TPValue * ptr() const noexcept
		{
			return _currentPtr;
		}

		MyType * operator->() const noexcept
		{
			return _currentPtr;
		}

		MyType & operator*() const noexcept
		{
			return *_currentPtr;
		}

		MyType & operator++()
		{
			_advance( 1 );
			return *this;
		}

		MyType operator++( int )
		{
			MyType iterCopy{ _currentPtr };
			_advance( 1 );
			return iterCopy;
		}

		MyType & operator--()
		{
			_advance( -1 );
			return *this;
		}

		MyType operator--( int )
		{
			MyType iterCopy{ _currentPtr };
			_advance( -1 );
			return iterCopy;
		}

		MyType & operator+=( intptr_t pOffset )
		{
			_advance( pOffset );
			return *this;
		}

		MyType & operator-=( intptr_t pOffset )
		{
			_advance( -pOffset );
			return *this;
		}

		void swap( MyType & pOther ) noexcept
		{
			std::swap( _currentPtr, pOther._currentPtr );
		}

	private:
		void _advance( intptr_t pOffset ) noexcept
		{
			const auto advanceOffset = advance_offset<tpIsReverse>::get( pOffset );
			_currentPtr = ( _currentPtr + advanceOffset );
		}

	private:
		TPValue * _currentPtr;
	};

	template <typename TPValue, bool tpIsReverse>
	inline void swap( const array_iterator<TPValue, tpIsReverse> & pFirst, const array_iterator<TPValue, tpIsReverse> & pSecond ) noexcept
	{
		pFirst.swap( pSecond );
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator==( const array_iterator<TPValue, tpIsReverse> & pLhs, const array_iterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.ptr() == pRhs.ptr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator!=( const array_iterator<TPValue, tpIsReverse> & pLhs, const array_iterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.ptr() != pRhs.ptr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator<( const array_iterator<TPValue, tpIsReverse> & pLhs, const array_iterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.ptr() < pRhs.ptr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator<=( const array_iterator<TPValue, tpIsReverse> & pLhs, const array_iterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.ptr() <= pRhs.ptr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator>( const array_iterator<TPValue, tpIsReverse> & pLhs, const array_iterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.ptr() > pRhs.ptr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator>=( const array_iterator<TPValue, tpIsReverse> & pLhs, const array_iterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.ptr() >= pRhs.ptr();
	}

} // namespace cppx

#endif // __CPPX_ARRAY_ITERATOR_H__
