
#ifndef __IC3_CPPX_ARRAY_ITERATOR_H__
#define __IC3_CPPX_ARRAY_ITERATOR_H__

#include "Prerequisites.h"

namespace Ic3::Cppx
{

	template <bool tpIsReverse>
	struct QGetAdvanceOffset;

	template <>
	struct QGetAdvanceOffset<false>
	{
		static intptr_t get( intptr_t pOffset ) noexcept
		{
			return pOffset;
		}
	};

	template <>
	struct QGetAdvanceOffset<true>
	{
		static intptr_t get( intptr_t pOffset ) noexcept
		{
			return -pOffset;
		}
	};

	template <typename TPValue, bool tpIsReverse>
	struct ArrayIterator
	{
	public:
		using MyType = ArrayIterator<TPValue, tpIsReverse>;

		ArrayIterator() = default;

		ArrayIterator( const ArrayIterator & ) = default;
		ArrayIterator & operator=( const ArrayIterator & ) = default;

		ArrayIterator( TPValue * pValuePtr )
		: _currentPtr( pValuePtr )
		{}

		IC3_ATTR_NO_DISCARD TPValue * ptr() const noexcept
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
			const auto advanceOffset = QGetAdvanceOffset<tpIsReverse>::get( pOffset );
			_currentPtr = ( _currentPtr + advanceOffset );
		}

	private:
		TPValue * _currentPtr;
	};

	template <typename TPValue, bool tpIsReverse>
	inline void swap( const ArrayIterator<TPValue, tpIsReverse> & pFirst, const ArrayIterator<TPValue, tpIsReverse> & pSecond ) noexcept
	{
		pFirst.swap( pSecond );
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator==( const ArrayIterator<TPValue, tpIsReverse> & pLhs, const ArrayIterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.valuePtr() == pRhs.valuePtr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator!=( const ArrayIterator<TPValue, tpIsReverse> & pLhs, const ArrayIterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.valuePtr() != pRhs.valuePtr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator<( const ArrayIterator<TPValue, tpIsReverse> & pLhs, const ArrayIterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.valuePtr() < pRhs.valuePtr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator<=( const ArrayIterator<TPValue, tpIsReverse> & pLhs, const ArrayIterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.valuePtr() <= pRhs.valuePtr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator>( const ArrayIterator<TPValue, tpIsReverse> & pLhs, const ArrayIterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.valuePtr() > pRhs.valuePtr();
	}

	template <typename TPValue, bool tpIsReverse>
	inline bool operator>=( const ArrayIterator<TPValue, tpIsReverse> & pLhs, const ArrayIterator<TPValue, tpIsReverse> & pRhs ) noexcept
	{
		return pLhs.valuePtr() >= pRhs.valuePtr();
	}

} // namespace Ic3::Cppx

#endif // __IC3_CPPX_ARRAY_ITERATOR_H__
