
#ifndef __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_CACHE_H__
#define __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_CACHE_H__

#include <Ic3/Graphics/HW3D/GL/State/GLInputAssemblerCommon.h>
#include <map>

namespace Ic3::Graphics::GCI
{

	struct GLIAVertexAttributeLayout;

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
				const GLIAVertexSourceBindingBase & pGLVertexSourceBinding );

		void Reset();

	private:
		using VertexArrayObjectMap = std::map<GLVertexArrayObjectCachedID, GLVertexArrayObjectHandle>;
		VertexArrayObjectMap _vertexArrayObjectMap;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_CACHE_H__
