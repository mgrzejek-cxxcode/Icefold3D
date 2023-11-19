
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

	class GeometryDataGpuTransfer;
	class GeometryManager;

	ic3DeclareClassHandle( GeometryBuffer );
	ic3DeclareClassHandle( IndexBuffer );
	ic3DeclareClassHandle( VertexBuffer );

	// using GeometryStoragePtr = std::unique_ptr<GeometryStorage>;
	// using SharedGeometryRefHandle = const GeometryReference *;

	namespace CxDef
	{

		constexpr auto GEOMETRY_INDEX_INVALID = QLimits<uint32>::maxValue;

	}

	// Describes a region of geometry data located within a single buffer (it can be a CPU-side buffer
	// located in RAM or a hardware GPU buffer). It is used to reference a subregion within large
	// buffers containing multiple geometry objects.
	struct GeometryBufferRegion
	{
		// Offset, from the start of the buffer, to the data referenced by this region, in number of elements.
		uint32 offsetInElementsNum = 0;

		// Size of the data referenced by this region, in number of elements.
		uint32 sizeInElementsNum = 0;

		// Appends an adjacent region to the current one. Appended region's offset must be equal
		// to current's region offset + size, otherwise the call is ignored and this region remains unchanged.
		bool append( const GeometryBufferRegion & pOther );
	};

	struct GeometryDataReference
	{
		GeometryBufferRegion indexDataRegion;
		GeometryBufferRegion vertexDataRegion;

		//
		bool append( const GeometryDataReference & pOther );
	};

	struct GeometryAttributeDataCPURef
	{
		const byte * dataPtr = nullptr;
		uint32 elementSize = 0;
		uint32 offsetInElementsNum = 0;
		uint32 sizeInElementsNum = 0;
	};

	struct GeometrySize
	{
		uint32 indexElementsNum = 0;
		uint32 vertexElementsNum = 0;

		void append( const GeometrySize & pOther );
	};

	template <typename TByte>
	struct GeometryBufferDataRef
	{
		TByte * baseDataPtr = nullptr;

		uint32 dataRegionSizeInBytes = 0;

		explicit operator bool() const noexcept
		{
			return baseDataPtr && ( dataRegionSizeInBytes > 0 );
		}

		IC3_ATTR_NO_DISCARD bool empty() noexcept
		{
			return !std::is_const_v<TByte> && baseDataPtr;
		}

		IC3_ATTR_NO_DISCARD bool isWritable() noexcept
		{
			return !std::is_const_v<TByte> && baseDataPtr;
		}
	};

	using GeometryBufferDataRefReadOnly = GeometryBufferDataRef<const byte>;
	using GeometryBufferDataRefReadWrite = GeometryBufferDataRef<byte>;

	template <typename TValue, typename TByte>
	struct GeometryBufferDataIterator
	{
	public:
		GeometryBufferDataIterator() = default;

		GeometryBufferDataIterator( const GeometryBufferDataRef<TByte> & pDataRef, uint32 pCurrentElementOffset )
		: _dataRef( pDataRef )
		, _currentElementOffset( pCurrentElementOffset )
		{}

		TValue * dataPtr() const noexcept
		{
			ic3DebugAssert( sizeof( TValue ) == _dataRef.elementSizeInBytes );
			return reinterpret_cast<TValue *>( _dataRef.baseElementPtr + ( _currentElementOffset * _dataRef.strideInBytes ) );
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
			GeometryBufferDataIterator result{ _dataRef, _currentElementOffset };
			++_currentElementOffset;
			return result;
		}

		GeometryBufferDataIterator operator--( int ) noexcept
		{
			GeometryBufferDataIterator result{ _dataRef, _currentElementOffset };
			--_currentElementOffset;
			return result;
		}

		TValue * operator->() const noexcept
		{
			auto * const currentPtr = dataPtr();
			return currentPtr;
		}

		TValue & operator*() const noexcept
		{
			auto * const currentPtr = dataPtr();
			return *currentPtr;
		}

	private:
		GeometryBufferDataRef<TByte> _dataRef;
		uint32 _currentElementOffset = 0;
	};

	template <typename TValue>
	using GeometryBufferDataIteratorReadOnly = GeometryBufferDataIterator<const TValue, const byte>;

	template <typename TValue>
	using GeometryBufferDataIteratorReadWrite = GeometryBufferDataIterator<TValue, byte>;

	struct GeometryReference22
	{
		const GeometryDataFormatBase * dataFormat = nullptr;

		GeometryContainerBase * geometryContainer = nullptr;

		uint32 geometryIndex = CX_UINT32_MAX;

		uint32 activeVertexStreamsNum;

		GeometryDataReference dataReference;

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		IC3_ATTR_NO_DISCARD bool isIndexedGeometry() const noexcept;

		IC3_ATTR_NO_DISCARD bool isVertexStreamActive( uint32 pVertexStreamIndex ) const;

		IC3_ATTR_NO_DISCARD uint32 vertexStreamElementSizeInBytes( uint32 pVertexStreamIndex ) const;

		IC3_ATTR_NO_DISCARD uint32 vertexDataOffsetInElementsNum() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 vertexDataSizeInElementsNum() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 vertexElementSizeInBytes() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 indexElementSizeInBytes() const noexcept;

		IC3_ATTR_NO_DISCARD GeometrySize calculateGeometrySize() const noexcept;

		void append( const GeometryReference & pOther );
	};

	template <typename TGeometryBufferType>
	struct GeometryBufferReference
	{
		SharedHandle<TGeometryBufferType> buffer;

		explicit operator bool() const noexcept
		{
			return static_cast<bool>( buffer );
		}

		IndexBuffer * operator->() const noexcept
		{
			ic3DebugAssert( buffer );
			return buffer.get();
		}

		IndexBuffer & operator*() const noexcept
		{
			ic3DebugAssert( buffer );
			return *buffer;
		}
	};

	using GeometryIndexBufferReference = GeometryBufferReference<IndexBuffer>;
	using GeometryVertexBufferReference = GeometryBufferReference<VertexBuffer>;

	namespace gmutil
	{

		template <size_t tVertexStreamArraySize>
		IC3_ATTR_NO_DISCARD GeometryReference<tVertexStreamArraySize> getGeometryDataReferenceBaseSubRegion(
				const GeometryDataReferenceBase & pGeometryDataRef,
				uint32 pVertexDataOffsetInElementsNum,
				uint32 pVertexElementsNum,
				uint32 pIndexDataOffsetInElementsNum,
				uint32 pIndexElementsNum );

		IC3_ATTR_NO_DISCARD GeometryDataReferenceBase advanceGeometryDataReferenceBase(
				const GeometryDataReferenceBase & pGeometryDataRef,
				uint32 pVertexElementsNum,
				uint32 pIndexElementsNum );

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_COMMON_DEFS_H__
