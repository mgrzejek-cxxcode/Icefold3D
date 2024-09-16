
#pragma once

#ifndef __IC3_CORELIB_MUTEX_LOCK_TYPES_H__
#define __IC3_CORELIB_MUTEX_LOCK_TYPES_H__

#include "MutexCommon.h"

namespace Ic3::Sync
{

    template <typename TPMutex, typename TPMutexInterface>
    class UniqueLock
    {
    public:
        UniqueLock( TPMutex & pMutex )
        : _mutex( &pMutex )
        , _ownsLock( false )
        {
            lock();
        }

        UniqueLock( TPMutex & pMutex, const std::defer_lock_t & )
        : _mutex( &pMutex )
        , _ownsLock( false )
        {}

        UniqueLock( TPMutex & pMutex, const std::try_to_lock_t & )
        : _mutex( &pMutex )
        , _ownsLock( false )
        {
            _ownsLock = tryLock();
        }

        UniqueLock( TPMutex & pMutex, const std::adopt_lock_t & )
        : _mutex( &pMutex )
        , _ownsLock( true )
        {}

        UniqueLock( UniqueLock && pSrcObject )
        : _mutex( pSrcObject._mutex )
        , _ownsLock( pSrcObject._ownsLock )
        {
            pSrcObject._mutex = nullptr;
            pSrcObject._ownsLock = false;
        }

        ~UniqueLock()
        {
            if( _ownsLock )
            {
                unlock();
            }
        }

        void lock()
        {
            TPMutexInterface::lock( *_mutex );
            _ownsLock = true;
        }

        bool tryLock()
        {
            _ownsLock = TPMutexInterface::tryLock( *_mutex );
            return _ownsLock;
        }

        void unlock()
        {
            TPMutexInterface::unlock( *_mutex );
            _ownsLock = false;
        }

        TPMutex * releaseMutex()
        {
            TPMutex * mutexPtr = _mutex;
            _mutex = nullptr;
            _ownsLock = false;
            return mutexPtr;

        }

        IC3_ATTR_NO_DISCARD bool checkLockOwnership() const
        {
            return _ownsLock;
        }

        void swap( UniqueLock & pOther )
        {
            std::swap( _mutex, pOther._mutex );
            std::swap( _ownsLock, pOther._ownsLock );
        }

    private:
        TPMutex * _mutex;
        bool _ownsLock;
    };

    template <typename TPMutex, typename TPMutexInterface>
    class SharedLock
    {
    public:
        explicit SharedLock( TPMutex & pMutex )
        : _mutex( &pMutex )
        , _ownsLock( false )
        {
            lock();
        }

        SharedLock( TPMutex & pMutex, const std::defer_lock_t & )
        : _mutex( &pMutex )
        , _ownsLock( false )
        {}

        SharedLock( TPMutex & pMutex, const std::try_to_lock_t & )
        : _mutex( &pMutex )
        , _ownsLock( false )
        {
            _ownsLock = tryLock();
        }

        SharedLock( TPMutex & pMutex, const std::adopt_lock_t & )
        : _mutex( &pMutex )
        , _ownsLock( true )
        {}

        SharedLock( SharedLock && pSrcObject ) noexcept
        : _mutex( pSrcObject._mutex )
        , _ownsLock( pSrcObject._ownsLock )
        {
            pSrcObject._mutex = nullptr;
            pSrcObject._ownsLock = false;
        }

        ~SharedLock()
        {
            if( _ownsLock )
            {
                unlock();
            }
        }

        SharedLock & operator=( SharedLock && pRhs ) noexcept
        {
            SharedLock( std::move( pRhs ) ).swap( *this );
            return *this;
        }

        void lock()
        {
            TPMutexInterface::lockShared( *_mutex );
            _ownsLock = true;
        }

        bool tryLock()
        {
            _ownsLock = TPMutexInterface::tryLockShared( *_mutex );
            return _ownsLock;
        }

        void unlock()
        {
            TPMutexInterface::unlockShared( *_mutex );
            _ownsLock = false;
        }

        TPMutex * releaseMutex()
        {
            TPMutex * mutexPtr = _mutex;
            _mutex = nullptr;
            _ownsLock = false;
            return mutexPtr;

        }

        IC3_ATTR_NO_DISCARD bool checkLockOwnership() const
        {
            return _ownsLock;
        }

        void swap( SharedLock & pOther )
        {
            std::swap( _mutex, pOther._mutex );
            std::swap( _ownsLock, pOther._ownsLock );
        }

    private:
        TPMutex * _mutex;
        bool _ownsLock;
    };


    template <typename TPMutex>
    using AutoUniqueLock = UniqueLock<TPMutex, typename QMutexInterfaceProxy<TPMutex>::ProxyType>;

    template <typename TPMutex>
    using AutoSharedLock = SharedLock<TPMutex, typename QMutexInterfaceProxy<TPMutex>::ProxyType>;

}

#endif // __IC3_CORELIB_MUTEX_LOCK_TYPES_H__
