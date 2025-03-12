
#ifndef __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_CACHE_H__
#define __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_CACHE_H__

#include "../GLPrerequisites.h"
#include <map>

namespace Ic3::Graphics::GCI
{

	struct GLIAVertexAttributeLayout;
	struct GLIAVertexSourceBinding;

	class GLVertexAttributeLayoutDescriptorCompat;
	class GLVertexSourceBindingDescriptor;

	Ic3GLDeclareOpenGLObjectHandle( GLVertexArrayObject );

	struct GLVertexArrayObjectCachedID
	{
		uint64 adObjAddr;
		uint64 bdObjAddr;

		bool operator==( const GLVertexArrayObjectCachedID & pRhs ) const noexcept
		{
			return ( adObjAddr == pRhs.adObjAddr ) && ( bdObjAddr == pRhs.bdObjAddr );
		}

		bool operator<( const GLVertexArrayObjectCachedID & pRhs ) const noexcept
		{
			return ( adObjAddr < pRhs.adObjAddr ) || ( ( adObjAddr == pRhs.adObjAddr ) && ( bdObjAddr < pRhs.bdObjAddr ) );
		}
	};

	class GLVertexArrayObjectCache
	{
	public:
		GLVertexArrayObjectCache();
		~GLVertexArrayObjectCache();

		const GLVertexArrayObject & GetOrCreate(
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
				const GLIAVertexSourceBinding & pGLVertexSourceBinding );

		void Reset();

	private:
		using VertexArrayObjectMap = std::map<GLVertexArrayObjectCachedID, GLVertexArrayObjectHandle>;
		VertexArrayObjectMap _vertexArrayObjectMap;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_CACHE_H__
