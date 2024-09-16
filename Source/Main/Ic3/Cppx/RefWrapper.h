
#ifndef __IC3_CPPX_REF_WRAPPER_H__
#define __IC3_CPPX_REF_WRAPPER_H__

#include "Utilities.h"

namespace Ic3::Cppx
{

	template <typename TPValue>
	struct TRefWrapper
	{
	public:
		constexpr TRefWrapper() noexcept
		: _valuePtr( nullptr )
		{}

		TRefWrapper( const TRefWrapper & pSource ) noexcept
		: _valuePtr( pSource._valuePtr )
		{}

		TRefWrapper & operator=( const TRefWrapper & pRhs ) noexcept
		{
			_valuePtr = pRhs._valuePtr;
			return *this;
		}

		TRefWrapper & operator=( TPValue & pRhs ) noexcept
		{
			_valuePtr = &pRhs;
			return *this;
		}

		operator TPValue &() const noexcept
		{
			return *_valuePtr;
		}

		TPValue * operator->() const noexcept
		{
			return _valuePtr;
		}

		IC3_ATTR_NO_DISCARD constexpr explicit operator bool() const noexcept
		{
			return _valuePtr != nullptr;
		}

		IC3_ATTR_NO_DISCARD TPValue & get() const noexcept
		{
			return *_valuePtr;
		}

		void swap( TRefWrapper<TPValue> & pOther ) noexcept
		{
			std::swap( _valuePtr, pOther._valuePtr );
		}

	private:
		TPValue * _valuePtr;
	};

	template <typename TPValue>
	using TRef = TRefWrapper<TPValue>;

	template <typename TPValue>
	using TConstRef = TRefWrapper<const TPValue>;

	template <typename TPValue>
	IC3_ATTR_NO_DISCARD inline constexpr TRef<TPValue> makeRef( TPValue & pRef ) noexcept
	{
		return TRef<TPValue>( pRef );
	}

	template <typename TPValue>
	IC3_ATTR_NO_DISCARD inline constexpr TConstRef<TPValue> makeConstRef( const TPValue & pRef ) noexcept
	{
		return TConstRef<TPValue>( pRef );
	}

}

#endif // __IC3_CPPX_REF_WRAPPER_H__
