
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

			if( pPLCreateInfo.displayConfigFlags.is_set( eDisplayConfigurationFlagFullscreenBit ) )
			{
				surfaceCreateInfo.flags.set( System::eOpenGLDisplaySurfaceCreateFlagFullscreenBit );
			}

			if( pPLCreateInfo.displayConfigFlags.is_set( eDisplayConfigurationFlagSyncModeAdaptiveBit ) )
			{
				surfaceCreateInfo.flags.set( System::eOpenGLDisplaySurfaceCreateFlagSyncAdaptiveBit );
			}
			else if( pPLCreateInfo.displayConfigFlags.is_set( eDisplayConfigurationFlagSyncModeVerticalBit ) )
			{
				surfaceCreateInfo.flags.set( System::eOpenGLDisplaySurfaceCreateFlagSyncVerticalBit );
			}

			auto sysGLSurface = pSysGLDriver->CreateDisplaySurface( surfaceCreateInfo );

			return sysGLSurface;
		}
		catch ( ... )
		{
			Ic3DebugInterrupt();
		}

		return nullptr;
	}

	GLPresentationLayer::GLPresentationLayer( GLGPUDevice & pGPUDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface )
	: PresentationLayer( pGPUDevice )
	, mSysGLDisplaySurface( pSysGLDisplaySurface )
	{ }

	GLPresentationLayer::~GLPresentationLayer() = default;

	System::EventSource * GLPresentationLayer::GetInternalSystemEventSource() const noexcept
	{
		return mSysGLDisplaySurface.get();
	}


	GLScreenPresentationLayer::GLScreenPresentationLayer(
			GLGPUDevice & pGPUDevice,
			System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface,
			GLRenderTargetDescriptorHandle pScreenRenderTargetDescriptor )
	: GLPresentationLayer( pGPUDevice, pSysGLDisplaySurface )
	, mScreenRenderTargetDescriptor( pScreenRenderTargetDescriptor )
	{ }

	GLScreenPresentationLayer::~GLScreenPresentationLayer() = default;

	GLScreenPresentationLayerHandle GLScreenPresentationLayer::Create( GLGPUDevice & pGPUDevice, const GLPresentationLayerCreateInfo & pCreateInfo )
	{
		auto sysGLSurface = createSysGLSurface( pGPUDevice.mSysGLDriver, pCreateInfo );
		Ic3DebugAssert( sysGLSurface );

		const auto surfaceVisualConfig = sysGLSurface->QueryVisualConfig();
		const auto surfaceSize = sysGLSurface->GetClientAreaSize();

		auto rtoLayoutForScreen = GCU::TranslateSystemVisualConfigToRenderTargetLayout( surfaceVisualConfig );
		rtoLayoutForScreen.sharedImageSize = { surfaceSize.x, surfaceSize.y };
		rtoLayoutForScreen.sharedMultiSamplingSettings.sampleCount = pCreateInfo.visualConfig.msaaDesc.bufferCount;
		rtoLayoutForScreen.sharedMultiSamplingSettings.sampleQuality = pCreateInfo.visualConfig.msaaDesc.quality;

		auto renderTargetDescriptor = GLRenderTargetDescriptor::CreateForScreen(
				pGPUDevice,
				kSDIRenderTargetDescriptorScreenDefault,
				rtoLayoutForScreen );
		Ic3DebugAssert( renderTargetDescriptor );
		
		auto presentationLayer = CreateGfxObject<GLScreenPresentationLayer>( pGPUDevice, sysGLSurface, renderTargetDescriptor );

		return presentationLayer;
	}

	void GLScreenPresentationLayer::BindRenderTarget( CommandContext & pCommandContext )
	{
		auto * directGraphicsContext = pCommandContext.QueryInterface<CommandContextDirectGraphics>();
		directGraphicsContext->SetRenderTargetDescriptor( *mScreenRenderTargetDescriptor );

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		Ic3OpenGLHandleLastError();

		glDrawBuffer( GL_BACK );
		Ic3OpenGLHandleLastError();
	}

	void GLScreenPresentationLayer::InvalidateRenderTarget( CommandContext & pCommandContext )
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
