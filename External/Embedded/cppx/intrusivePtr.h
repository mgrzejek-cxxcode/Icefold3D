
#ifndef __CPPX_INTRUSIVE_PTR_H__
#define __CPPX_INTRUSIVE_PTR_H__

#include "refCounter.h"
#include <cassert>
#include <functional>

namespace cppx
{

	template <typename TPCounter = ref_counter<>>
	class ref_counted_base
	{
		friend struct ref_counted_proxy;

	public:
		using self_type = ref_counted_base<TPCounter>;
		using CounterType = TPCounter;

	public:
		ref_counted_base( const ref_counted_base & ) = delete;
		ref_counted_base & operator=( const ref_counted_base & ) = delete;

		ref_counted_base() = default;

	protected:
		ref_counter_default_value_t add_ref() const
		{
			return _counter.increment();
		}

		ref_counter_default_value_t release_ref() const
		{
			return _counter.decrement();
		}

		ref_counter_default_value_t get_refs_num() const
		{
			return _counter.get_value();
		}

	private:
		mutable CounterType _counter;
	};


	struct ref_counted_proxy
	{
		template <typename TPCounter>
		static ref_counter_default_value_t add_ref( const ref_counted_base<TPCounter> * pRefCountedBase )
		{
			return pRefCountedBase->add_ref();
		}

		template <typename TPCounter>
		static ref_counter_default_value_t release_ref( const ref_counted_base<TPCounter> * pRefCountedBase )
		{
			return pRefCountedBase->release_ref();
		}
	};


	template <typename TPRefCounted>
	inline ref_counter_default_value_t add_ref_default( const TPRefCounted * pObjectPtr )
	{
		return ref_counted_proxy::add_ref( pObjectPtr );
	}

	template <typename TPRefCounted>
	inline ref_counter_default_value_t release_ref_default( const TPRefCounted * pObjectPtr )
	{
		return ref_counted_proxy::release_ref( pObjectPtr );
	}


	template <typename TPRefCounted, typename TPDeleter = default_delete<TPRefCounted>>
	class intrusive_ptr
	{
		template <typename, typename>
		friend class intrusive_ptr;

	public:
		using self_type = intrusive_ptr<TPRefCounted, TPDeleter>;

	public:
		constexpr intrusive_ptr() = default;

		intrusive_ptr( intrusive_ptr && pSrcObject )
		: _ptr( nullptr )
		{
			swap( pSrcObject );
		}

		intrusive_ptr( const intrusive_ptr & pSrcObject )
		: _ptr( nullptr )
		{
			_set_no_release( pSrcObject._ptr );
		}

		template <typename TPOther>
		intrusive_ptr( intrusive_ptr<TPOther> && pOther )
		: _ptr( static_cast<TPRefCounted *>( pOther._ptr ) )
		{
			pOther._ptr = nullptr;
		}

		template <typename TPOther>
		intrusive_ptr( const intrusive_ptr<TPOther> & pOther )
		: _ptr( nullptr )
		{
			_set_no_release( pOther._ptr );
		}

		intrusive_ptr( std::nullptr_t )
		: _ptr( nullptr )
		{}

		explicit intrusive_ptr( TPRefCounted * pObject )
		: _ptr( pObject )
		{}

		template <typename TPOther>
		explicit intrusive_ptr( TPOther * pObject )
		: _ptr( static_cast<TPRefCounted *>( pObject ) )
		{}

		~intrusive_ptr()
		{
			_release();
		}

		intrusive_ptr & operator=( intrusive_ptr && pRhs )
		{
			if( this != &pRhs )
			{
				intrusive_ptr( std::move( pRhs ) ).swap( *this );
			}

			return *this;
		}

		intrusive_ptr & operator=( const intrusive_ptr & pRhs )
		{
			if( this != &pRhs )
			{
				intrusive_ptr( pRhs ).swap( *this );
			}

			return *this;
		}

		template <typename TPOther>
		intrusive_ptr & operator=( intrusive_ptr<TPOther> && pRhs )
		{
			intrusive_ptr( std::move( pRhs ) ).swap( *this );
			return *this;
		}

		template <typename TPOther>
		intrusive_ptr & operator=( const intrusive_ptr<TPOther> & pRhs )
		{
			intrusive_ptr( pRhs ).swap( *this );
			return *this;
		}

		intrusive_ptr & operator=( std::nullptr_t )
		{
			_release();
			return *this;
		}

		intrusive_ptr & operator=( TPRefCounted * pRhs )
		{
			intrusive_ptr( pRhs ).swap( *this );
			return *this;
		}

		template <typename TPOther>
		intrusive_ptr & operator=( TPOther * pRhs )
		{
			intrusive_ptr( pRhs ).swap( *this );
			return *this;
		}

		explicit operator bool() const
		{
			return _ptr != nullptr;
		}

		explicit operator TPRefCounted *() const
		{
			return _ptr;
		}

		TPRefCounted & operator*() const
		{
			assert( _ptr != nullptr );
			return *( _ptr );
		}

		TPRefCounted * operator->() const
		{
			assert( _ptr != nullptr );
			return _ptr;
		}

		TPRefCounted * get() const
		{
			return _ptr;
		}

		void reset( TPRefCounted * pPointer = nullptr )
		{
			_set( pPointer );
		}

		void swap( intrusive_ptr & pOther )
		{
			std::swap( _ptr, pOther._ptr );
		}

		CPPX_ATTR_NO_DISCARD bool empty() const
		{
			return _ptr != nullptr;
		}

	private:
		void _release()
		{
			if( _ptr != nullptr )
			{
				_release_ptr( _ptr );
				_ptr = nullptr;
			}
		}

		void _set_no_release( TPRefCounted * pPointer )
		{
			if( ( pPointer != nullptr ) && ( add_ref_default( pPointer ) > 0 ) )
			{
				_ptr = pPointer;
			}
			else
			{
				_ptr = nullptr;
			}
		}

		template <typename TPOther>
		void _set_no_release( TPOther * pPointer )
		{
			_set_no_release( static_cast<TPRefCounted *>( pPointer ) );
		}

		void _set( TPRefCounted * pPointer )
		{
			if( pPointer != _ptr )
			{
				TPRefCounted * prevPtr = _ptr;
				_set_no_release( pPointer );
				if( prevPtr != nullptr )
				{
					_release_ptr( prevPtr );
				}
			}
		}

		template <typename TPOther>
		void _set( TPOther * pPointer )
		{
			_set( static_cast<TPRefCounted *>( pPointer ) );
		}

		static void _release_ptr( TPRefCounted * pPointer )
		{
			ref_counter_default_value_t remainingRefs = release_ref_default( pPointer );
			if( remainingRefs == 0 )
			{
				TPDeleter()( pPointer );
			}
		}

	private:
		TPRefCounted * _ptr = nullptr;
	};

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator==( const intrusive_ptr<TPRefCounted, TPDeleter> & pLhs, const intrusive_ptr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() == pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter>
	inline bool operator==( const intrusive_ptr<TPRefCounted, TPDeleter> & pLhs, std::nullptr_t )
	{
		return pLhs.empty();
	}

	template <typename TPRefCounted, typename TPDeleter>
	inline bool operator==( std::nullptr_t, const intrusive_ptr<TPRefCounted, TPDeleter> & pRhs )
	{
		return pRhs.empty();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator!=( const intrusive_ptr<TPRefCounted, TPDeleter> & pLhs, const intrusive_ptr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() != pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter>
	inline bool operator!=( const intrusive_ptr<TPRefCounted, TPDeleter> & pLhs, std::nullptr_t )
	{
		return !pLhs.empty();
	}

	template <typename TPRefCounted, typename TPDeleter>
	inline bool operator!=( std::nullptr_t, const intrusive_ptr<TPRefCounted, TPDeleter> & pRhs )
	{
		return !pRhs.empty();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator>( const intrusive_ptr<TPRefCounted, TPDeleter> & pLhs, const intrusive_ptr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() > pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator>=( const intrusive_ptr<TPRefCounted, TPDeleter> & pLhs, const intrusive_ptr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() >= pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator<( const intrusive_ptr<TPRefCounted, TPDeleter> & pLhs, const intrusive_ptr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() < pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator<=( const intrusive_ptr<TPRefCounted, TPDeleter> & pLhs, const intrusive_ptr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() <= pRhs.get();
	}


	template <typename TPRefCounted, typename TPDeleter>
	void swap( intrusive_ptr<TPRefCounted, TPDeleter> & pLeft, intrusive_ptr<TPRefCounted, TPDeleter> & pRight )
	{
		pLeft.swap( pRight );
	}


	template <typename TPRefCounted, class ...TPArgs>
	inline intrusive_ptr<TPRefCounted> make_intrusive( TPArgs && ...pArgs )
	{
		return intrusive_ptr<TPRefCounted>( new TPRefCounted( std::forward<TPArgs>( pArgs )... ) );
	}

}

#endif /* __CPPX_INTRUSIVE_PTR_H__ */
