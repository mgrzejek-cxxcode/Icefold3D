
#pragma once

#ifndef __IC3_CORELIB_RETURN_VALUE_OBJECT_H__
#define __IC3_CORELIB_RETURN_VALUE_OBJECT_H__

#include "ExceptionCommon.h"

namespace Ic3
{

	template <bool tpIncludeExceptionPtr>
	struct TPResultExceptionWrapper
	{
		CPPX_ATTR_NO_DISCARD constexpr bool hasException() const noexcept
		{
			return false;
		}
	};

	template <>
	struct TPResultExceptionWrapper<true>
	{
	public:
		//
		std::exception_ptr exceptionPtr;

	public:
		void setException( std::exception_ptr pExceptionPtr )
		{
			exceptionPtr = std::move( pExceptionPtr );
		}

		void saveCurrentException()
		{
			exceptionPtr = std::current_exception();
		}

		void rethrowException()
		{
			auto localException = std::exception_ptr();
			std::swap( exceptionPtr, localException );
			std::rethrow_exception( localException );
		}

		CPPX_ATTR_NO_DISCARD constexpr bool hasException() const noexcept
		{
			return exceptionPtr ? true : false;
		}
	};

    /// @brief
    template <bool tpIncludeExceptionPtr = true>
    struct TPResultWrapper : public TPResultExceptionWrapper<tpIncludeExceptionPtr>
    {
    public:
        //
        ResultCode resultCode;

    public:
	    TPResultWrapper() = default;

	    TPResultWrapper( ResultCode pResultCode )
		: resultCode( pResultCode )
	    {}

        explicit operator bool() const
        {
            return !isError();
        }

        void setResult( ResultCode pResultCode )
        {
            resultCode = pResultCode;
        }

        CPPX_ATTR_NO_DISCARD bool hasResult() const
        {
            return !resultCode.empty();
        }

        CPPX_ATTR_NO_DISCARD bool isError() const
        {
            return !resultCode || this->hasException();
        }
    };

    /// @brief
    template <typename TPValue, bool tpIncludeExceptionPtr = true>
    struct TReturnValueObject : public TPResultWrapper<tpIncludeExceptionPtr>
    {
    public:
		using ResultWrapperType = TPResultWrapper<tpIncludeExceptionPtr>;
		
        //
        mutable TPValue value;

    public:
        TReturnValueObject() = default;

        explicit TReturnValueObject( ResultWrapperType pResultWrapper )
        : ResultWrapperType( std::move( pResultWrapper ) )
        {}

        TPValue & operator*() const
        {
            return value;
        }

        TPValue * operator->() const
        {
            return &( value );
        }
    };

    /// @brief
    template <bool tpIncludeExceptionPtr>
    struct TReturnValueObject<void, tpIncludeExceptionPtr> : public TPResultWrapper<tpIncludeExceptionPtr>
    {
    public:
        explicit TReturnValueObject() = default;
    };

    /// @brief
    template <>
    struct TReturnValueObject<void, false> : public TPResultWrapper<false>
    {
    public:
        explicit TReturnValueObject() = default;
    };

} // namespace Ic3

#endif // __IC3_CORELIB_RETURN_VALUE_OBJECT_H__
