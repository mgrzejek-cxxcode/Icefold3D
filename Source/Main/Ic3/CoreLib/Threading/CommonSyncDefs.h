
#pragma once

#ifndef __IC3_CORELIB_COMMON_SYNC_DEFS_H__
#define __IC3_CORELIB_COMMON_SYNC_DEFS_H__

#include "../Prerequisites.h"

namespace Ic3
{

    /// @brief Returns a lock from a specified lockable object. Used by Sync macros.
    /// @tparam TPLock Type of the lockable object, deduced automatically from the parameter.
    /// @param  pLock  A lockable object. By default it is returned directly.
    /// @details
    /// This is a simple helper function that enables our sync macros to have a single, lock-type-agnostic
    /// implementation. In order to make custom lockable types working,
    template <typename TPLock>
    inline constexpr TPLock & GetLock( TPLock & pLock )
    {
	    return pLock;
    }

    // Helper macro for acquiring a lock using a provider guard/holder type.
    #define __Ic3AcquireUniqueLockHelper( pLockHolder, pLock ) \
        pLockHolder<typename std::remove_reference<decltype( pLock )>::type>{ pLock }

    /// @brief Acquires a unique access to a specified lock. Uses std::unique_lock<> for RAII.
    #define Ic3SyncAcquireUnique( pLock ) \
        __Ic3AcquireUniqueLockHelper( ::Ic3::Sync::TAutoUniqueLock, Sync::GetLock( pLock ) )

    /// @brief Acquires a shared access to a specified lock. Uses std::shared_lock<> for RAII.
    #define Ic3SyncAcquireShared( pLock ) \
        __Ic3AcquireUniqueLockHelper( ::Ic3::Sync::TAutoSharedLock, Sync::GetLock( pLock ) )

}

#endif // __IC3_CORELIB_COMMON_SYNC_DEFS_H__
