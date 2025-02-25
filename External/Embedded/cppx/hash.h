
#ifndef __CPPX_HASH_OBJECT_H__
#define __CPPX_HASH_OBJECT_H__

#include "arrayView.h"
#include <string>

namespace cppx
{

	enum class hash_algo : enum_default_value_t
	{
		adler32,
		crc32,
		djb2,
		fnv1a32,
		fnv1a64,
		sdbm
	};

	struct hash_input
	{
		const void * data;

		size_t dataSize;

		template <typename TChar>
		explicit hash_input( const std::basic_string<TChar> & pString )
		: data( pString.data() )
		, dataSize( pString.length() * sizeof( TChar ) )
		{}

		template <typename TChar>
		explicit hash_input( const std::basic_string_view<TChar> & pStrView )
		: data( pStrView.data() )
		, dataSize( pStrView.length() * sizeof( TChar ) )
		{}

		explicit hash_input( const char * pCStr )
		: data( pCStr )
		, dataSize( std::strlen( pCStr ) )
		{}

		explicit hash_input( const wchar_t * pWCStr )
		: data( pWCStr )
		, dataSize( std::wcslen( pWCStr ) )
		{}

		template <typename TPValue>
		explicit hash_input( const array_view<TPValue> & pArrayView )
		: data( pArrayView.data() )
		, dataSize( pArrayView.size() * sizeof( TPValue ) )
		{}

		template <typename TPValue>
		explicit hash_input( const TPValue & pObject )
		: data( &pObject )
		, dataSize( sizeof( TPValue ) )
		{}

		template <typename TPInput>
		explicit hash_input( const TPInput * pData, size_t pCount )
		: data( pData )
		, dataSize( pCount * sizeof( TPInput ) )
		{}
	};

	template <hash_algo tpHashAlgo>
	struct hash_traits;
	
	template <hash_algo tpHashAlgo>
	struct hash_object
	{
	public:
		using self_type = hash_object<tpHashAlgo>;
		using value_type = typename hash_traits<tpHashAlgo>::value_type;

		static constexpr hash_algo hash_algo = tpHashAlgo;

		value_type value;

	public:
		hash_object( const hash_object & ) = default;
		hash_object & operator=( const hash_object & ) = default;
		
		constexpr hash_object() noexcept
		: value( hash_traits<tpHashAlgo>::init_value )
		{}

		constexpr explicit hash_object( value_type pValue ) noexcept
		: value( pValue )
		{}

		constexpr operator value_type() const noexcept
		{
			return value;
		}

		void swap( hash_object & pOther ) noexcept
		{
			std::swap( value, pOther.value );
		}

		CPPX_ATTR_NO_DISCARD bool equals( const hash_object & pOther ) const noexcept
		{
			return value == pOther.value;
		}

		CPPX_ATTR_NO_DISCARD int32 compare( const hash_object & pOther ) const noexcept
		{
			return equals( pOther ) ? 0 : ( value > pOther.value ? 1 : -1 );
		}

		CPPX_ATTR_NO_DISCARD constexpr bool operator==( const hash_object<tpHashAlgo> & pRhs ) const noexcept
		{
			return value == pRhs.value;
		}

		CPPX_ATTR_NO_DISCARD constexpr bool operator!=( const hash_object<tpHashAlgo> & pRhs ) const noexcept
		{
			return value != pRhs.value;
		}

		CPPX_ATTR_NO_DISCARD constexpr bool operator<( const hash_object<tpHashAlgo> & pRhs ) const noexcept
		{
			return value < pRhs.value;
		}

		CPPX_ATTR_NO_DISCARD constexpr bool operator<=( const hash_object<tpHashAlgo> & pRhs ) const noexcept
		{
			return value <= pRhs.value;
		}

		CPPX_ATTR_NO_DISCARD constexpr bool operator>( const hash_object<tpHashAlgo> & pRhs ) const noexcept
		{
			return value > pRhs.value;
		}

		CPPX_ATTR_NO_DISCARD constexpr bool operator>=( const hash_object<tpHashAlgo> & pRhs ) const noexcept
		{
			return value >= pRhs.value;
		}
	};

	template <hash_algo tpHashAlgo>
	inline constexpr hash_object<tpHashAlgo> hash_val_empty{ 0u };

	template <hash_algo tpHashAlgo>
	inline constexpr hash_object<tpHashAlgo> hash_val_init{ hash_traits<tpHashAlgo>::init_value };



	template <hash_algo tpHashAlgo>
	CPPX_ATTR_NO_DISCARD inline hash_object<tpHashAlgo> hash_compute( const void * pData, size_t pDataLength )
	{
		return hash_object<tpHashAlgo>{ hash_traits<tpHashAlgo>::compute( pData, pDataLength ) };
	}

	template <hash_algo tpHashAlgo>
	CPPX_ATTR_NO_DISCARD inline hash_object<tpHashAlgo> hash_compute( const hash_input & pInput )
	{
		return hash_compute<tpHashAlgo>( pInput.data, pInput.dataSize );
	}

	template <hash_algo tpHashAlgo, typename TPValue>
	CPPX_ATTR_NO_DISCARD inline hash_object<tpHashAlgo> hash_compute( const TPValue & pValue )
	{
		return hash_compute<tpHashAlgo>( hash_input{ pValue } );
	}

	template <hash_algo tpHashAlgo>
	CPPX_ATTR_NO_DISCARD inline hash_object<tpHashAlgo> hash_compute_ex( const hash_object<tpHashAlgo> & pInitHash, const void * pData, size_t pDataLength )
	{
		return hash_object<tpHashAlgo>{ hash_traits<tpHashAlgo>::update( pInitHash.value, pData, pDataLength ) };
	}

	template <hash_algo tpHashAlgo>
	CPPX_ATTR_NO_DISCARD inline hash_object<tpHashAlgo> hash_compute_ex( const hash_object<tpHashAlgo> & pInitHash, const hash_input & pInput )
	{
		return hash_compute_ex<tpHashAlgo>( pInitHash, pInput.data, pInput.dataSize );
	}

	template <hash_algo tpHashAlgo, typename TPValue>
	CPPX_ATTR_NO_DISCARD inline hash_object<tpHashAlgo> hash_compute_ex( const hash_object<tpHashAlgo> & pInitHash, const TPValue & pValue )
	{
		return hash_compute_ex<tpHashAlgo>( pInitHash, hash_input{ pValue } );
	}

	template <hash_algo tpHashAlgo, typename TPValue, typename... TPRest>
	CPPX_ATTR_NO_DISCARD inline hash_object<tpHashAlgo> hash_compute( const TPValue & pValue, TPRest && ...pRest )
	{
		return hash_compute_ex<tpHashAlgo>( hash_compute<tpHashAlgo>( pValue ), std::forward<TPRest>( pRest )... );
	}

	template <hash_algo tpHashAlgo, typename TPValue, typename... TPRest>
	CPPX_ATTR_NO_DISCARD inline hash_object<tpHashAlgo> hash_compute_ex( const hash_object<tpHashAlgo> & pInitHash, const TPValue & pValue, TPRest && ...pRest )
	{
		return hash_compute_ex<tpHashAlgo>( hash_compute_ex<tpHashAlgo>( pInitHash, pValue ), std::forward<TPRest>( pRest )... );
	}


	template <hash_algo tpHashAlgo>
	inline void hash_compute_inplace( hash_object<tpHashAlgo> & pOutput, const void * pData, size_t pDataLength )
	{
		pOutput.value = hash_traits<tpHashAlgo>::compute( pData, pDataLength );
	}

	template <hash_algo tpHashAlgo>
	inline void hash_compute_inplace( hash_object<tpHashAlgo> & pOutput, const hash_input & pInput )
	{
		hash_compute_inplace<tpHashAlgo>( pOutput, pInput.data, pInput.dataSize );
	}

	template <hash_algo tpHashAlgo, typename TPValue>
	inline void hash_compute_inplace( hash_object<tpHashAlgo> & pOutput, const array_view<TPValue> & pInput )
	{
		hash_compute_inplace<tpHashAlgo>( pOutput, pInput.mData(), pInput.size() * sizeof( TPValue ) );
	}

	template <hash_algo tpHashAlgo>
	inline void hash_compute_inplace_ex( hash_object<tpHashAlgo> & pOutput, const hash_object<tpHashAlgo> & pInitHash, const void * pData, size_t pDataLength )
	{
		pOutput.value = hash_traits<tpHashAlgo>::update( pInitHash.value, pData, pDataLength );
	}

	template <hash_algo tpHashAlgo>
	inline void hash_compute_inplace_ex( hash_object<tpHashAlgo> & pOutput, const hash_object<tpHashAlgo> & pInitHash, const hash_input & pInput )
	{
		hash_compute_inplace_ex<tpHashAlgo>( pOutput, pInitHash, pInput.data, pInput.dataSize );
	}

	template <hash_algo tpHashAlgo, typename TPValue>
	inline void hash_compute_inplace_ex( hash_object<tpHashAlgo> & pOutput, const hash_object<tpHashAlgo> & pInitHash, const TPValue & pValue )
	{
		hash_compute_inplace_ex<tpHashAlgo>( pOutput, pInitHash, hash_input{ pValue } );
	}

	template <hash_algo tpHashAlgo, typename TPValue, typename... TPRest>
	inline void hash_compute_inplace( hash_object<tpHashAlgo> & pOutput, const TPValue & pValue, TPRest && ...pRest )
	{
		hash_compute_inplace_ex<tpHashAlgo>( pOutput, hash_compute<tpHashAlgo>( pValue ), std::forward<TPRest>( pRest )... );
	}

	template <hash_algo tpHashAlgo, typename TPValue, typename... TPRest>
	inline void hash_compute_inplace_ex( hash_object<tpHashAlgo> & pOutput, const hash_object<tpHashAlgo> & pInitHash, const TPValue & pValue, TPRest && ...pRest )
	{
		hash_compute_inplace_ex<tpHashAlgo>( pOutput, hash_compute_ex<tpHashAlgo>( pInitHash, pValue ), std::forward<TPRest>( pRest )... );
	}

	template <typename TPHashValue>
	struct hash_common_traits
	{
		/// Underlying type used to store hash value.
		using value_type = TPHashValue;

		/// Size of the hash value, in bytes.
		static constexpr size_t byte_size = sizeof( TPHashValue );

		/// Length of the hexadecimal string representation of a hash.
		static constexpr size_t hex_length = byte_size * 8 / 4;
	};

	template <>
	struct hash_traits<hash_algo::adler32> : public hash_common_traits<uint32>
	{
		static constexpr uint32 init_value = 1u;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct hash_traits<hash_algo::crc32> : public hash_common_traits<uint32>
	{
		static constexpr uint32 init_value = 0xFFFFFFFF;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct hash_traits<hash_algo::djb2> : public hash_common_traits<uint32>
	{
		static constexpr uint32 init_value = 0x1505;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct hash_traits<hash_algo::fnv1a32> : public hash_common_traits<uint32>
	{
		static constexpr uint32 init_value = 0x811C9DC5;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct hash_traits<hash_algo::fnv1a64> : public hash_common_traits<uint64>
	{
		static constexpr uint64 init_value = 0xCBF29CE484222325;
		static uint64 compute( const void * pInput, size_t pInputSize );
		static uint64 update( uint64 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct hash_traits<hash_algo::sdbm> : public hash_common_traits<uint32>
	{
		static constexpr uint32 init_value = 0u;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <typename TPValue, hash_algo tpHashAlgo = hash_algo::sdbm>
	struct hash_std_proxy
	{
		CPPX_ATTR_NO_DISCARD size_t operator()( const TPValue & pInput ) const noexcept
		{
			return hash_compute<tpHashAlgo>( hash_input{ pInput } );
		}
	};

	struct hash_str_std_fast
	{
		template <typename TPChar>
		CPPX_ATTR_NO_DISCARD size_t operator()( const std::basic_string<TPChar> & pInput ) const noexcept
		{
			const auto sdbmHash = hash_compute<hash_algo::sdbm>( hash_input{ pInput } );
			return sdbmHash.value;
		}

		template <typename TPChar>
		CPPX_ATTR_NO_DISCARD size_t operator()( const std::basic_string_view<TPChar> & pInput ) const noexcept
		{
			const auto sdbmHash = hash_compute<hash_algo::sdbm>( hash_input{ pInput } );
			return sdbmHash.value;
		}
	};

	struct hash_str_std_hybrid
	{
		template <typename TPChar>
		CPPX_ATTR_NO_DISCARD size_t operator()( const std::basic_string<TPChar> & pInput ) const noexcept
		{
			const auto hash1 = hash_compute<hash_algo::fnv1a32>( hash_input{ pInput } );
			const auto hash2 = hash_compute<hash_algo::sdbm>( hash_input{ pInput } );
			return static_cast<size_t>(( ( uint64 )hash1.value << 32 ) | hash2.value );
		}

		template <typename TPChar>
		CPPX_ATTR_NO_DISCARD size_t operator()( const std::basic_string_view<TPChar> & pInput ) const noexcept
		{
			const auto hash1 = hash_compute<hash_algo::fnv1a32>( hash_input{ pInput } );
			const auto hash2 = hash_compute<hash_algo::sdbm>( hash_input{ pInput } );
			return static_cast<size_t>(( ( uint64 )hash1.value << 32 ) | hash2.value );
		}
	};

}

namespace std
{

	template <cppx::hash_algo tpHashAlgo>
	struct hash< cppx::hash_object<tpHashAlgo> >
	{
		size_t operator()( const cppx::hash_object<tpHashAlgo> & pHashObject ) const noexcept
		{
			return cppx::numeric_cast<size_t>( pHashObject.value );
		}
	};

}

#endif // __CPPX_HASH_OBJECT_H__
