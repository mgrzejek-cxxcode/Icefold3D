
#ifndef __IC3_NXMAIN_GEOMETRY_COMMON_H__
#define __IC3_NXMAIN_GEOMETRY_COMMON_H__

#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>

namespace Ic3
{

	// Describes a region of a structured buffer, providing also a reference to one of the structure members
	// (expressed as its offset). A good example is one of the actual use cases: consider a Vertex struct:
	// struct Vertex { Vec3 pos, normal }.
	// The structure contains two sub-elements: a position and a normal. We can then reference e.g. normals
	// in a buffer (like an std::vector<Vertex> by specifying:
	// - baseDataPtr = buffer.data() + sizeof( Vec3 ) // Where is the first normal in the buffer?
	// - subElementSizeInBytes = sizeof( Vec3 ) // What is the size of the normal?
	// - strideInBytes = sizeof( Vertex ) // How much do we advance the baseDataPtr to get the next normal?
	// This is used extensively by mesh loaders/importers to handle various Vertex Formats in a uniform way.
	template <typename TByte>
	struct InterleavedBufferElementRef
	{
		using ByteType = TByte;

		TByte * baseDataPtr = nullptr;
		GCI::EVertexAttribFormat elementFormat = GCI::EVertexAttribFormat::Undefined;
		uint32 elementSizeInBytes = 0;
		uint32 elementStrideInBytes = 0;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return baseDataPtr && ( elementSizeInBytes > 0 );
		}
	};

	using InterleavedBufferElementRefReadOnly = InterleavedBufferElementRef<const byte>;
	using InterleavedBufferElementRefReadWrite = InterleavedBufferElementRef<byte>;

	using GeometrySubDataRefReadOnly = InterleavedBufferElementRefReadOnly;
	using GeometrySubDataRefReadWrite = InterleavedBufferElementRefReadWrite;

	template <typename TPValue>
	class InterleavedBufferElementIterator
	{
	public:
		using ByteType = typename cppx::value_byte_type<TPValue>::Type;
		using ElementRef = InterleavedBufferElementRef<ByteType>;

		InterleavedBufferElementIterator() = default;

		template <typename TByte>
		InterleavedBufferElementIterator( const InterleavedBufferElementRef<TByte> & pRef )
		: _elementBytePtr( pRef.baseDataPtr )
		, _elementSizeInBytes( pRef.elementSizeInBytes )
		, _elementStrideInBytes( pRef.mElementStrideInBytes )
		{}

		ByteType * bytePtr() const noexcept
		{
			return _elementBytePtr;
		}

		TPValue * valuePtr() const noexcept
		{
			return reinterpret_cast<TPValue *>( _elementBytePtr );
		}

		TPValue * operator->() const noexcept
		{
			return valuePtr();
		}

		TPValue & operator*() const noexcept
		{
			return *( valuePtr() );
		}

		InterleavedBufferElementIterator & operator++() noexcept
		{
			_elementBytePtr += _elementStrideInBytes;
			return *this;
		}

		InterleavedBufferElementIterator & operator--() noexcept
		{
			_elementBytePtr -= _elementStrideInBytes;
			return *this;
		}

		InterleavedBufferElementIterator operator++( int ) noexcept
		{
			InterleavedBufferElementIterator result{ *this };
			++( *this );
			return result;
		}

		InterleavedBufferElementIterator operator--( int ) noexcept
		{
			InterleavedBufferElementIterator result{ *this };
			--( *this );
			return result;
		}

		InterleavedBufferElementIterator & operator+=( intptr_t pDiff ) const noexcept
		{
			_elementBytePtr += ( pDiff * _elementStrideInBytes );
			return *this;
		}

		InterleavedBufferElementIterator & operator-=( intptr_t pDiff ) const noexcept
		{
			_elementBytePtr -= ( pDiff * _elementStrideInBytes );
			return *this;
		}

		InterleavedBufferElementIterator operator+( intptr_t pDiff ) const noexcept
		{
			InterleavedBufferElementIterator result{ *this };
			result += pDiff;
			return result;
		}

		InterleavedBufferElementIterator operator-( intptr_t pDiff ) const noexcept
		{
			InterleavedBufferElementIterator result{ *this };
			result -= pDiff;
			return result;
		}

	private:
		ByteType * _elementBytePtr = nullptr;
		uint32 _elementSizeInBytes = 0;
		uint32 _elementStrideInBytes = 0;
	};

	using VtxAttrPosition2D = Math::Vec2f;
	using VtxAttrPosition3D = Math::Vec3f;
	using VtxAttrNormal = Math::Vec3f;
	using VtxAttrTangent = Math::Vec3f;
	using VtxAttrTexCoord2D = Math::Vec2f;

	struct VtxAttrNormalTangent
	{
		VtxAttrNormal normal;
		VtxAttrTangent tangent;
	};

	struct VtxFormatCommon2D
	{
		VtxAttrPosition2D position;
		VtxAttrTexCoord2D texCoord0;
	};

	struct VtxFormatCommon3D
	{
		VtxAttrPosition3D position;
		VtxAttrNormal normal;
		VtxAttrTangent tangent;
		VtxAttrTexCoord2D texCoord0;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_COMMON_H__
