
#include "GLPresentationLayer.h"
#include "GLCommandList.h"
#include "GLGpuDevice.h"
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
			surfaceCreateInfo.frameGeometry.mPosition = System::cxFramePosAuto;
			surfaceCreateInfo.frameGeometry.size = pPLCreateInfo.screenRect.size;
			surfaceCreateInfo.frameGeometry.mStyle = System::EFrameStyle::Default;
			surfaceCreateInfo.visualConfig = pPLCreateInfo.visualConfig;
			surfaceCreateInfo.flags = 0u;

		#if( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_GL32 )
			surfaceCreateInfo.runtimeVersionDesc.apiVersion.num_major = 3;
			surfaceCreateInfo.runtimeVersionDesc.apiVersion.mNumMinor = 2;
		#elif( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_GL43 )
			surfaceCreateInfo.minimumAPIVersion = pSysGLDriver->GetVersionSupportInfo().apiVersion;
			surfaceCreateInfo.targetAPIClass = System::EOpenGLAPIClass::Desktop;
		#elif( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_ES31 )
			surfaceCreateInfo.runtimeVersionDesc.apiVersion.num_major = 3;
			surfaceCreateInfo.runtimeVersionDesc.apiVersion.mNumMinor = 1;
        #endif

			if( pPLCreateInfo.displayConfigFlags.is_set( E_DISPLAY_CONFIGURATION_FLAG_FULLSCREEN_BIT ) )
			{
				surfaceCreateInfo.flags.set( System::eOpenGLDisplaySurfaceCreateFlagFullscreenBit );
			}

			if( pPLCreateInfo.displayConfigFlags.is_set( E_DISPLAY_CONFIGURATION_FLAG_SYNC_MODE_ADAPTIVE_BIT ) )
			{
				surfaceCreateInfo.flags.set( System::eOpenGLDisplaySurfaceCreateFlagSyncAdaptiveBit );
			}
			else if( pPLCreateInfo.displayConfigFlags.is_set( E_DISPLAY_CONFIGURATION_FLAG_SYNC_MODE_VERTICAL_BIT ) )
			{
				surfaceCreateInfo.flags.set( System::eOpenGLDisplaySurfaceCreateFlagSyncVerticalBit );
			}

			auto sysGLSurface = pSysGLDriver->CreateDisplaySurface( surfaceCreateInfo );

			return sysGLSurface;
		}
		catch ( ... )
		{
			ic3DebugInterrupt();
		}

		return nullptr;
	}

	GLPresentationLayer::GLPresentationLayer( GLGpuDevice & pGpuDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface )
	: PresentationLayer( pGpuDevice )
	, mSysGLDisplaySurface( pSysGLDisplaySurface )
	{ }

	GLPresentationLayer::~GLPresentationLayer() = default;

	System::EventSource * GLPresentationLayer::GetInternalSystemEventSource() const noexcept
	{
		return mSysGLDisplaySurface.get();
	}


	GLScreenPresentationLayer::GLScreenPresentationLayer(
		GLGpuDevice & pGpuDevice,
		System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface,
		RenderTargetBindingImmutableStateHandle pScreenRenderTargetBindingState )
	: GLPresentationLayer( pGpuDevice, pSysGLDisplaySurface )
	, mScreenRenderTargetBindingState( pScreenRenderTargetBindingState )
	{ }

	GLScreenPresentationLayer::~GLScreenPresentationLayer() = default;

	GLScreenPresentationLayerHandle GLScreenPresentationLayer::Create( GLGpuDevice & pDevice, const GLPresentationLayerCreateInfo & pCreateInfo )
	{
		auto sysGLSurface = createSysGLSurface( pDevice.mSysGLDriver, pCreateInfo );
		ic3DebugAssert( sysGLSurface );

		const auto surfaceVisualConfig = sysGLSurface->QueryVisualConfig();
		const auto surfaceSize = sysGLSurface->GetClientAreaSize();

		auto screenRTLayout = SMU::TranslateSystemVisualConfigToRenderTargetLayout( surfaceVisualConfig );
		screenRTLayout.sharedImageRect = { surfaceSize.x, surfaceSize.y };

		auto renderTargetState = GLRenderTargetBindingImmutableState::CreateForScreen( pDevice, screenRTLayout );
		ic3DebugAssert( renderTargetState );
		
		auto presentationLayer = CreateGfxObject<GLScreenPresentationLayer>( pDevice, sysGLSurface, renderTargetState );

		return presentationLayer;
	}

	void GLScreenPresentationLayer::BindRenderTarget( CommandContext * pCmdContext )
	{
		auto * directGraphicsContext = pCmdContext->QueryInterface<CommandContextDirectGraphics>();
		directGraphicsContext->SetRenderTargetBindingState( *mScreenRenderTargetBindingState );

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		ic3OpenGLHandleLastError();

		glDrawBuffer( GL_BACK );
		ic3OpenGLHandleLastError();
	}

	void GLScreenPresentationLayer::InvalidateRenderTarget( CommandContext * pCmdContext )
	{
	}

	void GLScreenPresentationLayer::Present()
	{
		mSysGLDisplaySurface->SwapBuffers();
	}

	void GLScreenPresentationLayer::Resize( uint32 pWidth, uint32 pHeight )
	{
	}

	void GLScreenPresentationLayer::SetFullscreenMode( bool pEnable )
	{
		mSysGLDisplaySurface->SetFullscreenMode( pEnable );
	}

	Ic3::Math::Vec2u32 GLScreenPresentationLayer::QueryRenderTargetSize() const
	{
		return mSysGLDisplaySurface->QueryRenderAreaSize();
	}

} // namespace Ic3::Graphics::GCI
