
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
        E_EXCEPTION_CATEGORY_DEBUG =
            CxDef::declareExceptionCategory( EExceptionBaseType::DEBUG, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_ENGINE_SUBMODULE =
            CxDef::declareExceptionCategory( EExceptionBaseType::ENGINE_SUBMODULE, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_FRAMEWORK_CORE =
            CxDef::declareExceptionCategory( EExceptionBaseType::FRAMEWORK_CORE, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_INTERNAL =
            CxDef::declareExceptionCategory( EExceptionBaseType::INTERNAL, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_INTERRUPT =
            CxDef::declareExceptionCategory( EExceptionBaseType::INTERRUPT, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_MATH =
            CxDef::declareExceptionCategory( EExceptionBaseType::MATH, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_RESULT_PROXY =
            CxDef::declareExceptionCategory( EExceptionBaseType::RESULT, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_SYSTEM =
            CxDef::declareExceptionCategory( EExceptionBaseType::SYSTEM, ic3ExcCategoryIID( 0 ) ),

        E_EXCEPTION_CATEGORY_USER_EXTERNAL =
            CxDef::declareExceptionCategory( EExceptionBaseType::USER_EXTERNAL, ic3ExcCategoryIID( 0 ) ),
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
		exception_code_value_t mCode;

		//
		std::string mDescription;

		//
		FileLocationInfo mFileLocationInfo;

		IC3_ATTR_NO_DISCARD std::string toString() const;
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
		, mString( mExceptionInfo.toString() )
		{}

		virtual ~Exception() = default;

		/// @brief Returns the name of the exception.
		/// @return Name of the exception.
		virtual EExceptionBaseType getBaseType() const = 0;

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

		/// @refitem
		/// @return
		virtual EExceptionBaseType getBaseType() const override final
		{
			return tpExceptionBaseType;
		}

		/// @refitem
		/// @return
		virtual const std::string & getBaseTypeName() const override final
		{
			static std::once_flag initFlag;
			static std::string baseTypeName;

			 std::call_once( initFlag, []() {
			 	const auto & enumInfo = queryEnumTypeInfo<EExceptionBaseType>();
			 	const auto & baseTypeConstantInfo = enumInfo.getConstantMap().getByValue( tpExceptionBaseType );
			 	baseTypeName = baseTypeConstantInfo.name;
			 } );

			return baseTypeName;
		}
	};

	/// @brief Default base class for EExceptionBaseType::Debug.
	using UnknownException = TExceptionClass<EExceptionBaseType::UNKNOWN>;

	/// @brief Default base class for EExceptionBaseType::Debug.
	using DebugException = TExceptionClass<EExceptionBaseType::DEBUG>;

	/// @brief Default base class for EExceptionBaseType::EngineSubModule.
	using EngineSubModuleException = TExceptionClass<EExceptionBaseType::ENGINE_SUBMODULE>;

	/// @brief Default base class for EExceptionBaseType::FrameworkCore.
	using FrameworkCoreException = TExceptionClass<EExceptionBaseType::FRAMEWORK_CORE>;

	/// @brief Default base class for EExceptionBaseType::Internal.
	using InternalException = TExceptionClass<EExceptionBaseType::INTERNAL>;

	/// @brief Default base class for EExceptionBaseType::Interrupt.
	using InterruptException = TExceptionClass<EExceptionBaseType::INTERRUPT>;

	/// @brief Default base class for EExceptionBaseType::Math.
	using MathException = TExceptionClass<EExceptionBaseType::MATH>;

	/// @brief Default base class for EExceptionBaseType::System.
	using SystemException = TExceptionClass<EExceptionBaseType::SYSTEM>;

	/// @brief Default base class for EExceptionBaseType::UserExternal.
	using UserExternalException = TExceptionClass<EExceptionBaseType::USER_EXTERNAL>;

	/// @brief Specialized class for SResultWrapper exceptions. Adds ResultInfo object.
	template <typename TPResultValueType, typename TPErrorPredicate>
	class TPResultProxyException : public TExceptionClass<EExceptionBaseType::RESULT>
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
	struct ExceptionClassResolver<EExceptionBaseType::RESULT, true>
    {
		template <typename TPResultValueType, typename TPErrorPredicate>
	    using Type = TPResultProxyException<TPResultValueType, TPErrorPredicate>;
    };

	template <EExceptionBaseType tpExceptionBaseType>
	struct ExceptionBaseTypeClassProxy
    {
	    using Type = typename ExceptionClassResolver<tpExceptionBaseType, CxDef::isExceptionBaseTypeValid( tpExceptionBaseType )>::Type;
    };

	template <exception_category_value_t tpExceptionCategory>
	struct ExceptionCategoryClassProxy
    {
	    using Type = typename ExceptionBaseTypeClassProxy<CxDef::getExceptionCategoryBaseType( tpExceptionCategory )>::Type;
    };

	template <exception_code_value_t tpExceptionCode>
	struct ExceptionCodeClassProxy
    {
	    using Type = typename ExceptionCategoryClassProxy<CxDef::getExceptionCodeCategory( tpExceptionCode )>::Type;
    };

	template <>
	struct ExceptionCodeClassProxy<E_EXC_RESULT_CODE_ERROR>
	{
		using Type = ResultCodeException;
	};

    #define ic3EnableCustomExceptionSupport() \
        template <exception_category_value_t tpExceptionCategory> \
        struct ExceptionCategoryClassProxy \
        { \
            using Type = typename ::Ic3::ExceptionBaseTypeClassProxy<Ic3::CxDef::getExceptionCategoryBaseType( tpExceptionCategory )>::Type; \
        }; \
        template <exception_code_value_t tpExceptionCode> \
        struct ExceptionCodeClassProxy \
        { \
            using Type = typename ExceptionCategoryClassProxy<Ic3::CxDef::getExceptionCodeCategory( tpExceptionCode )>::Type; \
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
		// TException is a class derived from TExceptionClass<EExceptionBaseType>. It contains 'baseType'
		// member with type tag. It should match the type embedded within the code. In case of mismatch, there is
		// either a typo (in case of manual call) or a problem with the throwException() function defined below.
		ic3DebugAssert( TException::mBaseType == CxDef::getExceptionCodeBaseType( pExceptionInfo.mCode ) );

		throw TException( std::move( pExceptionInfo ), std::forward<TArgs>( pArgs )... );
	}


	template <typename TException, typename... TArgs>
	IC3_PCL_ATTR_NO_RETURN inline void throwException( exception_code_value_t pExceptionCode,
                                                       std::string pDescription,
                                                       const FileLocationInfo & pFileLocationInfo,
                                                       TArgs &&... pArgs )
	{
	    ExceptionInfo exceptionInfo;
	    exceptionInfo.mCode = pExceptionCode;
	    exceptionInfo.mDescription = std::move( pDescription );
	    exceptionInfo.mFileLocationInfo = pFileLocationInfo;

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
