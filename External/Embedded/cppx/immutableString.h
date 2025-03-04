
#ifndef __CPPX_IMMUTABLE_STRING_H__
#define __CPPX_IMMUTABLE_STRING_H__

#include "intrusivePtr.h"
#include "stringView.h"
#include "stringUtils.h"

#define CPPX_ENABLE_IMMUTABLE_STRING_DEBUG_VIEW 0

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
	class immutable_base_string
	{
	public:
		using self_type = immutable_base_string<TPChar>;
		using underlying_string_type = std::basic_string<TPChar>;

		immutable_base_string() = default;
		~immutable_base_string() = default;

		immutable_base_string( immutable_base_string && ) = default;
		immutable_base_string & operator=( immutable_base_string && ) = default;

		immutable_base_string( const immutable_base_string & ) = default;
		immutable_base_string & operator=( const immutable_base_string & ) = default;

		immutable_base_string( std::nullptr_t )
		{}

		immutable_base_string( const TPChar * pStr, size_t pLength = cppx::cve::max_size )
		{
			_init_str( pStr, pLength );
		}

		immutable_base_string( const TPChar pCh, size_t pCount = 1 )
		{
			_init_str( pCh, pCount );
		}

		immutable_base_string( const string_base_view<TPChar> & pStrView )
		{
			_init_str( pStrView.str(), pStrView.length() );
		}

		immutable_base_string( const std::basic_string_view<TPChar> & pStdStrView )
		{
			_init_str( pStdStrView.data(), pStdStrView.length() );
		}

		immutable_base_string( const std::basic_string<TPChar> & pStdString )
		{
			_init_str( std::move( pStdString ) );
		}

		immutable_base_string & operator=( std::nullptr_t )
		{
			_stringData.reset();
			return *this;
		}

		immutable_base_string & operator=( const TPChar * pRhs )
		{
			if( !_stringData || ( _stringData->internal_str != pRhs ) )
			{
				_init_str( pRhs );
			}
			return *this;
		}

		immutable_base_string & operator=( const string_base_view<TPChar> & pRhs )
		{
			if( !_stringData || ( _stringData->internal_str != pRhs ) )
			{
				_init_str( pRhs.str(), pRhs.length() );
			}
			return *this;
		}

		immutable_base_string & operator=( const std::basic_string_view<TPChar> & pRhs )
		{
			if( !_stringData || ( _stringData->internal_str != pRhs ) )
			{
				_init_str( pRhs.data(), pRhs.length() );
			}
			return *this;
		}

		immutable_base_string & operator=( const std::basic_string<TPChar> & pRhs )
		{
			if( !_stringData || ( _stringData->internal_str != pRhs ) )
			{
				_init_str( pRhs.c_str(), pRhs.length() );
			}
			return *this;
		}

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		CPPX_ATTR_NO_DISCARD operator cppx::string_base_view<TPChar>() const noexcept
		{
			return str_view();
		}

		CPPX_ATTR_NO_DISCARD TPChar operator[]( size_t pIndex ) const noexcept
		{
			cppx_debug_assert( pIndex < length() );
			return _stringData->internal_str[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const TPChar * data() const noexcept
		{
			return _stringData ? _stringData->internal_str.data() : cve::str_empty<TPChar>;
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

		CPPX_ATTR_NO_DISCARD cppx::string_base_view<TPChar> str_view() const noexcept
		{
			return _stringData ? _stringData->internal_str : cppx::string_base_view<TPChar>{};
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

		void _init_str( std::basic_string<TPChar> pString )
		{
			_stringData = make_intrusive<StringDataType>( std::move( pString ) );
		#if( PCL_DEBUG && CPPX_ENABLE_IMMUTABLE_STRING_DEBUG_VIEW )
			_debugView = _stringData->internal_str.data();
		#endif
		}

		void _init_str( const TPChar * pStr, size_t pLength )
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

		void _init_str( const TPChar pCh, size_t pCount )
		{
			_stringData = make_intrusive<StringDataType>( std::basic_string<TPChar>{ pCount, pCh } );
		#if( PCL_DEBUG && CPPX_ENABLE_IMMUTABLE_STRING_DEBUG_VIEW )
			_debugView = _stringData->internal_str.data();
		#endif
		}

	private:
		intrusive_ptr<StringDataType> _stringData;
	#if( PCL_DEBUG && CPPX_ENABLE_IMMUTABLE_STRING_DEBUG_VIEW )
		const TPChar * _debugView = cve::str_empty<TPChar>;
	#endif
	};


	template <typename TPChar>
	inline bool operator==( const immutable_base_string<TPChar> & pLhs, const immutable_base_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() == pRhs.str();
	}

	template <typename TPChar>
	inline bool operator!=( const immutable_base_string<TPChar> & pLhs, const immutable_base_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() != pRhs.str();
	}

	template <typename TPChar>
	inline bool operator<( const immutable_base_string<TPChar> & pLhs, const immutable_base_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() < pRhs.str();
	}

	template <typename TPChar>
	inline bool operator<=( const immutable_base_string<TPChar> & pLhs, const immutable_base_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() <= pRhs.str();
	}

	template <typename TPChar>
	inline bool operator>( const immutable_base_string<TPChar> & pLhs, const immutable_base_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() > pRhs.str();
	}

	template <typename TPChar>
	inline bool operator>=( const immutable_base_string<TPChar> & pLhs, const immutable_base_string<TPChar> & pRhs ) noexcept
	{
		return pLhs.str() >= pRhs.str();
	}

	using immutable_string = immutable_base_string<char>;
	using immutable_wstring = immutable_base_string<wchar_t>;

}

namespace std
{

	template <typename TPChar>
	struct hash< cppx::immutable_base_string<TPChar> >
	{
		size_t operator()( const cppx::immutable_base_string<TPChar> & pString ) const noexcept
		{
			using StringType = typename cppx::immutable_base_string<TPChar>::underlying_string_type;
			return !pString.empty() ? hash<StringType>()( pString.str() ) : hash<StringType>()( StringType{} );
		}
	};

}

#endif /* __CPPX_IMMUTABLE_STRING_H__ */
