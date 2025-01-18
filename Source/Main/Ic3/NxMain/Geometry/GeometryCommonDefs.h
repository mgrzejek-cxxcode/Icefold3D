
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_COMMON_DEFS_H__
#define __IC3_NXMAIN_GEOMETRY_COMMON_DEFS_H__

#include "../GCI/ShaderInputSemantics.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>
#include <Ic3/Graphics/GCI/Resources/GPUBufferReference.h>
#include <list>

namespace Ic3
{

	struct GeometryReference;
	struct GeometryStorageCreateInfo;

	class GeometryDataGPUTransfer;
	class GeometryManager;

	Ic3DeclareInterfaceHandle( IGeometryDataStorage );
	Ic3DeclareInterfaceHandle( IGeometryDataStorageManaged );
	Ic3DeclareInterfaceHandle( IGeometryStorageShared );

	Ic3DeclareClassHandle( GeometryBuffer );
	Ic3DeclareClassHandle( IndexBuffer );
	Ic3DeclareClassHandle( VertexBuffer );

	using RGeometryReferenceKey = uint64;

	template <typename TElement>
	using RGeometryVertexBufferGenericArray = std::array<TElement, GCM::cxIAMaxVertexBufferBindingsNum>;

	struct GeometryBufferInfo
	{
		uint16 baseAlignment = 0;
		uint16 elementStrideInBytes = 0;
		size_t sizeInBytes = 0;

		std::atomic<uintptr_t> currentAllocationOffset = 0;

		CPPX_ATTR_NO_DISCARD uintptr_t getCurrentAllocationOffset() const noexcept
		{
			const auto currentOffset = currentAllocationOffset.load( std::memory_order_relaxed );
			return currentOffset;
		}

		CPPX_ATTR_NO_DISCARD uintptr_t getFreeSpace() const noexcept
		{
			const auto currentOffset = currentAllocationOffset.load( std::memory_order_relaxed );
			return sizeInBytes - currentOffset;
		}
	};

	struct GeometryStorageMemoryRef
	{
		uint32 bufferIndex = 0;

		MemoryRegion memoryRegion;

		native_uint internalMemoryKey = 0;

		explicit operator bool() const noexcept
		{
			return !memoryRegion.empty();
		}
	};

	struct GeometryMemoryReference
	{
		IGeometryDataStorage * storage = nullptr;
		RGeometryReferenceKey memoryRefKey = 0;

		explicit operator bool() const noexcept
		{
			return storage != nullptr;
		}
	};

	struct GeometryMemorySubAllocationDesc
	{
		uint16 alignmentRestriction = 0;
		uint16 elementSizeInBytes = 0;
		uint32 elementsNum = 0;
		uint32 explicitAllocationSize = 0;

		explicit operator bool() const noexcept
		{
			return ( elementSizeInBytes > 0 ) && ( elementsNum > 0 );
		}
	};

	struct GeometryMemoryAllocationDesc
	{
		GeometryMemorySubAllocationDesc indexDataAllocDesc;
		RGeometryVertexBufferGenericArray<GeometryMemorySubAllocationDesc> vertexDataAllocDescArray;
	};

	struct GeometryExternalMemoryDesc
	{
		uintptr_t memoryAddress = 0;
		uint32 elementStrideInBytes = 0;
		uint32 elementsNum = 0;

		explicit operator bool() const noexcept
		{
			return ( memoryAddress != 0 ) && ( elementStrideInBytes > 0 ) && ( elementsNum > 0 );
		}

		CPPX_ATTR_NO_DISCARD size_t size() const noexcept
		{
			return elementStrideInBytes * elementsNum;
		}
	};

	struct GeometryInstanceExternalDataDesc
	{
		GeometryExternalMemoryDesc indexDataDesc;
		RGeometryVertexBufferGenericArray<GeometryExternalMemoryDesc> vertexDataDescArray;
	};





	enum EGeometryBufferDataRefFlags : uint16
	{
		E_GEOMETRY_BUFFER_DATA_REF_FLAG_DIRECT_MEMORY_ACCESS_BIT = 0x0001,
	};

	/**
	 * Contains information required to reference a portion of data within a geometry buffer. Allows to
	 * perform various operations on geometry buffers like data fetch or update. @see IGeometryDataStorage.
	 * Note, that GeometryBufferDataRefs *always* reference a range of homogenous elements, for example
	 * positions, normals or interleaved vertex attributes.
	 * @tparam TPByte Type
	 */
	template <typename TPByte>
	struct GeometryBufferDataRef
	{
		// Base ref pointer identifying the referenced storage region. Note, that this is *NOT* (at least: not always)
		// a direct pointer that can be used to write (or even read!) the data. In case of storages without DMA, this
		// is simply a storage-specific implementation detail.
		TPByte * baseDataPtr = nullptr;

		// Size, in bytes, of the geometry storage region referenced by this instance.
		uint32 dataRegionSizeInBytes = 0;

		// Size, in bytes, of a stride. Stride is the number of bytes between start of an element and the next element.
		// For example, if a given geometry buffer stores 3D float vertices (Vec3f) with no padding, stride is 12 bytes.
		uint16 elementStrideInBytes = 0;

		// Additional flags describing this data ref.
		cppx::bitmask<EGeometryBufferDataRefFlags> flags = 0;

		explicit operator bool() const noexcept
		{
			return baseDataPtr && ( dataRegionSizeInBytes > 0 );
		}

		CPPX_ATTR_NO_DISCARD constexpr bool empty() const noexcept
		{
			return dataRegionSizeInBytes == 0;
		}

		CPPX_ATTR_NO_DISCARD constexpr bool hasDirectMemoryAccess() const noexcept
		{
			return flags.is_set( E_GEOMETRY_BUFFER_DATA_REF_FLAG_DIRECT_MEMORY_ACCESS_BIT );
		}

		CPPX_ATTR_NO_DISCARD constexpr bool isWritable() const noexcept
		{
			return !std::is_const_v<TPByte> && hasDirectMemoryAccess();
		}
	};

	using GeometryBufferDataRefReadOnly = GeometryBufferDataRef<const byte>;
	using GeometryBufferDataRefReadWrite = GeometryBufferDataRef<byte>;

	template <typename TPValue, typename TPByte>
	struct GeometryBufferDataIterator
	{
	public:
		GeometryBufferDataIterator() = default;

		GeometryBufferDataIterator( const GeometryBufferDataRef<TPByte> & pDataRef, uint32 pCurrentElementOffset )
		: _dataRef( pDataRef )
		, _currentElementOffset( pCurrentElementOffset )
		{}

		TPValue * dataPtr() const noexcept
		{
			Ic3DebugAssert( sizeof( TPValue ) == _dataRef.elementSizeInBytes );
			return reinterpret_cast<TPValue *>( _dataRef.baseElementPtr + ( _currentElementOffset * _dataRef.strideInBytes ) );
		}

		GeometryBufferDataIterator & operator++() noexcept
		{
			++_currentElementOffset;
			return *this;
		}

		GeometryBufferDataIterator & operator--() noexcept
		{
			--_currentElementOffset;
			return *this;
		}

		GeometryBufferDataIterator operator++( int ) noexcept
		{
			GeometryBufferDataIterator result{ *this };
			++_currentElementOffset;
			return result;
		}

		const GeometryBufferDataIterator operator--( int ) noexcept
		{
			GeometryBufferDataIterator result{ *this };
			--_currentElementOffset;
			return result;
		}

		TPValue * operator->() const noexcept
		{
			auto * const currentPtr = dataPtr();
			return currentPtr;
		}

		TPValue & operator*() const noexcept
		{
			auto * const currentPtr = dataPtr();
			return *currentPtr;
		}

	private:
		GeometryBufferDataRef<TPByte> _dataRef;
		uint32 _currentElementOffset = 0;
	};

	template <typename TPValue>
	using GeometryBufferDataIteratorReadOnly = GeometryBufferDataIterator<const TPValue, const byte>;

	template <typename TPValue>
	using GeometryBufferDataIteratorReadWrite = GeometryBufferDataIterator<TPValue, byte>;

	/*
	struct GeometryReference22
	{
		const GeometryDataFormatBase * dataFormat = nullptr;

		GeometryContainerBase * geometryContainer = nullptr;

		uint32 geometryIndex = CX_UINT32_MAX;

		uint32 activeVertexStreamsNum;

		GeometryDataReference dataReference;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		CPPX_ATTR_NO_DISCARD bool isIndexedGeometry() const noexcept;

		CPPX_ATTR_NO_DISCARD bool isVertexStreamActive( uint32 pVertexStreamIndex ) const;

		CPPX_ATTR_NO_DISCARD uint32 vertexStreamElementSizeInBytes( uint32 pVertexStreamIndex ) const;

		CPPX_ATTR_NO_DISCARD uint32 vertexDataOffsetInElementsNum() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 vertexDataSizeInElementsNum() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 vertexElementSizeInBytes() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 indexElementSizeInBytes() const noexcept;

		CPPX_ATTR_NO_DISCARD GeometrySize calculateGeometrySize() const noexcept;

		void append( const GeometryReference & pOther );
	};

	template <typename TGeometryBufferType>
	struct GeometryBufferReference
	{
		TSharedHandle<TGeometryBufferType> buffer;

		explicit operator bool() const noexcept
		{
			return static_cast<bool>( buffer );
		}

		IndexBuffer * operator->() const noexcept
		{
			Ic3DebugAssert( buffer );
			return buffer.get();
		}

		IndexBuffer & operator*() const noexcept
		{
			Ic3DebugAssert( buffer );
			return *buffer;
		}
	};

	using GeometryIndexBufferReference = GeometryBufferReference<IndexBuffer>;
	using GeometryVertexBufferReference = GeometryBufferReference<VertexBuffer>;

	namespace gmutil
	{

		template <size_t tVertexStreamUsageSize>
		CPPX_ATTR_NO_DISCARD GeometryReference<tVertexStreamUsageSize> getGeometryDataReferenceBaseSubRegion(
				const GeometryDataReferenceBase & pGeometryDataRef,
				uint32 pVertexDataOffsetInElementsNum,
				uint32 pVertexElementsNum,
				uint32 pIndexDataOffsetInElementsNum,
				uint32 pIndexElementsNum );

		CPPX_ATTR_NO_DISCARD GeometryDataReferenceBase advanceGeometryDataReferenceBase(
				const GeometryDataReferenceBase & pGeometryDataRef,
				uint32 pVertexElementsNum,
				uint32 pIndexElementsNum );

	}
	 */

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_COMMON_DEFS_H__
