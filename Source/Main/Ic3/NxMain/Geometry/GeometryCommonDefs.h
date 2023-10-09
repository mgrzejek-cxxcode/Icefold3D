
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_COMMON_DEFS_H__
#define __IC3_NXMAIN_GEOMETRY_COMMON_DEFS_H__

#include "../GCI/CommonGCIDefs.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>
#include <Ic3/Graphics/GCI/Resources/GPUBufferReference.h>
#include <list>

namespace Ic3
{

	struct GeometryReference;
	struct GeometryStorageCreateInfo;

	class GeometryContainerBase;
	class GeometryDataFormatBase;
	class GeometryDataGpuTransfer;
	class GeometryManager;

	ic3DeclareClassHandle( GeometryBuffer );
	ic3DeclareClassHandle( IndexBuffer );
	ic3DeclareClassHandle( VertexBuffer );
	ic3DeclareClassHandle( GeometryDataSourceBase );
	ic3DeclareClassHandle( GeometrySharedStorageBase );

	using GeometryStoragePtr = std::unique_ptr<GeometryStorage>;
	using SharedGeometryRefHandle = const GeometryReference *;

	namespace CxDefs
	{

		constexpr auto GEOMETRY_INDEX_INVALID = Limits<uint32>::maxValue;

	}

	enum EVertexAttributeSemanticsFlags : uint32
	{
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_POSITION_BIT    =    0x01,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_NORMAL_BIT      =    0x02,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TANGENT_BIT     =    0x04,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_FIXED_COLOR_BIT =    0x08,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_0_BIT =   0x010,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_1_BIT =   0x020,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_2_BIT =   0x040,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_3_BIT =   0x080,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_4_BIT =   0x100,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_5_BIT =   0x200,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_6_BIT =   0x400,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_7_BIT =   0x800,
		E_VERTEX_ATTRIBUTE_SEMANTICS_MASK_TEX_COORD_ALL   =   0xFF0,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_RESERVED_13_BIT =  0x1000,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_RESERVED_14_BIT =  0x2000,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_RESERVED_15_BIT =  0x4000,
		E_VERTEX_ATTRIBUTE_SEMANTICS_MASK_RESERVED_ALL    =  0x7000,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_INSTANCE_0_BIT  = 0x10000,
		E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_INSTANCE_1_BIT  = 0x20000,
		E_VERTEX_ATTRIBUTE_SEMANTICS_MASK_INSTANCE_ALL    = 0x30000,
		E_VERTEX_ATTRIBUTE_SEMANTICS_MASK_ALL             = 0x37FFF,

		E_VERTEX_ATTRIBUTE_SEMANTICS_MASK_INSTANCE_0_OVERLAP =
				E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_3_BIT |
				E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_4_BIT |
				E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_5_BIT |
				E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_6_BIT,

		E_VERTEX_ATTRIBUTE_SEMANTICS_MASK_INSTANCE_1_OVERLAP =
				E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_7_BIT |
				E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_RESERVED_13_BIT |
				E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_RESERVED_14_BIT |
				E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_RESERVED_15_BIT,
	};

	namespace CxDefs
	{

		inline constexpr uint32 declareVertexAttributeSemanticsID( EVertexAttributeSemanticsFlags pSemanticsFlags )
		{
			return static_cast<uint32>( pSemanticsFlags );
		}

		inline constexpr uint64 declareFixedVertexAttributeID(
				uint16 pBaseIndex,
				uint16 pComponentsNum,
				GCI::EVertexAttribFormat pBaseFormat,
				Bitmask<EVertexAttributeSemanticsFlags> pSemanticsFlags )
		{
			return static_cast<uint64>(
					( ( ( uint64 )pBaseIndex & 0xF ) << 60 ) |
					( ( ( uint64 )pComponentsNum & 0xF ) << 56 ) |
					( ( ( uint64 )pBaseFormat & Limits<uint32>::maxValue ) << 24 ) |
					( ( uint64 )pSemanticsFlags & E_VERTEX_ATTRIBUTE_SEMANTICS_MASK_ALL ) );
		}

	}

	enum class EVertexAttributeSemanticsID : uint32
	{
		Undefined  = 0,
		Custom     = static_cast<uint32>( E_VERTEX_ATTRIBUTE_SEMANTICS_MASK_ALL ) ^ Limits<uint32>::maxValue,
		Position   = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_POSITION_BIT    ),
		Normal     = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_NORMAL_BIT      ),
		Tangent    = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TANGENT_BIT     ),
		FixedColor = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_FIXED_COLOR_BIT ),
		TexCoord0  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_0_BIT ),
		TexCoord1  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_1_BIT ),
		TexCoord2  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_2_BIT ),
		TexCoord3  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_3_BIT ),
		TexCoord4  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_4_BIT ),
		TexCoord5  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_5_BIT ),
		TexCoord6  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_6_BIT ),
		TexCoord7  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_7_BIT ),
		Instance0  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_INSTANCE_0_BIT  ),
		Instance1  = CxDefs::declareVertexAttributeSemanticsID( E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_INSTANCE_1_BIT  ),
	};

	enum class EFixedVertexAttributeID : uint64
	{
		Undefined  = 0,
		Position   = CxDefs::declareFixedVertexAttributeID(  0, 1, GCI::EVertexAttribFormat::Vec3F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_POSITION_BIT ),
		Normal     = CxDefs::declareFixedVertexAttributeID(  1, 1, GCI::EVertexAttribFormat::Vec3F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_NORMAL_BIT ),
		Tangent    = CxDefs::declareFixedVertexAttributeID(  2, 1, GCI::EVertexAttribFormat::Vec3F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TANGENT_BIT ),
		FixedColor = CxDefs::declareFixedVertexAttributeID(  4, 1, GCI::EVertexAttribFormat::Vec4F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_FIXED_COLOR_BIT ),
		TexCoord0  = CxDefs::declareFixedVertexAttributeID(  5, 1, GCI::EVertexAttribFormat::Vec2F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_0_BIT ),
		TexCoord1  = CxDefs::declareFixedVertexAttributeID(  6, 1, GCI::EVertexAttribFormat::Vec2F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_1_BIT ),
		TexCoord2  = CxDefs::declareFixedVertexAttributeID(  7, 1, GCI::EVertexAttribFormat::Vec2F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_2_BIT ),
		TexCoord3  = CxDefs::declareFixedVertexAttributeID(  8, 1, GCI::EVertexAttribFormat::Vec2F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_3_BIT ),
		TexCoord4  = CxDefs::declareFixedVertexAttributeID(  9, 1, GCI::EVertexAttribFormat::Vec2F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_4_BIT ),
		TexCoord5  = CxDefs::declareFixedVertexAttributeID( 10, 1, GCI::EVertexAttribFormat::Vec2F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_5_BIT ),
		TexCoord6  = CxDefs::declareFixedVertexAttributeID( 11, 1, GCI::EVertexAttribFormat::Vec2F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_6_BIT ),
		TexCoord7  = CxDefs::declareFixedVertexAttributeID( 12, 1, GCI::EVertexAttribFormat::Vec2F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_TEX_COORD_7_BIT ),
		Reserved13 = CxDefs::declareFixedVertexAttributeID( 13, 1, GCI::EVertexAttribFormat::Vec4F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_RESERVED_13_BIT ),
		Reserved14 = CxDefs::declareFixedVertexAttributeID( 14, 1, GCI::EVertexAttribFormat::Vec4F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_RESERVED_14_BIT ),
		Reserved15 = CxDefs::declareFixedVertexAttributeID( 15, 1, GCI::EVertexAttribFormat::Vec4F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_RESERVED_15_BIT ),
		Instance0  = CxDefs::declareFixedVertexAttributeID(  8, 4, GCI::EVertexAttribFormat::Vec4F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_INSTANCE_0_BIT ),
		Instance1  = CxDefs::declareFixedVertexAttributeID( 12, 4, GCI::EVertexAttribFormat::Vec4F32, E_VERTEX_ATTRIBUTE_SEMANTICS_FLAG_INSTANCE_1_BIT ),
	};

	namespace CxDefs
	{

		inline uint16 getFixedVertexAttributeBaseIndex( EFixedVertexAttributeID pFixedAttributeID )
		{
			return static_cast<uint16>( ( ( uint64 )pFixedAttributeID >> 60 ) & 0xF );
		}

		inline uint16 getFixedVertexAttributeComponentsNum( EFixedVertexAttributeID pFixedAttributeID )
		{
			return static_cast<uint16>( ( ( uint64 )pFixedAttributeID >> 56 ) & 0xF );
		}

		inline GCI::EVertexAttribFormat getFixedVertexAttributeBaseFormat( EFixedVertexAttributeID pFixedAttributeID )
		{
			return static_cast<GCI::EVertexAttribFormat>( ( ( uint64 )pFixedAttributeID >> 24 ) & Limits<uint32>::maxValue );
		}

		inline uint32 getFixedVertexAttributeComponentByteSize( EFixedVertexAttributeID pFixedAttributeID )
		{
			return GCI::CxDefs::getVertexAttribFormatByteSize( getFixedVertexAttributeBaseFormat( pFixedAttributeID ) );
		}

		inline uint32 getFixedVertexAttributeByteSize( EFixedVertexAttributeID pFixedAttributeID )
		{
			return getFixedVertexAttributeComponentsNum( pFixedAttributeID ) * getFixedVertexAttributeComponentByteSize( pFixedAttributeID );
		}

		inline Bitmask<EVertexAttributeSemanticsFlags> getFixedVertexAttributeSemanticsFlags( EFixedVertexAttributeID pFixedAttributeID )
		{
			return static_cast<uint32>( ( uint64 )pFixedAttributeID & 0xFFFFFF );
		}

		inline EVertexAttributeSemanticsID getFixedVertexAttributeSemanticsID( EFixedVertexAttributeID pFixedAttributeID )
		{
			return static_cast<EVertexAttributeSemanticsID>( declareVertexAttributeSemanticsID( getFixedVertexAttributeSemanticsFlags( pFixedAttributeID ) ) );
		}

		inline bool isVertexAttributeSemanticsValid( EVertexAttributeSemanticsID pSemanticsID )
		{
			const auto semanticsMask = makeBitmask( pSemanticsID );
			return semanticsMask.isSetAnyOf( E_VERTEX_ATTRIBUTE_SEMANTICS_MASK_ALL ) && ( semanticsMask.countBits() == 1 );
		}

		inline bool isVertexAttributeSemanticsDefined( EVertexAttributeSemanticsID pSemanticsID )
		{
			return isVertexAttributeSemanticsValid( pSemanticsID ) || ( pSemanticsID == EVertexAttributeSemanticsID::Custom );
		}

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

	template <typename TByte>
	struct GeometryBufferDataRef
	{
		TByte * baseElementPtr = nullptr;

		uint32 dataSizeInElementsNum = 0;

		uint32 elementSizeInBytes = 0;

		uint32 strideInBytes = 0;

		explicit operator bool() const noexcept
		{
			return baseElementPtr && ( dataSizeInElementsNum > 0 );
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

	struct GeometrySize
	{
		uint32 indexElementsNum = 0;
		uint32 vertexElementsNum = 0;

		void append( const GeometrySize & pOther );
	};

	struct GeometryDataReference
	{
		GeometryBufferRegion indexDataRegion;
		GeometryBufferRegion vertexDataRegion;

		//
		bool append( const GeometryDataReference & pOther );
	};

	struct GeometryReference
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
