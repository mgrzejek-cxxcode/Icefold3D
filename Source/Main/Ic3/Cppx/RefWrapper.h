
#ifndef __IC3_CPPX_REF_WRAPPER_H__
#define __IC3_CPPX_REF_WRAPPER_H__

#include "Utilities.h"

namespace Ic3::Cppx
{

	template <typename TValue>
	struct RefWrapper
	{
	public:
		constexpr RefWrapper() noexcept
		: _valuePtr( nullptr )
		{}

		RefWrapper( const RefWrapper & pSource ) noexcept
		: _valuePtr( pSource._valuePtr )
		{}

		RefWrapper & operator=( const RefWrapper & pRhs ) noexcept
		{
			_valuePtr = pRhs._valuePtr;
			return *this;
		}

		RefWrapper & operator=( TValue & pRhs ) noexcept
		{
			_valuePtr = &pRhs;
			return *this;
		}

		operator TValue &() const noexcept
		{
			return *_valuePtr;
		}

		TValue * operator->() const noexcept
		{
			return _valuePtr;
		}

		IC3_ATTR_NO_DISCARD constexpr explicit operator bool() const noexcept
		{
			return _valuePtr != nullptr;
		}

		IC3_ATTR_NO_DISCARD TValue & get() const noexcept
		{
			return *_valuePtr;
		}

		void swap( RefWrapper<TValue> & pOther ) noexcept
		{
			std::swap( _valuePtr, pOther._valuePtr );
		}

	private:
		TValue * _valuePtr;
	};

	template <typename TValue>
	IC3_ATTR_NO_DISCARD inline constexpr RefWrapper<TValue> makeRef( TValue & pRef ) noexcept
	{
		return RefWrapper<TValue>( pRef );
	}

}

#endif // __IC3_CPPX_REF_WRAPPER_H__
