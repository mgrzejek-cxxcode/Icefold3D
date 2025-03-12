
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_GPU_COMMAND_LIST_H__
#define __IC3_GRAPHICS_HW3D_GLC_GPU_COMMAND_LIST_H__

#include "GLPrerequisites.h"
#include "State/GLGraphicsPipelineStateController.h"
#include <Ic3/Graphics/GCI/CommandListImplRenderPassGeneric.h>

namespace Ic3::Graphics::GCI
{

	class GLShaderProgramObject;

	/// @brief
	class GLCommandList : public CommandListRenderPassGeneric
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

		virtual void BeginCommandSequence() override;
		virtual void EndCommandSequence() override;

		virtual void CmdDrawDirectIndexed(
			native_uint pIndicesNum,
			native_uint pIndicesOffset,
			native_uint pBaseVertexIndex ) override;

		virtual void CmdDrawDirectIndexedInstanced(
			native_uint pIndicesNumPerInstance,
			native_uint pInstancesNum,
			native_uint pIndicesOffset ) override;

		virtual void CmdDrawDirectNonIndexed(
			native_uint pVerticesNum,
			native_uint pVerticesOffset ) override;

		virtual void CmdDrawDirectNonIndexedInstanced(
			native_uint pVerticesNumPerInstance,
			native_uint pInstancesNum,
			native_uint pVerticesOffset ) override;

		virtual void CmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext ) override;

	private:
		virtual void ExecuteRenderPassLoadActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig ) override;

		virtual void ExecuteRenderPassStoreActions(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig ) override;

	private:
		GLGraphicsPipelineStateController * _glcGraphicsPipelineStateController;
	};

	class GLCommandListCore : public GLCommandList
	{
	public:
		GLCommandListCore(
				GLCommandSystem & pGLCommandSystem,
				ECommandListType pListType,
				System::OpenGLRenderContextHandle pSysGLRenderContext )
		: GLCommandList( pGLCommandSystem, pListType, pSysGLRenderContext, _graphicsPipelineStateControllerCore )
		, _graphicsPipelineStateControllerCore( *this )
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
		, _graphicsPipelineStateControllerCompat( *this )
		{}

	private:
		GLGraphicsPipelineStateControllerCompat _graphicsPipelineStateControllerCompat;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_GPU_COMMAND_LIST_H__
