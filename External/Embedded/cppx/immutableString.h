
#ifndef __CPPX_IMMUTABLE_STRING_H__
#define __CPPX_IMMUTABLE_STRING_H__

#include "IntrusivePtr.h"
#include "StringView.h"
#include "StringUtils.h"

#define CPPX_ENABLE_IMMUTABLE_STRING_DEBUG_VIEW 1

namespace cppx
{

	template <typename TPChar>
	struct immutable_string_data
	{
		const std::basic_string<TPChar> internal_str;

		immutable_string_data( std::basic_string<TPChar> pSource )
		: internal_str( std::move( pSource ) )
		{}
	};

	template <typename TPChar>
	struct shared_immutable_string_data : public immutable_string_data<TPChar>, public ref_counted_base<atomic_ref_counter<uint32>>
	{
		shared_immutable_string_data( std::basic_string<TPChar> pSource )
		: immutable_string_data<TPChar>( std::move( pSource ) )
		{}
	};

	template <typename TPChar>
	struct empty_cstr;

	template <>
	struct empty_cstr<char>
	{
		static constexpr const char * str_ptr = "\0";
	};

	template <>
	struct empty_cstr<wchar_t>
	{
		static constexpr const wchar_t * str_ptr = L"\0";
	};

	template <typename TPChar>
	class immutable_string
	{
	public:
		using self_type = immutable_string<TPChar>;
		using underlying_string_type = std::basic_string<TPChar>;

		immutable_string() = default;
		~immutable_string() = default;

		immutable_string( immutable_string && ) = default;
		immutable_string & operator=( immutable_string && ) = default;

		immutable_string( const immutable_string & ) = default;
		immutable_string & operator=( const immutable_string & ) = default;

		immutable_string( std::nullptr_t )
		{}

		immutable_string( const TPChar * pStr, size_t pLength = cppx::cve::max_size )
		{
			_initStr( pStr, pLength );
		}

		immutable_string( const TPChar pCh, size_t pCount = 1 )
		{
			_initStr( pCh, pCount );
		}

		immutable_string( const string_base_view<TPChar> & pStrView )
		{
			_initStr( pStrView.str(), pStrView.length() );
		}

		immutable_string( const std::basic_string_view<TPChar> & pStdStrView )
		{
			_initStr( pStdStrView.data(), pStdStrView.length() );
		}

		immutable_string( const std::basic_string<TPChar> & pStdString )
		{
			_initStr( std::move( pStdString ) );
		}

		immutable_string & operator=( std::nullptr_t )
		{
			_stringData.reset();
			return *this;
		}

		immutable_string & operator=( const TPChar * pRhs )
		{
			if( !_stringData || ( _stringData->internal_str != pRhs ) )
			{
				_initStr( pRhs );
			}
			return *this;
		}

		immutable_string & operator=( const string_base_view<TPChar> & pRhs )
		{
			if( !_stringData || ( _stringData->internal_str != pRhs ) )
			{
				_initStr( pRhs.str(), pRhs.length() );
			}
			return *this;
		}

		immutable_string & operator=( const std::basic_string_view<TPChar> & pRhs )
		{
			if( !_stringData || ( _stringData->internal_str != pRhs ) )
			{
				_initStr( pRhs.data(), pRhs.length() );
			}
			return *this;
		}

		immutable_string & operator=( const std::basic_string<TPChar> & pRhs )
		{
			if( !_stringData || ( _stringData->internal_str != pRhs ) )
			{
				_initStr( pRhs.c_str(), pRhs.length() );
			}
			return *this;
		}

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		CPPX_ATTR_NO_DISCARD TPChar operator[]( size_t pIndex ) const noexcept
		{
			cppx_debug_assert( pIndex < length() );
			return _stringData->internal_str[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const TPChar * data() const noexcept
		{
			return _stringData ? _stringData->internal_str.data() : empty_cstr<TPChar>::sBuffer;
		}

		CPPX_ATTR_NO_DISCARD const TPChar * begin() const noexcept
		{
			return data();
		}

		CPPX_ATTR_NO_DISCARD const TPChar * end() const noexcept
		{
			return data() + length();
		}

		CPPX_ATTR_NO_DISCARD const std::basic_string<TPChar> & str() const noexcept
		{
			return _stringData->internal_str;
		}

		CPPX_ATTR_NO_DISCARD cppx::string_base_view<TPChar> strView() const noexcept
		{
			return _stringData ? _stringData->internal_str : cppx::string_base_view < TPChar > {};
		}

		CPPX_ATTR_NO_DISCARD uint32 length() const noexcept
		{
			return _stringData ? numeric_cast<uint32>( _stringData->internal_str.length() ) : 0u;
		}

		CPPX_ATTR_NO_DISCARD uint32 size() const noexcept
		{
			return numeric_cast<uint32>( _stringData->internal_str.size() );
		}

		CPPX_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return !_stringData || _stringData->internal_str.empty();
		}

		void clear()
		{
			_stringData.reset();
		}

	private:
		using StringDataType = shared_immutable_string_data<TPChar>;

		void _initStr( std::basic_string<TPChar> pString )
		{
			_stringData = make_intrusive<StringDataType>( std::move( pString ) );
		#if( PCL_DEBUG && CPPX_ENABLE_IMMUTABLE_STRING_DEBUG_VIEW )
			_debugView = _stringData->internal_str.data();
		#endif
		}

		void _initStr( const TPChar * pStr, size_t pLength )
		{
			if( pLength == cve::max_size )
			{
				pLength = pStr ? std::char_traits<TPChar>::length( pStr ) : 0u;
			}

			_stringData = make_intrusive<StringDataType>( std::basic_string<TPChar>{ pStr, pLength } );
		#if( PCL_DEBUG && CPPX_ENABLE_IMMUTABLE_STRING_DEBUG_VIEW )
			_debugView = _stringData->internal_str.data();
		#endif
		}

		void _initStr( const TPChar pCh, size_t pCount )
		{
			_stringData = make_intrusive<StringDataType>( std::basic_string<TPChar>{ pCount, pCh } );
		#if( PCL_DEBUG && CPPX_ENABLE_IMMUTABLE_STRING_DEBUG_VIEW )
			_debugView = _stringData->internal_str.data();
		#endif
		}

	private:
		intrusive_ptr<StringDataType> _stringData;
	#if( PCL_DEBUG && CPPX_ENABLE_IMMUTABLE_STRING_DEBUG_VIEW )
		const TPChar * _debugView = empty_cstr<TPChar>::sBuffer;
	#endif
	};


	template <typename TPChar>
	inline bool operator==( const immutable_string<TPChar> & pLhs, const immutable_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() == pRhs.str();
	}

	template <typename TPChar>
	inline bool operator!=( const immutable_string<TPChar> & pLhs, const immutable_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() != pRhs.str();
	}

	template <typename TPChar>
	inline bool operator<( const immutable_string<TPChar> & pLhs, const immutable_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() < pRhs.str();
	}

	template <typename TPChar>
	inline bool operator<=( const immutable_string<TPChar> & pLhs, const immutable_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() <= pRhs.str();
	}

	template <typename TPChar>
	inline bool operator>( const immutable_string<TPChar> & pLhs, const immutable_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() > pRhs.str();
	}

	template <typename TPChar>
	inline bool operator>=( const immutable_string<TPChar> & pLhs, const immutable_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() >= pRhs.str();
	}

	using ImmutableString = immutable_string<char>;
	using ImmutableWString = immutable_string<wchar_t>;

}

namespace std
{

	template <typename TPChar>
	struct hash< cppx::immutable_string<TPChar> >
	{
		size_t operator()( const cppx::immutable_string<TPChar> & pString ) const noexcept
		{
			using StringType = typename cppx::immutable_string<TPChar>::underlying_string_type;
			using HashType = hash<StringType>;
			return pString.empty() ? HashType()( pString.str() ) : HashType()( StringType{} );
		}
	};

}

#endif /* __CPPX_IMMUTABLE_STRING_H__ */
