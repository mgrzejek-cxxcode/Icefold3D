
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_INPUT_ASSEMBLER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_INPUT_ASSEMBLER_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerImmutableStates.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLVertexArrayObject );

	enum class EGLVertexBufferBindingType : uint32
	{
		Undefined,
		Interleaved,
		Separate
	};

	struct GLIAVertexAttributeInfo
	{
		uint16 streamIndex;
		uint8 normalized;
		uint8 componentsNum;
		GLenum baseType;
		uint32 byteSize;
		uint32 relativeOffset;
		uint32 instanceRate;
	};

	using GLIAVertexAttributeInfoArray = std::array<GLIAVertexAttributeInfo, GCM::cxIAMaxVertexAttributesNum>;

	struct GLIAInputLayoutDefinition
	{
		cppx::bitmask<EIAVertexAttributeFlags> activeAttributesMask;
		GLIAVertexAttributeInfoArray attributeArray;
		GLenum primitiveTopology;

		void reset();
	};

	struct GLIAIndexBufferBinding
	{
		GLuint handle;
		GLintptr offset;
		GLenum format;
		GLuint elementByteSize;

		void reset()
		{
			cppx::mem_set_zero( *this );
		}

		explicit operator bool() const noexcept
		{
			return handle != 0;
		}
	};

	struct GLIAVertexBufferBinding
	{
		GLuint handle;
		GLintptr offset;
		GLsizei stride;

		explicit operator bool() const noexcept
		{
			return handle != 0;
		}
	};

	struct GLIAVertexBuffersBindings
	{
		/// Separate VB bindings (a struct with per-property arrays).
		struct SeparateBindings
		{
			/// Array of GL-specific handles. Zero at index N means the binding for stream N is not active.
			GLuint handleArray[GCM::cxIAMaxVertexBufferBindingsNum];
			/// Array of offsets, in bytes, from the beginning of each buffer storage. Undefined for inactive bindings.
			GLintptr offsetArray[GCM::cxIAMaxVertexBufferBindingsNum];
			/// Array of data strides, in bytes, of the data in each buffer. Undefined for inactive bindings.
			GLsizei strideArray[GCM::cxIAMaxVertexBufferBindingsNum];
		};

		/// Interleaved VB bindings (an array of structs).
		using InterleavedBindingsArray = std::array<GLIAVertexBufferBinding, GCM::cxIAMaxVertexBufferBindingsNum>;

		union
		{
			InterleavedBindingsArray interleavedBindings;
			SeparateBindings separateBindings;
		};

		IAVertexBufferRangeList activeRanges;

		EGLVertexBufferBindingType bindingType = EGLVertexBufferBindingType::Undefined;

		void initializeInterleaved();
		void initializeSeparate();
		void reset();

		CPPX_ATTR_NO_DISCARD GLIAVertexBufferBinding GetBinding( native_uint pStreamIndex ) const;
	};

	/// @brief
	struct GLIAVertexStreamDefinition
	{
		cppx::bitmask<EIAVertexStreamBindingFlags> activeBindingsMask;
		GLIAVertexBuffersBindings vertexBufferBindings;
		GLIAIndexBufferBinding indexBufferBinding;

		void reset();
	};

	struct GLDrawTopologyProperties
	{
		GLuint indexBufferBaseOffset = 0;
		GLenum indexBufferDataType = 0;
		GLuint indexBufferElementByteSize = 0;
		GLenum primitiveTopology = 0;

		void reset()
		{
			indexBufferBaseOffset = 0;
			indexBufferDataType = 0;
			indexBufferElementByteSize = 0;
			primitiveTopology = 0;
		}
	};

	///
	class GLIAVertexStreamImmutableState : public IAVertexStreamImmutableState
	{
	public:
		GLIAVertexStreamDefinition const mGLVertexStreamDefinition;

	public:
		GLIAVertexStreamImmutableState(
				GLGpuDevice & pGpuDevice,
				const IAVertexStreamStateCommonProperties & pCommonProperties,
				const GLIAVertexStreamDefinition & pGLVertexStreamDefinition );

		virtual ~GLIAVertexStreamImmutableState();

		static TGfxHandle<GLIAVertexStreamImmutableState> CreateInstance(
				GLGpuDevice & pGpuDevice,
				const IAVertexStreamDefinition & pVertexStreamDefinition );
	};

	///
	class GLIAInputLayoutImmutableState : public IAInputLayoutImmutableState
	{
	public:
		GLIAInputLayoutImmutableState(
				GLGpuDevice & pGpuDevice,
				const IAInputLayoutStateCommonProperties & pCommonProperties );

		virtual ~GLIAInputLayoutImmutableState();
	};

	///
	class GLIAInputLayoutImmutableStateCore : public GLIAInputLayoutImmutableState
	{
	public:
		GLVertexArrayObjectHandle const mVertexArrayObject;
		GLenum const mGLPrimitiveTopology;

	public:
		GLIAInputLayoutImmutableStateCore(
				GLGpuDevice & pGpuDevice,
				const IAInputLayoutStateCommonProperties & pCommonProperties,
				GLVertexArrayObjectHandle pVertexArrayObject,
				GLenum pGLPrimitiveTopology );

		virtual ~GLIAInputLayoutImmutableStateCore();

		static TGfxHandle<GLIAInputLayoutImmutableStateCore> CreateInstance(
				GLGpuDevice & pGpuDevice,
				const IAInputLayoutDefinition & pInputLayoutDefinition );
	};

	///
	class GLIAInputLayoutImmutableStateCompat : public GLIAInputLayoutImmutableState
	{
	public:
		GLIAInputLayoutDefinition const mGLInputLayoutDefinition;

	public:
		GLIAInputLayoutImmutableStateCompat(
				GLGpuDevice & pGpuDevice,
				const IAInputLayoutStateCommonProperties & pCommonProperties,
				const GLIAInputLayoutDefinition & pGLInputLayoutDefinition );

		virtual ~GLIAInputLayoutImmutableStateCompat();

		static TGfxHandle<GLIAInputLayoutImmutableStateCompat> CreateInstance(
				GLGpuDevice & pGpuDevice,
				const IAInputLayoutDefinition & pInputLayoutDefinition );
	};

	namespace SMU
	{

		CPPX_ATTR_NO_DISCARD GLIAVertexAttributeInfo TranslateIAVertexAttributeInfoGL(
				const IAVertexAttributeInfo & pAttributeInfo );

		CPPX_ATTR_NO_DISCARD GLIAInputLayoutDefinition TranslateIAInputLayoutDefinitionGL(
				const IAInputLayoutDefinition & pDefinition );

		CPPX_ATTR_NO_DISCARD GLIAVertexStreamDefinition TranslateIAVertexStreamDefinitionGL(
				const IAVertexStreamDefinition & pDefinition );

		uint32 TranslateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVBReferences,
				cppx::bitmask<EIAVertexStreamBindingFlags> pBindingMask,
				GLIAVertexBuffersBindings & pOutGLBindings);

		bool TranslateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIBReference,
				GLIAIndexBufferBinding & pOutGLBinding );

		CPPX_ATTR_NO_DISCARD GLIAVertexBuffersBindings TranslateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVBReferences,
				cppx::bitmask<EIAVertexStreamBindingFlags> pBindingMask );

		CPPX_ATTR_NO_DISCARD GLIAIndexBufferBinding TranslateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIBReference );

		CPPX_ATTR_NO_DISCARD GLVertexArrayObjectHandle CreateGLVertexArrayObjectLayoutOnlyGL(
				const GLIAInputLayoutDefinition & pInputLayoutDefinition ) noexcept;

		bool UpdateGLVertexArrayObjectLayoutOnlyGL(
				GLVertexArrayObject & pVertexArrayObject,
				const GLIAInputLayoutDefinition & pInputLayoutDefinition ) noexcept;

		CPPX_ATTR_NO_DISCARD GLVertexArrayObjectHandle CreateGLVertexArrayObjectLayoutStreamCombinedGL(
				const GLIAInputLayoutDefinition & pInputLayoutDefinition,
				const GLIAVertexStreamDefinition & pVertexStreamDefinition ) noexcept;

		bool UpdateGLVertexArrayObjectLayoutStreamCombinedGL(
				GLVertexArrayObject & pVertexArrayObject,
				const GLIAInputLayoutDefinition & pInputLayoutDefinition,
				const GLIAVertexStreamDefinition & pVertexStreamDefinition ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_INPUT_ASSEMBLER_H__
