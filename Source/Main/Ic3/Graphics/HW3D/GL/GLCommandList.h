
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_COMMAND_LIST_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_COMMAND_LIST_H__

#include "GLPrerequisites.h"
#include "State/GLPipelineStateController.h"
#include <Ic3/Graphics/GCI/CommandList.h>

namespace Ic3::Graphics::GCI
{

	class GLShaderProgramObject;

	/// @brief
	class GLCommandList : public CommandListRenderPassDefault
	{
	public:
		System::OpenGLRenderContextHandle const mSysGLRenderContext;

	public:
		GLCommandList(
			GLCommandSystem & pGLCommandSystem,
			ECommandListType pListType,
			System::OpenGLRenderContextHandle pSysGLRenderContext,
			GLGraphicsPipelineStateController & pStateController );

		virtual ~GLCommandList();

		virtual void beginCommandSequence() override;
		virtual void endCommandSequence() override;

		virtual void cmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex ) override;
		virtual void cmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset ) override;
		virtual void cmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset ) override;
		virtual void cmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset ) override;

		virtual void cmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext ) override;

	private:
		virtual void executeRenderPassLoadActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState ) override;

		virtual void executeRenderPassStoreActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState ) override;

	private:
		GLGraphicsPipelineStateController * _graphicsPipelineStateControllerGL;
	};

	class GLCommandListCore : public GLCommandList
	{
	public:
		GLCommandListCore(
				GLCommandSystem & pGLCommandSystem,
				ECommandListType pListType,
				System::OpenGLRenderContextHandle pSysGLRenderContext )
		: GLCommandList( pGLCommandSystem, pListType, pSysGLRenderContext, _graphicsPipelineStateControllerCore )
		{}

	private:
		GLGraphicsPipelineStateControllerCore _graphicsPipelineStateControllerCore;
	};

	class GLCommandListCompat : public GLCommandList
	{
	public:
		GLCommandListCompat(
				GLCommandSystem & pGLCommandSystem,
				ECommandListType pListType,
				System::OpenGLRenderContextHandle pSysGLRenderContext )
		: GLCommandList( pGLCommandSystem, pListType, pSysGLRenderContext, _graphicsPipelineStateControllerCompat )
		{}

	private:
		GLGraphicsPipelineStateControllerCompat _graphicsPipelineStateControllerCompat;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_COMMAND_LIST_H__
