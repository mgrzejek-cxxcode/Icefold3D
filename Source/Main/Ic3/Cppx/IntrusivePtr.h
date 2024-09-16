
#ifndef __IC3_CPPX_INTRUSIVE_PTR_H__
#define __IC3_CPPX_INTRUSIVE_PTR_H__

#include "RefCounter.h"
#include <cassert>
#include <functional>

namespace Ic3::Cppx
{

	template <typename TPCounter = RefCounter>
	class RefCountedBase
	{
		friend struct RefCountedProxy;

	public:
		using MyType = RefCountedBase<TPCounter>;
		using CounterType = TPCounter;

	public:
		RefCountedBase( const RefCountedBase & ) = delete;
		RefCountedBase & operator=( const RefCountedBase & ) = delete;

		RefCountedBase() = default;

	protected:
		ref_counter_value_t addRef() const
		{
			return _counter.increment();
		}

		ref_counter_value_t releaseRef() const
		{
			return _counter.decrement();
		}

		ref_counter_value_t getRefsNum() const
		{
			return _counter.getValue();
		}

	private:
		mutable CounterType _counter;
	};


	struct RefCountedProxy
	{
		template <typename TPCounter>
		static ref_counter_value_t addRef( const RefCountedBase<TPCounter> * pRefCountedBase )
		{
			return pRefCountedBase->addRef();
		}

		template <typename TPCounter>
		static ref_counter_value_t releaseRef( const RefCountedBase<TPCounter> * pRefCountedBase )
		{
			return pRefCountedBase->releaseRef();
		}
	};


	template <typename TPRefCounted>
	inline ref_counter_value_t addRefDefault( const TPRefCounted * pObjectPtr )
	{
		return RefCountedProxy::addRef( pObjectPtr );
	}

	template <typename TPRefCounted>
	inline ref_counter_value_t releaseRefDefault( const TPRefCounted * pObjectPtr )
	{
		return RefCountedProxy::releaseRef( pObjectPtr );
	}


	template <typename TPRefCounted, typename TPDeleter = DefaultDelete<TPRefCounted>>
	class IntrusivePtr
	{
		template <typename, typename>
		friend class IntrusivePtr;

	public:
		using MyType = IntrusivePtr<TPRefCounted, TPDeleter>;

	public:
		IntrusivePtr()
		: _ptr( nullptr )
		{}

		IntrusivePtr( IntrusivePtr && pSrcObject )
		: _ptr( nullptr )
		{
			swap( pSrcObject );
		}

		IntrusivePtr( const IntrusivePtr & pSrcObject )
		: _ptr( nullptr )
		{
			_setNoRelease( pSrcObject._ptr );
		}

		template <typename TPOther>
		IntrusivePtr( IntrusivePtr<TPOther> && pOther )
		: _ptr( static_cast<TPRefCounted *>( pOther._ptr ) )
		{
			pOther._ptr = nullptr;
		}

		template <typename TPOther>
		IntrusivePtr( const IntrusivePtr<TPOther> & pOther )
		: _ptr( nullptr )
		{
			_setNoRelease( pOther._ptr );
		}

		IntrusivePtr( std::nullptr_t )
		: _ptr( nullptr )
		{}

		explicit IntrusivePtr( TPRefCounted * pObject )
		: _ptr( pObject )
		{}

		template <typename TPOther>
		explicit IntrusivePtr( TPOther * pObject )
		: _ptr( static_cast<TPRefCounted *>( pObject ) )
		{}

		~IntrusivePtr()
		{
			_release();
		}

		IntrusivePtr & operator=( IntrusivePtr && pRhs )
		{
			if( this != &pRhs )
			{
				IntrusivePtr( std::move( pRhs ) ).swap( *this );
			}

			return *this;
		}

		IntrusivePtr & operator=( const IntrusivePtr & pRhs )
		{
			if( this != &pRhs )
			{
				IntrusivePtr( pRhs ).swap( *this );
			}

			return *this;
		}

		template <typename TPOther>
		IntrusivePtr & operator=( IntrusivePtr<TPOther> && pRhs )
		{
			IntrusivePtr( std::move( pRhs ) ).swap( *this );
			return *this;
		}

		template <typename TPOther>
		IntrusivePtr & operator=( const IntrusivePtr<TPOther> & pRhs )
		{
			IntrusivePtr( pRhs ).swap( *this );
			return *this;
		}

		IntrusivePtr & operator=( std::nullptr_t )
		{
			_release();
			return *this;
		}

		IntrusivePtr & operator=( TPRefCounted * pRhs )
		{
			IntrusivePtr( pRhs ).swap( *this );
			return *this;
		}

		template <typename TPOther>
		IntrusivePtr & operator=( TPOther * pRhs )
		{
			IntrusivePtr( pRhs ).swap( *this );
			return *this;
		}

		explicit operator bool() const
		{
			return _ptr != nullptr;
		}

		explicit operator TPRefCounted * ( ) const
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

		void swap( IntrusivePtr & pOther )
		{
			std::swap( _ptr, pOther._ptr );
		}

		IC3_ATTR_NO_DISCARD bool empty() const
		{
			return _ptr != nullptr;
		}

	private:
		void _release()
		{
			if( _ptr != nullptr )
			{
				_releasePtr( _ptr );
				_ptr = nullptr;
			}
		}

		void _setNoRelease( TPRefCounted * pPointer )
		{
			if( ( pPointer != nullptr ) && ( addRefDefault( pPointer ) > 0 ) )
			{
				_ptr = pPointer;
			}
			else
			{
				_ptr = nullptr;
			}
		}

		template <typename TPOther>
		void _setNoRelease( TPOther * pPointer )
		{
			_setNoRelease( static_cast<TPRefCounted *>( pPointer ) );
		}

		void _set( TPRefCounted * pPointer )
		{
			if( pPointer != _ptr )
			{
				TPRefCounted * prevPtr = _ptr;
				_setNoRelease( pPointer );
				if( prevPtr != nullptr )
				{
					_releasePtr( prevPtr );
				}
			}
		}

		template <typename TPOther>
		void _set( TPOther * pPointer )
		{
			_set( static_cast<TPRefCounted *>( pPointer ) );
		}

		static void _releasePtr( TPRefCounted * pPointer )
		{
			ref_counter_value_t remainingRefs = releaseRefDefault( pPointer );
			if( remainingRefs == 0 )
			{
				TPDeleter()( pPointer );
			}
		}

	private:
		TPRefCounted * _ptr;
	};

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator==( const IntrusivePtr<TPRefCounted, TPDeleter> & pLhs, const IntrusivePtr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() == pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter>
	inline bool operator==( const IntrusivePtr<TPRefCounted, TPDeleter> & pLhs, std::nullptr_t )
	{
		return pLhs.empty();
	}

	template <typename TPRefCounted, typename TPDeleter>
	inline bool operator==( std::nullptr_t, const IntrusivePtr<TPRefCounted, TPDeleter> & pRhs )
	{
		return pRhs.empty();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator!=( const IntrusivePtr<TPRefCounted, TPDeleter> & pLhs, const IntrusivePtr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() != pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter>
	inline bool operator!=( const IntrusivePtr<TPRefCounted, TPDeleter> & pLhs, std::nullptr_t )
	{
		return !pLhs.empty();
	}

	template <typename TPRefCounted, typename TPDeleter>
	inline bool operator!=( std::nullptr_t, const IntrusivePtr<TPRefCounted, TPDeleter> & pRhs )
	{
		return !pRhs.empty();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator>( const IntrusivePtr<TPRefCounted, TPDeleter> & pLhs, const IntrusivePtr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() > pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator>=( const IntrusivePtr<TPRefCounted, TPDeleter> & pLhs, const IntrusivePtr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() >= pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator<( const IntrusivePtr<TPRefCounted, TPDeleter> & pLhs, const IntrusivePtr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() < pRhs.get();
	}

	template <typename TPRefCounted, typename TPDeleter, typename TPRefCounted2, typename TPDeleter2>
	inline bool operator<=( const IntrusivePtr<TPRefCounted, TPDeleter> & pLhs, const IntrusivePtr<TPRefCounted2, TPDeleter2> & pRhs )
	{
		return pLhs.get() <= pRhs.get();
	}


	template <typename TPRefCounted, typename TPDeleter>
	void swap( IntrusivePtr<TPRefCounted, TPDeleter> & pLeft, IntrusivePtr<TPRefCounted, TPDeleter> & pRight )
	{
		pLeft.swap( pRight );
	}


	template <typename TPRefCounted, class ...TPArgs>
	inline IntrusivePtr<TPRefCounted> makeIntrusive( TPArgs && ...pArgs )
	{
		return IntrusivePtr<TPRefCounted>( new TPRefCounted( std::forward<TPArgs>( pArgs )... ) );
	}

}

#endif /* __IC3_CPPX_INTRUSIVE_PTR_H__ */
