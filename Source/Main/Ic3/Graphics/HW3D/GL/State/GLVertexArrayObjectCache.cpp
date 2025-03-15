
#include "GLVertexArrayObjectCache.h"
#include "../Objects/GLVertexArrayObject.h"
#include "../State/GLGraphicsPipelineStateIA.h"

namespace Ic3::Graphics::GCI
{

	GLVertexArrayObjectCache::GLVertexArrayObjectCache() = default;

	GLVertexArrayObjectCache::~GLVertexArrayObjectCache() = default;

	const GLVertexArrayObject & GLVertexArrayObjectCache::GetOrCreate(
			const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
			const GLIAVertexSourceBindingBase & pGLVertexSourceBinding )
	{
		GLVertexArrayObjectCachedID cachedID{
			reinterpret_cast<uint64>( &pGLAttributeLayoutDefinition ),
			reinterpret_cast<uint64>( &pGLVertexSourceBinding )
		};

		auto cachedEntryIter = _vertexArrayObjectMap.find( cachedID );
		if( cachedEntryIter == _vertexArrayObjectMap.end() )
		{
			auto vertexArrayObject = Utilities::IACreateVertexArrayObjectLayoutStreamCombinedGL( pGLAttributeLayoutDefinition, pGLVertexSourceBinding );

			auto insertResult = _vertexArrayObjectMap.emplace( cachedID, std::move( vertexArrayObject ) );

			cachedEntryIter = insertResult.first;
		}

		return *cachedEntryIter->second;
	}

	void GLVertexArrayObjectCache::Reset()
	{
		_vertexArrayObjectMap.clear();
	}

}
