
#include "GLVertexArrayObjectCache.h"
#include "../Objects/GLVertexArrayObject.h"
#include "../State/GLInputAssembler.h"

namespace Ic3::Graphics::GCI
{

	GLVertexArrayObjectCache::GLVertexArrayObjectCache() = default;

	GLVertexArrayObjectCache::~GLVertexArrayObjectCache() = default;

	const GLVertexArrayObject & GLVertexArrayObjectCache::GetOrCreate(
			const GLIAInputLayoutImmutableStateCompat & pInputLayoutState,
			const GLIAVertexStreamImmutableState & pVertexStreamState )
	{
		GLVertexArrayObjectCachedID cachedID{
			reinterpret_cast<uint64>( &pInputLayoutState ),
			reinterpret_cast<uint64>( &pVertexStreamState )
		};

		auto cachedEntryIter = _persistentVertexArrayObjectMap.find( cachedID );
		if( cachedEntryIter == _persistentVertexArrayObjectMap.end() )
		{
			auto vertexArrayObject = SMU::CreateGLVertexArrayObjectLayoutStreamCombinedGL(
					pInputLayoutState.mGLInputLayoutDefinition,
					pVertexStreamState.mGLVertexStreamDefinition );

			auto insertResult = _persistentVertexArrayObjectMap.emplace( cachedID, std::move( vertexArrayObject ) );

			cachedEntryIter = insertResult.first;
		}

		return *cachedEntryIter->second;
	}


	const GLVertexArrayObject & GLVertexArrayObjectCache::GetOrCreate(
			const GLIAInputLayoutDefinition & pInputLayoutDefinition,
			const GLIAVertexStreamDefinition & pVertexStreamDefinition )
	{
		const auto hash1 = cppx::hash_compute<GfxObjectID::sHashAlgo1>( pInputLayoutDefinition );
		const auto hash2 = cppx::hash_compute<GfxObjectID::sHashAlgo2>( pVertexStreamDefinition );

		GfxObjectID vaoRefID{ hash1, hash2 };

		auto cachedEntryIter = _transientVertexArrayObjectMap.find( vaoRefID );
		if( cachedEntryIter == _transientVertexArrayObjectMap.end() )
		{
			auto vertexArrayObject = SMU::CreateGLVertexArrayObjectLayoutStreamCombinedGL(
					pInputLayoutDefinition,
					pVertexStreamDefinition );

			auto insertResult = _transientVertexArrayObjectMap.emplace( vaoRefID, std::move( vertexArrayObject ) );

			cachedEntryIter = insertResult.first;
		}

		return *cachedEntryIter->second;
	}

	void GLVertexArrayObjectCache::reset()
	{
		_persistentVertexArrayObjectMap.clear();
		_transientVertexArrayObjectMap.clear();
	}

}
