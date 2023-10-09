
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GRAPHICS_SHADER_STATE_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GRAPHICS_SHADER_STATE_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsShaderLinkageImmutableState.h>
#include <Ic3/Graphics/GCI/State/SeparablePipelineState.h>

namespace Ic3::Graphics::GCI
{

	ic3GLDeclareOpenGLObjectHandle( GLShaderPipelineObject );
	ic3GLDeclareOpenGLObjectHandle( GLShaderProgramObject );

	/// @brief
	class GLGraphicsShaderLinkageImmutableState : public GraphicsShaderLinkageImmutableState
	{
	public:
		GLGraphicsShaderLinkageImmutableState(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderLinkageCommonProperties & pCommonProperties );

		virtual ~GLGraphicsShaderLinkageImmutableState();
	};

	/// @brief
	class GLGraphicsShaderLinkageImmutableStateCore : public GLGraphicsShaderLinkageImmutableState
	{
	public:
		GLShaderPipelineObjectHandle const mGLShaderPipelineObject;

	public:
		GLGraphicsShaderLinkageImmutableStateCore(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderLinkageCommonProperties & pCommonProperties,
				GLShaderPipelineObjectHandle pGLShaderPipelineObject );

		virtual ~GLGraphicsShaderLinkageImmutableStateCore();

		IC3_ATTR_NO_DISCARD static GpaHandle<GLGraphicsShaderLinkageImmutableStateCore> createInstance(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderSet & pShaderSet );
	};

	/// @brief
	class GLGraphicsShaderLinkageImmutableStateCompat : public GLGraphicsShaderLinkageImmutableState
	{
	public:
		GLShaderProgramObjectHandle const mGLShaderProgramObject;

	public:
		GLGraphicsShaderLinkageImmutableStateCompat(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderLinkageCommonProperties & pCommonProperties,
				GLShaderProgramObjectHandle pGLShaderProgramObject );

		virtual ~GLGraphicsShaderLinkageImmutableStateCompat();

		IC3_ATTR_NO_DISCARD static GpaHandle<GLGraphicsShaderLinkageImmutableStateCompat> createInstance(
				GLGPUDevice & pGPUDevice,
				const GraphicsShaderSet & pShaderSet );
	};

	namespace smutil
	{

		IC3_ATTR_NO_DISCARD GLShaderPipelineObjectHandle createGraphicsShaderPipelineObjectGL(
				const GraphicsShaderSet & pShaderSet );

		IC3_ATTR_NO_DISCARD GLShaderProgramObjectHandle createGraphicsShaderProgramObjectGL(
				const GraphicsShaderSet & pShaderSet );

		void updateUniformDataCurrentGL(
				GLShaderPipelineObject & pShaderPipeline,
				uint32 pUniformIndex,
				EBaseDataType pBaseType,
				uint32 pLength,
				const void * pData );

		void updateUniformDataExplicitGL(
				GLShaderProgramObject & pShaderProgram,
				uint32 pUniformIndex,
				EBaseDataType pBaseType,
				uint32 pLength,
				const void * pData );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GRAPHICS_SHADER_STATE_H__
