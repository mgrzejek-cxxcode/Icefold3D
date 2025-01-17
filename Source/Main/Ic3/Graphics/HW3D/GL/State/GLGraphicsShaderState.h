
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GRAPHICS_SHADER_STATE_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GRAPHICS_SHADER_STATE_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsShaderLinkageImmutableState.h>
#include <Ic3/Graphics/GCI/State/SeparablePipelineState.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLShaderPipelineObject );
	Ic3GLDeclareOpenGLObjectHandle( GLShaderProgramObject );

	/// @brief
	class GLGraphicsShaderLinkageImmutableState : public GraphicsShaderLinkageImmutableState
	{
	public:
		GLGraphicsShaderLinkageImmutableState(
				GLGpuDevice & pGpuDevice,
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
				GLGpuDevice & pGpuDevice,
				const GraphicsShaderLinkageCommonProperties & pCommonProperties,
				GLShaderPipelineObjectHandle pGLShaderPipelineObject );

		virtual ~GLGraphicsShaderLinkageImmutableStateCore();

		CPPX_ATTR_NO_DISCARD static TGfxHandle<GLGraphicsShaderLinkageImmutableStateCore> CreateInstance(
				GLGpuDevice & pGpuDevice,
				const GraphicsShaderSet & pShaderSet );
	};

	/// @brief
	class GLGraphicsShaderLinkageImmutableStateCompat : public GLGraphicsShaderLinkageImmutableState
	{
	public:
		GLShaderProgramObjectHandle const mGLShaderProgramObject;

	public:
		GLGraphicsShaderLinkageImmutableStateCompat(
				GLGpuDevice & pGpuDevice,
				const GraphicsShaderLinkageCommonProperties & pCommonProperties,
				GLShaderProgramObjectHandle pGLShaderProgramObject );

		virtual ~GLGraphicsShaderLinkageImmutableStateCompat();

		CPPX_ATTR_NO_DISCARD static TGfxHandle<GLGraphicsShaderLinkageImmutableStateCompat> CreateInstance(
				GLGpuDevice & pGpuDevice,
				const GraphicsShaderSet & pShaderSet );
	};

	namespace SMU
	{

		CPPX_ATTR_NO_DISCARD GLShaderPipelineObjectHandle CreateGraphicsShaderPipelineObjectGL(
				const GraphicsShaderSet & pShaderSet );

		CPPX_ATTR_NO_DISCARD GLShaderProgramObjectHandle CreateGraphicsShaderProgramObjectGL(
				const GraphicsShaderSet & pShaderSet );

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

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GRAPHICS_SHADER_STATE_H__
