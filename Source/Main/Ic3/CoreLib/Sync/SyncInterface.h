
#pragma once

#ifndef __IC3_COMMON_SYNC_INTERFACE_H__
#define __IC3_COMMON_SYNC_INTERFACE_H__

#include "MutexLockTypes.h"

namespace Ic3
{

    // Helper macro for acquiring a lock using a provider guard/holder type.
    #define __ic3AcquireUniqueLockHelper( pLockHolder, pLock ) \
        pLockHolder<typename std::remove_reference<decltype( pLock )>::type>{ pLock }

    /// @brief Acquires a unique access to a specified lock. Uses std::unique_lock<> for RAII.
    #define ic3SyncAcquireUnique( pLock ) \
        __ic3AcquireUniqueLockHelper( ::Ic3::Sync::AutoUniqueLock, getLock( pLock ) )

    /// @brief Acquires a shared access to a specified lock. Uses std::shared_lock<> for RAII.
    #define ic3SyncAcquireShared( pLock ) \
        __ic3AcquireUniqueLockHelper( ::Ic3::Sync::AutoSharedLock, getLock( pLock ) )

}

#endif // __IC3_COMMON_SYNC_INTERFACE_H__
