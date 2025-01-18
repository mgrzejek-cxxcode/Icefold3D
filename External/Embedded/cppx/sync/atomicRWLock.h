
#pragma once

#ifndef __CPPX_ATOMIC_RWLOCK_H__
#define __CPPX_ATOMIC_RWLOCK_H__

#include "syncCommon.h"
#include <atomic>

namespace cppx::sync
{

    enum class rw_lock_state : uint32
    {
        available,
        locked
    };

    /// @brief Atomic-based spin-lock that provides the ability of observing the status of the lock.
    class AtomicRWLock
    {
    public:
        AtomicRWLock() = default;

        void lock()
        {
            for( auto spinCounter = 0; ; ++spinCounter )
            {
                auto expectedFlagState = rw_lock_state::available;
                if( _lockFlag.compare_exchange_weak( expectedFlagState,
                                                     rw_lock_state::locked,
                                                     std::memory_order_acq_rel,
                                                     std::memory_order_relaxed ) )
                {
                    break;
                }

                yield_current_thread_auto( spinCounter );
            }
        }

        void unlock()
        {
            _lockFlag.store( rw_lock_state::Available, std::memory_order_release );
        }

        bool tryLock()
        {
            auto currentFlagState = _lockFlag.load( std::memory_order_acquire );
            if( currentFlagState == rw_lock_state::Available )
            {
                if( _lockFlag.compare_exchange_strong( currentFlagState,
                                                       rw_lock_state::Locked,
                                                       std::memory_order_acq_rel,
                                                       std::memory_order_relaxed ) )
                {
                    return true;
                }
            }
            return false;
        }

        CPPX_ATTR_NO_DISCARD rw_lock_state state() const
        {
            return _lockFlag.load( std::memory_order_acquire );
        }

        CPPX_ATTR_NO_DISCARD bool locked() const
        {
            return state() == rw_lock_state::Locked;
        }

    private:
        std::atomic<rw_lock_state> _lockFlag;
    };

}

#endif // __CPPX_ATOMIC_RWLOCK_H__
