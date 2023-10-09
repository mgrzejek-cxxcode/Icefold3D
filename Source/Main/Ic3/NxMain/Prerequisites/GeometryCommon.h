
#ifndef __IC3_NXMAIN_GEOMETRY_COMMON_H__
#define __IC3_NXMAIN_GEOMETRY_COMMON_H__

#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>

namespace Ic3
{

	// Describes a region of a structured buffer, providing also a reference to one of the structure members
	// (expressed as its offset). A good example is one of the actual use cases. Consider a Vertex struct:
	// struct Vertex { Vec3 pos, normal }.
	// The structure contains two sub-elements: a position and a normal. We can then reference e.g. normals
	// in a buffer (like an std::vector<Vertex> by specifying:
	// - basePtr = buffer.data() + sizeof( Vec3 ) // Where is the first normal in the buffer?
	// - subElementSizeInBytes = sizeof( Vec3 ) // What is the size of the normal?
	// - strideInBytes = sizeof( Vertex ) // How much do we advance the basePtr to get the next normal?
	// This is used extensively by mesh loaders/importers to handle various Vertex Formats in a uniform way.
	template <typename TByte>
	struct DataBufferRegionSubElementRef
	{
		TByte * basePtr = nullptr;

		uint32 subElementSizeInBytes = 0;

		uint32 strideInBytes = 0;

		explicit operator bool() const noexcept
		{
			return basePtr != nullptr;
		}
	};

	using DataBufferRegionSubElementRefReadOnly = DataBufferRegionSubElementRef<const byte>;

	using DataBufferRegionSubElementRefReadWrite = DataBufferRegionSubElementRef<byte>;

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
