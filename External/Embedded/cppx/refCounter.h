
#ifndef __CPPX_REF_COUNTER_H__
#define __CPPX_REF_COUNTER_H__

#include "prerequisites.h"
#include <atomic>

namespace cppx
{

	/// @brief
	using ref_counter_default_value_t = native_uint;

	/// @brief
	enum class ref_counter_type : enum_default_value_t
	{
		atomic = 1,
		non_atomic,
	};

	/// @brief
	template <typename TPRefCounterValue = ref_counter_default_value_t>
	class ref_counter
	{
	public:
		ref_counter( const ref_counter & ) = delete;
		ref_counter& operator=( const ref_counter & ) = delete;

		explicit ref_counter( TPRefCounterValue pInitialCounter = 1 )
		: _refsNum( pInitialCounter )
		{}

		TPRefCounterValue increment()
		{
			return ++_refsNum;
		}

		TPRefCounterValue increment_cnz()
		{
			if( _refsNum > 0 )
			{
				++_refsNum;
			}

			return _refsNum;
		}

		TPRefCounterValue decrement()
		{
			return --_refsNum;
		}

		CPPX_ATTR_NO_DISCARD TPRefCounterValue get_value() const
		{
			return _refsNum;
		}

	private:
		TPRefCounterValue _refsNum;
	};

	/// @brief
	template <typename TPRefCounterValue = ref_counter_default_value_t>
	class atomic_ref_counter
	{
	public:
		atomic_ref_counter( const atomic_ref_counter & ) = delete;
		atomic_ref_counter& operator=( const atomic_ref_counter & ) = delete;

		explicit atomic_ref_counter( TPRefCounterValue pInitialCounter = 1 )
		: _refsNum( pInitialCounter )
		{}

		TPRefCounterValue increment()
		{
			TPRefCounterValue refsNum = _refsNum.fetch_add( 1, std::memory_order_acq_rel );
			return refsNum + 1;
		}

		TPRefCounterValue increment_cnz()
		{
			for( TPRefCounterValue refsNum = _refsNum.load( std::memory_order_relaxed ); ; )
			{
				if( refsNum == 0 )
				{
					return 0;
				}

				if( _refsNum.compare_exchange_weak( refsNum, refsNum + 1, std::memory_order_acq_rel, std::memory_order_relaxed ) )
				{
					return refsNum + 1;
				}
			}
		}

		TPRefCounterValue decrement()
		{
			TPRefCounterValue refsNum = _refsNum.fetch_sub( 1, std::memory_order_acq_rel );
			return refsNum - 1;
		}

		CPPX_ATTR_NO_DISCARD TPRefCounterValue get_value( std::memory_order pMemoryOrder = std::memory_order_relaxed ) const
		{
			return _refsNum.load( pMemoryOrder );
		}

	private:
		std::atomic<TPRefCounterValue> _refsNum;
	};


	template <ref_counter_type, typename TPRefCounterValue = ref_counter_default_value_t>
	struct ref_counter_class;

	template <typename TPRefCounterValue>
	struct ref_counter_class<ref_counter_type::atomic, TPRefCounterValue>
	{
		using type = atomic_ref_counter<TPRefCounterValue>;
	};

	template <typename TPRefCounterValue>
	struct ref_counter_class<ref_counter_type::non_atomic, TPRefCounterValue>
	{
		using type = ref_counter<TPRefCounterValue>;
	};

}

#endif /* __CPPX_REF_COUNTER_H__ */
