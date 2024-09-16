
#ifndef __IC3_CPPX_STRING_VIEW_H__
#define __IC3_CPPX_STRING_VIEW_H__

#include "StringExt.h"
#include "StaticAlgo.h"
#include "TypeTraits.h"

namespace Ic3::Cppx
{

	template <typename TPChar = char>
	class TStringView
	{
		static_assert( QIsTypeOnTypeList<TPChar, char, wchar_t, char16_t, char32_t>::value, "Invalid char type for StringView" );

	public:
		using CharType = typename std::remove_cv<TPChar>::type;

		TStringView()
		: _basePtr( nullptr )
		, _length( 0 )
		{}

		TStringView( const CharType * pStr )
		: _basePtr( pStr )
		, _length( std::char_traits<CharType>::length( pStr ) )
		{}

		TStringView( const CharType * pBegin, const CharType * pEnd )
		: _basePtr( pBegin )
		, _length( pEnd - pBegin )
		{}

		TStringView( const CharType * pStr, size_t pLength )
		: _basePtr( pStr )
		, _length( pLength )
		{}

		TStringView( const std::basic_string<TPChar> & pString )
		: _basePtr( pString.c_str() )
		, _length( pString.length() )
		{}

		template <typename TPOther>
		TStringView( const TStringView<TPOther> & pOther )
		: _basePtr( pOther.data() )
		, _length( pOther.length() )
		{}

		template <size_t tSize>
		explicit TStringView( CharType( &pArray )[tSize] )
		: TStringView( &( pArray[0] ), tSize )
		{}

		template <size_t tSize>
		explicit TStringView( const CharType( &pArray )[tSize] )
		: TStringView( &( pArray[0] ), tSize )
		{}

		explicit operator bool() const
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD const CharType * str() const
		{
			return _basePtr;
		}

		IC3_ATTR_NO_DISCARD size_t length() const
		{
			return _length;
		}

		IC3_ATTR_NO_DISCARD size_t size() const
		{
			return _length;
		}

		IC3_ATTR_NO_DISCARD size_t byteSize() const
		{
			return _length * sizeof( TPChar );
		}

		IC3_ATTR_NO_DISCARD bool empty() const
		{
			return _basePtr && ( _length > 0 );
		}

		void swap( TStringView & pOther )
		{
			std::swap( _basePtr, pOther._basePtr );
			std::swap( _length, pOther._length );
		}

	private:
		const CharType * _basePtr;
		size_t _length;
	};

	template <typename TPChar>
	inline void swap( TStringView<TPChar> & pFirst, TStringView<TPChar> & pSecond )
	{
		pFirst.swap( pSecond );
	}

	template <typename TPChar>
	inline TPChar * begin( const TStringView<TPChar> & pStringView )
	{
		return pStringView.str();
	}

	template <typename TPChar>
	inline TPChar * end( const TStringView<TPChar> & pStringView )
	{
		return pStringView.str() + pStringView.length();
	}

	template <typename TPChar>
	inline bool operator==( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator==( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), getMinOf( pLhs.length(), pRhs.length() ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator==( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) == 0;
	}

	template <typename TPChar>
	inline bool operator!=( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator!=( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), getMinOf( pLhs.length(), pRhs.length() ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator!=( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) != 0;
	}

	template <typename TPChar>
	inline bool operator<( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), getMinOf( pLhs.length(), pRhs.length() ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) < 0;
	}

	template <typename TPChar>
	inline bool operator<=( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator<=( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), getMinOf( pLhs.length(), pRhs.length() ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator<=( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) <= 0;
	}

	template <typename TPChar>
	inline bool operator>( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), getMinOf( pLhs.length(), pRhs.length() ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) > 0;
	}

	template <typename TPChar>
	inline bool operator>=( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) >= 0;
	}

	template <typename TPChar>
	inline bool operator>=( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.str(), pRhs.c_str(), getMinOf( pLhs.length(), pRhs.length() ) ) >= 0;
	}

	template <typename TPChar>
	inline bool operator>=( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs )
	{
		return std::char_traits<TPChar>::compare( pLhs.c_str(), pRhs.str(), getMinOf( pLhs.length(), pRhs.length() ) ) >= 0;
	}


	template <typename TPChar>
	struct TStringViewCmpEqual
	{
		bool operator()( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}

		bool operator()( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs == pRhs;
		}
	};

	template <typename TPChar>
	struct TStringViewCmpNotEqual
	{
		bool operator()( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}

		bool operator()( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs != pRhs;
		}
	};

	template <typename TPChar>
	struct TStringViewCmpLess
	{
		bool operator()( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}

		bool operator()( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs < pRhs;
		}
	};

	template <typename TPChar>
	struct TStringViewCmpLessEqual
	{
		bool operator()( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}

		bool operator()( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs <= pRhs;
		}
	};

	template <typename TPChar>
	struct TStringViewCmpGreater
	{
		bool operator()( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}

		bool operator()( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs > pRhs;
		}
	};

	template <typename TPChar>
	struct TStringViewCmpGreaterEqual
	{
		bool operator()( const TStringView<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}

		bool operator()( const TStringView<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const TStringView<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}

		bool operator()( const std::basic_string<TPChar> & pLhs, const std::basic_string<TPChar> & pRhs ) const
		{
			return pLhs >= pRhs;
		}
	};

}

#endif /* __IC3_CPPX_STRING_VIEW_H__ */
