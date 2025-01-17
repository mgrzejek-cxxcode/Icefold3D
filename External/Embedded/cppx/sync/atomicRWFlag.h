
#pragma once

#ifndef __CPPX_ATOMIC_RW_FLAG_H__
#define __CPPX_ATOMIC_RW_FLAG_H__

#include "commonSyncDefs.h"
#include <atomic>

namespace cppx::sync
{

	enum class atomic_rw_flag_state : uint32
	{
		set,
		unset
	};

	class atomic_rw_flag
	{
	public:
		atomic_rw_flag( const atomic_rw_flag & ) = delete;
		atomic_rw_flag& operator=( const atomic_rw_flag & ) = delete;

		atomic_rw_flag()
		: _internalState{ atomic_rw_flag_state::unset }
		{}

		bool test( std::memory_order pMemoryOrder = std::memory_order_acquire ) const noexcept
		{
			return _internalState.load( pMemoryOrder ) == atomic_rw_flag_state::set;
		}

		bool test_and_set( std::memory_order pMemoryOrder = std::memory_order_acq_rel ) noexcept
		{
			return _internalState.exchange( atomic_rw_flag_state::set, pMemoryOrder ) == atomic_rw_flag_state::set;
		}

		void set( std::memory_order pMemoryOrder = std::memory_order_release ) noexcept
		{
			_internalState.store( atomic_rw_flag_state::set, pMemoryOrder );
		}

		void clear( std::memory_order pMemoryOrder = std::memory_order_release ) noexcept
		{
			_internalState.store( atomic_rw_flag_state::unset, pMemoryOrder );
		}

	private:
		std::atomic<atomic_rw_flag_state> _internalState;
	};

}

#endif // __CPPX_ATOMIC_RW_FLAG_H__
