
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_SHADER_H__
#define __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_SHADER_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorShader.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateImplSeparableState.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLShaderPipelineObject );
	Ic3GLDeclareOpenGLObjectHandle( GLShaderProgramObject );

	/// @brief
	class GLGraphicsShaderLinkageDescriptor : public HW3DPipelineStateDescriptor<GraphicsShaderLinkageDescriptor>
	{
	public:
		Ic3DeclareNonCopyable( GLGraphicsShaderLinkageDescriptor )

		GLGraphicsShaderLinkageDescriptor(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig );

		virtual ~GLGraphicsShaderLinkageDescriptor();

	private:
		GraphicsShaderBindingCommonConfig const _commonShaderBindingConfig;
	};

	/// @brief
	///
	class GLGraphicsShaderLinkageDescriptorCore final : public GLGraphicsShaderLinkageDescriptor
	{
	public:
		GLShaderPipelineObjectHandle const mGLShaderPipelineObject;

	public:
		Ic3DeclareNonCopyable( GLGraphicsShaderLinkageDescriptorCore )

		GLGraphicsShaderLinkageDescriptorCore(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig,
				GLShaderPipelineObjectHandle pGLShaderPipelineObject );

		virtual ~GLGraphicsShaderLinkageDescriptorCore();

		CPPX_ATTR_NO_DISCARD static TGfxHandle<GLGraphicsShaderLinkageDescriptorCore> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo );
	};

	/// @brief
	class GLGraphicsShaderLinkageDescriptorCompat final : public GLGraphicsShaderLinkageDescriptor
	{
	public:
		GLShaderProgramObjectHandle const mGLShaderProgramObject;

	public:
		Ic3DeclareNonCopyable( GLGraphicsShaderLinkageDescriptorCompat )

		GLGraphicsShaderLinkageDescriptorCompat(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig,
				GLShaderProgramObjectHandle pGLShaderProgramObject );

		virtual ~GLGraphicsShaderLinkageDescriptorCompat();

		CPPX_ATTR_NO_DISCARD static TGfxHandle<GLGraphicsShaderLinkageDescriptorCompat> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo );
	};

	namespace Utilities
	{

		CPPX_ATTR_NO_DISCARD GLShaderPipelineObjectHandle CreateGraphicsShaderPipelineObjectGL(
				const GraphicsShaderBinding & pBindingConfiguration );

		CPPX_ATTR_NO_DISCARD GLShaderProgramObjectHandle CreateGraphicsShaderProgramObjectGL(
				const GraphicsShaderBinding & pBindingConfiguration );

		void UpdateUniformDataCurrentGL(
				GLShaderPipelineObject & pShaderPipeline,
				uint32 pUniformIndex,
				EBaseDataType pBaseType,
				uint32 pLength,
				const void * pData );

		void UpdateUniformDataExplicitGL(
				GLShaderProgramObject & pShaderProgram,
				uint32 pUniformIndex,
				EBaseDataType pBaseType,
				uint32 pLength,
				const void * pData );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_SHADER_H__
