
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

	ic3DeclareInterfaceHandle( IGeometryStorage );
	ic3DeclareInterfaceHandle( IGeometryStorageManaged );
	ic3DeclareInterfaceHandle( IGeometryStorageShared );

	template <typename TStruct>
	using RGeometryVertexBufferGenericArray = std::array<TStruct, GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM>;

	struct SGeometryBufferMemoryRef
	{
		uint32 bufferIndex = 0;

		uintptr_t internalMemoryRef = 0;

		SMemoryRegion memoryRegion;

		explicit operator bool() const noexcept
		{
			return !memoryRegion.empty();
		}
	};

	struct SGeometryInstanceMemoryRef
	{
		IGeometryStorage * storage = nullptr;

		SGeometryBufferMemoryRef indexBufferMemoryRef;

		RGeometryVertexBufferGenericArray<SGeometryBufferMemoryRef> vertexBufferMemoryRefArray;
	};

	template <typename TByte>
	struct GeometryBufferDataRef
	{
		TByte * baseDataPtr = nullptr;

		uint32 dataRegionSizeInBytes = 0;

		uint32 elementStrideInBytes = 0;

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
