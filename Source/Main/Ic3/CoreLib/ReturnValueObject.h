
#pragma once

#ifndef __IC3_CORELIB_RETURN_VALUE_OBJECT_H__
#define __IC3_CORELIB_RETURN_VALUE_OBJECT_H__

#include "ExceptionCommon.h"

namespace Ic3
{

	template <bool tpIncludeExceptionPtr>
	struct TPResultExceptionWrapper
	{
		IC3_ATTR_NO_DISCARD constexpr bool hasException() const noexcept
		{
			return false;
		}
	};

	template <>
	struct TPResultExceptionWrapper<true>
	{
	public:
		//
		std::exception_ptr mException;

	public:
		void setException( std::exception_ptr pExceptionPtr )
		{
			mException = std::move( pExceptionPtr );
		}

		void saveCurrentException()
		{
			mException = std::current_exception();
		}

		void rethrowException()
		{
			auto localException = std::exception_ptr();
			std::swap( mException, localException );
			std::rethrow_exception( localException );
		}

		IC3_ATTR_NO_DISCARD constexpr bool hasException() const noexcept
		{
			return mException ? true : false;
		}
	};

    /// @brief
    template <bool tpIncludeExceptionPtr = true>
    struct TPResultWrapper : public TPResultExceptionWrapper<tpIncludeExceptionPtr>
    {
    public:
        //
        ResultCode mResultCode;

    public:
	    TPResultWrapper() = default;

	    TPResultWrapper( ResultCode pResultCode )
		: mResultCode( pResultCode )
	    {}

        explicit operator bool() const
        {
            return !isError();
        }

        void setResult( ResultCode pResultCode )
        {
            mResultCode = pResultCode;
        }

        IC3_ATTR_NO_DISCARD bool hasResult() const
        {
            return !mResultCode.empty();
        }

        IC3_ATTR_NO_DISCARD bool isError() const
        {
            return !mResultCode || this->hasException();
        }
    };

    /// @brief
    template <typename TPValue, bool tpIncludeExceptionPtr = true>
    struct TReturnValueObject : public TPResultWrapper<tpIncludeExceptionPtr>
    {
    public:
		using ResultWrapperType = TPResultWrapper<tpIncludeExceptionPtr>;
		
        //
        mutable TPValue mValue;

    public:
        TReturnValueObject() = default;

        explicit TReturnValueObject( ResultWrapperType pResultWrapper )
        : ResultWrapperType( std::move( pResultWrapper ) )
        {}

        TPValue & operator*() const
        {
            return mValue;
        }

        TPValue * operator->() const
        {
            return &( mValue );
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
