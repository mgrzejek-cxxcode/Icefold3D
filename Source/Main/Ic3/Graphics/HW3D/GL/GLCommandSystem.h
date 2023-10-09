
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

	ICFGX_GL_API void releaseGLCommandSyncData( void * pSyncData );

	/// @brief
	class ICFGX_GL_CLASS GLCommandSystem : public CommandSystem
	{
	public:
		explicit GLCommandSystem( GLGPUDevice & pGPUDevice );
		virtual ~GLCommandSystem();

		virtual std::unique_ptr<CommandContext> acquireCommandContext( ECommandContextType pContextType ) override;

		virtual CommandSync submitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo ) override;

		// OpenGL-specific thing coming from the fact, that context creation requires target surface (Drawable on X11,
		// HDC on Win32, EGLSurface on Android, etc.). GPUDevice::setPresentationLayer was actually introduced only to
		// make tis possible in at least a bit natural way. So, when PresentationLayer is set, GL-level onSet() gets
		// the surface and passes it here, so CmdManager has the surface when a CmdContext is created.
		// Also, that is the reason why no context is created in initialize() function and the requirement of having
		// a surface bound to the device when acquireContext() is called.
		void setTargetGLSurface( System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface );

	private:
		GLCommandList * acquireCommandList( ECommandExecutionMode pCommandExecutionMode );

		bool initializeMainCommandList();

		static System::OpenGLRenderContextHandle createSysGLRenderContext( GLGPUDevice & pGPUDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface );

	protected:
		System::OpenGLDisplaySurfaceHandle _targetSysGLSurface;
		GLCommandListHandle _mainCommandList;
	};
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_CMD_MANAGER_H__
