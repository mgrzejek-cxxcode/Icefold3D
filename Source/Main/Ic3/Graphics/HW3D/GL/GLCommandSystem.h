
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_CMD_MANAGER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_CMD_MANAGER_H__

#include "GLPrerequisites.h"
#include <Ic3/Graphics/GCI/CommandSystem.h>

namespace Ic3::Graphics::GCI
{

	struct GLCommandSyncData
	{
		GLsync openglSyncFence = nullptr;
	};

	IC3_GX_GL_API void releaseGLCommandSyncData( void * pSyncData );

	/// @brief
	class IC3_GX_GL_CLASS GLCommandSystem : public CommandSystem
	{
	public:
		explicit GLCommandSystem( GLGPUDevice & pGPUDevice );
		virtual ~GLCommandSystem();

		virtual std::unique_ptr<CommandContext> AcquireCommandContext( ECommandContextType pContextType ) override;

		virtual CommandSync SubmitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo ) override;

		// OpenGL-specific thing coming from the fact, that context creation requires target surface (Drawable on X11,
		// HDC on Win32, EGLSurface on Android, etc.). GPUDevice::setPresentationLayer was actually introduced only to
		// make tis possible in at least a bit natural way. So, when PresentationLayer is set, GL-level onSet() gets
		// the surface and passes it here, so CmdManager has the surface when a CmdContext is created.
		// Also, that is the reason why no context is created in Initialize() function and the requirement of having
		// a surface bound to the device when AcquireContext() is called.
		void SetTargetGLSurface( System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface );

	private:
		GLCommandList * AcquireCommandList( ECommandExecutionMode pCommandExecutionMode );

		bool InitializeMainCommandList();

		static System::OpenGLRenderContextHandle CreateSysGLRenderContext( GLGPUDevice & pGPUDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface );

	protected:
		System::OpenGLDisplaySurfaceHandle _targetSysGLSurface;
		GLCommandListHandle _mainCommandList;
	};
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_CMD_MANAGER_H__
