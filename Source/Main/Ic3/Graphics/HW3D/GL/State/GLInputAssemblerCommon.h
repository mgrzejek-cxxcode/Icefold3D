
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_INPUT_ASSEMBLER_COMMON_H__
#define __IC3_GRAPHICS_HW3D_GLC_INPUT_ASSEMBLER_COMMON_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>
#include <Ic3/Graphics/GCI/Utilities/DSTVertexSourceBinding.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLVertexArrayObject );

	/**
	 * @brief Represents OpenGL-specific vertex attribute definition.
	 */
	struct GLIAVertexAttributeDesc
	{
		/// Index of an Input Assembler slot this attribute is bound to.
		uint8 attributeSlot;

		/// Index of an Input Assembler slot this attribute receives its data from.
		uint8 vertexStreamSlot;

		/// Boolean flag, indicates that this is a normalized attribute.
		uint8 normalized;

		/// Number of components of the baseType that form this attribute. For example,
		/// a vertx attribute of type vec4 has 4 components, eachof type GL_FLOAT.
		uint8 componentsNum;

		/// Base type of the attribute (i.e. type of its individual components).
		GLenum baseType;

		/// Size, in bytes, of the attribute data.
		uint16 byteSize;

		/// Instance rate, i.e. how many instances are processed by the pipeline before a next
		/// attribute is fetched from the stream. For non-instance attributes this is always 0.
		uint16 instanceRate;

		/// Relative offset, in bytes, between the start of the buffer bound to this attribute's
		/// vertex stream (specified by vertexStreamSlot member) and start of this attribute.
		uint32 vertexStreamRelativeOffset;
	};

	/**
	 * @brief Translated, GL-specific properties of the geometry topology used for rendering.
	 */
	struct GLDrawTopologyProperties
	{
		/// Binding offset, in bytes, for the index buffer bound to the pipeline.
		GLuint indexBufferBaseOffset = 0;

		/// OpenGL-specific type of the data in the index buffer. Either GL_UNSIGNED_INT
		/// for 32-bit integers or GL_UNSIGNED_SHORT for 16-bit equivalent.
		GLenum indexBufferDataType = 0;

		/// Size, in bytes, of a single element in the index buffer. Either 2 (16-bit) or 4 (32-bit).
		GLuint indexBufferElementByteSize = 0;

		/// Primitive topology (geometry format) used to build primitives from individual vertices.
		GLenum primitiveTopology = 0;
	};

	/// An alias for a fixed array containing attribute data for all available IA attribute slots.
	using GLIAVertexAttributeDescArray = std::array<GLIAVertexAttributeDesc, GCM::kIAMaxVertexAttributesNum>;

	/**
	 * @brief Complete OpenGL-specific definition of vertex attribute layout for the Input Assembler.
	 */
	struct GLIAVertexAttributeLayout : public IAVertexAttributeLayoutMetaData
	{
		/// An array of GLIAVertexAttributeDesc values for every attribute in the IA array.
		GLIAVertexAttributeDescArray glcAttributeArray;

		/// Primitive topology translated to the GL format.
		GLenum glcPrimitiveTopology;
	};

	/**
	 * @brief Base configuration of a buffer binding to the IA stage, common for both types (vertex and index buffers).
	 */
	struct GLIABufferBinding
	{
		/// OpenGL-specific handle to the buffer object.
		GLuint handle = 0;

		/// Offset, from the physical start of the buffer, to the data that should be bound.
		GLintptr offset;

		/// Returns true if the binding is active (i.e. handle is non-zero).
		explicit operator bool() const noexcept
		{
			return handle != 0;
		}
	};

	/**
	 * @brief Configuration of a vertex buffer binding. Inherits from the common GLIABufferBinding.
	 */
	struct GLIAVertexBufferBinding : public GLIABufferBinding
	{
		/// Index of the vertex stream slot this buffer is bound to.
		GLuint streamIndex;

		/// Data stride, i.e. offset between consecutive portions of data for the same logical attribute contained within this buffer.
		GLsizei stride;
	};

	/**
	 * @brief Configuration of an index buffer binding. Inherits from the common GLIABufferBinding.
	 */
	struct GLIAIndexBufferBinding : public GLIABufferBinding
	{
		/// OpenGL-specific data format stored in this buffer.
		GLenum format;

		/// Size, in bytes, of a single element in this buffer.
		GLuint elementByteSize;
	};

	/**
	 * @brief Base class for the vertex source binding data. Uses the DynamicallySizedTemplate pattern.
	 */
	using GLIAVertexSourceBindingBase = DSTVertexSourceBindingBase<GLIAVertexBufferBinding, GLIAIndexBufferBinding>;

	/**
	 * @brief The actual data class got OpenGL vertex source binding data. Contains storage for binding configuration,
	 * passed over to the base class so it can be access through its members.
	 * 
	 * @tparam tpActiveVertexBuffersNum Number of active vertex buffers to allocate storage for.
	 * 
	 * @note
	 * Instances of this class should be most likely created dynamically via CreateDynamicallySizedTemplate(),
	 * unless there is a specific reason for not using it.
	 */
	template <native_uint tpActiveVertexBuffersNum>
	using GLIAVertexSourceBinding = DSTVertexSourceBinding<GLIAVertexSourceBindingBase, tpActiveVertexBuffersNum>;

	/// An alias for unique_ptr<> to the base vertex source bidning class (GLIAVertexSourceBindingBase).
	using GLIAVertexSourceBindingPtr = std::unique_ptr<GLIAVertexSourceBindingBase>;

	namespace Utilities
	{

		// VertexAttributeLayout

		CPPX_ATTR_NO_DISCARD GLIAVertexAttributeDesc IATranslateVertexAttributeDescGL(
			const IAVertexAttributeDesc & pVertexAttributeDesc );

		CPPX_ATTR_NO_DISCARD GLIAVertexAttributeLayout IATranslateVertexAttributeLayoutDefinitionGL(
			const IAVertexAttributeLayoutDefinition & pVertexAttributeLayoutDefinition );

		// VertexSourceBinding

		CPPX_ATTR_NO_DISCARD GLIAVertexSourceBindingPtr IATranslateVertexSourceBindingDefinitionGL(
			const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition );

		void IAUpdateVertexSourceBindingDefinitionGL(
			const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition,
			GLIAVertexSourceBindingBase & pOutGLVertexSourceBinding );

		// VertexArrayObject

		CPPX_ATTR_NO_DISCARD GLVertexArrayObjectHandle IACreateVertexArrayObjectLayoutOnlyGL(
			const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition ) noexcept;

		bool IAUpdateGLVertexArrayObjectLayoutOnly(
			GLVertexArrayObject & pGLVertexArrayObject,
			const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition ) noexcept;

		CPPX_ATTR_NO_DISCARD GLVertexArrayObjectHandle IACreateVertexArrayObjectLayoutStreamCombinedGL(
			const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
			const GLIAVertexSourceBindingBase & pGLVertexSourceBinding ) noexcept;

		bool IAUpdateVertexArrayObjectLayoutStreamCombinedGL(
			GLVertexArrayObject & pVertexArrayObject,
			const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
			const GLIAVertexSourceBindingBase & pGLVertexSourceBinding ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_INPUT_ASSEMBLER_COMMON_H__
