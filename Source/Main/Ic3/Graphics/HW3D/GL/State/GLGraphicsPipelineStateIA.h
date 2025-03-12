
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_IA_H__
#define __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_IA_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIA.h>

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
		uint8 attributeIndex;
		uint8 streamIndex;
		uint8 normalized;
		uint8 componentsNum;
		GLenum baseType;
		uint32 byteSize;
		uint32 relativeOffset;
		uint32 instanceRate;
	};

	using GLIAVertexAttributeDescArray = std::array<GLIAVertexAttributeInfo, GCM::kIAMaxVertexAttributesNum>;

	struct GLIAVertexAttributeLayout : public IAVertexAttributeLayoutCommonConfig
	{
		GLIAVertexAttributeDescArray glcAttributeArray;
		GLenum glcPrimitiveTopology;

		void Reset();
	};

	struct GLIAVertexStreamBufferInfo
	{
		GLuint handle;
		GLintptr offset;

		explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return handle == 0;
		}
	};

	struct GLIAIndexBufferBinding : public GLIAVertexStreamBufferInfo
	{
		GLenum format;
		GLuint elementByteSize;

		void Reset()
		{
			cppx::mem_set_zero( *this );
		}
	};

	struct GLIAVertexBufferBinding : public GLIAVertexStreamBufferInfo
	{
		GLsizei stride;

		void Reset()
		{
			cppx::mem_set_zero( *this );
		}
	};

	// IAVertexBufferReferenceArray
	struct GLIAVertexBufferArrayBindings
	{
		/// Separate VB bindings (a struct with per-property arrays).
		struct SeparateBindings
		{
			/// Array of GL-specific handles. Zero at index N means the binding for stream N is not active.
			GLuint handleArray[GCM::kIAMaxDataStreamVertexBuffersNum];
			/// Array of offsets, in bytes, from the beginning of each buffer storage. Undefined for inactive bindings.
			GLintptr offsetArray[GCM::kIAMaxDataStreamVertexBuffersNum];
			/// Array of data strides, in bytes, of the data in each buffer. Undefined for inactive bindings.
			GLsizei strideArray[GCM::kIAMaxDataStreamVertexBuffersNum];
		};

		/// Interleaved VB bindings (an array of structs).
		using InterleavedBindingsArray = std::array<GLIAVertexBufferBinding, GCM::kIAMaxDataStreamVertexBuffersNum>;

		union
		{
			InterleavedBindingsArray interleavedBindings;

			SeparateBindings separateBindings;
		};

		VertexSourceBindingRageList activeRanges;

		EGLVertexBufferBindingType bindingType = EGLVertexBufferBindingType::Undefined;

		void InitializeInterleaved();
		void InitializeSeparate();
		void Reset();

		CPPX_ATTR_NO_DISCARD GLIAVertexBufferBinding GetBinding( native_uint pStreamIndex ) const;
	};

	/// @brief
	struct GLIAVertexSourceBinding : public IAVertexSourceBindingCommonConfig
	{
		GLIAVertexBufferArrayBindings vertexBufferBindings;
		GLIAIndexBufferBinding indexBufferBinding;

		void Reset();
	};

	struct GLDrawTopologyProperties
	{
		GLuint indexBufferBaseOffset = 0;
		GLenum indexBufferDataType = 0;
		GLuint indexBufferElementByteSize = 0;
		GLenum primitiveTopology = 0;

		void Reset()
		{
			indexBufferBaseOffset = 0;
			indexBufferDataType = 0;
			indexBufferElementByteSize = 0;
			primitiveTopology = 0;
		}
	};

	///
	class GLVertexAttributeLayoutDescriptor : public HW3DPipelineStateDescriptor<VertexAttributeLayoutDescriptor>
	{
	public:
		GLVertexAttributeLayoutDescriptor(
				GLGPUDevice & pGPUDevice,
				const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig );

		virtual ~GLVertexAttributeLayoutDescriptor();
	};

	/**
	 * Implements VertexAttributeLayoutDescriptor for the Core version of the GL driver.
	 * Eliminates all redundant GL-level state and uses pre-configured VertexArrayObject (VAO).
	 */
	class GLVertexAttributeLayoutDescriptorCore : public GLVertexAttributeLayoutDescriptor
	{
	public:
		GLVertexArrayObjectHandle const mGLVertexArrayObject;
		GLenum const mGLPrimitiveTopology;

	public:
		GLVertexAttributeLayoutDescriptorCore(
				GLGPUDevice & pGPUDevice,
				const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig,
				GLVertexArrayObjectHandle pGLVertexArrayObject,
				GLenum pGLPrimitiveTopology );

		virtual ~GLVertexAttributeLayoutDescriptorCore();

		static TGfxHandle<GLVertexAttributeLayoutDescriptorCore> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo );

	private:
		// Common config of the attribute layout. Since we removed all the data, we need to explicitly store this here.
		IAVertexAttributeLayoutCommonConfig _commonAttributeLayoutConfig;
	};

	///
	class GLVertexAttributeLayoutDescriptorCompat : public GLVertexAttributeLayoutDescriptor
	{
	public:
		GLIAVertexAttributeLayout const mGLVertexAttributeLayout;

	public:
		GLVertexAttributeLayoutDescriptorCompat(
				GLGPUDevice & pGPUDevice,
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition );

		virtual ~GLVertexAttributeLayoutDescriptorCompat();

		static TGfxHandle<GLVertexAttributeLayoutDescriptorCompat> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo );
	};

	///
	class GLVertexSourceBindingDescriptor : public HW3DPipelineStateDescriptor<VertexSourceBindingDescriptor>
	{
	public:
		GLIAVertexSourceBinding const mGLVertexSourceBinding;

	public:
		GLVertexSourceBindingDescriptor(
				GLGPUDevice & pGPUDevice,
				const GLIAVertexSourceBinding & pGLVertexSourceBinding );

		virtual ~GLVertexSourceBindingDescriptor();

		static TGfxHandle<GLVertexSourceBindingDescriptor> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const VertexSourceBindingDescriptorCreateInfo & pCreateInfo );
	};

	namespace GCU
	{

		// VertexAttributeLayout

		CPPX_ATTR_NO_DISCARD GLIAVertexAttributeInfo IATranslateVertexAttributeDescGL(
				const IAVertexAttributeDesc & pVertexAttributeDesc );

		CPPX_ATTR_NO_DISCARD GLIAVertexAttributeLayout IATranslateVertexAttributeLayoutDefinitionGL(
				const IAVertexAttributeLayoutDefinition & pVertexAttributeLayoutDefinition );

		// VertexSourceBinding

		CPPX_ATTR_NO_DISCARD GLIAVertexSourceBinding IATranslateVertexSourceBindingDefinitionGL(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition );

		void IAUpdateVertexSourceBindingDefinitionGL(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition,
				GLIAVertexSourceBinding & pOutGLVertexSourceBinding );

		CPPX_ATTR_NO_DISCARD GLIAVertexBufferArrayBindings IATranslateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVertexBufferReferences,
				cppx::bitmask<EIAVertexSourceBindingFlags> pBindingMask );

		uint32 IAUpdateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVertexBufferReferences,
				cppx::bitmask<EIAVertexSourceBindingFlags> pBindingMask,
				GLIAVertexBufferArrayBindings & pOutGLBindings );

		CPPX_ATTR_NO_DISCARD GLIAIndexBufferBinding IATranslateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIndexBufferReference );

		bool IAUpdateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIndexBufferReference,
				GLIAIndexBufferBinding & pOutGLBinding );

		CPPX_ATTR_NO_DISCARD GLIAIndexBufferBinding IATranslateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIndexBufferReference );

		// VertexArrayObject

		CPPX_ATTR_NO_DISCARD GLVertexArrayObjectHandle IACreateVertexArrayObjectLayoutOnlyGL(
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition ) noexcept;

		bool IAUpdateGLVertexArrayObjectLayoutOnly(
				GLVertexArrayObject & pGLVertexArrayObject,
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition ) noexcept;

		CPPX_ATTR_NO_DISCARD GLVertexArrayObjectHandle IACreateVertexArrayObjectLayoutStreamCombinedGL(
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
				const GLIAVertexSourceBinding & pSourceBindingDefinition ) noexcept;

		bool IAUpdateVertexArrayObjectLayoutStreamCombinedGL(
				GLVertexArrayObject & pVertexArrayObject,
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
				const GLIAVertexSourceBinding & pSourceBindingDefinition ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_IA_H__
