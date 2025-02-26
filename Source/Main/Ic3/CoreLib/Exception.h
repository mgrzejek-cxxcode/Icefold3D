
#pragma once

#ifndef __IC3_CORELIB_EXCEPTION_H__
#define __IC3_CORELIB_EXCEPTION_H__

#include "ExceptionCommon.h"
#include "Reflection/EnumTypeInfo.h"
#include <exception>
#include <mutex>

namespace Ic3
{

    enum : exception_category_value_t
    {
        eExceptionCategoryDebug =
            CXU::declareExceptionCategory( EExceptionBaseType::Debug, Ic3ExcCategoryIID( 0 ) ),

        eExceptionCategoryEngineSubmodule =
            CXU::declareExceptionCategory( EExceptionBaseType::EngineSubmodule, Ic3ExcCategoryIID( 0 ) ),

        eExceptionCategoryFrameworkCore =
            CXU::declareExceptionCategory( EExceptionBaseType::FrameworkCore, Ic3ExcCategoryIID( 0 ) ),

        eExceptionCategoryInternal =
            CXU::declareExceptionCategory( EExceptionBaseType::Internal, Ic3ExcCategoryIID( 0 ) ),

        eExceptionCategoryInterrupt =
            CXU::declareExceptionCategory( EExceptionBaseType::Interrupt, Ic3ExcCategoryIID( 0 ) ),

        eExceptionCategoryMath =
            CXU::declareExceptionCategory( EExceptionBaseType::Math, Ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_RESULT_PROXY =
            CXU::declareExceptionCategory( EExceptionBaseType::Result, Ic3ExcCategoryIID( 0 ) ),

        eExceptionCategorySystem =
            CXU::declareExceptionCategory( EExceptionBaseType::System, Ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_USER_EXTERNAL =
            CXU::declareExceptionCategory( EExceptionBaseType::UserExternal, Ic3ExcCategoryIID( 0 ) ),
    };

    enum : exception_code_value_t
    {
        eExcCodeDebugPlaceholder =
            CXU::declareExceptionCode( eExceptionCategoryDebug, Ic3ExcCodeIID( 0x01 ) ),

        eExcCodeResultCodeError =
            CXU::declareExceptionCode( E_EXCEPTION_CATEGORY_RESULT_PROXY, Ic3ExcCodeIID( 0x01 ) ),
    };

	/// @brief
	struct ExceptionInfo
	{
		// Exception code
		exception_code_value_t code;

		//
		std::string description;

		//
		cppx::file_location_info fileLocationInfo;

		CPPX_ATTR_NO_DISCARD std::string ToString() const;
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
		/// @param pType Type of the exception, represented as EExceptionBaseType enum.
		/// @param pInfo Exception description, containing basic info about it.
		Exception( ExceptionInfo pExceptionInfo )
		: mExceptionInfo( std::move( pExceptionInfo ) )
		, mString( mExceptionInfo.ToString() )
		{}

		virtual ~Exception() = default;

		/// @brief Returns the name of the exception.
		/// @return Name of the exception.
		virtual EExceptionBaseType GetBaseType() const = 0;

		/// @brief Returns the name of the exception.
		/// @return Name of the exception.
		virtual const std::string & GetBaseTypeName() const = 0;

		/// @brief
		/// @return
		virtual const char * what() const noexcept override
		{
		    return mString.c_str();
		}
	};

	/// @brief
	/// @tparam tpExceptionBaseType
	template <EExceptionBaseType tpExceptionBaseType>
	class TExceptionClass : public Exception
	{
	public:
		/// @brief
		static constexpr auto mBaseType = tpExceptionBaseType;

	public:
		explicit TExceptionClass( ExceptionInfo pExceptionInfo )
		: Exception( std::move( pExceptionInfo ) )
		{}

		/// @return
		virtual EExceptionBaseType GetBaseType() const override final
		{
			return tpExceptionBaseType;
		}

		/// @return
		virtual const std::string & GetBaseTypeName() const override final
		{
			static std::once_flag initFlag{};
			static std::string baseTypeName{};

			 std::call_once( initFlag, []() {
			 	const auto & enumInfo = QueryEnumTypeInfo<EExceptionBaseType>();
			 	const auto & baseTypeConstantInfo = enumInfo.GetConstantMap().GetByValue( tpExceptionBaseType );
			 	baseTypeName = baseTypeConstantInfo.name;
			 } );

			return baseTypeName;
		}
	};

	/// @brief Default base class for EExceptionBaseType::Debug.
	using UnknownException = TExceptionClass<EExceptionBaseType::Unknown>;

	/// @brief Default base class for EExceptionBaseType::Debug.
	using DebugException = TExceptionClass<EExceptionBaseType::Debug>;

	/// @brief Default base class for EExceptionBaseType::EngineSubModule.
	using EngineSubModuleException = TExceptionClass<EExceptionBaseType::EngineSubmodule>;

	/// @brief Default base class for EExceptionBaseType::FrameworkCore.
	using FrameworkCoreException = TExceptionClass<EExceptionBaseType::FrameworkCore>;

	/// @brief Default base class for EExceptionBaseType::Internal.
	using InternalException = TExceptionClass<EExceptionBaseType::Internal>;

	/// @brief Default base class for EExceptionBaseType::Interrupt.
	using InterruptException = TExceptionClass<EExceptionBaseType::Interrupt>;

	/// @brief Default base class for EExceptionBaseType::Math.
	using MathException = TExceptionClass<EExceptionBaseType::Math>;

	/// @brief Default base class for EExceptionBaseType::System.
	using SystemException = TExceptionClass<EExceptionBaseType::System>;

	/// @brief Default base class for EExceptionBaseType::UserExternal.
	using UserExternalException = TExceptionClass<EExceptionBaseType::UserExternal>;

	/// @brief Specialized class for SResultWrapper exceptions. Adds ResultInfo object.
	template <typename TPResultValueType, typename TPErrorPredicate>
	class TPResultProxyException : public TExceptionClass<EExceptionBaseType::Result>
    {
    public:
        ResultInfo<TPResultValueType, TPErrorPredicate> mResult;

    public:
		TPResultProxyException( ExceptionInfo pExceptionInfo, ResultInfo<TPResultValueType, TPErrorPredicate> pResult )
        : TExceptionClass( std::move( pExceptionInfo ) )
        , mResult( pResult )
        {}
    };

	class ResultCodeException : public TPResultProxyException<ResultCode::ValueType, ResultCode::ErrorPredicateType>
	{
	public:
		ResultCodeException( ExceptionInfo pExceptionInfo, ResultCode pResultCode )
		: TPResultProxyException( std::move( pExceptionInfo ), pResultCode )
		{}
	};


	template <EExceptionBaseType tpExceptionBaseType, bool tpIsValidType>
	struct ExceptionClassResolver
    {
	    using Type = UnknownException;
    };

	template <EExceptionBaseType tpExceptionBaseType>
	struct ExceptionClassResolver<tpExceptionBaseType, true>
    {
	    using Type = TExceptionClass<tpExceptionBaseType>;
    };

	template <>
	struct ExceptionClassResolver<EExceptionBaseType::Result, true>
    {
		template <typename TPResultValueType, typename TPErrorPredicate>
	    using Type = TPResultProxyException<TPResultValueType, TPErrorPredicate>;
    };

	template <EExceptionBaseType tpExceptionBaseType>
	struct ExceptionBaseTypeClassProxy
    {
	    using Type = typename ExceptionClassResolver<tpExceptionBaseType, CXU::isExceptionBaseTypeValid( tpExceptionBaseType )>::Type;
    };

	template <exception_category_value_t tpExceptionCategory>
	struct ExceptionCategoryClassProxy
    {
	    using Type = typename ExceptionBaseTypeClassProxy<CXU::getExceptionCategoryBaseType( tpExceptionCategory )>::Type;
    };

	template <exception_code_value_t tpExceptionCode>
	struct ExceptionCodeClassProxy
    {
	    using Type = typename ExceptionCategoryClassProxy<CXU::getExceptionCodeCategory( tpExceptionCode )>::Type;
    };

	template <>
	struct ExceptionCodeClassProxy<eExcCodeResultCodeError>
	{
		using Type = ResultCodeException;
	};

    #define Ic3EnableCustomExceptionSupport() \
        template <exception_category_value_t tpExceptionCategory> \
        struct ExceptionCategoryClassProxy \
        { \
            using Type = typename ::Ic3::ExceptionBaseTypeClassProxy<Ic3::CXU::getExceptionCategoryBaseType( tpExceptionCategory )>::Type; \
        }; \
        template <exception_code_value_t tpExceptionCode> \
        struct ExceptionCodeClassProxy \
        { \
            using Type = typename ExceptionCategoryClassProxy<Ic3::CXU::getExceptionCodeCategory( tpExceptionCode )>::Type; \
        }

    #define Ic3SetExceptionCategoryType( pExceptionCategory, pType ) \
        template <> \
        struct ExceptionCategoryClassProxy<pExceptionCategory> \
        { \
            using Type = pType; \
        }

    #define Ic3SetExceptionCodeType( pExceptionCode, pType ) \
        template <> \
        struct ExceptionCodeClassProxy<pExceptionCode> \
        { \
            using Type = pType; \
        }

	template <typename TException, typename... TArgs>
	PCL_ATTR_NO_RETURN inline void ThrowException( ExceptionInfo pExceptionInfo, TArgs &&... pArgs )
	{
		// TException is a class derived from TExceptionClass<EExceptionBaseType>. It contains 'baseType'
		// member with type tag. It should match the type embedded within the code. In case of mismatch, there is
		// either a typo (in case of manual call) or a problem with the ThrowException() function defined below.
		Ic3DebugAssert( TException::mBaseType == CXU::getExceptionCodeBaseType( pExceptionInfo.code ) );

		Ic3DebugInterrupt();

		throw TException( std::move( pExceptionInfo ), std::forward<TArgs>( pArgs )... );
	}


	template <typename TException, typename... TArgs>
	PCL_ATTR_NO_RETURN inline void ThrowException( exception_code_value_t pExceptionCode,
                                                       std::string pDescription,
                                                       const cppx::file_location_info & pFileLocationInfo,
                                                       TArgs &&... pArgs )
	{
	    ExceptionInfo exceptionInfo;
	    exceptionInfo.code = pExceptionCode;
	    exceptionInfo.description = std::move( pDescription );
	    exceptionInfo.fileLocationInfo = pFileLocationInfo;

	    ThrowException<TException>( std::move( exceptionInfo ), std::forward<TArgs>( pArgs )... );
    }

} // namespace Ic3


#define Ic3Throw( pExceptionCode ) \
    ::Ic3::ThrowException<typename ExceptionCodeClassProxy<pExceptionCode>::Type>( pExceptionCode, #pExceptionCode, cppx_current_file_location_info() )

#define Ic3ThrowDesc( pExceptionCode, pDescription ) \
    ::Ic3::ThrowException<typename ExceptionCodeClassProxy<pExceptionCode>::Type>( pExceptionCode, pDescription, cppx_current_file_location_info() )

#define Ic3ThrowEx( pExceptionCode, ... ) \
    ::Ic3::ThrowException<typename ExceptionCodeClassProxy<pExceptionCode>::Type>( pExceptionCode, #pExceptionCode, cppx_current_file_location_info(), __VA_ARGS__ )

#define Ic3ThrowExDesc( pExceptionCode, pDescription, ... ) \
    ::Ic3::ThrowException<typename ExceptionCodeClassProxy<pExceptionCode>::Type>( pExceptionCode, pDescription, cppx_current_file_location_info(), __VA_ARGS__ )

#define Ic3CatchIntoWrapper( pResultWrapper ) \
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
