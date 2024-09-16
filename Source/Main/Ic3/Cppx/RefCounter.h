
#ifndef __IC3_CPPX_REF_COUNTER_H__
#define __IC3_CPPX_REF_COUNTER_H__

#include "Prerequisites.h"
#include <atomic>

namespace Ic3::Cppx
{

	/// @brief
	using ref_counter_value_t = native_uint;

	/// @brief
	enum class ERefCounterType : enum_default_value_t
	{
		Atomic = 1,
		NonAtomic,
	};

	/// @brief
	class RefCounter
	{
	public:
		RefCounter( const RefCounter & ) = delete;
		RefCounter& operator=( const RefCounter & ) = delete;

		explicit RefCounter( ref_counter_value_t pInitialCounter = 1 )
			: _refsNum( pInitialCounter )
		{}

		ref_counter_value_t increment()
		{
			return ++_refsNum;
		}

		ref_counter_value_t incrementCnz()
		{
			if( _refsNum > 0 )
			{
				++_refsNum;
			}

			return _refsNum;
		}

		ref_counter_value_t decrement()
		{
			return --_refsNum;
		}

		IC3_ATTR_NO_DISCARD ref_counter_value_t getValue() const
		{
			return _refsNum;
		}

	private:
		ref_counter_value_t _refsNum;
	};

	/// @brief
	class AtomicRefCounter
	{
	public:
		AtomicRefCounter( const AtomicRefCounter & ) = delete;
		AtomicRefCounter& operator=( const AtomicRefCounter & ) = delete;

		explicit AtomicRefCounter( ref_counter_value_t pInitialCounter = 1 )
		: _refsNum( pInitialCounter )
		{}

		ref_counter_value_t increment()
		{
			ref_counter_value_t refsNum = _refsNum.fetch_add( 1, std::memory_order_acq_rel );
			return refsNum + 1;
		}

		ref_counter_value_t incrementCnz()
		{
			for( ref_counter_value_t refsNum = _refsNum.load( std::memory_order_relaxed ); ; )
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

		ref_counter_value_t decrement()
		{
			ref_counter_value_t refsNum = _refsNum.fetch_sub( 1, std::memory_order_acq_rel );
			return refsNum - 1;
		}

		IC3_ATTR_NO_DISCARD ref_counter_value_t getValue() const
		{
			return _refsNum.load( std::memory_order_relaxed );
		}

	private:
		std::atomic<ref_counter_value_t> _refsNum;
	};


	template <ERefCounterType>
	struct RefCounterClass;

	template <>
	struct RefCounterClass<ERefCounterType::Atomic>
	{
		using Type = AtomicRefCounter;
	};

	template <>
	struct RefCounterClass<ERefCounterType::NonAtomic>
	{
		using Type = RefCounter;
	};

}

#endif /* __IC3_CPPX_REF_COUNTER_H__ */
