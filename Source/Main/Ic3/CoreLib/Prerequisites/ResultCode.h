
#pragma once

#define IC3_CORELIB_ENABLE_EXTENDED_RESULT_CODE_INFO 1

namespace Ic3
{

	/// @brief
	using result_code_value_t = uint32;

	/// @brief
	/// @tparam TPValue
	template <typename TPValue, typename TPErrorPredicate>
	struct ResultInfo
	{
	public:
		using ValueType = TPValue;
		using ErrorPredicateType = TPErrorPredicate;

	public:
		// Value of the result code.
		TPValue code = 0u;

		// A string identifying the result code. Usually - name of the code used across sources.
		const char * codeStr = kStrCharEmpty;

		// Additional info string, optional.
		const char * infoStr = kStrCharEmpty;

	public:
		ResultInfo( const ResultInfo & ) = default;
		ResultInfo & operator=( const ResultInfo & ) = default;

		/// @brief Trivial default constructor.
		ResultInfo() = default;

		/// @brief Explicit constructor which initialises result object with a specified result code.
		/// @param code Result code.
		template <typename TOtherValue>
		explicit ResultInfo( TOtherValue pCode )
		: code( static_cast<TPValue>( pCode ) )
		, codeStr( kStrCharEmpty )
		, infoStr( kStrCharEmpty )
		{}

		/// @brief Initializes result object with specified content.
		/// @param code Result code.
		/// @param codeStr Code string (text representation of the code, for example).
		/// @param infoStr Additional info string.
		template <typename TOtherValue>
		ResultInfo( TOtherValue pCode, const char * pCodeStr, const char * pInfoStr = nullptr )
		: code( static_cast<TPValue>( pCode ) )
		, codeStr( pCodeStr ? pCodeStr : kStrCharEmpty )
		, infoStr( pInfoStr ? pCodeStr : kStrCharEmpty )
		{}

		constexpr explicit operator bool() const
		{
			return !TPErrorPredicate()( code );
		}

		/// @brief Swaps two results.
		/// @param other Result object to swap with.
		void Swap( ResultInfo & pOther )
		{
			std::swap( code, pOther.code );
			std::swap( codeStr, pOther.codeStr );
			std::swap( infoStr, pOther.infoStr );
		}

		/// @brief Returns whether the result object is empty (i.e. has no explicit code set or code is zero).
		/// @return True if result object is empty or false otherwise.
		CPPX_ATTR_NO_DISCARD bool IsEmpty() const
		{
			return code == 0;
		}
	};

	/// @brief
	enum class EResultCodeType : uint8
	{
		Success,
		Warning,
		Error
	};

	namespace CXU
	{

		// Value BitMasks for result codes.

		/// @brief Control Key required for a valid result code value.
		constexpr auto kVbmResultCodeControlKey   = static_cast<result_code_value_t>( 0x18000000 );

		/// @brief Bitmask for the 'Type' component of a result code value.
		constexpr auto kVbmResultCodeTypeMask     = static_cast<result_code_value_t>( 0x00FF0000 );

		/// @brief Bitmask for the 'Category' component of a result code value.
		constexpr auto kVbmResultCodeCategoryMask = static_cast<result_code_value_t>( 0x0000FF00 );

		/// @brief Bitmask for the 'IID' (Internal ID) component of a result code value.
		constexpr auto kVbmResultCodeIIDMask      = static_cast<result_code_value_t>( 0x000000FF );

		///
		inline constexpr result_code_value_t DeclareResultCode( EResultCodeType pType, uint8 pCategory, uint8 pIID )
		{
			return ( kVbmResultCodeControlKey | ( ( result_code_value_t )( pType ) << 16 ) | ( ( result_code_value_t )( pCategory ) << 8 ) | pIID );
		}

		///
		inline constexpr EResultCodeType GetResultCodeType( result_code_value_t pResultCode )
		{
			return ( EResultCodeType )( ( pResultCode & kVbmResultCodeTypeMask ) >> 16 );
		}

		///
		inline constexpr uint8 GetResultCodeCategory( result_code_value_t pResultCode )
		{
			return ( uint8 )( ( pResultCode & kVbmResultCodeCategoryMask ) >> 8 );
		}

		///
		inline constexpr bool ValidateResultCode( result_code_value_t pResultCode )
		{
			return ( pResultCode & kVbmResultCodeControlKey ) == kVbmResultCodeControlKey;
		}

	}

	enum : uint8
	{
		eResultCategoryGeneric = 0,
	};
	
	enum : result_code_value_t
	{
		eResultCodeGenericSuccess = CXU::DeclareResultCode( EResultCodeType::Success, eResultCategoryGeneric, 0 )
	};

	struct ResultCodeErrorPredicate
	{
		constexpr bool operator()( result_code_value_t pResultCode ) const
		{
			return CXU::GetResultCodeType( pResultCode ) == EResultCodeType::Error;
		}
	};

#if( IC3_CORELIB_ENABLE_EXTENDED_RESULT_CODE_INFO )

	using ResultCode = ResultInfo<result_code_value_t, ResultCodeErrorPredicate>;

	/// @brief Creates result from code.
	#define Ic3MakeResult( pCode ) ::Ic3::ResultCode( pCode, #pCode )

	/// @brief Creates result from code and additional info string.
	#define Ic3MakeResultEx( pCode, pInfo ) ::Ic3::ResultCode( pCode, #pCode, pInfo )

	/// @brief
	#define Ic3ResultGetCode( pResult ) ( pResult.code )

	/// @brief
	#define Ic3ResultGetCodeStr( pResult ) ( pResult.codeStr )

	/// @brief
	#define Ic3ResultGetInfoStr( pResult ) ( pResult.infoStr )

#else

	/// @brief Result code.
	using ResultCode = result_code_value_t;

	/// @brief Creates result from code.
	#define Ic3MakeResult( pCode ) ( pCode )

	/// @brief Creates result from code and additional info string.
	#define Ic3MakeResultEx( pCode, pInfo ) ( pCode )

	/// @brief
	#define Ic3ResultGetCode( pResult ) ( pResult )

	/// @brief
	#define Ic3ResultGetCodeStr( pResult ) CxDef::STR_CHAR_EMPTY

	/// @brief
	#define Ic3ResultGetInfoStr( pResult ) CxDef::STR_CHAR_EMPTY

#endif

	template <typename TPValue, typename TPErrorPredicate>
	inline constexpr bool operator==(
			const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
			const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs.code == pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator==( const ResultInfo<TPValue, TPErrorPredicate> & pLhs, TIntegral pRhs )
	{
		return pLhs.code == pRhs;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator==( TIntegral pLhs, const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs == pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate>
	inline constexpr bool operator!=(
			const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
			const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs.code != pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator!=( const ResultInfo<TPValue, TPErrorPredicate> & pLhs, TIntegral pRhs )
	{
		return pLhs.code != pRhs;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator!=( TIntegral pLhs, const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs != pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate>
	inline constexpr bool operator<(
			const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
			const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs.code < pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator<( const ResultInfo<TPValue, TPErrorPredicate> & pLhs, TIntegral pRhs )
	{
		return pLhs.code < pRhs;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator<( TIntegral pLhs, const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs < pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate>
	inline constexpr bool operator<=(
			const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
			const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs.code <= pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator<=( const ResultInfo<TPValue, TPErrorPredicate> & pLhs, TIntegral pRhs )
	{
		return pLhs.code <= pRhs;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator<=( TIntegral pLhs, const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs <= pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate>
	inline constexpr bool operator>(
			const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
			const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs.code > pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator>( const ResultInfo<TPValue, TPErrorPredicate> & pLhs, TIntegral pRhs )
	{
		return pLhs.code > pRhs;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator>( TIntegral pLhs, const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs > pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate>
	inline constexpr bool operator>=(
			const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
			const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs.code >= pRhs.code;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator>=( const ResultInfo<TPValue, TPErrorPredicate> & pLhs, TIntegral pRhs )
	{
		return pLhs.code >= pRhs;
	}

	template <typename TPValue, typename TPErrorPredicate, typename TIntegral>
	inline constexpr bool operator>=( TIntegral pLhs, const ResultInfo<TPValue, TPErrorPredicate> & pRhs )
	{
		return pLhs >= pRhs.code;
	}

}
