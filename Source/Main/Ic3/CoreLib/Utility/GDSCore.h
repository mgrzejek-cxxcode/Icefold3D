
#ifndef __IC3_CORELIB_GDS_CORE_H__
#define __IC3_CORELIB_GDS_CORE_H__

#include "GDSCommon.h"
#include "../Exception.h"

#include <Ic3/Platform/GDS.h>
#include <cppx/byteArray.h>

#include <array>
#include <functional>
#include <vector>

namespace Ic3
{

	template <typename TPIntegral>
	class atomic_bitmask;

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	class SortedArray;

	namespace GDSCore
	{
		
		/// @brief Typedef for a callable compatible with ReadCallback requirements for GDS deserializeExternal() functions.
		using DataReadCallback = std::function<uint64 /* pReadBytes */( void * /* pTargetBuffer */, uint64 /* pReadSize */ )>;

		/// @brief Typedef for a callable compatible with WriteCallback requirements for GDS serializeExternal() functions.
		using DataWriteCallback = std::function<uint64 /* pWrittenBytes */( const void * /* pDataToWrite */, uint64 /* pWriteSize */ )>;

		/***********************************************************************************************************/
		/**************************************** Core API - evalByteSize() ****************************************/
		/***********************************************************************************************************/

		template <typename TPValue, std::enable_if_t<IsGdsSerializable<TPValue>::sValue, int>>
		gds_size_t evalByteSize( const TPValue & pValue );

		template <typename TPValue>
		gds_size_t evalByteSize( const TPValue & pValue );

		template <typename TRef, typename TInternal>
		gds_size_t evalByteSize( const GDS::ValueRef<TRef, TInternal> & pValueRef );

		template <typename TPValue, typename TInternal, std::enable_if_t<std::is_void<TInternal>::value, int>>
		gds_size_t evalByteSize( const TPValue & pValue, const GDS::TypeCastTag<TInternal> & );

		template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
		gds_size_t evalByteSize( const TPValue & pValue, const GDS::TypeCastTag<TInternal> & );

		template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
		gds_size_t evalByteSize( const GDS::TypeCastInfo<TPValue, TInternal> & pCastInfo );

		/***********************************************************************************************************/
		/***************************************** Core API - serialize() ******************************************/
		/***********************************************************************************************************/

		template <typename TPValue, std::enable_if_t<IsGdsSerializable<TPValue>::sValue, int>>
		gds_size_t serialize( byte * pOutputBuffer, const TPValue & pValue );

		template <typename TPValue>
		gds_size_t serialize( byte * pOutputBuffer, const TPValue & pValue );

		template <typename TRef, typename TInternal>
		gds_size_t serialize( byte * pOutputBuffer, const GDS::ValueRef<TRef, TInternal> & pValueRef );

		template <typename TPValue, typename TInternal, std::enable_if_t<std::is_void<TInternal>::value, int>>
		gds_size_t serialize( byte * pOutputBuffer, const TPValue & pValue, const GDS::TypeCastTag<TInternal> & );

		template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
		gds_size_t serialize( byte * pOutputBuffer, const TPValue & pValue, const GDS::TypeCastTag<TInternal> & );

		template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
		gds_size_t serialize( byte * pOutputBuffer, const GDS::TypeCastInfo<TPValue, TInternal> & pCastInfo );

		/***********************************************************************************************************/
		/**************************************** Core API - deserialize() *****************************************/
		/***********************************************************************************************************/

		template <typename TPValue, std::enable_if_t<IsGdsSerializable<TPValue>::sValue, int>>
		gds_size_t deserialize( const byte * pInputData, TPValue & pValue );

		template <typename TPValue>
		gds_size_t deserialize( const byte * pInputData, TPValue & pValue );

		template <typename TRef, typename TInternal>
		gds_size_t deserialize( const byte * pInputData, const GDS::ValueRef<TRef, TInternal> & pValueRef );

		template <typename TPValue, typename TInternal, std::enable_if_t<std::is_void<TInternal>::value, int>>
		gds_size_t deserialize( const byte * pInputData, TPValue & pRef, const GDS::TypeCastTag<TInternal> & );

		template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
		gds_size_t deserialize( const byte * pInputData, TPValue & pRef, const GDS::TypeCastTag<TInternal> & );

		template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
		gds_size_t deserialize( const byte * pInputData, const GDS::TypeCastInfo<TPValue, TInternal> & pCastInfo );

		/***********************************************************************************************************/
		/******************************************** Core API - MetaData ******************************************/
		/***********************************************************************************************************/

		/// @brief Returns an evaluated byte size of the specified object which will be serialized using metadata.
		template <typename TPValue>
		gds_size_t evalByteSizeWithMetaData( const TPValue & pValue );

		/// @brief Serializes given object with additional metadata.
		template <typename TPValue>
		gds_size_t serializeWithMetaData( byte * pOutputBuffer, const TPValue & pValue );

		/// @brief Performs deserialization of the specified data, written with serializeWithMetaData().
		template <typename TPValue>
		gds_size_t deserializeWithMetaData( const byte * pInputData, TPValue & pValue );

		/***********************************************************************************************************/
		/******************************************** Core API - Variadic ******************************************/
		/***********************************************************************************************************/

		template <typename TPValue>
		gds_size_t evalByteSizeAll( const TPValue & pValue );

		template <typename TPValue, typename... TRest>
		gds_size_t evalByteSizeAll( const TPValue & pValue, TRest && ...pRest );

		template <typename TPValue>
		gds_size_t serializeAll( byte * pOutputBuffer, const TPValue & pValue );

		template <typename TPValue, typename... TRest>
		gds_size_t serializeAll( byte * pOutputBuffer, const TPValue & pValue, TRest && ...pRest );

		template <typename TPValue>
		gds_size_t deserializeAll( const byte * pInputData, TPValue & pValue );

		template <typename TPValue, typename... TRest>
		gds_size_t deserializeAll( const byte * pInputData, TPValue & pValue, TRest && ...pRest );

		/***********************************************************************************************************/
		/********************************************** Core API - Auto ********************************************/
		/***********************************************************************************************************/

		template <typename TPValue>
		gds_size_t serializeAuto( const read_write_memory_view & pOutputBuffer, const TPValue & pValue );

		template <typename TPValue, size_t tpSize>
		gds_size_t serializeAuto( std::array<byte, tpSize> & pOutputBuffer, const TPValue & pValue );

		template <typename TPValue>
		gds_size_t serializeAutoWithMetaData( const read_write_memory_view & pOutputBuffer, const TPValue & pValue );

		template <typename TPValue, size_t tpSize>
		gds_size_t serializeAutoWithMetaData( std::array<byte, tpSize> & pOutputBuffer, const TPValue & pValue );

		template <typename TPValue>
		gds_size_t serializeAuto( Dynamicbyte_array & pOutputBuffer, const TPValue & pValue );

		template <typename TPValue>
		gds_size_t serializeAuto( dynamic_memory_buffer & pOutputBuffer, const TPValue & pValue );

		template <typename TPValue>
		gds_size_t serializeAuto( std::vector<byte> & pOutputBuffer, const TPValue & pValue );

		template <typename TPValue>
		gds_size_t serializeAutoWithMetaData( Dynamicbyte_array & pOutputBuffer, const TPValue & pValue );

		template <typename TPValue>
		gds_size_t serializeAutoWithMetaData( dynamic_memory_buffer & pOutputBuffer, const TPValue & pValue );

		template <typename TPValue>
		gds_size_t serializeAutoWithMetaData( std::vector<byte> & pOutputBuffer, const TPValue & pValue );

		template <typename TBuffer, typename TPValue>
		gds_size_t deserializeAuto( const TBuffer & pInputData, TPValue & pValue );

		template <typename TBuffer, typename TPValue>
		gds_size_t deserializeAutoWithMetaData( const TBuffer & pInputData, TPValue & pValue );

		/***********************************************************************************************************/
		/******************************************** Core API - External ******************************************/
		/***********************************************************************************************************/

		// External serialization/deserialization support.
		//
		// This functionality is probably the most commonly used in the real-life scenarios (including some engine parts).
		// Most of the time, serialized data ends up in some fom of an external storage (a file, a network, etc).
		// For big portions of data, loading everything into the memory for deserialization (or serializing everything
		// into a single buffer) may not be an option. Loading chunks creates a problem, though - we can end up having
		// half of the object's data in the buffer (we cannot know how much data to load).
		//
		// MetaData solves this problem by adding fixed-size information before the data itself. External API further
		// enables serialization/deserialization directly to/from an external storage.
		// Each of these functions accepts a write (serialize()) or a read (deserialize()) callback for interaction
		// with an external medium.

		// Serialization is not really a problem. We simply serialize the specified object into a temporary
		// buffer (optionally specified by the user) and write the data to the output using the write callback.

		/// @brief Serializes the specified object and writes the byte representation using the specified callback.
		/// @param pValue The object to be serialized
		/// @param pWriteCallback The write callback for data writing. Cannot be empty.
		/// @param pGdsCache Custom cache to which the data is serialized before writing.
		template <typename TBuffer, typename TPValue>
		gds_size_t serializeExternal( const TPValue & pValue, const DataWriteCallback & pWriteCallback, TBuffer & pGdsCache );

		/// @brief Serializes the specified object and writes the byte representation using the specified callback.
		/// @param pValue The object to be serialized
		/// @param pWriteCallback The write callback for data writing. Cannot be empty.
		template <typename TPValue>
		gds_size_t serializeExternal( const TPValue & pValue, const DataWriteCallback & pWriteCallback );

		// Deserialization is more problematic, because the data is usually dynamically-sized (containers, strings, ranges).
		// For the best flexibility, we assume, that the data source is a one-way stream (we can read bytes in order
		// and cannot rewind back). This assumption allows us to support all kind of streams directly.
		//
		// To achieve this, we use the MetaData feature (this is why serializeExternal() always does the process using
		// serializeAutoWithMetaData() internally) and obtain the info about how much data we need to fetch from the stream.
		//
		// The data must be obviously loaded into some kind of buffer first, so just like in case of serializeAuto(),
		// we use proxy class to enable usage of different types of buffers as this temporary storage.

		/// @brief Deserializes object of the specified type and stores it in pValue. Binary data is read using the specified read callback.
		/// @param pValue The object to store the deserialized state.
		/// @param pReadCallback The read callback for data reading. Cannot be empty.
		/// @param pGdsCache Custom cache that will be used as a read buffer.
		template <typename TPValue>
		gds_size_t deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, const read_write_memory_view & pGdsCache );

		template <typename TPValue, size_t tpSize>
		gds_size_t deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, std::array<byte, tpSize> & pGdsCache );

		template <typename TPValue>
		gds_size_t deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, Dynamicbyte_array & pGdsCache );

		template <typename TPValue>
		gds_size_t deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, dynamic_memory_buffer & pGdsCache );

		template <typename TPValue>
		gds_size_t deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, std::vector<byte> & pGdsCache );

		/// @brief Deserializes object of the specified type and stores it in pValue. Binary data is read using the specified read callback.
		/// @param pValue The object to store the deserialized state.
		/// @param pReadCallback The read callback for data reading. Cannot be empty.
		template <typename TPValue>
		gds_size_t deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback );

		/***********************************************************************************************************/
		/******************************************* Container/Map Helpers *****************************************/
		/***********************************************************************************************************/

		struct StdListInsert
		{
			template <typename L, typename E>
			void operator()( L & pList, size_t /* pIndex */, E pElement ) const
			{
				pList.push_back( std::forward<E>( pElement ) );
			}
		};

		struct StdDequeInsert
		{
			template <typename D, typename E>
			void operator()( D & pDeque, size_t /* pIndex */, E pElement ) const
			{
				pDeque.push_back( std::forward<E>( pElement ) );
			}
		};

		struct StdQueueInsert
		{
			template <typename Q, typename E>
			void operator()( Q & pQueue, size_t /* pIndex */, E pElement ) const
			{
				pQueue.push( std::forward<E>( pElement ) );
			}
		};

		struct StdStackInsert
		{
			template <typename S, typename E>
			void operator()( S & pStack, size_t /* pIndex */, E pElement ) const
			{
				pStack.push( std::forward<E>( pElement ) );
			}
		};

		struct StdSetInsert
		{
			template <typename S, typename E>
			void operator()( S & pSet, size_t /* pIndex */, E pElement ) const
			{
				pSet.insert( std::forward<E>( pElement ) );
			}
		};

		struct StdUnorderedSetInsert
		{
			template <typename UOS, typename E>
			void operator()( UOS & pUnorderedSet, size_t /* pIndex */, E pElement ) const
			{
				pUnorderedSet.insert( std::forward<E>( pElement ) );
			}
		};

		template <template <typename...> typename TC, typename... TCArgs>
		gds_size_t evalByteSizeContainer( const TC<TCArgs...> & pContainer );

		template <template <typename...> typename TC, typename... TCArgs>
		gds_size_t serializeContainer( byte * pOutputBuffer, const TC<TCArgs...> & pContainer );

		template <template <typename...> typename TC, typename... TCArgs, typename TIns>
		gds_size_t deserializeContainer( const byte * pInputData, TC<TCArgs...> & pContainer, const TIns & pInsertCallback );

		template <template <typename...> typename TC, typename... TCArgs>
		gds_size_t evalMapByteSize( const TC<TCArgs...> & pMap );

		template <template <typename...> typename TM, typename TK, typename TV, typename... TCArgs>
		gds_size_t serializeMap( byte * pOutputBuffer, const TM<TK, TV, TCArgs...> & pMap );

		template <template <typename...> typename TM, typename TK, typename TV, typename... TCArgs>
		gds_size_t deserializeMap( const byte * pInputData, TM<TK, TV, TCArgs...> & pMap );

		/***********************************************************************************************************/
		/************************************************* ArrayView ***********************************************/
		/***********************************************************************************************************/

		template <typename TPValue>
		gds_size_t evalByteSize( const cppx::array_view<TPValue> & pArrayView );

		template <typename TPValue>
		gds_size_t serialize( byte * pOutputBuffer, const cppx::array_view<TPValue> & pArrayView );

		template <typename TPValue>
		gds_size_t deserialize( const byte * pInputData, cppx::array_view<TPValue> & pArrayView );

		/***********************************************************************************************************/
		/*********************************************** AtomicBitmask *********************************************/
		/***********************************************************************************************************/

		template <typename TPIntegral>
		gds_size_t evalByteSize( const AtomicBitmask<TPIntegral> & pBitmask );

		template <typename TPIntegral>
		gds_size_t serialize( byte * pOutputBuffer, const AtomicBitmask<TPIntegral> & pBitmask );

		template <typename TPIntegral>
		gds_size_t deserialize( const byte * pInputData, AtomicBitmask<TPIntegral> & pBitmask );

		/***********************************************************************************************************/
		/************************************************** Bitmask ************************************************/
		/***********************************************************************************************************/

		template <typename TPIntegral>
		gds_size_t evalByteSize( const cppx::bitmask<TPIntegral> & pBitmask );

		template <typename TPIntegral>
		gds_size_t serialize( byte * pOutputBuffer, const cppx::bitmask<TPIntegral> & pBitmask );

		template <typename TPIntegral>
		gds_size_t deserialize( const byte * pInputData, cppx::bitmask<TPIntegral> & pBitmask );

		/***********************************************************************************************************/
		/************************************************ SortedArray **********************************************/
		/***********************************************************************************************************/

		template <typename TPValue, typename TPComparator, typename TPAllocator>
		gds_size_t evalByteSize( const SortedArray<TPValue, TPComparator, TPAllocator> & pSortedArray );

		template <typename TPValue, typename TPComparator, typename TPAllocator>
		gds_size_t serialize( byte * pOutputBuffer, const SortedArray<TPValue, TPComparator, TPAllocator> & pSortedArray );

		template <typename TPValue, typename TPComparator, typename TPAllocator>
		gds_size_t deserialize( const byte * pInputData, const SortedArray<TPValue, TPComparator, TPAllocator> & pSortedArray );

		/***********************************************************************************************************/
		/******************************************** std::basic_string ********************************************/
		/***********************************************************************************************************/

		template <typename TCh, typename TTr, typename TPAllocator>
		gds_size_t evalByteSize( const std::basic_string<TCh, TTr, TPAllocator> & pString );
		
		template <typename TCh, typename TTr, typename TPAllocator>
		gds_size_t serialize( byte * pOutputBuffer, const std::basic_string<TCh, TTr, TPAllocator> & pString );
		
		template <typename TCh, typename TTr, typename TPAllocator>
		gds_size_t deserialize( const byte * pInputData, std::basic_string<TCh, TTr, TPAllocator> & pString );

		/***********************************************************************************************************/
		/************************************************ std::pair ************************************************/
		/***********************************************************************************************************/
		
		template <typename T1, typename T2>
		gds_size_t evalByteSize( const std::pair<T1, T2> & pPair );

		template <typename T1, typename T2>
		gds_size_t serialize( byte * pOutputBuffer, const std::pair<T1, T2> & pPair );

		template <typename T1, typename T2>
		gds_size_t deserialize( const byte * pInputData, std::pair<T1, T2> & pPair );

		/***********************************************************************************************************/
		/************************************************ std::array ***********************************************/
		/***********************************************************************************************************/

		template <typename TPValue, size_t tpSize>
		gds_size_t evalByteSize( const std::array<TPValue, tpSize> & pArray );

		template <typename TPValue, size_t tpSize>
		gds_size_t serialize( byte * pOutputBuffer, const std::array<TPValue, tpSize> & pArray );

		template <typename TPValue, size_t tpSize>
		gds_size_t deserialize( const byte * pInputData, std::array<TPValue, tpSize> & pArray );

		/***********************************************************************************************************/
		/*********************************************** std::vector ***********************************************/
		/***********************************************************************************************************/

		template <typename TPValue, typename TPAllocator>
		gds_size_t evalByteSize( const std::vector<TPValue, TPAllocator> & pVector );

		template <typename TPValue, typename TPAllocator>
		gds_size_t serialize( byte * pOutputBuffer, const std::vector<TPValue, TPAllocator> & pVector );

		template <typename TPValue, typename TPAllocator>
		gds_size_t deserialize( const byte * pInputData, std::vector<TPValue, TPAllocator> & pVector );

		namespace internal
		{

			template <typename TFxBuffer, typename TPValue>
			gds_size_t serializeAutoFixed( TFxBuffer & pOutputBuffer, const TPValue & pValue );

			template <typename TFxBuffer, typename TPValue>
			gds_size_t serializeAutoWithMetaDataFixed( TFxBuffer & pOutputBuffer, const TPValue & pValue );

			template <typename TRsBuffer, typename TPValue>
			gds_size_t serializeAutoResizable( TRsBuffer & pOutputBuffer, const TPValue & pValue );

			template <typename TRsBuffer, typename TPValue>
			gds_size_t serializeAutoWithMetaDataResizable( TRsBuffer & pOutputBuffer, const TPValue & pValue );

			template <typename TPValue, typename TFxBuffer>
			gds_size_t deserializeExternalFixed( TPValue & pValue, const DataReadCallback & pReadCallback, TFxBuffer & pGdsCache );

			template <typename TPValue, typename TRsBuffer>
			gds_size_t deserializeExternalResizable( TPValue & pValue, const DataReadCallback & pReadCallback, TRsBuffer & pGdsCache );

			template <typename TFxBuffer>
			bool readDataExternalFixed( const DataReadCallback & pReadCallback, TFxBuffer & pReadBuffer );

			template <typename TRsBuffer>
			bool readDataExternalResizable( const DataReadCallback & pReadCallback, TRsBuffer & pReadBuffer );

		}

	} // namespace GDSCore

	/***********************************************************************************************************/
	/**************************************** Core API - evalByteSize() ****************************************/
	/***********************************************************************************************************/

	template <typename TPValue, std::enable_if_t<IsGdsSerializable<TPValue>::sValue, int>>
	inline gds_size_t GDSCore::evalByteSize( const TPValue & pValue )
	{
		return pValue.evalByteSize();
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::evalByteSize( const TPValue & pValue )
	{
		return GDS::evalByteSize( pValue );
	}

	template <typename TRef, typename TInternal>
	inline gds_size_t GDSCore::evalByteSize( const GDS::ValueRef<TRef, TInternal> & pValueRef )
	{
		return evalByteSize( pValueRef.get() );
	}

	template <typename TPValue, typename TInternal, std::enable_if_t<std::is_void<TInternal>::value, int>>
	inline gds_size_t GDSCore::evalByteSize( const TPValue & pValue, const GDS::TypeCastTag<TInternal> & )
	{
		return evalByteSize( pValue );
	}

	template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
	inline gds_size_t GDSCore::evalByteSize( const TPValue & pValue, const GDS::TypeCastTag<TInternal> & )
	{
		return evalByteSize( GDS::ValueRef<const TPValue, TInternal>{ pValue } );
	}

	template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
	inline gds_size_t GDSCore::evalByteSize( const GDS::TypeCastInfo<TPValue, TInternal> & pCastInfo )
	{
		return evalByteSize( pCastInfo.refWrapper.get(), pCastInfo.castTag );
	}

	/***********************************************************************************************************/
	/***************************************** Core API - serialize() ******************************************/
	/***********************************************************************************************************/

	template <typename TPValue, std::enable_if_t<IsGdsSerializable<TPValue>::sValue, int>>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const TPValue & pValue )
	{
		return pValue.serialize( pOutputBuffer );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const TPValue & pValue )
	{
		return GDS::serialize( pOutputBuffer, pValue );
	}

	template <typename TRef, typename TInternal>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const GDS::ValueRef<TRef, TInternal> & pValueRef )
	{
		return serialize( pOutputBuffer, pValueRef.get() );
	}

	template <typename TPValue, typename TInternal, std::enable_if_t<std::is_void<TInternal>::value, int>>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const TPValue & pValue, const GDS::TypeCastTag<TInternal> & )
	{
		return serialize( pOutputBuffer, pValue );
	}

	template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const TPValue & pValue, const GDS::TypeCastTag<TInternal> & )
	{
		return serialize( pOutputBuffer, GDS::ValueRef<TPValue, TInternal>{ pValue } );
	}

	template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const GDS::TypeCastInfo<TPValue, TInternal> & pCastInfo )
	{
		return serialize( pOutputBuffer, pCastInfo.refWrapper.get(), pCastInfo.castTag );
	}

	/***********************************************************************************************************/
	/**************************************** Core API - deserialize() *****************************************/
	/***********************************************************************************************************/

	template <typename TPValue, std::enable_if_t<IsGdsSerializable<TPValue>::sValue, int>>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, TPValue & pValue )
	{
		return pValue.deserialize( pInputData );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, TPValue & pValue )
	{
		return GDS::deserialize( pInputData, pValue );
	}

	template <typename TRef, typename TInternal>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, const GDS::ValueRef<TRef, TInternal> & pValueRef )
	{
		return deserialize( pInputData, pValueRef.get() );
	}

	template <typename TPValue, typename TInternal, std::enable_if_t<std::is_void<TInternal>::value, int>>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, TPValue & pRef, const GDS::TypeCastTag<TInternal> & )
	{
		return deserialize( pInputData, pRef );
	}

	template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, TPValue & pRef, const GDS::TypeCastTag<TInternal> & )
	{
		return deserialize( pInputData, GDS::ValueRef<TPValue, TInternal>{ pRef } );
	}

	template <typename TPValue, typename TInternal, std::enable_if_t<!std::is_void<TInternal>::value, int>>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, const GDS::TypeCastInfo<TPValue, TInternal> & pCastInfo )
	{
		return deserialize( pInputData, pCastInfo.refWrapper.get(), pCastInfo.castTag );
	}

	/***********************************************************************************************************/
	/******************************************** Core API - MetaData ******************************************/
	/***********************************************************************************************************/

	/// @brief Returns an evaluated byte size of the specified object which will be serialized using metadata.
	template <typename TPValue>
	inline gds_size_t GDSCore::evalByteSizeWithMetaData( const TPValue & pValue )
	{
		const auto metaDataSize = GDS::getInstanceMetaDataSize();
		const auto valueByteSize = evalByteSize( pValue );

		// Technically, user-defined evalByteSize() can be defined to return 0.
		// We always skip empty entries, so in case of metadata let's do the same.
		// If the object itself has no data, stick to that and report 0 size.
		return ( valueByteSize > 0 ) ? ( metaDataSize + valueByteSize ) : 0u;
	}

	/// @brief Serializes given object with additional metadata.
	template <typename TPValue>
	inline gds_size_t GDSCore::serializeWithMetaData( byte * pOutputBuffer, const TPValue & pValue )
	{
		// Size of the metadata, in bytes.
		const auto metaDataSize = GDS::getInstanceMetaDataSize();

		// The actual serialization. Skip 'metaDataSize' bytes needed for metadata
		// and write after that. serialize() returns the number of bytes written.
		const auto valueByteSize = serialize( pOutputBuffer + metaDataSize, pValue );

		// MetaData for this instance.
		GDS::InstanceMetaData metaData;
		metaData.controlKey = GDS::CX_GDS_VALUE_META_DATA_CONTROL_KEY;
		metaData.objectDataSize = valueByteSize; // Size of the serialized data of the OBJECT only.
		metaData.outputBlockSize = metaDataSize + valueByteSize; // Size of the whole data block (including MetaData).

		// Now, serialize the metadata itself.
		GDS::serialize( pOutputBuffer, metaData );

		return metaData.outputBlockSize;
	}

	/// @brief Performs deserialization of the specified data, written with serializeWithMetaData().
	template <typename TPValue>
	inline gds_size_t GDSCore::deserializeWithMetaData( const byte * pInputData, TPValue & pValue )
	{
		// The metadata object to which we will deserialize.
		GDS::InstanceMetaData metaData;

		// Deserialize the metadata first. It is always at the beginning of the data block.
		const auto metaDataSize = GDS::deserialize( pInputData, metaData );

		// deserialize() returns the number of bytes read from the buffer.
		// This should match the byte size of the InstanceMetaData struct.
		Ic3DebugAssert( metaDataSize == GDS::getInstanceMetaDataSize() );

		const auto valueByteSize = deserialize( pInputData + metaDataSize, pValue );

		return metaDataSize + valueByteSize;
	}

	/***********************************************************************************************************/
	/******************************************** Core API - Variadic ******************************************/
	/***********************************************************************************************************/

	template <typename TPValue>
	inline gds_size_t GDSCore::evalByteSizeAll( const TPValue & pValue )
	{
		return evalByteSize( pValue );
	}

	template <typename TPValue, typename... TRest>
	inline gds_size_t GDSCore::evalByteSizeAll( const TPValue & pValue, TRest && ...pRest )
	{
		gds_size_t byteSize = evalByteSize( pValue );
		byteSize += evalByteSizeAll( std::forward<TRest>( pRest )... );
		return byteSize;
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeAll( byte * pOutputBuffer, const TPValue & pValue )
	{
		return serialize( pOutputBuffer, pValue );
	}

	template <typename TPValue, typename... TRest>
	inline gds_size_t GDSCore::serializeAll( byte * pOutputBuffer, const TPValue & pValue, TRest && ...pRest )
	{
		gds_size_t byteSize = serialize( pOutputBuffer, pValue );
		byteSize += serializeAll( pOutputBuffer + byteSize, std::forward<TRest>( pRest )... );
		return byteSize;
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::deserializeAll( const byte * pInputData, TPValue & pValue )
	{
		return deserialize( pInputData, pValue );
	}

	template <typename TPValue, typename... TRest>
	inline gds_size_t GDSCore::deserializeAll( const byte * pInputData, TPValue & pValue, TRest && ...pRest )
	{
		gds_size_t byteSize = deserialize( pInputData, pValue );
		byteSize += deserializeAll( pInputData + byteSize, std::forward<TRest>( pRest )... );
		return byteSize;
	}

	/***********************************************************************************************************/
	/********************************************** Core API - Auto ********************************************/
	/***********************************************************************************************************/

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeAuto( const read_write_memory_view & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoFixed( pOutputBuffer, pValue );
	}

	template <typename TPValue, size_t tpSize>
	inline gds_size_t GDSCore::serializeAuto( std::array<byte, tpSize> & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoFixed( pOutputBuffer, pValue );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeAutoWithMetaData( const read_write_memory_view & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoWithMetaDataFixed( pOutputBuffer, pValue );
	}

	template <typename TPValue, size_t tpSize>
	inline gds_size_t GDSCore::serializeAutoWithMetaData( std::array<byte, tpSize> & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoWithMetaDataFixed( pOutputBuffer, pValue );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeAuto( Dynamicbyte_array & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoResizable( pOutputBuffer, pValue );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeAuto( dynamic_memory_buffer & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoResizable( pOutputBuffer, pValue );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeAuto( std::vector<byte> & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoResizable( pOutputBuffer, pValue );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeAutoWithMetaData( Dynamicbyte_array & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoWithMetaDataResizable( pOutputBuffer, pValue );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeAutoWithMetaData( dynamic_memory_buffer & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoWithMetaDataResizable( pOutputBuffer, pValue );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeAutoWithMetaData( std::vector<byte> & pOutputBuffer, const TPValue & pValue )
	{
		return internal::serializeAutoWithMetaDataResizable( pOutputBuffer, pValue );
	}

	template <typename TBuffer, typename TPValue>
	inline gds_size_t GDSCore::deserializeAuto( const TBuffer & pInputData, TPValue & pValue )
	{
		return deserialize( pInputData.data(), pValue );
	}

	template <typename TBuffer, typename TPValue>
	inline gds_size_t GDSCore::deserializeAutoWithMetaData( const TBuffer & pInputData, TPValue & pValue )
	{
		return deserializeWithMetaData( pInputData.data(), pValue );
	}

	/***********************************************************************************************************/
	/******************************************** Core API - External ******************************************/
	/***********************************************************************************************************/

	template <typename TBuffer, typename TPValue>
	inline gds_size_t GDSCore::serializeExternal( const TPValue & pValue, const DataWriteCallback & pWriteCallback, TBuffer & pGdsCache )
	{
		// Serialize the object into the cache. We can use our neat serializeAuto()
		// to properly handle different type of buffers here!
		const auto writeSize = serializeAutoWithMetaData( pGdsCache, pValue );

		if( writeSize > 0 )
		{
			if( !pWriteCallback )
			{
				// This cannot be an empty function.
				Ic3ThrowDesc( eExcCodeDebugPlaceholder, "Empty write callback." );
			}

			// Write the data using the callback.
			pWriteCallback( pGdsCache.data(), writeSize );
		}

		return writeSize;
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serializeExternal( const TPValue & pValue, const DataWriteCallback & pWriteCallback )
	{
		Dynamicbyte_array localGdsCache{};
		return serializeExternal( pValue, pWriteCallback, localGdsCache );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, const read_write_memory_view & pGdsCache )
	{
		return internal::deserializeExternalFixed( pValue, pReadCallback, pGdsCache );
	}

	template <typename TPValue, size_t tpSize>
	inline gds_size_t GDSCore::deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, std::array<byte, tpSize> & pGdsCache )
	{
		return internal::deserializeExternalFixed( pValue, pReadCallback, pGdsCache );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, Dynamicbyte_array & pGdsCache )
	{
		return internal::deserializeExternalResizable( pValue, pReadCallback, pGdsCache );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, dynamic_memory_buffer & pGdsCache )
	{
		return internal::deserializeExternalResizable( pValue, pReadCallback, pGdsCache );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback, std::vector<byte> & pGdsCache )
	{
		return internal::deserializeExternalResizable( pValue, pReadCallback, pGdsCache );
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::deserializeExternal( TPValue & pValue, const DataReadCallback & pReadCallback )
	{
		Dynamicbyte_array localGdsCache{};
		return deserializeExternal( pValue, pReadCallback, localGdsCache );
	}

	/***********************************************************************************************************/
	/******************************************* Container/Map Helpers *****************************************/
	/***********************************************************************************************************/

	template <template <typename...> typename TC, typename... TCArgs>
	inline gds_size_t GDSCore::evalByteSizeContainer( const TC<TCArgs...> & pContainer )
	{
		// Compute the size for the specified container.
		// The space required is the serialized size + binary representation of all elements in the container.
		gds_size_t byteSize = 0;

		// The first member is the number of elements in a container. We are not interested in its value, though.
		byteSize += GDS::evalByteSize( GDS::emptySizeType() );

		// Enumerate over the container's elements.
		for( const auto & entry : pContainer )
		{
			// Evaluate the size of each element and add it to the result.
			byteSize += GDSCore::evalByteSize( entry );
		}

		return byteSize;
	}

	template <template <typename...> typename TC, typename... TCArgs>
	inline gds_size_t GDSCore::serializeContainer( byte * pOutputBuffer, const TC<TCArgs...> & pContainer )
	{
		// First, get the size of the container (number of elements).
		const size_t containerSize = pContainer.size();

		// size_t may have different sizes on different platforms. asSizeTypeValue() ensures this is properly
		// handled by the GDS, so we can deserialize the data on a different platform, and get expected results.
		gds_size_t byteSize = GDS::serialize( pOutputBuffer, GDS::asSizeType( containerSize ) );

		// Enumerate over the container's elements.
		for( const auto & entry : pContainer )
		{
			// Serialize the element, add the size to the total byte size. Propagate cast tag.
			byteSize += GDSCore::serialize( pOutputBuffer + byteSize, entry );
		}

		return byteSize;
	}

	template <template <typename...> typename TC, typename... TCArgs, typename TIns>
	inline gds_size_t GDSCore::deserializeContainer( const byte * pInputData, TC<TCArgs...> & pContainer, const TIns & pInsertCallback )
	{
		// The variable to store the container's size.
		size_t containerSize = 0;

		// Deserialize the size. Since we used asSizeTypeValue() during serialization, we must now use asSizeTypeRef()
		// to properly retrieve the deserialized value. The output will be properly stored in the wrapped variable.
		gds_size_t byteSize = GDS::deserialize( pInputData, GDS::asSizeType( containerSize ) );

		for( size_t entryIndex = 0; entryIndex < containerSize; ++entryIndex )
		{
			// Temporary for the deserialized state.
			typename TC<TCArgs...>::value_type tempEntry{};

			// Deserialize current element. Propagate cast tag to support container-wise value casting.
			byteSize += GDSCore::deserialize( pInputData + byteSize, tempEntry );

			// Call the insert callback to add it to the container. Index is needed e.g. for std::array.
			pInsertCallback( pContainer, entryIndex, std::move( tempEntry ) );
		}

		return byteSize;
	}

	template <template <typename...> typename TC, typename... TCArgs>
	inline gds_size_t GDSCore::evalMapByteSize( const TC<TCArgs...> & pMap )
	{
		gds_size_t byteSize = 0;

		// The first member is the number of elements in a container. We are not interested in its value, though.
		byteSize += GDS::evalByteSize( GDS::emptySizeType() );

		// Enumerate over the container's elements.
		for( const auto & mapEntry : pMap )
		{
			// Evaluate the size of each element. Propagate cast tag to support container-wise value casting.
			byteSize += GDSCore::evalByteSize( mapEntry.first );
			byteSize += GDSCore::evalByteSize( mapEntry.second );
		}

		return byteSize;
	}

	template <template <typename...> typename TM, typename TK, typename TV, typename... TCArgs>
	inline gds_size_t GDSCore::serializeMap( byte * pOutputBuffer, const TM<TK, TV, TCArgs...> & pMap )
	{
		{
			const size_t mapSize = pMap.size();

			gds_size_t byteSize = GDS::serialize( pOutputBuffer, GDS::asSizeType( mapSize ) );

			for( const auto & mapEntry : pMap )
			{
				byteSize += GDSCore::serialize( pOutputBuffer + byteSize, mapEntry.first );
				byteSize += GDSCore::serialize( pOutputBuffer + byteSize, mapEntry.second );
			}

			return byteSize;
		}
	}

	template <template <typename...> typename TM, typename TK, typename TV, typename... TCArgs>
	inline gds_size_t GDSCore::deserializeMap( const byte * pInputData, TM<TK, TV, TCArgs...> & pMap )
	{
		size_t mapSize = 0;

		gds_size_t byteSize = GDS::deserialize( pInputData, GDS::asSizeType( mapSize ) );

		for( size_t entryIndex = 0; entryIndex < mapSize; ++entryIndex )
		{
			TK tempKey{};
			byteSize += GDSCore::deserialize( pInputData + byteSize, tempKey );

			TV tempValue{};
			byteSize += GDSCore::deserialize( pInputData + byteSize, tempValue );

			pMap.emplace( std::move( tempKey ), std::move( tempValue ) );
		}

		return byteSize;
	}

	/***********************************************************************************************************/
	/************************************************* ArrayView ***********************************************/
	/***********************************************************************************************************/

	template <typename TPValue>
	inline gds_size_t GDSCore::evalByteSize( const cppx::array_view<TPValue> & pArrayView )
	{
		gds_size_t byteSize = GDS::evalByteSize( GDS::emptySizeType() );
		for( const auto & entry : pArrayView )
		{
			byteSize += GDSCore::evalByteSize( entry );
		}
		return byteSize;
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const cppx::array_view<TPValue> & pArrayView )
	{
		gds_size_t byteSize = GDS::serialize( pOutputBuffer, GDS::asSizeType( pArrayView.size() ) );
		for( const auto & value : pArrayView )
		{
			byteSize += GDSCore::serialize(  pOutputBuffer + byteSize, value );
		}
		return byteSize;
	}

	template <typename TPValue>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, cppx::array_view<TPValue> & pArrayView )
	{
		size_t arraySize = 0;
		gds_size_t byteSize = GDS::deserialize( pInputData, GDS::asSizeType( arraySize ) );
		for( auto & value : pArrayView )
		{
			byteSize += GDSCore::deserialize( pInputData + byteSize, value );
		}
		return byteSize;
	}

	/***********************************************************************************************************/
	/*********************************************** AtomicBitmask *********************************************/
	/***********************************************************************************************************/

	template <typename TPIntegral>
	inline gds_size_t GDSCore::evalByteSize( const AtomicBitmask<TPIntegral> & pBitmask )
	{
		return GDS::evalByteSize( static_cast<TPIntegral>( pBitmask ) );
	}

	template <typename TPIntegral>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const AtomicBitmask<TPIntegral> & pBitmask )
	{
		return GDS::serialize( pOutputBuffer, static_cast<TPIntegral>( pBitmask ) );
	}

	template <typename TPIntegral>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, AtomicBitmask<TPIntegral> & pBitmask )
	{
		return deserialize( pInputData, static_cast<TPIntegral>( pBitmask ) );
	}

	/***********************************************************************************************************/
	/************************************************** Bitmask ************************************************/
	/***********************************************************************************************************/

	template <typename TPIntegral>
	inline gds_size_t GDSCore::evalByteSize( const cppx::bitmask<TPIntegral> & pBitmask )
	{
		return evalByteSize( static_cast<TPIntegral>( pBitmask ) );
	}

	template <typename TPIntegral>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const cppx::bitmask<TPIntegral> & pBitmask )
	{
		return serialize( pOutputBuffer, static_cast<TPIntegral>( pBitmask ) );
	}

	template <typename TPIntegral>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, cppx::bitmask<TPIntegral> & pBitmask )
	{
		return deserialize( pInputData, static_cast<TPIntegral>( pBitmask ) );
	}

	/***********************************************************************************************************/
	/************************************************ SortedArray **********************************************/
	/***********************************************************************************************************/

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline gds_size_t GDSCore::evalByteSize( const SortedArray<TPValue, TPComparator, TPAllocator> & pSortedArray )
	{
		return evalByteSizeContainer( pSortedArray );
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const SortedArray<TPValue, TPComparator, TPAllocator> & pSortedArray )
	{
		return serializeContainer( pOutputBuffer, pSortedArray );
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, const SortedArray<TPValue, TPComparator, TPAllocator> & pSortedArray )
	{
		return deserializeContainer( pInputData, pSortedArray,
			[]( auto & SA, auto, auto E ) -> void { SA.insert( std::forward<decltype( E )>( E ) ); } );
	}

	/***********************************************************************************************************/
	/******************************************** std::basic_string ********************************************/
	/***********************************************************************************************************/

	template <typename TCh, typename TTr, typename TPAllocator>
	inline gds_size_t GDSCore::evalByteSize( const std::basic_string<TCh, TTr, TPAllocator> & pString )
	{
		gds_size_t byteSize = 0;

		// The first member is the length of the specified string.
		byteSize += GDS::evalByteSize( GDS::emptySizeType() );

		// Then we have the characters (potentially cast).
		byteSize += pString.length() * evalByteSize( TCh{} );

		return byteSize;
	}

	template <typename TCh, typename TTr, typename TPAllocator>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const std::basic_string<TCh, TTr, TPAllocator> & pString )
	{
		const auto strLength = pString.length();

		gds_size_t byteSize = GDS::serialize( pOutputBuffer, GDS::asSizeType( strLength ) );

		for( size_t charIndex = 0; charIndex < strLength; ++charIndex )
		{
			byteSize += serialize( pOutputBuffer + byteSize, pString[charIndex] );
		}

		return byteSize;
	}

	template <typename TCh, typename TTr, typename TPAllocator>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, std::basic_string<TCh, TTr, TPAllocator> & pString )
	{
		size_t strLength = 0;
		gds_size_t byteSize = GDS::deserialize( pInputData, GDS::asSizeType( strLength ) );

		pString.resize( strLength );

		for( size_t charIndex = 0; charIndex < strLength; ++charIndex )
		{
			byteSize += deserialize( pInputData + byteSize, pString[charIndex] );
		}

		return byteSize;
	}

	/***********************************************************************************************************/
	/************************************************ std::pair ************************************************/
	/***********************************************************************************************************/

	template <typename T1, typename T2>
	inline gds_size_t GDSCore::evalByteSize( const std::pair<T1, T2> & pPair )
	{
		gds_size_t byteSize = 0;
		byteSize += evalByteSize( pPair.first );
		byteSize += evalByteSize( pPair.second );
		return byteSize;
	}

	template <typename T1, typename T2>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const std::pair<T1, T2> & pPair )
	{
		gds_size_t byteSize = 0;
		byteSize += serialize( pOutputBuffer, pPair.first );
		byteSize += serialize( pOutputBuffer, pPair.second );
		return byteSize;
	}

	template <typename T1, typename T2>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, std::pair<T1, T2> & pPair )
	{
		gds_size_t byteSize = 0;
		byteSize += deserialize( pInputData, pPair.first );
		byteSize += deserialize( pInputData, pPair.second );
		return byteSize;
	}

	/***********************************************************************************************************/
	/************************************************ std::array ***********************************************/
	/***********************************************************************************************************/

	template <typename TPValue, size_t tpSize>
	inline gds_size_t GDSCore::evalByteSize( const std::array<TPValue, tpSize> & pArray )
	{
		return evalByteSizeContainer( pArray );
	}

	template <typename TPValue, size_t tpSize>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const std::array<TPValue, tpSize> & pArray )
	{
		return serializeContainer( pOutputBuffer, pArray );
	}

	template <typename TPValue, size_t tpSize>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, std::array<TPValue, tpSize> & pArray )
	{
		// std::array doesn't have any push/insert methods, so that's the one case where index becomes useful.
		// We simply assign the incoming elements under their indexes passed by the helper method. Simple.
		return deserializeContainer( pInputData, pArray,
			[]( auto & A, auto I, auto && E ) -> void { A[I] = std::forward<decltype( E )>( E ); } );
	}

	/***********************************************************************************************************/
	/*********************************************** std::vector ***********************************************/
	/***********************************************************************************************************/

	template <typename TPValue, typename TPAllocator>
	inline gds_size_t GDSCore::evalByteSize( const std::vector<TPValue, TPAllocator> & pVector )
	{
		return evalByteSizeContainer( pVector );
	}

	template <typename TPValue, typename TPAllocator>
	inline gds_size_t GDSCore::serialize( byte * pOutputBuffer, const std::vector<TPValue, TPAllocator> & pVector )
	{
		return serializeContainer( pOutputBuffer, pVector );
	}

	template <typename TPValue, typename TPAllocator>
	inline gds_size_t GDSCore::deserialize( const byte * pInputData, std::vector<TPValue, TPAllocator> & pVector )
	{
		return deserializeContainer( pInputData, pVector,
			[]( auto & V, auto, auto && E ) -> void { V.push_back( std::forward<decltype( E )>( E ) ); } );
	}

	//
	//
	//
	//
	//

	template <typename TFxBuffer, typename TPValue>
	inline gds_size_t GDSCore::internal::serializeAutoFixed( TFxBuffer & pOutputBuffer, const TPValue & pValue )
	{
		// Get the size of the serialized object (i.e. the minimum required size of the target buffer).
		const auto totalByteSize = evalByteSize( pValue );

		if( totalByteSize > 0 )
		{
			// The buffer is a fixed one - check if its size is large enough to store the whole data.
			if( totalByteSize > pOutputBuffer.size() )
			{
				// Buffer is too small, throw an error, nothing can be done here.
				Ic3ThrowDesc( eExcCodeDebugPlaceholder, "Fixed buffer is to small for the output data." );
			}

			// Buffer is large enough, so we can use it as the target for the output.
			// Serialize the data into the buffer. This is the call to the actual, value-specific serialize().
			serialize( pOutputBuffer.data(), pValue );
		}

		return totalByteSize;
	}

	template <typename TFxBuffer, typename TPValue>
	inline gds_size_t GDSCore::internal::serializeAutoWithMetaDataFixed( TFxBuffer & pOutputBuffer, const TPValue & pValue )
	{
		// Same thing as above, but we also include the MetaData block alongside the object itself.

		const auto totalByteSize = evalByteSizeWithMetaData( pValue );

		if( totalByteSize > 0 )
		{
			if( totalByteSize > pOutputBuffer.size() )
			{
				Ic3ThrowDesc( eExcCodeDebugPlaceholder, "Fixed buffer is to small for the output data." );
			}
			serializeWithMetaData( pOutputBuffer.data(), pValue );
		}

		return totalByteSize;
	}

	template <typename TRsBuffer, typename TPValue>
	inline gds_size_t GDSCore::internal::serializeAutoResizable( TRsBuffer & pOutputBuffer, const TPValue & pValue )
	{
		// Get the size of the serialized object (i.e. the minimum required size of the target buffer).
		const auto totalByteSize = evalByteSize( pValue );

		if( totalByteSize > 0 )
		{
			// Resize the output buffer.
			pOutputBuffer.resize( cppx::numeric_cast<size_t>( totalByteSize ) );

			// Serialize the data into the buffer. This is the call to the actual, value-specific serialize().
			serialize( pOutputBuffer.data(), pValue );
		}

		return totalByteSize;
	}

	template <typename TRsBuffer, typename TPValue>
	inline gds_size_t GDSCore::internal::serializeAutoWithMetaDataResizable( TRsBuffer & pOutputBuffer, const TPValue & pValue )
	{
		// Same thing as above, but we also include the MetaData block alongside the object itself.

		const auto totalByteSize = evalByteSizeWithMetaData( pValue );

		if( totalByteSize > 0 )
		{
			pOutputBuffer.resize( cppx::numeric_cast<size_t>( totalByteSize ) );
			serializeWithMetaData( pOutputBuffer.data(), pValue );
		}

		return totalByteSize;
	}

	template <typename TPValue, typename TFxBuffer>
	inline gds_size_t GDSCore::internal::deserializeExternalFixed( TPValue & pValue, const DataReadCallback & pReadCallback, TFxBuffer & pGdsCache )
	{
		if( !readDataExternalFixed( pReadCallback, pGdsCache ) )
		{
			return 0;
		}
		return deserializeWithMetaData( pGdsCache.data(), pValue );
	}

	template <typename TPValue, typename TRsBuffer>
	inline gds_size_t GDSCore::internal::deserializeExternalResizable( TPValue & pValue, const DataReadCallback & pReadCallback, TRsBuffer & pGdsCache )
	{
		// Read data from the stream using the MetaData info. If this succeeds, pGdsCache will have
		// a byte representation of a serialized object (including its MetaData block, of course).
		if( !readDataExternalResizable( pReadCallback, pGdsCache ) )
		{
			return 0;
		}

		// Deserialize the object into the specified value. This will once again perform reading of the
		// MetaData block, but this is super cheap and we will get some additional validation as a bonus.
		return deserializeWithMetaData( pGdsCache.data(), pValue );
	}

	template <typename TFxBuffer>
	inline bool GDSCore::internal::readDataExternalFixed( const DataReadCallback & pReadCallback, TFxBuffer & pReadBuffer )
	{
		// Get the size of the metadata. This is the amount of data we need to read from the stream first.
		const auto metaDataSize = GDS::getInstanceMetaDataSize();

		// This is a fixed buffer and cannot be resized. If there is not enough space, we need to abort.
		if( metaDataSize > pReadBuffer.size() )
		{
			Ic3ThrowDesc( eExcCodeDebugPlaceholder, "Fixed buffer is to small for the output data." );
		}

		const auto metaDataReadSize = pReadCallback( pReadBuffer.data(), metaDataSize );
		if( metaDataReadSize != metaDataSize )
		{
			return false;
		}

		const auto metaData = GDS::readInstanceMetaData( pReadBuffer.data() );
		if( !metaData )
		{
			return false;
		}

		// Again - we cannot resize the buffer. If the additional data does not fit, report an error and quit.
		if( metaData.outputBlockSize > pReadBuffer.size() )
		{
			Ic3ThrowDesc( eExcCodeDebugPlaceholder, "Fixed buffer is to small for the output data." );
		}

		const auto objectDataReadSize = pReadCallback( pReadBuffer.data() + metaDataSize, metaData.objectDataSize );
		if( objectDataReadSize != metaData.objectDataSize )
		{
			return false;
		}

		return true;
	}

	template <typename TRsBuffer>
	inline bool GDSCore::internal::readDataExternalResizable( const DataReadCallback & pReadCallback, TRsBuffer & pReadBuffer )
	{
		// Get the size of the metadata. This is the amount of data we need to read from the stream first.
		const auto metaDataSize = GDS::getInstanceMetaDataSize();

		// Resize the buffer accordingly.
		pReadBuffer.resize( metaDataSize );

		// Read the data from the stream. If there is not enough data, we cannot proceed.
		const auto metaDataReadSize = pReadCallback( pReadBuffer.data(), metaDataSize );
		if( metaDataReadSize != metaDataSize )
		{
			return false;
		}

		// Read the MetaData object itself. It is boolean-convertible, so we can validate it.
		const auto metaData = GDS::readInstanceMetaData( pReadBuffer.data() );
		if( !metaData )
		{
			return false;
		}

		// MetaData contains information about the size. ::outputBlockSize is the size of the whole data block
		// (i.e. MetaData + ObjectData). Resize the buffer so we can read the remaining object data now.
		pReadBuffer.resize( cppx::numeric_cast<size_t>( metaData.outputBlockSize ) );

		// Read the object data. Save it after the MetaData (hence the offset), so we have a full representation.
		const auto objectDataReadSize = pReadCallback( pReadBuffer.data() + metaDataSize, metaData.objectDataSize );
		if( objectDataReadSize != metaData.objectDataSize )
		{
			return false;
		}

		return true;
	}

} // namespace Ic3

#endif // __IC3_CORELIB_GDS_CORE_H__
