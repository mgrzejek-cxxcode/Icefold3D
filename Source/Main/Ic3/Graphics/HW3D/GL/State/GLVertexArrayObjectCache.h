
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
		uint64 vertexAttributeLayoutDescriptorPtr;
		uint64 vertexSourceBindingDescriptorPtr;

		bool operator==( const GLVertexArrayObjectCachedID & pRhs ) const
		{
			return ( vertexAttributeLayoutDescriptorPtr == pRhs.vertexAttributeLayoutDescriptorPtr ) &
				   ( vertexSourceBindingDescriptorPtr == pRhs.vertexSourceBindingDescriptorPtr );
		}

		bool operator<( const GLVertexArrayObjectCachedID & pRhs ) const
		{
			return ( vertexAttributeLayoutDescriptorPtr < pRhs.vertexAttributeLayoutDescriptorPtr ) ||
			       ( ( vertexAttributeLayoutDescriptorPtr == pRhs.vertexAttributeLayoutDescriptorPtr ) &&
			       	 ( vertexSourceBindingDescriptorPtr < pRhs.vertexSourceBindingDescriptorPtr ) );
		}
	};

	class GLVertexArrayObjectCache
	{
	public:
		GLVertexArrayObjectCache();
		~GLVertexArrayObjectCache();

		const GLVertexArrayObject & GetOrCreate(
				const GLVertexAttributeLayoutDescriptorCompat & pVertexAttributeLayoutDescriptor,
				const GLVertexSourceBindingDescriptor & pVertexSourceBindingDescriptor );

		void Reset();

	private:
		using PersistentVertexArrayObjectMap = std::map<GLVertexArrayObjectCachedID, GLVertexArrayObjectHandle>;
		PersistentVertexArrayObjectMap _persistentVertexArrayObjectMap;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_CACHE_H__
