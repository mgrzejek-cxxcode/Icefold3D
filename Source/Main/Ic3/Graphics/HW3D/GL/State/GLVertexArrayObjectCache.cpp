
#include "GLVertexArrayObjectCache.h"
#include "../Objects/GLVertexArrayObject.h"
#include "../State/GLGraphicsPipelineStateIA.h"

namespace Ic3::Graphics::GCI
{

	GLVertexArrayObjectCache::GLVertexArrayObjectCache() = default;

	GLVertexArrayObjectCache::~GLVertexArrayObjectCache() = default;

	const GLVertexArrayObject & GLVertexArrayObjectCache::GetOrCreate(
			const GLIAVertexAttributeLayout & pGLVertexAttributeLayout,
			const GLIAVertexSourceBinding & pGLVertexSourceBinding )
	{
		GLVertexArrayObjectCachedID cachedID{
			reinterpret_cast<uint64>( &pGLVertexAttributeLayout ),
			reinterpret_cast<uint64>( &pGLVertexSourceBinding )
		};

		auto cachedEntryIter = _persistentVertexArrayObjectMap.find( cachedID );
		if( cachedEntryIter == _persistentVertexArrayObjectMap.end() )
		{
			auto vertexArrayObject = GCU::IACreateVertexArrayObjectLayoutStreamCombinedGL( pGLVertexAttributeLayout, pGLVertexSourceBinding );

			auto insertResult = _persistentVertexArrayObjectMap.emplace( cachedID, std::move( vertexArrayObject ) );

			cachedEntryIter = insertResult.first;
		}

		return *cachedEntryIter->second;
	}

	void GLVertexArrayObjectCache::Reset()
	{
		_persistentVertexArrayObjectMap.clear();
	}

}
