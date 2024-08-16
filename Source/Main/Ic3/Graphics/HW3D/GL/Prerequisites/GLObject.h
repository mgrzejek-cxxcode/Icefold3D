
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_OBJECT_H__

namespace Ic3::Graphics::GCI
{

	constexpr auto CX_GL_OBJECT_HANDLE_INVALID = QLimits<GLuint>::maxValue;

	constexpr auto CX_GL_BIND_TARGET_UNKNOWN = QLimits<GLenum>::maxValue;

	enum class GLObjectBaseType : enum_default_value_t
	{
		// Type for buffer objects (glGenBuffers).
		Buffer,
		// Type for framebuffer objects (glGenFramebuffers).
		Framebuffer,
		// Type for buffer objects (glGenRenderbuffers).
		Renderbuffer,
		// Type for buffer objects (glGenSamplers).
		Sampler,
		// Type for buffer objects (glCreateShader).
		Shader,
		// Type for buffer objects (glGenProgramPipelines).
		ShaderPipeline,
		// Type for shader program objects (glCreateProgram).
		ShaderProgram,
		// Type for texture objects (glGenTextures).
		Texture,
		// Type for transform feedback objects (glGenTransformFeedbacks).
		TransformFeedback,
		// Type for vertex array objects (glGenVertexArrays).
		VertexArray,
		//
		Unknown
	};

	class GLObject
	{
	public:
		GLObjectBaseType const mBaseType;

		GLuint const mGLHandle;

		GLObject( GLObjectBaseType pBaseType, GLuint pHandle );
		virtual ~GLObject();

		virtual bool release();

		virtual bool validateHandle() const;

		bool checkHandle() const;
	};

	inline bool GLObject::checkHandle() const
	{
		return mGLHandle != CX_GL_OBJECT_HANDLE_INVALID;
	}

	struct GLObjectDeleter
	{
		void operator()( GLObject * pObject ) const
		{
			pObject->release();
			delete pObject;
		}
	};

	template <typename TObject>
	using GLObjectHandle = std::unique_ptr<TObject, GLObjectDeleter>;

	template <typename TObject, typename... TArgs>
	inline GLObjectHandle<TObject> createGLObject( TArgs && ...pArgs )
	{
		GLObjectHandle<TObject> result{ new TObject( std::forward<TArgs>( pArgs )... ) };
		return result;
	}

#define ic3GLDeclareOpenGLObjectHandle( pObjectClass ) \
	class pObjectClass; \
	using pObjectClass##Handle = GLObjectHandle<pObjectClass>

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_OBJECT_H__
