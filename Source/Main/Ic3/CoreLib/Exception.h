
#pragma once

#ifndef __IC3_CORELIB_EXCEPTION_H__
#define __IC3_CORELIB_EXCEPTION_H__

#include "ExceptionCommon.h"
#include <exception>

namespace Ic3
{

    enum : exception_category_value_t
    {
        E_EXCEPTION_CATEGORY_DEBUG =
            CxDef::declareExceptionCategory( ExceptionBaseType::Debug, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_ENGINE_SUBMODULE =
            CxDef::declareExceptionCategory( ExceptionBaseType::EngineSubModule, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_FRAMEWORK_CORE =
            CxDef::declareExceptionCategory( ExceptionBaseType::FrameworkCore, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_INTERNAL =
            CxDef::declareExceptionCategory( ExceptionBaseType::Internal, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_INTERRUPT =
            CxDef::declareExceptionCategory( ExceptionBaseType::Interrupt, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_MATH =
            CxDef::declareExceptionCategory( ExceptionBaseType::Math, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_RESULT_PROXY =
            CxDef::declareExceptionCategory( ExceptionBaseType::ResultProxy, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_SYSTEM =
            CxDef::declareExceptionCategory( ExceptionBaseType::System, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_USER_EXTERNAL =
            CxDef::declareExceptionCategory( ExceptionBaseType::UserExternal, ic3ExcCategoryIID( 0 ) ),
    };

    enum : exception_code_value_t
    {
        E_EXC_DEBUG_PLACEHOLDER =
            CxDef::declareExceptionCode( E_EXCEPTION_CATEGORY_DEBUG, ic3ExcCodeIID( 0x01 ) ),

        E_EXC_RESULT_CODE_ERROR =
            CxDef::declareExceptionCode( E_EXCEPTION_CATEGORY_RESULT_PROXY, ic3ExcCodeIID( 0x01 ) ),
    };

	/// @brief
	struct ExceptionInfo
	{
		// Exception code
		exception_code_value_t code;
		//
		std::string description;
		//
		FileLocationInfo fileLocationInfo;

		std::string toString() const;
	};

	/// @brief
	class Exception : public std::exception
	{
	public:
	    // Exception info.
	    ExceptionInfo mExceptionInfo;

	    // Text representation of the exception info.
	    std::string mString;

	public:
		/// @brief Constructor.
		/// @param pType Type of the exception, represented as ExceptionBaseType enum.
		/// @param pInfo Exception description, containing basic info about it.
		Exception( ExceptionInfo pExceptionInfo )
		: mExceptionInfo( std::move( pExceptionInfo ) )
		, mString( mExceptionInfo.toString() )
		{}

		virtual ~Exception() = default;

		/// @brief Returns the name of the exception.
		/// @return Name of the exception.
		virtual ExceptionBaseType getBaseType() const = 0;

		/// @brief Returns the name of the exception.
		/// @return Name of the exception.
		virtual const std::string & getBaseTypeName() const = 0;

		/// @brief
		/// @return
		virtual const char * what() const noexcept override
		{
		    return mString.c_str();
		}
	};

	/// @brief
	/// @tparam tExceptionBaseType
	template <ExceptionBaseType tExceptionBaseType>
	class ExceptionClass : public Exception
	{
	public:
		/// @brief
		static constexpr auto mBaseType = tExceptionBaseType;

	public:
		explicit ExceptionClass( ExceptionInfo pExceptionInfo )
		: Exception( std::move( pExceptionInfo ) )
		{}

		/// @refitem
		/// @return
		virtual ExceptionBaseType getBaseType() const override final
		{
			return tExceptionBaseType;
		}

		/// @refitem
		/// @return
		virtual const std::string & getBaseTypeName() const override final
		{
			// static std::once_flag initFlag;
			static std::string baseTypeName;

			// std::call_once( initFlag, []() {
			// 	const auto & enumInfo = queryEnumTypeInfo<ExceptionBaseType>();
			// 	const auto & baseTypeConstantInfo = enumInfo.getConstantMap().getByValue( tExceptionBaseType );
			// 	baseTypeName = baseTypeConstantInfo.name;
			// } );

			return baseTypeName;
		}
	};

	/// @brief Default base class for ExceptionBaseType::Debug.
	using UnknownException = ExceptionClass<ExceptionBaseType::Unknown>;

	/// @brief Default base class for ExceptionBaseType::Debug.
	using DebugException = ExceptionClass<ExceptionBaseType::Debug>;

	/// @brief Default base class for ExceptionBaseType::EngineSubModule.
	using EngineSubModuleException = ExceptionClass<ExceptionBaseType::EngineSubModule>;

	/// @brief Default base class for ExceptionBaseType::FrameworkCore.
	using FrameworkCoreException = ExceptionClass<ExceptionBaseType::FrameworkCore>;

	/// @brief Default base class for ExceptionBaseType::Internal.
	using InternalException = ExceptionClass<ExceptionBaseType::Internal>;

	/// @brief Default base class for ExceptionBaseType::Interrupt.
	using InterruptException = ExceptionClass<ExceptionBaseType::Interrupt>;

	/// @brief Default base class for ExceptionBaseType::Math.
	using MathException = ExceptionClass<ExceptionBaseType::Math>;

	/// @brief Default base class for ExceptionBaseType::System.
	using SystemException = ExceptionClass<ExceptionBaseType::System>;

	/// @brief Default base class for ExceptionBaseType::UserExternal.
	using UserExternalException = ExceptionClass<ExceptionBaseType::UserExternal>;

	/// @brief Specialized class for ResultWrapper exceptions. Adds ResultProxy object.
	template <typename TValue, typename TErrorPredicate>
	class ResultProxyException : public ExceptionClass<ExceptionBaseType::ResultProxy>
    {
    public:
        ResultProxy<TValue, TErrorPredicate> mResult;

    public:
        ResultProxyException( ExceptionInfo pExceptionInfo, ResultProxy<TValue, TErrorPredicate> pResult )
        : ExceptionClass( std::move( pExceptionInfo ) )
        , mResult( pResult )
        {}
    };

	class ResultCodeException : public ResultProxyException<ResultCode::ValueType, ResultCode::ErrorPredicateType>
	{
	public:
		ResultCodeException( ExceptionInfo pExceptionInfo, ResultCode pResultCode )
		: ResultProxyException( std::move( pExceptionInfo ), pResultCode )
		{}
	};


	template <ExceptionBaseType tExceptionBaseType, bool tIsValidType>
	struct ExceptionClassResolver
    {
	    using Type = UnknownException;
    };

	template <ExceptionBaseType tExceptionBaseType>
	struct ExceptionClassResolver<tExceptionBaseType, true>
    {
	    using Type = ExceptionClass<tExceptionBaseType>;
    };

	template <>
	struct ExceptionClassResolver<ExceptionBaseType::ResultProxy, true>
    {
		template <typename TValue, typename TErrorPredicate>
	    using Type = ResultProxyException<TValue, TErrorPredicate>;
    };

	template <ExceptionBaseType tExceptionBaseType>
	struct ExceptionBaseTypeClassProxy
    {
	    using Type = typename ExceptionClassResolver<tExceptionBaseType, CxDef::isExceptionBaseTypeValid( tExceptionBaseType )>::Type;
    };

	template <exception_category_value_t tExceptionCategory>
	struct ExceptionCategoryClassProxy
    {
	    using Type = typename ExceptionBaseTypeClassProxy<CxDef::getExceptionCategoryBaseType( tExceptionCategory )>::Type;
    };

	template <exception_code_value_t tExceptionCode>
	struct ExceptionCodeClassProxy
    {
	    using Type = typename ExceptionCategoryClassProxy<CxDef::getExceptionCodeCategory( tExceptionCode )>::Type;
    };

	template <>
	struct ExceptionCodeClassProxy<E_EXC_RESULT_CODE_ERROR>
	{
		using Type = ResultCodeException;
	};

    #define ic3EnableCustomExceptionSupport() \
        template <exception_category_value_t tExceptionCategory> \
        struct ExceptionCategoryClassProxy \
        { \
            using Type = typename ::Ic3::ExceptionBaseTypeClassProxy<Ic3::CxDef::getExceptionCategoryBaseType( tExceptionCategory )>::Type; \
        }; \
        template <exception_code_value_t tExceptionCode> \
        struct ExceptionCodeClassProxy \
        { \
            using Type = typename ExceptionCategoryClassProxy<Ic3::CxDef::getExceptionCodeCategory( tExceptionCode )>::Type; \
        }

    #define ic3SetExceptionCategoryType( pExceptionCategory, pType ) \
        template <> \
        struct ExceptionCategoryClassProxy<pExceptionCategory> \
        { \
            using Type = pType; \
        }

    #define ic3SetExceptionCodeType( pExceptionCode, pType ) \
        template <> \
        struct ExceptionCodeClassProxy<pExceptionCode> \
        { \
            using Type = pType; \
        }

	template <typename TException, typename... TArgs>
	IC3_PCL_ATTR_NO_RETURN inline void throwException( ExceptionInfo pExceptionInfo, TArgs &&... pArgs )
	{
		// TException is a class derived from ExceptionClass<ExceptionBaseType>. It contains 'baseType'
		// member with type tag. It should match the type embedded within the code. In case of mismatch, there is
		// either a typo (in case of manual call) or a problem with the throwException() function defined below.
		ic3DebugAssert( TException::mBaseType == CxDef::getExceptionCodeBaseType( pExceptionInfo.code ) );

		throw TException( std::move( pExceptionInfo ), std::forward<TArgs>( pArgs )... );
	}

	template <typename TException, typename... TArgs>
	IC3_PCL_ATTR_NO_RETURN inline void throwException( exception_code_value_t pExceptionCode,
                                                       std::string pDescription,
                                                       const FileLocationInfo & pFileLocationInfo,
                                                       TArgs &&... pArgs )
	{
	    ExceptionInfo exceptionInfo;
	    exceptionInfo.code = pExceptionCode;
	    exceptionInfo.description = std::move( pDescription );
	    exceptionInfo.fileLocationInfo = pFileLocationInfo;

	    throwException<TException>( std::move( exceptionInfo ), std::forward<TArgs>( pArgs )... );
    }

} // namespace Ic3


#define ic3Throw( pExceptionCode ) \
    ::Ic3::throwException<typename ExceptionCodeClassProxy<pExceptionCode>::Type>( pExceptionCode, #pExceptionCode, ic3CurrentFileLocationInfo() )

#define ic3ThrowDesc( pExceptionCode, pDescription ) \
    ::Ic3::throwException<typename ExceptionCodeClassProxy<pExceptionCode>::Type>( pExceptionCode, pDescription, ic3CurrentFileLocationInfo() )

#define ic3ThrowEx( pExceptionCode, ... ) \
    ::Ic3::throwException<typename ExceptionCodeClassProxy<pExceptionCode>::Type>( pExceptionCode, #pExceptionCode, ic3CurrentFileLocationInfo(), __VA_ARGS__ )

#define ic3ThrowExDesc( pExceptionCode, pDescription, ... ) \
    ::Ic3::throwException<typename ExceptionCodeClassProxy<pExceptionCode>::Type>( pExceptionCode, pDescription, ic3CurrentFileLocationInfo(), __VA_ARGS__ )

#define ic3CatchIntoWrapper( pResultWrapper ) \
	catch( const ::Ic3::Result & eResult ) \
	{ \
        pResultWrapper.setResult( eResult ); \
    } \
    catch( const ::Ic3::Result & eResult ) \
    { \
        pResultWrapper.setResult( eResult ); \
    } \
	catch( ... ) \
	{ \
		pResultWrapper.setException( std::current_exception() ); \
	}

#endif // __IC3_CORELIB_EXCEPTION_H__
