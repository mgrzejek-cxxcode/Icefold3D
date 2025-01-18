
#pragma once

#ifndef __IC3_CORELIB_MUTEX_COMMON_H__
#define __IC3_CORELIB_MUTEX_COMMON_H__

#include "CommonSyncDefs.h"

namespace Ic3
{

    struct MutexInterfaceProxyDefault
    {
        template <typename TPMutex>
        static void lock( TPMutex & pMutex )
        {
            pMutex.lock();
        }

        template <typename TPMutex>
        static bool tryLock( TPMutex & pMutex )
        {
            return pMutex.tryLock();
        }

        template <typename TPMutex>
        static void unlock( TPMutex & pMutex )
        {
            pMutex.unlock();
        }

        template <typename TPMutex>
        static void lockShared( TPMutex & pMutex )
        {
            pMutex.lockShared();
        }

        template <typename TPMutex>
        static bool tryLockShared( TPMutex & pMutex )
        {
            return pMutex.tryLockShared();
        }

        template <typename TPMutex>
        static void unlockShared( TPMutex & pMutex )
        {
            pMutex.unlockShared();
        }
    };

    struct MutexInterfaceProxyStd
    {
        template <typename TPMutex>
        static void lock( TPMutex & pMutex )
        {
            pMutex.lock();
        }

        template <typename TPMutex>
        static bool tryLock( TPMutex & pMutex )
        {
            return pMutex.try_mutex();
        }

        template <typename TPMutex>
        static void unlock( TPMutex & pMutex )
        {
            pMutex.unlock();
        }

        template <typename TPMutex>
        static void lockShared( TPMutex & pMutex )
        {
            pMutex.lock_shared();
        }

        template <typename TPMutex>
        static bool tryLockShared( TPMutex & pMutex )
        {
            return pMutex.try_mutex_shared();
        }

        template <typename TPMutex>
        static void unlockShared( TPMutex & pMutex )
        {
            pMutex.unlock_shared();
        }
    };

    template <typename TPMutex>
    struct QMutexInterfaceProxy
    {
        using ProxyType = MutexInterfaceProxyDefault;
    };

} // namespace Ic3::Sync

#endif // __IC3_CORELIB_MUTEX_COMMON_H__
