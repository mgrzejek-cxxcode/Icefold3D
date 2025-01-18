
#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_VERTEX_ARRAY_OBJECT_CACHE_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_VERTEX_ARRAY_OBJECT_CACHE_H__

#include "../GLPrerequisites.h"
#include <map>

namespace Ic3::Graphics::GCI
{

	struct GLIAInputLayoutDefinition;
	struct GLIAVertexStreamDefinition;

	class GLIAInputLayoutImmutableStateCompat;
	class GLIAVertexStreamImmutableState;

	Ic3GLDeclareOpenGLObjectHandle( GLVertexArrayObject );

	struct GLVertexArrayObjectCachedID
	{
		uint64 inputLayoutStatePtr;
		uint64 vertexStreamStatePtr;

		bool operator==( const GLVertexArrayObjectCachedID & pRhs ) const
		{
			return ( inputLayoutStatePtr == pRhs.inputLayoutStatePtr ) && ( vertexStreamStatePtr == pRhs.vertexStreamStatePtr );
		}

		bool operator<( const GLVertexArrayObjectCachedID & pRhs ) const
		{
			return ( inputLayoutStatePtr < pRhs.inputLayoutStatePtr ) ||
			       ( ( inputLayoutStatePtr == pRhs.inputLayoutStatePtr ) && ( vertexStreamStatePtr < pRhs.vertexStreamStatePtr ) );
		}
	};

	class GLVertexArrayObjectCache
	{
	public:
		GLVertexArrayObjectCache();
		~GLVertexArrayObjectCache();

		const GLVertexArrayObject & GetOrCreate(
				const GLIAInputLayoutImmutableStateCompat & pInputLayoutState,
				const GLIAVertexStreamImmutableState & pVertexStreamState );

		const GLVertexArrayObject & GetOrCreate(
				const GLIAInputLayoutDefinition & pInputLayoutDefinition,
				const GLIAVertexStreamDefinition & pVertexStreamDefinition );

		void reset();

	private:
		using PersistentVertexArrayObjectMap = std::map<GLVertexArrayObjectCachedID, GLVertexArrayObjectHandle>;
		PersistentVertexArrayObjectMap _persistentVertexArrayObjectMap;

		using TransientVertexArrayObjectMap = std::map<GfxObjectID, GLVertexArrayObjectHandle>;
		TransientVertexArrayObjectMap _transientVertexArrayObjectMap;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_VERTEX_ARRAY_OBJECT_CACHE_H__
