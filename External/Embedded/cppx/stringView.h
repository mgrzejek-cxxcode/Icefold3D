
#ifndef __CPPX_STRING_VIEW_H__
#define __CPPX_STRING_VIEW_H__

#include "stringExt.h"
#include "staticAlgo.h"
#include "typeTraits.h"
#include <string_view>

namespace cppx
{

	template <typename TPChar>
	class string_base_view
	{
		static_assert( meta::is_type_on_type_list<TPChar, char, wchar_t, char16_t, char32_t>::value, "Invalid char type for StringView" );

	public:
		using char_type = typename std::remove_cv<TPChar>::type;

		constexpr string_base_view()
		: _basePtr( cve::str_empty<TPChar> )
		, _length( 0 )
		{}

		constexpr string_base_view( const char_type * pStr )
		: _basePtr( pStr ? pStr : cve::str_empty<TPChar> )
		, _length( pStr ? std::char_traits<char_type>::length( pStr ) : 0u )
		{}

		constexpr string_base_view( const char_type * pBegin, const char_type * pEnd )
		: _basePtr( pBegin ? pBegin : cve::str_empty<TPChar> )
		, _length( pBegin ? pEnd - pBegin : 0u )
		{}

		constexpr string_base_view( const char_type * pStr, size_t pLength )
		: _basePtr( pStr ? pStr : cve::str_empty<TPChar> )
		, _length( pStr ? pLength : 0u )
		{}

		string_base_view( const std::basic_string<TPChar> & pString )
		: _basePtr( pString.c_str() )
		, _length( pString.length() )
		{}

		string_base_view( const std::basic_string_view<TPChar> & pString )
		: _basePtr( pString.data() )
		, _length( pString.length() )
		{}

		template <typename TPOther>
		string_base_view( const string_base_view<TPOther> & pOther )
		: _basePtr( pOther.data() )
		, _length( pOther.length() )
		{}

		template <size_t tpSize>
		constexpr explicit string_base_view( char_type( &pArray )[tpSize] )
		: string_base_view( &( pArray[0] ), tpSize )
		{}

		template <size_t tpSize>
		constexpr explicit string_base_view( const char_type( &pArray )[tpSize] )
		: string_base_view( &( pArray[0] ), tpSize )
		{}

		explicit operator bool() const
		{
			return !empty();
		}

		operator std::basic_string_view<TPChar>() const noexcept
		{
			return { _basePtr, _length };
		}

		CPPX_ATTR_NO_DISCARD const char_type * data() const
		{
			return _basePtr;
		}

		CPPX_ATTR_NO_DISCARD const char_type * str() const
		{
			return _basePtr;
		}

		CPPX_ATTR_NO_DISCARD size_t length() const
		{
			return _length;
		}

		CPPX_ATTR_NO_DISCARD size_t size() const
		{
			return _length;
		}

		CPPX_ATTR_NO_DISCARD size_t byte_size() const
		{
			return _length * sizeof( TPChar );
		}

		CPPX_ATTR_NO_DISCARD bool empty() const
		{
			return !_basePtr || ( _length == 0 );
		}

		void swap( string_base_view & pOther )
		{
			std::swap( _basePtr, pOther._basePtr );
			std::swap( _length, pOther._length );
		}

	private:
		const char_type * _basePtr;
		size_t _length;
	};

	template <typename TPChar>
	inline void swap( string_base_view<TPChar> & pFirst, string_base_view<TPChar> & pSecond )
	{
		pFirst.swap( pSecond );
	}

	template <typename TPChar>
	inline TPChar * begin( const string_base_view<TPChar> & pStrView )
	{
		return pStrView.str();
	}

	template <typename TPChar>
	inline TPChar * end( const string_base_view<TPChar> & pStrView )
	{
		return pStrView.str() + pStrView.length();
	}

	template <typename TPChar>
	inline bool operator==( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator==( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator==( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator==( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator==( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator==( const string_base_view<TPChar> & pLhs, const TPChar * pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs, get_min_of( pLhs.length(), std::char_traits<TPChar>::length( pRhs ) ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator==( const TPChar * pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs, pRhs.str(), get_min_of( std::char_traits<TPChar>::length( pLhs ), pRhs.length() ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator!=( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator!=( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator!=( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator!=( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator!=( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator!=( const string_base_view<TPChar> & pLhs, const TPChar * pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs, get_min_of( pLhs.length(), std::char_traits<TPChar>::length( pRhs ) ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator!=( const TPChar * pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs, pRhs.str(), get_min_of( std::char_traits<TPChar>::length( pLhs ), pRhs.length() ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator<( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<( const string_base_view<TPChar> & pLhs, const TPChar * pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs, get_min_of( pLhs.length(), std::char_traits<TPChar>::length( pRhs ) ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<( const TPChar * pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs, pRhs.str(), get_min_of( std::char_traits<TPChar>::length( pLhs ), pRhs.length() ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<=( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator<=( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator<=( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator<=( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator<=( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator<=( const string_base_view<TPChar> & pLhs, const TPChar * pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs, get_min_of( pLhs.length(), std::char_traits<TPChar>::length( pRhs ) ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator<=( const TPChar * pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs, pRhs.str(), get_min_of( std::char_traits<TPChar>::length( pLhs ), pRhs.length() ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator>( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>( const string_base_view<TPChar> & pLhs, const TPChar * pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs, get_min_of( pLhs.length(), std::char_traits<TPChar>::length( pRhs ) ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>( const TPChar * pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs, pRhs.str(), get_min_of( std::char_traits<TPChar>::length( pLhs ), pRhs.length() ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>=( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) >= 0;
	}

	template <typename TPChar>
	inline bool operator>=( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) >= 0;
	}

	template <typename TPChar>
	inline bool operator>=( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) >= 0;
	}

	template <typename TPChar>
	inline bool operator>=( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), get_min_of( pLhs.length(), pRhs.length() ) ) >= 0;
	}

	template <typename TPChar>
	inline bool operator>=( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), get_min_of( pLhs.length(), pRhs.length() ) ) >= 0;
	}

	template <typename TPChar>
	inline bool operator>=( const string_base_view<TPChar> & pLhs, const TPChar * pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs, get_min_of( pLhs.length(), std::char_traits<TPChar>::length( pRhs ) ) ) >= 0;
	}

	template <typename TPChar>
	inline bool operator>=( const TPChar * pLhs, const string_base_view<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs, pRhs.str(), get_min_of( std::char_traits<TPChar>::length( pLhs ), pRhs.length() ) ) >= 0;
	}


	template <typename TPChar>
	struct string_view_cmp_equal
	{
		bool operator()( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}

		bool operator()( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const TPChar * pRhs ) const
		{
			return pLhs == pRhs;
		}

		bool operator()( const TPChar * pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}
	};

	template <typename TPChar>
	struct string_view_cmp_not_equal
	{
		bool operator()( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}

		bool operator()( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const TPChar * pRhs ) const
		{
			return pLhs != pRhs;
		}

		bool operator()( const TPChar * pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}
	};

	template <typename TPChar>
	struct string_view_cmp_less
	{
		bool operator()( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}

		bool operator()( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const TPChar * pRhs ) const
		{
			return pLhs < pRhs;
		}

		bool operator()( const TPChar * pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}
	};

	template <typename TPChar>
	struct string_view_cmp_less_equal
	{
		bool operator()( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}

		bool operator()( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const TPChar * pRhs ) const
		{
			return pLhs <= pRhs;
		}

		bool operator()( const TPChar * pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}
	};

	template <typename TPChar>
	struct string_view_cmp_greater
	{
		bool operator()( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}

		bool operator()( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const TPChar * pRhs ) const
		{
			return pLhs > pRhs;
		}

		bool operator()( const TPChar * pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}
	};

	template <typename TPChar>
	struct string_view_cmp_greater_equal
	{
		bool operator()( const string_base_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const std::basic_string_view<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}

		bool operator()( const std::basic_string_view<TPChar> & pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}

		bool operator()( const string_base_view<TPChar> & pLhs, const TPChar * pRhs ) const
		{
			return pLhs >= pRhs;
		}

		bool operator()( const TPChar * pLhs, const string_base_view<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}
	};

	using string_view = string_base_view<char>;
	using wstring_view = string_base_view<wchar_t>;

}

#endif /* __CPPX_STRING_VIEW_H__ */
