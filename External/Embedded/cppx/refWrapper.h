
#ifndef __CPPX_REF_WRAPPER_H__
#define __CPPX_REF_WRAPPER_H__

#include "utilities.h"

namespace cppx
{

	template <typename TPValue>
	struct ref_wrapper
	{
	public:
		constexpr ref_wrapper() noexcept
		: _valuePtr( nullptr )
		{}

		ref_wrapper( const ref_wrapper & pSource ) noexcept
		: _valuePtr( pSource._valuePtr )
		{}

		ref_wrapper & operator=( const ref_wrapper & pRhs ) noexcept
		{
			_valuePtr = pRhs._valuePtr;
			return *this;
		}

		ref_wrapper & operator=( TPValue & pRhs ) noexcept
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

		CPPX_ATTR_NO_DISCARD constexpr explicit operator bool() const noexcept
		{
			return _valuePtr != nullptr;
		}

		CPPX_ATTR_NO_DISCARD TPValue & get() const noexcept
		{
			return *_valuePtr;
		}

		void swap( ref_wrapper<TPValue> & pOther ) noexcept
		{
			std::swap( _valuePtr, pOther._valuePtr );
		}

	private:
		TPValue * _valuePtr;
	};

	template <typename TPValue>
	using ref = ref_wrapper<TPValue>;

	template <typename TPValue>
	using cref = ref_wrapper<const TPValue>;

	template <typename TPValue>
	CPPX_ATTR_NO_DISCARD inline constexpr ref<TPValue> make_ref( TPValue & pRef ) noexcept
	{
		return ref<TPValue>( pRef );
	}

	template <typename TPValue>
	CPPX_ATTR_NO_DISCARD inline constexpr cref<TPValue> make_cref( const TPValue & pRef ) noexcept
	{
		return cref<TPValue>( pRef );
	}

}

#endif // __CPPX_REF_WRAPPER_H__
