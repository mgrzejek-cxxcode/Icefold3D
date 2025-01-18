
#pragma once

#ifndef __CPPX_SPIN_LOCK_H__
#define __CPPX_SPIN_LOCK_H__

#include "atomicRWFlag.h"
#include "../bitmask.h"
#include <atomic>

namespace cppx::sync
{

	enum shader_spin_lock_flags : uint32
	{
		shader_spin_lock_flag_use_write_request_policy_bit = 0x001
	};

    /// @brief
    class spin_lock
    {
    public:
	    spin_lock( const spin_lock & ) = delete;
	    spin_lock& operator=( const spin_lock & ) = delete;

	    spin_lock()
        : _flag{ ATOMIC_FLAG_INIT }
        {}

        /// @brief
        void lock() volatile noexcept
        {
            for( auto spinCounter = 0; _flag.test_and_set( std::memory_order_acq_rel ); ++spinCounter )
            {
                yield_current_thread_auto( spinCounter );
            }
        }

        /// @brief
        bool try_lock() volatile noexcept
        {
            return !_flag.test_and_set( std::memory_order_acq_rel );
        }

        /// @brief
        void unlock() volatile noexcept
        {
            _flag.clear( std::memory_order_release );
        }

    private:
        std::atomic_flag _flag;
    };

    /// @brief
    class shared_spin_lock
    {
    public:
	    shared_spin_lock( const shared_spin_lock & ) = delete;
	    shared_spin_lock& operator=( const shared_spin_lock & ) = delete;

	    shared_spin_lock( bitmask<shader_spin_lock_flags> pFlags = 0 )
        : _flags( pFlags )
		, _sharedCounter{ 0 }
        {}

        /// @brief
        void lock() noexcept
        {
			if( _flags.is_set( shader_spin_lock_flag_use_write_request_policy_bit ) )
			{
				// Set write request flag - it will reduce the read priority
				_writeRequestFlag.test_and_set( std::memory_order_release );
			}

            for( auto spinCounter = 0; ; ++spinCounter )
            {
                // Acquire unique access. If some other thread is holding it, this loop will block the current thread.
                if( !_uniqueAccessFlag.test_and_set( std::memory_order_acq_rel ) )
                {
	                if( _flags.is_set( shader_spin_lock_flag_use_write_request_policy_bit ) )
	                {
	                    _writeRequestFlag.clear( std::memory_order_release );
	                }

	                // Wait until all threads which hold shared access will release it.
                    for( spinCounter = 0; _sharedCounter.load( std::memory_order_acquire ) > 0; ++spinCounter )
                    {
                        yield_current_thread_auto( spinCounter );
                    }

                    break;
                }

	            yield_current_thread_auto( spinCounter );
            }
        }

        /// @brief
        bool try_lock() noexcept
        {
            if( !_uniqueAccessFlag.test_and_set( std::memory_order_acq_rel ) )
            {
                if( _sharedCounter.load( std::memory_order_acquire ) == 0 )
                {
                    // Unique access flag has been set, shared access counter is zero - unique access has been acquired.
                    return true;
                }

				//
                _uniqueAccessFlag.clear();
            }

            return false;
        }

        /// @brief
        void unlock() noexcept
        {
            _uniqueAccessFlag.clear();
        }

        /// @brief
        void lock_shared() noexcept
        {
	        if( _flags.is_set( shader_spin_lock_flag_use_write_request_policy_bit ) )
	        {
		        for( auto spinCounter = 0; _writeRequestFlag.test( std::memory_order_acquire ); ++spinCounter )
		        {
			        yield_current_thread_auto( spinCounter );
		        }
	        }

	        // Acquire temporary unique access.
            for( auto spinCounter = 0; !_uniqueAccessFlag.test_and_set( std::memory_order_acq_rel ); ++spinCounter )
            {
	            yield_current_thread_auto( spinCounter );
            }

	        // Add 1 to shared counter. Unique access will not be allowed as long as shared counter is greater than zero.
	        _sharedCounter.fetch_add( 1, std::memory_order_acq_rel );

	        _uniqueAccessFlag.clear( std::memory_order_release );
        }

        /// @brief
        bool try_lock_shared() noexcept
        {
	        if( _flags.is_set( shader_spin_lock_flag_use_write_request_policy_bit ) )
	        {
		        if( _writeRequestFlag.test( std::memory_order_acquire ) )
		        {
			        return false;
		        }
	        }

            // Acquire temporary unique access.
            if( !_uniqueAccessFlag.test( std::memory_order_acquire ) )
            {
                // Add 1 to shared counter. Unique access will not be allowed as long as shared counter is greater than zero.
                _sharedCounter.fetch_add( 1, std::memory_order_acq_rel );

                // Clear unique access flag, as we acquired shared access.
                _uniqueAccessFlag.clear();

                // Exit.
                return true;
            }

            return false;
        }

        /// @brief
        void unlockShared() noexcept
        {
            _sharedCounter.fetch_sub( 1, std::memory_order_acq_rel );
        }

    private:
		const bitmask<shader_spin_lock_flags> _flags;
        atomic_rw_flag _writeRequestFlag;
	    atomic_rw_flag _uniqueAccessFlag;
        std::atomic<uint32> _sharedCounter;
    };

}

#endif // __CPPX_SPIN_LOCK_H__
