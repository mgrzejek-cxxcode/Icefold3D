
#include "GLPresentationLayer.h"
#include "GLCommandList.h"
#include "GLGPUDevice.h"
#include <Ic3/Graphics/GCI/CommandContext.h>

namespace Ic3::Graphics::GCI
{

	static System::OpenGLDisplaySurfaceHandle createSysGLSurface(
			System::OpenGLSystemDriverHandle pSysGLDriver,
			const GLPresentationLayerCreateInfo & pPLCreateInfo )
	{
		try
		{
		    System::OpenGLDisplaySurfaceCreateInfo surfaceCreateInfo;
			surfaceCreateInfo.frameGeometry.position = System::CX_FRAME_POS_AUTO;
			surfaceCreateInfo.frameGeometry.size = pPLCreateInfo.screenRect.size;
			surfaceCreateInfo.frameGeometry.style = System::EFrameStyle::Default;
			surfaceCreateInfo.visualConfig = pPLCreateInfo.visualConfig;
			surfaceCreateInfo.flags = 0u;

		#if( ICFGX_GL_TARGET == ICFGX_GL_TARGET_GL32 )
			surfaceCreateInfo.runtimeVersionDesc.apiVersion.major = 3;
			surfaceCreateInfo.runtimeVersionDesc.apiVersion.minor = 2;
		#elif( ICFGX_GL_TARGET == ICFGX_GL_TARGET_GL43 )
			surfaceCreateInfo.minimumAPIVersion = pSysGLDriver->getVersionSupportInfo().apiVersion;
			surfaceCreateInfo.targetAPIClass = System::EOpenGLAPIClass::OpenGLDesktop;
		#elif( ICFGX_GL_TARGET == ICFGX_GL_TARGET_ES31 )
			surfaceCreateInfo.runtimeVersionDesc.apiVersion.major = 3;
			surfaceCreateInfo.runtimeVersionDesc.apiVersion.minor = 1;
        #endif

			if( pPLCreateInfo.displayConfigFlags.isSet( E_DISPLAY_CONFIGURATION_FLAG_FULLSCREEN_BIT ) )
			{
				surfaceCreateInfo.flags.set( System::E_OPENGL_DISPLAY_SURFACE_CREATE_FLAG_FULLSCREEN_BIT );
			}

			if( pPLCreateInfo.displayConfigFlags.isSet( E_DISPLAY_CONFIGURATION_FLAG_SYNC_MODE_ADAPTIVE_BIT ) )
			{
				surfaceCreateInfo.flags.set( System::E_OPENGL_DISPLAY_SURFACE_CREATE_FLAG_SYNC_ADAPTIVE_BIT );
			}
			else if( pPLCreateInfo.displayConfigFlags.isSet( E_DISPLAY_CONFIGURATION_FLAG_SYNC_MODE_VERTICAL_BIT ) )
			{
				surfaceCreateInfo.flags.set( System::E_OPENGL_DISPLAY_SURFACE_CREATE_FLAG_SYNC_VERTICAL_BIT );
			}

			auto sysGLSurface = pSysGLDriver->createDisplaySurface( surfaceCreateInfo );

			return sysGLSurface;
		}
		catch ( ... )
		{
			ic3DebugInterrupt();
		}

		return nullptr;
	}

	GLPresentationLayer::GLPresentationLayer( GLGPUDevice & pGPUDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface )
	: PresentationLayer( pGPUDevice )
	, mSysGLDisplaySurface( pSysGLDisplaySurface )
	{ }

	GLPresentationLayer::~GLPresentationLayer() = default;

	System::EventSource * GLPresentationLayer::getInternalSystemEventSource() const noexcept
	{
		return mSysGLDisplaySurface.get();
	}


	GLScreenPresentationLayer::GLScreenPresentationLayer(
		GLGPUDevice & pGPUDevice,
		System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface,
		RenderTargetBindingImmutableStateHandle pScreenRenderTargetBindingState )
	: GLPresentationLayer( pGPUDevice, pSysGLDisplaySurface )
	, mScreenRenderTargetBindingState( pScreenRenderTargetBindingState )
	{ }

	GLScreenPresentationLayer::~GLScreenPresentationLayer() = default;

	GLScreenPresentationLayerHandle GLScreenPresentationLayer::create( GLGPUDevice & pDevice, const GLPresentationLayerCreateInfo & pCreateInfo )
	{
		auto sysGLSurface = createSysGLSurface( pDevice.mSysGLDriver, pCreateInfo );
		ic3DebugAssert( sysGLSurface );

		const auto surfaceVisualConfig = sysGLSurface->queryVisualConfig();
		const auto surfaceSize = sysGLSurface->getClientAreaSize();

		auto screenRTLayout = smutil::translateSystemVisualConfigToRenderTargetLayout( surfaceVisualConfig );
		screenRTLayout.sharedImageRect = { surfaceSize.x, surfaceSize.y };

		auto renderTargetState = GLRenderTargetBindingImmutableState::createForScreen( pDevice, screenRTLayout );
		ic3DebugAssert( renderTargetState );
		
		auto presentationLayer = createGPUAPIObject<GLScreenPresentationLayer>( pDevice, sysGLSurface, renderTargetState );

		return presentationLayer;
	}

	void GLScreenPresentationLayer::bindRenderTarget( CommandContext * pCmdContext )
	{
		auto * directGraphicsContext = pCmdContext->queryInterface<CommandContextDirectGraphics>();
		directGraphicsContext->setRenderTargetBindingState( *mScreenRenderTargetBindingState );

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		ic3OpenGLHandleLastError();

		glDrawBuffer( GL_BACK );
		ic3OpenGLHandleLastError();
	}

	void GLScreenPresentationLayer::invalidateRenderTarget( CommandContext * pCmdContext )
	{
	}

	void GLScreenPresentationLayer::present()
	{
		mSysGLDisplaySurface->swapBuffers();
	}

	void GLScreenPresentationLayer::resize( uint32 pWidth, uint32 pHeight )
	{
	}

	void GLScreenPresentationLayer::setFullscreenMode( bool pEnable )
	{
		mSysGLDisplaySurface->setFullscreenMode( pEnable );
	}

	Ic3::Math::Vec2u32 GLScreenPresentationLayer::queryRenderTargetSize() const
	{
		return mSysGLDisplaySurface->queryRenderAreaSize();
	}

} // namespace Ic3::Graphics::GCI
