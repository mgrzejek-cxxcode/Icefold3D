
#include "GLCommandSystem.h"
#include "GLCommandList.h"
#include "GLGpuDevice.h"
#include <Ic3/Graphics/GCI/CommandContext.h>
#include <mutex>

namespace Ic3::Graphics::GCI
{

	static void initializeOpenGLAPI()
	{
		static std::atomic_flag sInitFlag = { ATOMIC_FLAG_INIT };

		if( !sInitFlag.test_and_set( std::memory_order_acq_rel ) )
		{
			::glewInit();
		}
	}

	GLCommandSystem::GLCommandSystem( GLGpuDevice & pGpuDevice )
	: CommandSystem( pGpuDevice )
	, _targetSysGLSurface( nullptr )
	{}

	GLCommandSystem::~GLCommandSystem() = default;

	std::unique_ptr<CommandContext> GLCommandSystem::AcquireCommandContext( ECommandContextType pContextType )
	{
		std::unique_ptr<CommandContext> commandContext;

		auto contextExecutionMode = CxDef::GetCommandObjectExecutionMode( pContextType );
		if( auto * commandList = AcquireCommandList( contextExecutionMode ) )
		{
			ic3DebugAssert( contextExecutionMode == ECommandExecutionMode::Direct );
			commandContext = std::make_unique<CommandContextDirectGraphics>( *commandList );
			commandList->mSysGLRenderContext->BindForCurrentThread( *_targetSysGLSurface );

			// A dirty workaround. GLEW is no longer used at the system level (now we use only
			// the minimal set of GLX/WGL functions), so it needs to happen here. This is best
			// to be replaced with a custom API loader/manager.
			initializeOpenGLAPI();
		}

		if( commandContext )
		{
			auto * openglGpuDevice = mGpuDevice.QueryInterface<GLGpuDevice>();
			auto * openglDebugOutput = openglGpuDevice->GetDebugOutputInterface();

			const auto EnableDebugOutput = false;

			if( openglDebugOutput && EnableDebugOutput )
			{
			    openglDebugOutput->EnableDebugOutput( true );
			    openglDebugOutput->EnableBreakOnEvent( true );
			    openglDebugOutput->EnableSync( true );
			}
		}

		return commandContext;
	}

	CommandSync GLCommandSystem::SubmitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo )
	{
		CommandSync cmdSyncObject;

		if( pSubmitInfo.syncMode == ECommandSubmitSyncMode::Default )
		{
			auto * openglCommandSyncData = new GLCommandSyncData();
			cmdSyncObject.syncData = openglCommandSyncData;
			cmdSyncObject.syncDataReleaseFunc = releaseGLCommandSyncData;

			auto syncFence = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
			ic3OpenGLCheckLastResult();

			glFlush();
			ic3OpenGLCheckLastResult();

			openglCommandSyncData->openglSyncFence = syncFence;
		}

		return cmdSyncObject;
	}

	void GLCommandSystem::SetTargetGLSurface( System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface )
	{
		// Surface is required to create a GL context. Since CommandSystem is bound to a device
		// and PresentationLayer is created when device is already available, there is no way
		// we can know the surface in advance. Hence, we set it manually (eh...) and use it to
		// create a GL context when a new CommandList is instantiated.
		_targetSysGLSurface = pSysGLDisplaySurface;
	}

	GLCommandList * GLCommandSystem::AcquireCommandList( ECommandExecutionMode pCommandExecutionMode )
	{
		GLCommandList * commandList = nullptr;

		if( pCommandExecutionMode == ECommandExecutionMode::Direct )
		{
			if( !_mainCommandList )
			{
				InitializeMainCommandList();
			}
			if( _mainCommandList && _mainCommandList->AcquireList() )
			{
				commandList = _mainCommandList.get();
			}
		}

		return commandList;
	}

	bool GLCommandSystem::InitializeMainCommandList()
	{
		ic3DebugAssert( !_mainCommandList );

		auto * openglGpuDevice = mGpuDevice.QueryInterface<GLGpuDevice>();
		ic3DebugAssert( openglGpuDevice );

		auto sysGLRenderContext = GLCommandSystem::CreateSysGLRenderContext( *openglGpuDevice, _targetSysGLSurface );
		ic3DebugAssert( sysGLRenderContext );

		if( openglGpuDevice->IsCompatibilityDevice() )
		{
			_mainCommandList = CreateGfxObject<GLCommandListCompat>( *this, ECommandListType::DirectGraphics, sysGLRenderContext );
		}
		else
		{
			_mainCommandList = CreateGfxObject<GLCommandListCore>( *this, ECommandListType::DirectGraphics, sysGLRenderContext );
		}

		return true;
	}

	System::OpenGLRenderContextHandle GLCommandSystem::CreateSysGLRenderContext( GLGpuDevice & pGpuDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface )
	{
		System::OpenGLRenderContextHandle sysGLRenderContext = nullptr;

		try
		{
			System::OpenGLRenderContextCreateInfo contextCreateInfo;
			contextCreateInfo.shareContext = nullptr;

		#if( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_GL43 )
			contextCreateInfo.requestedAPIVersion.num_major = 4;
			contextCreateInfo.requestedAPIVersion.num_minor = 3;
			contextCreateInfo.contextAPIProfile = System::EOpenGLAPIProfile::Core;
		#elif( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_ES31 )
			contextCreateInfo.runtimeVersionDesc.apiVersion.num_major = 3;
			contextCreateInfo.runtimeVersionDesc.apiVersion.mNumMinor = 1;
			contextCreateInfo.runtimeVersionDesc.apiProfile = System::EGLAPIProfile::OpenGLES;
		#endif

			if( pGpuDevice.IsDebugDevice() )
			{
				contextCreateInfo.flags.set( System::eOpenGLRenderContextCreateFlagEnableDebugBit );
			}

			sysGLRenderContext = pSysGLDisplaySurface->mGLSystemDriver->CreateRenderContext( *pSysGLDisplaySurface, contextCreateInfo );
			sysGLRenderContext->BindForCurrentThread( *pSysGLDisplaySurface );

			auto sysVersionInfo = sysGLRenderContext->QuerySystemVersionInfo();
			auto sysVersionInfoStr = sysVersionInfo.toString();
			ic3DebugOutputFmt( "%s\n", sysVersionInfoStr.c_str() );
		}
		catch ( ... )
		{
			ic3DebugInterrupt();
		}

		return sysGLRenderContext;
	}


	void releaseGLCommandSyncData( void * pSyncData )
	{
		if( pSyncData )
		{
			auto * openglCommandSyncData = static_cast<GLCommandSyncData *>( pSyncData );
			openglCommandSyncData->openglSyncFence = nullptr;
			delete openglCommandSyncData;
		}
	}

} // namespace Ic3::Graphics::GCI
