
#include "GLGpuDevice.h"
#include "GLGpuDriver.h"
#include "GLCommandSystem.h"
#include "GLPresentationLayer.h"
#include "Resources/GLGpuBuffer.h"
#include "Resources/GLSampler.h"
#include "Resources/GLShader.h"
#include "Resources/GLTexture.h"
#include "State/GLPipelineStateObject.h"
#include "State/GLPipelineStateController.h"

namespace Ic3::Graphics::GCI
{

	GLGpuDevice::GLGpuDevice( GLGpuDriver & pGpuDriver, GLPipelineImmutableStateFactory & pImmutableStateFactory )
	: GpuDevice( pGpuDriver )
	, mSysGLDriver( pGpuDriver.mSysGLDriver )
	, mSysGLSupportInfo( mSysGLDriver->GetVersionSupportInfo() )
	, mGLRuntimeSupportFlags( QueryGLRuntimeSupportFlags( mSysGLSupportInfo ) )
	, _immutableStateFactoryGL( &pImmutableStateFactory )
	, _immutableStateCache( pImmutableStateFactory )
	{
		SetImmutableStateCache( _immutableStateCache );
	}

	GLGpuDevice::~GLGpuDevice() = default;

	GLDebugOutput * GLGpuDevice::GetDebugOutputInterface() const
	{
		return _glDebugOutput.get();
	}

	void GLGpuDevice::WaitForCommandSync( CommandSync & pCommandSync )
	{
		if( pCommandSync )
		{
			auto * glcCommandSyncData = static_cast<GLCommandSyncData *>( pCommandSync.syncData );

			glClientWaitSync( glcCommandSyncData->openglSyncFence, 0, cppx::meta::limits<GLuint64>::max_value );
			Ic3OpenGLCheckLastResult();

			releaseGLCommandSyncData( pCommandSync.syncData );
			pCommandSync.syncData = nullptr;
		}
	}

	bool GLGpuDevice::InitializeGLDebugOutput()
	{
		if( !_glDebugOutput )
		{
			auto glcDebugOutput = GLDebugOutput::CreateInterface( GLDebugOutputVersion::ARBExt );
			if( glcDebugOutput )
			{
				_glDebugOutput = std::move( glcDebugOutput );
			}
		}
		return _glDebugOutput ? true : false;
	}

	void GLGpuDevice::InitializeCommandSystem()
	{
		Ic3DebugAssert( !_commandSystem );
		_commandSystem = CreateGfxObject<GLCommandSystem>( *this );
	}

	bool GLGpuDevice::_DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{
	    auto * glcPresentationLayer = pPresentationLayer->QueryInterface<GLPresentationLayer>();
	    if ( !glcPresentationLayer->mSysGLDisplaySurface )
	    {
	        return false;
	    }

	    auto * glcCommandSystem = _commandSystem->QueryInterface<GLCommandSystem>();
	    glcCommandSystem->SetTargetGLSurface( glcPresentationLayer->mSysGLDisplaySurface );

	    return true;
	}

	GpuBufferHandle GLGpuDevice::_DrvCreateGpuBuffer( const GpuBufferCreateInfo & pCreateInfo )
	{
	    auto glcBuffer = GLGpuBuffer::CreateInstance( *this, pCreateInfo );
	    Ic3DebugAssert( glcBuffer );
	    return glcBuffer;
	}

	SamplerHandle GLGpuDevice::_DrvCreateSampler( const SamplerCreateInfo & pCreateInfo )
	{
	    auto glcSampler = GLSampler::CreateSampler( *this, pCreateInfo );
	    Ic3DebugAssert( glcSampler );
	    return glcSampler;
	}

	ShaderHandle GLGpuDevice::_DrvCreateShader( const ShaderCreateInfo & pCreateInfo )
	{
		ShaderHandle glcShader;
		glcShader = GLShader::CreateInstance( *this, pCreateInfo );
	    return glcShader;
	}

	TextureHandle GLGpuDevice::_DrvCreateTexture( const TextureCreateInfo & pCreateInfo )
	{
	    auto glcTexture = GLTexture::CreateDefault( *this, pCreateInfo );
	    Ic3DebugAssert( glcTexture );
	    return glcTexture;
	}

	RenderTargetTextureHandle GLGpuDevice::_DrvCreateRenderTargetTexture(
			const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		auto glcRTTextureView = GLTexture::CreateRenderTargetTextureView( *this, pCreateInfo );
		Ic3DebugAssert( glcRTTextureView );
		return glcRTTextureView;
	}

	GraphicsPipelineStateObjectHandle GLGpuDevice::_DrvCreateGraphicsPipelineStateObject(
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		if( !pCreateInfo.shaderLinkageState )
		{
			pCreateInfo.shaderLinkageState = _immutableStateFactoryGL->CreateGraphicsShaderLinkageState( pCreateInfo.shaderSet );
		}

		auto glcGraphicsPSO = GLGraphicsPipelineStateObject::Create( *this, pCreateInfo );
		Ic3DebugAssert( glcGraphicsPSO );

		return glcGraphicsPSO;
	}


	GLGpuDeviceCore::GLGpuDeviceCore( GLGpuDriver & pGpuDriver )
	: GLGpuDevice( pGpuDriver, _immutableStateFactoryCore )
	, _immutableStateFactoryCore( *this )
	{}

	GLGpuDeviceCore::~GLGpuDeviceCore() = default;

	bool GLGpuDeviceCore::IsCompatibilityDevice() const noexcept
	{
		return false;
	}


	GLGpuDeviceCompat::GLGpuDeviceCompat( GLGpuDriver & pGpuDriver )
	: GLGpuDevice( pGpuDriver, _immutableStateFactoryCompat )
	, _immutableStateFactoryCompat( *this )
	{}

	GLGpuDeviceCompat::~GLGpuDeviceCompat() = default;

	bool GLGpuDeviceCompat::IsCompatibilityDevice() const noexcept
	{
		return true;
	}


} // namespace Ic3::Graphics::GCI
