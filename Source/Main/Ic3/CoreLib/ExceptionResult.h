
#pragma once

#ifndef __IC3_CORELIB_EXCEPTION_RESULT_H__
#define __IC3_CORELIB_EXCEPTION_RESULT_H__

#include "ExceptionCommon.h"

namespace Ic3
{

    /// @brief
    struct ResultWrapper
    {
    public:
        //
        ResultCode resultCode;
        //
        std::exception_ptr exception;

    public:
        ResultWrapper() = default;

        explicit operator bool() const
        {
            return !isError();
        }

        void setResult( ResultCode pResultCode )
        {
            resultCode = pResultCode;
        }

        void setException( std::exception_ptr pExceptionPtr )
        {
            exception = std::move( pExceptionPtr );
        }

        void saveCurrentException()
        {
            exception = std::current_exception();
        }

        void rethrowException()
        {
            auto localException = std::exception_ptr();
            std::swap( exception, localException );
            std::rethrow_exception( localException );
        }

        IC3_ATTR_NO_DISCARD bool hasException() const
        {
            return exception ? true : false;
        }

        IC3_ATTR_NO_DISCARD bool hasResult() const
        {
            return !resultCode.empty();
        }

        IC3_ATTR_NO_DISCARD bool isError() const
        {
            return !resultCode || exception;
        }
    };

    /// @brief
    template <typename TValue>
    struct ReturnValueWrapper : public ResultWrapper
    {
    public:
        //
        mutable TValue value;

    public:
        ReturnValueWrapper() = default;

        explicit ReturnValueWrapper( ResultWrapper pResultWrapper )
        : ResultWrapper( std::move( pResultWrapper ) )
        {}

        TValue & operator*() const
        {
            return value;
        }

        TValue * operator->() const
        {
            return &( value );
        }
    };

    /// @brief
    template <>
    struct ReturnValueWrapper<void> : public ResultWrapper
    {
    public:
        explicit ReturnValueWrapper() = default;
    };

} // namespace Ic3

#endif // __IC3_CORELIB_EXCEPTION_RESULT_H__
