
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
		const char * codeStr = CxDef::STR_CHAR_EMPTY;

		// Additional info string, optional.
		const char * infoStr = CxDef::STR_CHAR_EMPTY;

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
		, codeStr( CxDef::STR_CHAR_EMPTY )
		, infoStr( CxDef::STR_CHAR_EMPTY )
		{}

		/// @brief Initializes result object with specified content.
		/// @param code Result code.
		/// @param codeStr Code string (text representation of the code, for example).
		/// @param infoStr Additional info string.
		template <typename TOtherValue>
		ResultInfo( TOtherValue pCode, const char * pCodeStr, const char * pInfoStr = nullptr )
		: code( static_cast<TPValue>( pCode ) )
		, codeStr( pCodeStr ? pCodeStr : CxDef::STR_CHAR_EMPTY )
		, infoStr( pInfoStr ? pCodeStr : CxDef::STR_CHAR_EMPTY )
		{}

		constexpr explicit operator bool() const
		{
			return !TPErrorPredicate()( code );
		}

		/// @brief Swaps two results.
		/// @param other Result object to swap with.
		void swap( ResultInfo & pOther )
		{
			std::swap( code, pOther.code );
			std::swap( codeStr, pOther.codeStr );
			std::swap( infoStr, pOther.infoStr );
		}

		/// @brief Returns whether the result object is empty (i.e. has no explicit code set or code is zero).
		/// @return True if result object is empty or false otherwise.
		IC3_ATTR_NO_DISCARD bool empty() const
		{
			return code == 0;
		}
	};

	/// @brief
	enum class EResultCodeType : uint8
	{
		SUCCESS,
		WARNING,
		ERROR
	};

	namespace CxDef
	{

		// Value BitMasks for result codes.

		/// @brief Control Key required for a valid result code value.
		constexpr auto VBM_RESULT_CODE_CONTROL_KEY   = static_cast<result_code_value_t>( 0x18000000 );

		/// @brief Bitmask for the 'Type' component of a result code value.
		constexpr auto VBM_RESULT_CODE_TYPE_MASK     = static_cast<result_code_value_t>( 0x00FF0000 );

		/// @brief Bitmask for the 'Category' component of a result code value.
		constexpr auto VBM_RESULT_CODE_CATEGORY_MASK = static_cast<result_code_value_t>( 0x0000FF00 );

		/// @brief Bitmask for the 'IID' (Internal ID) component of a result code value.
		constexpr auto VBM_RESULT_CODE_IID_MASK      = static_cast<result_code_value_t>( 0x000000FF );

		///
		inline constexpr result_code_value_t declareResultCode( EResultCodeType pType, uint8 pCategory, uint8 pIID )
		{
			return ( VBM_RESULT_CODE_CONTROL_KEY | ( ( result_code_value_t )( pType ) << 16 ) | ( ( result_code_value_t )( pCategory ) << 8 ) | pIID );
		}

		///
		inline constexpr EResultCodeType getResultCodeType( result_code_value_t pResultCode )
		{
			return ( EResultCodeType )( ( pResultCode & VBM_RESULT_CODE_TYPE_MASK ) >> 16 );
		}

		///
		inline constexpr uint8 getResultCodeCategory( result_code_value_t pResultCode )
		{
			return ( uint8 )( ( pResultCode & VBM_RESULT_CODE_CATEGORY_MASK ) >> 8 );
		}

		///
		inline constexpr bool validateResultCode( result_code_value_t pResultCode )
		{
			return ( pResultCode & VBM_RESULT_CODE_CONTROL_KEY ) == VBM_RESULT_CODE_CONTROL_KEY;
		}

	}

	enum : uint8
	{
		E_RESULT_CATEGORY_GENERIC = 0,
	};
	
	enum : result_code_value_t
	{
		E_RESULT_CODE_GENERIC_SUCCESS = CxDef::declareResultCode( EResultCodeType::SUCCESS, E_RESULT_CATEGORY_GENERIC, 0 )
	};

	struct ResultCodeErrorPredicate
	{
		constexpr bool operator()( result_code_value_t pResultCode ) const
		{
			return CxDef::getResultCodeType( pResultCode ) == EResultCodeType::ERROR;
		}
	};

#if( IC3_CORELIB_ENABLE_EXTENDED_RESULT_CODE_INFO )

	using ResultCode = ResultInfo<result_code_value_t, ResultCodeErrorPredicate>;

	/// @brief Creates result from code.
	#define ic3MakeResult( pCode ) ::Ic3::ResultCode( pCode, #pCode )

	/// @brief Creates result from code and additional info string.
	#define ic3MakeResultEx( pCode, pInfo ) ::Ic3::ResultCode( pCode, #pCode, pInfo )

	/// @brief
	#define ic3ResultGetCode( pResult ) ( pResult.code )

	/// @brief
	#define ic3ResultGetCodeStr( pResult ) ( pResult.codeStr )

	/// @brief
	#define ic3ResultGetInfoStr( pResult ) ( pResult.infoStr )

#else

	/// @brief Result code.
	using ResultCode = result_code_value_t;

	/// @brief Creates result from code.
	#define ic3MakeResult( pCode ) ( pCode )

	/// @brief Creates result from code and additional info string.
	#define ic3MakeResultEx( pCode, pInfo ) ( pCode )

	/// @brief
	#define ic3ResultGetCode( pResult ) ( pResult )

	/// @brief
	#define ic3ResultGetCodeStr( pResult ) CxDef::STR_CHAR_EMPTY

	/// @brief
	#define ic3ResultGetInfoStr( pResult ) CxDef::STR_CHAR_EMPTY

#endif

	template <typename TPValue, typename TPErrorPredicate>
	inline constexpr bool operator==( const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
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
	inline constexpr bool operator!=( const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
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
	inline constexpr bool operator<( const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
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
	inline constexpr bool operator<=( const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
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
	inline constexpr bool operator>( const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
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
	inline constexpr bool operator>=( const ResultInfo<TPValue, TPErrorPredicate> & pLhs,
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
