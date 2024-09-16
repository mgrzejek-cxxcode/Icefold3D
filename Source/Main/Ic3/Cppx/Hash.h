
#ifndef __IC3_CPPX_HASH_OBJECT_H__
#define __IC3_CPPX_HASH_OBJECT_H__

#include "ArrayView.h"
#include <string>

namespace Ic3::Cppx
{

	enum class EHashAlgo : enum_default_value_t
	{
		Adler32,
		CRC32,
		DJB2,
		FNV1A32,
		FNV1A64,
		SDBM
	};

	struct HashInput
	{
		const void * mData;
		size_t mDataSize;

		template <typename TChar>
		explicit HashInput( const std::basic_string<TChar> & pString )
		: mData( pString.mData() )
		, mDataSize( pString.length() * sizeof( TChar ) )
		{}

		explicit HashInput( const char * pCStr )
		: mData( pCStr )
		, mDataSize( std::strlen( pCStr ) )
		{}

		explicit HashInput( const wchar_t * pWCStr )
		: mData( pWCStr )
		, mDataSize( std::wcslen( pWCStr ) )
		{}

		template <typename TPValue>
		explicit HashInput( const TArrayView<TPValue> & pArrayView )
		: mData( pArrayView.mData() )
		, mDataSize( pArrayView.size() * sizeof( TPValue ) )
		{}

		template <typename TPValue>
		explicit HashInput( const TPValue & pObject )
		: mData( &pObject )
		, mDataSize( sizeof( TPValue ) )
		{}

		template <typename TPInput>
		explicit HashInput( const TPInput * pData, size_t pCount )
		: mData( pData )
		, mDataSize( pCount * sizeof( TPInput ) )
		{}
	};

	template <EHashAlgo tpHashAlgo>
	struct QHashTraits;
	
	template <EHashAlgo tpHashAlgo>
	struct THashObject
	{
	public:
		using ValueType = typename QHashTraits<tpHashAlgo>::ValueType;

		static constexpr EHashAlgo sHashAlgo = tpHashAlgo;

		ValueType mHashValue;

	public:
		THashObject( const THashObject & ) = default;
		THashObject & operator=( const THashObject & ) = default;
		
		constexpr THashObject() noexcept
		: mHashValue( QHashTraits<tpHashAlgo>::sHashInitValue )
		{}

		constexpr explicit THashObject( ValueType pValue ) noexcept
		: mHashValue( pValue )
		{}

		constexpr explicit operator ValueType() const noexcept
		{
			return mHashValue;
		}

		void swap( THashObject & pOther ) noexcept
		{
			std::swap( mHashValue, pOther.mHashValue );
		}

		IC3_ATTR_NO_DISCARD bool equals( const THashObject & pOther ) const noexcept
		{
			return mHashValue == pOther.mHashValue;
		}

		IC3_ATTR_NO_DISCARD int32 compare( const THashObject & pOther ) const noexcept
		{
			return equals( pOther ) ? 0 : ( mHashValue > pOther.mHashValue ? 1 : -1 );
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator==( const THashObject<tpHashAlgo> & pRhs ) const noexcept
		{
			return mHashValue == pRhs.mHashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator!=( const THashObject<tpHashAlgo> & pRhs ) const noexcept
		{
			return mHashValue != pRhs.mHashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator<( const THashObject<tpHashAlgo> & pRhs ) const noexcept
		{
			return mHashValue < pRhs.mHashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator<=( const THashObject<tpHashAlgo> & pRhs ) const noexcept
		{
			return mHashValue <= pRhs.mHashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator>( const THashObject<tpHashAlgo> & pRhs ) const noexcept
		{
			return mHashValue > pRhs.mHashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator>=( const THashObject<tpHashAlgo> & pRhs ) const noexcept
		{
			return mHashValue >= pRhs.mHashValue;
		}
	};

	template <EHashAlgo tpHashAlgo>
	inline constexpr THashObject<tpHashAlgo> cvHashEmpty{ 0u };

	template <EHashAlgo tpHashAlgo>
	inline constexpr THashObject<tpHashAlgo> cvHashInit{ QHashTraits<tpHashAlgo>::sHashInitValue };



	template <EHashAlgo tpHashAlgo>
	IC3_ATTR_NO_DISCARD inline THashObject<tpHashAlgo> hashCompute( const void * pData, size_t pDataLength )
	{
		return THashObject<tpHashAlgo>{ QHashTraits<tpHashAlgo>::compute( pData, pDataLength ) };
	}

	template <EHashAlgo tpHashAlgo>
	IC3_ATTR_NO_DISCARD inline THashObject<tpHashAlgo> hashCompute( const HashInput & pInput )
	{
		return hashCompute<tpHashAlgo>( pInput.mData, pInput.mDataSize );
	}

	template <EHashAlgo tpHashAlgo, typename TPValue>
	IC3_ATTR_NO_DISCARD inline THashObject<tpHashAlgo> hashCompute( const TPValue & pValue )
	{
		return hashCompute<tpHashAlgo>( HashInput{ pValue } );
	}

	template <EHashAlgo tpHashAlgo>
	IC3_ATTR_NO_DISCARD inline THashObject<tpHashAlgo> hashComputeEx( const THashObject<tpHashAlgo> & pInitHash, const void * pData, size_t pDataLength )
	{
		return THashObject<tpHashAlgo>{ QHashTraits<tpHashAlgo>::update( pInitHash.mHashValue, pData, pDataLength ) };
	}

	template <EHashAlgo tpHashAlgo>
	IC3_ATTR_NO_DISCARD inline THashObject<tpHashAlgo> hashComputeEx( const THashObject<tpHashAlgo> & pInitHash, const HashInput & pInput )
	{
		return hashComputeEx<tpHashAlgo>( pInitHash, pInput.mData, pInput.mDataSize );
	}

	template <EHashAlgo tpHashAlgo, typename TPValue>
	IC3_ATTR_NO_DISCARD inline THashObject<tpHashAlgo> hashComputeEx( const THashObject<tpHashAlgo> & pInitHash, const TPValue & pValue )
	{
		return hashComputeEx<tpHashAlgo>( pInitHash, HashInput{ pValue } );
	}

	template <EHashAlgo tpHashAlgo, typename TPValue, typename... TPRest>
	IC3_ATTR_NO_DISCARD inline THashObject<tpHashAlgo> hashCompute( const TPValue & pValue, TPRest && ...pRest )
	{
		return hashComputeEx<tpHashAlgo>( hashCompute<tpHashAlgo>( pValue ), std::forward<TPRest>( pRest )... );
	}

	template <EHashAlgo tpHashAlgo, typename TPValue, typename... TPRest>
	IC3_ATTR_NO_DISCARD inline THashObject<tpHashAlgo> hashComputeEx( const THashObject<tpHashAlgo> & pInitHash, const TPValue & pValue, TPRest && ...pRest )
	{
		return hashComputeEx<tpHashAlgo>( hashComputeEx<tpHashAlgo>( pInitHash, pValue ), std::forward<TPRest>( pRest )... );
	}


	template <EHashAlgo tpHashAlgo>
	inline void hashComputeInplace( THashObject<tpHashAlgo> & pOutput, const void * pData, size_t pDataLength )
	{
		pOutput.mHashValue = QHashTraits<tpHashAlgo>::compute( pData, pDataLength );
	}

	template <EHashAlgo tpHashAlgo>
	inline void hashComputeInplace( THashObject<tpHashAlgo> & pOutput, const HashInput & pInput )
	{
		hashComputeInplace<tpHashAlgo>( pOutput, pInput.mData, pInput.mDataSize );
	}

	template <EHashAlgo tpHashAlgo, typename TPValue>
	inline void hashComputeInplace( THashObject<tpHashAlgo> & pOutput, const TArrayView<TPValue> & pInput )
	{
		hashComputeInplace<tpHashAlgo>( pOutput, pInput.mData(), pInput.size() * sizeof( TPValue ) );
	}

	template <EHashAlgo tpHashAlgo>
	inline void hashComputeInplaceEx( THashObject<tpHashAlgo> & pOutput, const THashObject<tpHashAlgo> & pInitHash, const void * pData, size_t pDataLength )
	{
		pOutput.mHashValue = QHashTraits<tpHashAlgo>::update( pInitHash.mHashValue, pData, pDataLength );
	}

	template <EHashAlgo tpHashAlgo>
	inline void hashComputeInplaceEx( THashObject<tpHashAlgo> & pOutput, const THashObject<tpHashAlgo> & pInitHash, const HashInput & pInput )
	{
		hashComputeInplaceEx<tpHashAlgo>( pOutput, pInitHash, pInput.mData, pInput.mDataSize );
	}

	template <EHashAlgo tpHashAlgo, typename TPValue>
	inline void hashComputeInplaceEx( THashObject<tpHashAlgo> & pOutput, const THashObject<tpHashAlgo> & pInitHash, const TPValue & pValue )
	{
		hashComputeInplaceEx<tpHashAlgo>( pOutput, pInitHash, HashInput{ pValue } );
	}

	template <EHashAlgo tpHashAlgo, typename TPValue, typename... TPRest>
	inline void hashComputeInplace( THashObject<tpHashAlgo> & pOutput, const TPValue & pValue, TPRest && ...pRest )
	{
		hashComputeInplaceEx<tpHashAlgo>( pOutput, hashCompute<tpHashAlgo>( pValue ), std::forward<TPRest>( pRest )... );
	}

	template <EHashAlgo tpHashAlgo, typename TPValue, typename... TPRest>
	inline void hashComputeInplaceEx( THashObject<tpHashAlgo> & pOutput, const THashObject<tpHashAlgo> & pInitHash, const TPValue & pValue, TPRest && ...pRest )
	{
		hashComputeInplaceEx<tpHashAlgo>( pOutput, hashComputeEx<tpHashAlgo>( pInitHash, pValue ), std::forward<TPRest>( pRest )... );
	}

	template <typename TPHashValue>
	struct QHashCommonTraits
	{
		/// Underlying type used to store hash value.
		using ValueType = TPHashValue;

		/// Size of the hash value, in bytes.
		static constexpr size_t sByteSize = sizeof( TPHashValue );

		/// Length of the hexadecimal string representation of a hash.
		static constexpr size_t sHexLength = sByteSize * 8 / 4;
	};

	template <>
	struct QHashTraits<EHashAlgo::Adler32> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 1u;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::CRC32> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 0xFFFFFFFF;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::DJB2> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 0x1505;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::FNV1A32> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 0x811C9DC5;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::FNV1A64> : public QHashCommonTraits<uint64>
	{
		static constexpr uint64 sHashInitValue = 0xCBF29CE484222325;
		static uint64 compute( const void * pInput, size_t pInputSize );
		static uint64 update( uint64 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::SDBM> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 0u;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

}

namespace std
{

	template <Ic3::Cppx::EHashAlgo tpHashAlgo>
	struct hash< Ic3::Cppx::THashObject<tpHashAlgo> >
	{
		size_t operator()( const Ic3::Cppx::THashObject<tpHashAlgo> & pHashObject ) const noexcept
		{
			return Ic3::numeric_cast<size_t>( pHashObject.mHashValue );
		}
	};

}

#endif // __IC3_CPPX_HASH_OBJECT_H__
