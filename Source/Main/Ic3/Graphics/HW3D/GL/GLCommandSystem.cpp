
#include "GLCommandSystem.h"
#include "GLCommandList.h"
#include "GLGPUDevice.h"
#include <Ic3/Graphics/GCI/CommandContext.h>
#include <mutex>

namespace Ic3::Graphics::GCI
{

	static void InitializeOpenGLAPI()
	{
		static std::atomic_flag sInitFlag = { ATOMIC_FLAG_INIT };

		if( !sInitFlag.test_and_set( std::memory_order_acq_rel ) )
		{
			::glewInit();
		}
	}

	GLCommandSystem::GLCommandSystem( GLGPUDevice & pGPUDevice )
	: CommandSystem( pGPUDevice )
	, _targetSysGLSurface( nullptr )
	{}

	GLCommandSystem::~GLCommandSystem() = default;

	std::unique_ptr<CommandContext> GLCommandSystem::AcquireCommandContext( ECommandContextType pContextType )
	{
		std::unique_ptr<CommandContext> commandContext;

		const auto contextExecutionMode = CXU::GetCommandObjectExecutionMode( pContextType );
		if( auto * commandList = AcquireCommandList( contextExecutionMode ) )
		{
			Ic3DebugAssert( contextExecutionMode == ECommandExecutionMode::Direct );
			commandContext = std::make_unique<CommandContextDirectGraphics>( *commandList );
			commandList->mSysGLRenderContext->BindForCurrentThread( *_targetSysGLSurface );

			// A dirty workaround. GLEW is no longer used at the system level (now we use only
			// the minimal set of GLX/WGL functions), so it needs to happen here. This is best
			// to be replaced with a custom API loader/manager.
			InitializeOpenGLAPI();
		}

	#if( IC3_GX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
		if( commandContext )
		{
			const auto * openglGPUDevice = mGPUDevice.QueryInterface<GLGPUDevice>();
			if( auto * openglDebugOutput = openglGPUDevice->GetDebugOutputInterface() )
			{
			    openglDebugOutput->EnableDebugOutput( true );
			    openglDebugOutput->EnableBreakOnEvent( true );
			    openglDebugOutput->EnableSync( true );
			}
		}
	#endif

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
			Ic3OpenGLCheckLastResult();

			glFlush();
			Ic3OpenGLCheckLastResult();

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
		Ic3DebugAssert( !_mainCommandList );

		auto * openglGPUDevice = mGPUDevice.QueryInterface<GLGPUDevice>();
		Ic3DebugAssert( openglGPUDevice );

		auto sysGLRenderContext = GLCommandSystem::CreateSysGLRenderContext( *openglGPUDevice, _targetSysGLSurface );
		Ic3DebugAssert( sysGLRenderContext );

		if( openglGPUDevice->IsCompatibilityDevice() )
		{
			_mainCommandList = CreateGfxObject<GLCommandListCompat>( *this, ECommandListType::DirectGraphics, sysGLRenderContext );
		}
		else
		{
			_mainCommandList = CreateGfxObject<GLCommandListCore>( *this, ECommandListType::DirectGraphics, sysGLRenderContext );
		}

		return true;
	}

	System::OpenGLRenderContextHandle GLCommandSystem::CreateSysGLRenderContext( GLGPUDevice & pGPUDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface )
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

			if( pGPUDevice.IsDebugDevice() )
			{
				contextCreateInfo.flags.set( System::eOpenGLRenderContextCreateFlagEnableDebugBit );
			}

			sysGLRenderContext = pSysGLDisplaySurface->mGLSystemDriver->CreateRenderContext( *pSysGLDisplaySurface, contextCreateInfo );
			sysGLRenderContext->BindForCurrentThread( *pSysGLDisplaySurface );

			auto sysVersionInfo = sysGLRenderContext->QuerySystemVersionInfo();
			auto sysVersionInfoStr = sysVersionInfo.ToString();
			Ic3DebugOutputFmt( "%s\n", sysVersionInfoStr.c_str() );
		}
		catch ( ... )
		{
			Ic3DebugInterrupt();
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
