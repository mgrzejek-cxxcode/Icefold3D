
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_IA_H__
#define __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_IA_H__

#include "GLInputAssemblerCommon.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIA.h>

namespace Ic3::Graphics::GCI
{

	///
	class GLVertexAttributeLayoutDescriptor : public HW3DPipelineStateDescriptor<VertexAttributeLayoutDescriptor>
	{
	public:
		GLVertexAttributeLayoutDescriptor(
				GLGPUDevice & pGPUDevice,
				const IAVertexAttributeLayoutMetaData & pAttributeLayoutMetaData );

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
				const IAVertexAttributeLayoutMetaData & pAttributeLayoutMetaData,
				GLVertexArrayObjectHandle pGLVertexArrayObject,
				GLenum pGLPrimitiveTopology );

		virtual ~GLVertexAttributeLayoutDescriptorCore();

		static TGfxHandle<GLVertexAttributeLayoutDescriptorCore> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo );

	private:
		// Common config of the attribute layout. Since we removed all the data, we need to explicitly store this here.
		IAVertexAttributeLayoutMetaData _attributeLayoutMetaData;
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
		GLIAVertexSourceBindingPtr const mGLVertexSourceBinding;

	public:
		GLVertexSourceBindingDescriptor(
				GLGPUDevice & pGPUDevice,
				GLIAVertexSourceBindingPtr pGLVertexSourceBinding );

		virtual ~GLVertexSourceBindingDescriptor();

		static TGfxHandle<GLVertexSourceBindingDescriptor> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const VertexSourceBindingDescriptorCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_IA_H__
