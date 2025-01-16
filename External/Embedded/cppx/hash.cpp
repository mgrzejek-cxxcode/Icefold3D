
#include "Hash.h"
#include "StaticLimits.h"
#include <zlib/zlib.h>

namespace cppx
{

	uint32 hash_traits<hash_algo::adler32>::compute( const void * pInput, size_t pInputSize )
	{
		return update( init_value, pInput, pInputSize );
	}

	uint32 hash_traits<hash_algo::adler32>::update( uint32 pHash, const void * pInput, size_t pInputSize )
	{
		uint32 result = pHash;
		if( pInputSize > 0 )
		{
			const byte * inputBytes = reinterpret_cast<const byte *>( pInput );
			auto tmpRes = adler32( pHash, inputBytes, static_cast<uInt>( pInputSize ) );
			result = static_cast<uint32>( tmpRes );
		}
		return result;
	}


	uint32 hash_traits<hash_algo::crc32>::compute( const void * pInput, size_t pInputSize )
	{
		return update( init_value, pInput, pInputSize );
	}

	uint32 hash_traits<hash_algo::crc32>::update( uint32 pHash, const void * pInput, size_t pInputSize )
	{
		uint32 result = pHash;
		if( pInputSize > 0 )
		{
			const byte * inputBytes = reinterpret_cast<const byte *>( pInput );
		#if( EBS_EIS_SUPPORT_LEVEL & ECL_EIS_FEATURE_SSE42 )
			for( ; pInputSize >= 4; inputBytes += 4, pInputSize -= 4 )
			{
				auto tmpres = _mm_crc32_u32( result, *( reinterpret_cast<const uint32 *>( inputBytes ) ) );
				result = static_cast<uint32>( tmpres );
			}
		#endif
			auto tmpres = crc32( pHash, inputBytes, static_cast<uInt>( pInputSize ) );
			result = static_cast<uint32>( tmpres );
		}
		return result;
	}


	uint32 hash_traits<hash_algo::djb2>::compute( const void * pInput, size_t pInputSize )
	{
		return update( init_value, pInput, pInputSize );
	}

	uint32 hash_traits<hash_algo::djb2>::update( uint32 pHash, const void * pInput, size_t pInputSize )
	{
		uint32 result = pHash;
		if( pInputSize > 0 )
		{
			const byte * inputBytes = reinterpret_cast<const byte *>( pInput );
			for( size_t byteIndex = 0; byteIndex < pInputSize; ++byteIndex )
			{
				result = ( ( result << 5 ) + result ) ^ ( inputBytes[byteIndex] );
			}
		}
		return result;
	}


	constexpr uint32 sFNV32PrimeValue = 0x01000193;

	uint32 hash_traits<hash_algo::fnv1a32>::compute( const void * pInput, size_t pInputSize )
	{
		return update( init_value, pInput, pInputSize );
	}

	uint32 hash_traits<hash_algo::fnv1a32>::update( uint32 pHash, const void * pInput, size_t pInputSize )
	{
		uint32 result = pHash;
		if( pInputSize > 0 )
		{
			const byte * inputBytes = reinterpret_cast<const byte *>( pInput );
			for( size_t byteIndex = 0; byteIndex < pInputSize; ++byteIndex )
			{
				result = result ^ static_cast<uint32>( inputBytes[byteIndex] );
				result = result * sFNV32PrimeValue;
			}
		}
		return result;
	}


	constexpr uint64 sFNV64PrimeValue = 0x100000001B3;
	constexpr uint64 sU64Mask = limits<uint64>::max_value << 8;

	uint64 hash_traits<hash_algo::fnv1a64>::compute( const void * pInput, size_t pInputSize )
	{
		return update( init_value, pInput, pInputSize );
	}

	uint64 hash_traits<hash_algo::fnv1a64>::update( uint64 pHash, const void * pInput, size_t pInputSize )
	{
		uint64 result = pHash;
		if( pInputSize > 0 )
		{
			const byte * inputBytes = reinterpret_cast<const byte *>( pInput );
			for( size_t byteIndex = 0; byteIndex < pInputSize; ++byteIndex )
			{
				result = result ^ static_cast<uint64>( inputBytes[byteIndex] );
				result = result * sFNV64PrimeValue;
			}
		}
		return result;
	}


	uint32 hash_traits<hash_algo::sdbm>::compute( const void * pInput, size_t pInputSize )
	{
		return update( init_value, pInput, pInputSize );
	}

	uint32 hash_traits<hash_algo::sdbm>::update( uint32 pHash, const void * pInput, size_t pInputSize )
	{
		uint32 result = pHash;
		if( pInputSize > 0 )
		{
			const byte * inputBytes = reinterpret_cast<const byte *>( pInput );
			for( size_t byteIndex = 0; byteIndex < pInputSize; ++byteIndex )
			{
				result = ( inputBytes[byteIndex] ) + ( result << 6 ) + ( result << 16 ) - result;
			}
		}
		return result;
	}

}
