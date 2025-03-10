
#ifndef __TS3SAMPLE_GFXTEST_MESH_DEFS_H__
#define __TS3SAMPLE_GFXTEST_MESH_DEFS_H__

#include <cxm/color.h>
#include <cxm/vector.h>
#include <cxm/matrix.h>

#include <cppx/memory.h>

namespace Math = cxm;

#define V3( pVal ) cxm::vec3f{ pVal, pVal, pVal }
#define V4( pVal ) cxm::vec4f{ pVal, pVal, pVal, pVal }
#define VCOL() cxm::make_color_vec4f(cxm::kColorBlueSignatory)

struct VertexPNT0
{
	cxm::vec3f position;
	cxm::vec4f color;
	cxm::vec3f normal;
	cxm::vec2f texCoord0;
};

inline const VertexPNT0 cvMeshTexUnitCubeVertexData[] =
{
	// Front face
	VertexPNT0 { cxm::vec3f{ -0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{ -0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },

	// Right face
	VertexPNT0 { cxm::vec3f{  0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f, -0.5f,  0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f,  0.5f,  0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },

	// Back face
	VertexPNT0 { cxm::vec3f{  0.5f, -0.5f, 0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{ -0.5f, -0.5f, 0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{ -0.5f,  0.5f, 0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f,  0.5f, 0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },

	// Left face
	VertexPNT0 { cxm::vec3f{ -0.5f, -0.5f,  0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{ -0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{ -0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{ -0.5f,  0.5f,  0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },

	// Top face
	VertexPNT0 { cxm::vec3f{ -0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f,  0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f,  0.5f,  0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{ -0.5f,  0.5f,  0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },

	// Bottom face
	VertexPNT0 { cxm::vec3f{ -0.5f, -0.5f,  0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f, -0.5f,  0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{ -0.5f, -0.5f, -0.5f }, V4(0.0f), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Render rectangle
	VertexPNT0 { cxm::vec3f{ -1.92f, -1.08f,  0.0f }, V4( 0.0f ), V3( 0.0f ), cxm::vec2f{ 0.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  1.92f, -1.08f,  0.0f }, V4( 0.0f ), V3( 0.0f ), cxm::vec2f{ 1.0f, 1.0f } },
	VertexPNT0 { cxm::vec3f{  1.92f,  1.08f,  0.0f }, V4( 0.0f ), V3( 0.0f ), cxm::vec2f{ 1.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{ -1.92f,  1.08f,  0.0f }, V4( 0.0f ), V3( 0.0f ), cxm::vec2f{ 0.0f, 0.0f } },

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Plane rectangle
	VertexPNT0 { cxm::vec3f{ -6.0f, 0.5f,  0.0f }, VCOL(), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{  6.0f, 0.5f,  0.0f }, VCOL(), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{  6.0f, 0.5f,  16.0f }, VCOL(), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },
	VertexPNT0 { cxm::vec3f{ -6.0f, 0.5f,  16.0f }, VCOL(), V3(0.0f), cxm::vec2f{ 0.0f, 0.0f } },
};

inline const cxm::vec3f cvMeshTexUnitCubeNormals[] =
{
	cxm::vec3f{ 0.0f, 0.0f, -1.0f },
	cxm::vec3f{ 1.0f, 0.0f, 0.0f },
	cxm::vec3f{ 0.0f, 0.0f, 1.0f },
	cxm::vec3f{ -1.0f, 0.0f, 0.0f },
	cxm::vec3f{ 0.0f, 1.0f, 0.0f },
	cxm::vec3f{ 0.0f, -1.0f, 0.0f },
	cxm::vec3f{ 0.0f, 0.0f, -1.0f },
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
