
#pragma once

#ifndef __IC3_CORELIB_LOCKABLE_H__
#define __IC3_CORELIB_LOCKABLE_H__

#include "MutexCommon.h"

namespace Ic3
{

    /// @brief Specifies how lock can be accessed and retrieved.
    enum class ELockableMutexAccess : enum_default_value_t
    {
        // Lock is always retrieved as non-const reference, which allows the object to be locked
        // in any possible state. Note, that this implies lock member to be declared as mutable.
        Relaxed,

        // The type of retrieved lock (in terms of its const-ness) matches the object's type.
        // This makes impossible to lock an object, that was declared/retrieved as const-qualified.
        Strict
    };


    /// @brief Helper base class for types representing lockable objects.
    /// @tparam TPMutex      Type of the mutex an instance of the class will contain.
    /// @tparam tpLockAccess Specifies how the mutex can be accessed. Modifies how the public interface is defined.
    /// TLockable<> is a simple base class which can be used to inject a locking behaviour. Type of the actual lock
    /// (like std::mutex or some custom ones) can be controlled via first template parameter. The second one controls
    /// the lock access (see ELockableMutexAccess for more details).
    /// @example
    /// TLockable<> also works with Ic3SyncAcquire*() macros. This enables a simple syntax to acquire locks:
    /// @code
    /// class SharedResource : public TLockable<std::mutex> {};
    /// SharedResource res;
    /// // Some time later:
    /// auto lockGuard = Ic3SyncAcquireUnique( res );
    /// @endcode
    template <typename TPMutex, ELockableMutexAccess tpLockAccess = ELockableMutexAccess::Strict>
    class TLockable;

    /// @brief Specialization of Lockable<> for ELockableMutexAccess::RELAXED.
    /// RELAXED lock access means, that internal lock is a mutable variable and all public
    /// lock-related methods are defined as const-qualified (const objects can be locked).
    template <typename TPMutex>
    class TLockable<TPMutex, ELockableMutexAccess::Relaxed>
    {
    public:
        using MutexType = TPMutex;
        using MutexInterfaceProxy = typename QMutexInterfaceProxy<TPMutex>::Type;

    public:
        TLockable() = default;

        virtual void Lock() const
        {
            MutexInterfaceProxy::lock( _mutex );
        }

        virtual bool TryLock() const
        {
            return MutexInterfaceProxy::tryLock( _mutex );
        }

        virtual void Unlock() const
        {
            MutexInterfaceProxy::unlock( _mutex );
        }

        TPMutex & GetMutex() const
        {
            return _mutex;
        }

    protected:
        mutable TPMutex _mutex;
    };

    /// @brief Specialization of TLockable<> for ELockableMutexAccess::STRICT.
    /// STRICT lock access means, that internal lock is a "normal" variable and all public
    /// lock-related methods are defined as non-const-qualified (const objects CANNOT be locked).
    template <typename TPMutex>
    class TLockable<TPMutex, ELockableMutexAccess::Strict>
    {
    public:
        using MutexType = TPMutex;
        using MutexInterfaceProxy = typename QMutexInterfaceProxy<TPMutex>::Type;

    public:
        TLockable() = default;

        virtual void Lock()
        {
            MutexInterfaceProxy::lock( _mutex );
        }

        virtual bool TryLock()
        {
            return MutexInterfaceProxy::tryLock( _mutex );
        }

        virtual void Unlock()
        {
            MutexInterfaceProxy::unlock( _mutex );
        }

        TPMutex & GetMutex()
        {
            return _mutex;
        }

        const TPMutex & GetMutex() const
        {
            return _mutex;
        }

    protected:
        TPMutex _mutex;
    };

    // getMutex() implementation for TLockable<>. Returns the internal lock. It enables all
    // TLockable objects to be used with Ic3SyncAcquireUnique()/Ic3SyncAcquireShared() macros.

    template <typename TPMutex>
    inline constexpr TPMutex & GetLock( const TLockable<TPMutex, ELockableMutexAccess::Relaxed> & pLock )
    {
        return pLock.getMutex();
    }

    template <typename TPMutex>
    inline constexpr TPMutex & GetLock( TLockable<TPMutex, ELockableMutexAccess::Strict> & pLock )
    {
        return pLock.getMutex();
    }

}

#endif // __IC3_CORELIB_LOCKABLE_H__
