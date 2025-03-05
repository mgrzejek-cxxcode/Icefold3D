
#ifndef __TS3SAMPLE_GFXTEST_MESH_DEFS_H__
#define __TS3SAMPLE_GFXTEST_MESH_DEFS_H__

#include <Ic3/Math/Color.h>
#include <Ic3/Math/Vector.h>
#include <Ic3/Math/Matrix.h>
#include <cppx/memory.h>

namespace Math = Ic3::Math;

#define V3( pVal ) Math::Vec3f{ pVal, pVal, pVal }
#define V4( pVal ) Math::Vec4f{ pVal, pVal, pVal, pVal }
#define VCOL() Math::MakeColorVec4f(Math::kColorBlueSignatory)

struct VertexPNT0
{
	Math::Vec3f position;
	Math::Vec4f color;
	Math::Vec3f normal;
	Math::Vec2f texCoord0;
};

inline const VertexPNT0 cvMeshTexUnitCubeVertexData[] =
{
	// Front face
	VertexPNT0 { Math::Vec3f{ -0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{ -0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },

	// Right face
	VertexPNT0 { Math::Vec3f{  0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f, -0.5f,  0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f,  0.5f,  0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },

	// Back face
	VertexPNT0 { Math::Vec3f{  0.5f, -0.5f, 0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{ -0.5f, -0.5f, 0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{ -0.5f,  0.5f, 0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f,  0.5f, 0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },

	// Left face
	VertexPNT0 { Math::Vec3f{ -0.5f, -0.5f,  0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{ -0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{ -0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{ -0.5f,  0.5f,  0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },

	// Top face
	VertexPNT0 { Math::Vec3f{ -0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f,  0.5f,  0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{ -0.5f,  0.5f,  0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },

	// Bottom face
	VertexPNT0 { Math::Vec3f{ -0.5f, -0.5f,  0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f, -0.5f,  0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{ -0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Render rectangle
	VertexPNT0 { Math::Vec3f{ -1.92f, -1.08f,  0.0f }, V4( 0.0f ), V3( 0.0f ), Math::Vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  1.92f, -1.08f,  0.0f }, V4( 0.0f ), V3( 0.0f ), Math::Vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { Math::Vec3f{  1.92f,  1.08f,  0.0f }, V4( 0.0f ), V3( 0.0f ), Math::Vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{ -1.92f,  1.08f,  0.0f }, V4( 0.0f ), V3( 0.0f ), Math::Vec2f{ 0.0f, 0.0f } },

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Plane rectangle
	VertexPNT0 { Math::Vec3f{ -6.0f, 0.5f,  0.0f }, VCOL(), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{  6.0f, 0.5f,  0.0f }, VCOL(), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{  6.0f, 0.5f,  16.0f }, VCOL(), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },
	VertexPNT0 { Math::Vec3f{ -6.0f, 0.5f,  16.0f }, VCOL(), V3(0.0f), Math::Vec2f{ 0.0f, 0.0f } },
};

inline const Math::Vec3f cvMeshTexUnitCubeNormals[] =
{
	Math::Vec3f{ 0.0f, 0.0f, -1.0f },
	Math::Vec3f{ 1.0f, 0.0f, 0.0f },
	Math::Vec3f{ 0.0f, 0.0f, 1.0f },
	Math::Vec3f{ -1.0f, 0.0f, 0.0f },
	Math::Vec3f{ 0.0f, 1.0f, 0.0f },
	Math::Vec3f{ 0.0f, -1.0f, 0.0f },
	Math::Vec3f{ 0.0f, 0.0f, -1.0f },
};

inline const uint32 cvMeshTexUnitCubeIndexData[] =
{
	 0,  1,  2,  2,  3,  0,
	 4,  5,  6,  6,  7,  4,
	 8,  9, 10, 10, 11,  8,
	12, 13, 14, 14, 15, 12,
	16, 17, 18, 18, 19, 16,
	20, 21, 22, 22, 23, 20,
	///////////////////////
	24, 25, 26, 26, 27, 24,
	///////////////////////
	28, 29, 30, 30, 31, 28
};

std::vector<VertexPNT0> generateVertexPNT0Data()
{
	std::vector<VertexPNT0> result;

	const auto verticesNum = cppx::static_array_size( cvMeshTexUnitCubeVertexData );
	result.resize( verticesNum );

	for( uint32 vertexIdx = 0; vertexIdx < verticesNum; ++vertexIdx )
	{
		result[vertexIdx] = cvMeshTexUnitCubeVertexData[vertexIdx];
		result[vertexIdx].normal = cvMeshTexUnitCubeNormals[vertexIdx/4];
	}

	return result;
}

std::vector<uint32> generateIndexPNT0Data()
{
	std::vector<uint32> result;

	const auto indicesNum = cppx::static_array_size( cvMeshTexUnitCubeIndexData );
	result.resize( indicesNum );

	cppx::mem_copy( result.data(), result.size(), cvMeshTexUnitCubeIndexData, indicesNum );

	return result;
}

#endif // __TS3SAMPLE_GFXTEST_MESH_DEFS_H__
