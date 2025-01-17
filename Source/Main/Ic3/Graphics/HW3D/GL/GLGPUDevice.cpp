
#include "GLGPUDevice.h"
#include "GLGPUDriver.h"
#include "GLCommandSystem.h"
#include "GLPresentationLayer.h"
#include "Resources/GLGPUBuffer.h"
#include "Resources/GLSampler.h"
#include "Resources/GLShader.h"
#include "Resources/GLTexture.h"
#include "State/GLPipelineStateObject.h"
#include "State/GLPipelineStateController.h"

namespace Ic3::Graphics::GCI
{

	GLGPUDevice::GLGPUDevice( GLGPUDriver & pGPUDriver, GLPipelineImmutableStateFactory & pImmutableStateFactory )
	: GPUDevice( pGPUDriver )
	, mSysGLDriver( pGPUDriver.mSysGLDriver )
	, mSysGLSupportInfo( mSysGLDriver->GetVersionSupportInfo() )
	, mGLRuntimeSupportFlags( QueryGLRuntimeSupportFlags( mSysGLSupportInfo ) )
	, _immutableStateFactoryGL( &pImmutableStateFactory )
	, _immutableStateCache( pImmutableStateFactory )
	{
		SetImmutableStateCache( _immutableStateCache );
	}

	GLGPUDevice::~GLGPUDevice() = default;

	GLDebugOutput * GLGPUDevice::GetDebugOutputInterface() const
	{
		return _glDebugOutput.get();
	}

	void GLGPUDevice::WaitForCommandSync( CommandSync & pCommandSync )
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

	bool GLGPUDevice::InitializeGLDebugOutput()
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

	void GLGPUDevice::InitializeCommandSystem()
	{
		Ic3DebugAssert( !_commandSystem );
		_commandSystem = CreateGfxObject<GLCommandSystem>( *this );
	}

	bool GLGPUDevice::_DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer )
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

	GPUBufferHandle GLGPUDevice::_DrvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo )
	{
	    auto glcBuffer = GLGPUBuffer::CreateInstance( *this, pCreateInfo );
	    Ic3DebugAssert( glcBuffer );
	    return glcBuffer;
	}

	SamplerHandle GLGPUDevice::_DrvCreateSampler( const SamplerCreateInfo & pCreateInfo )
	{
	    auto glcSampler = GLSampler::CreateSampler( *this, pCreateInfo );
	    Ic3DebugAssert( glcSampler );
	    return glcSampler;
	}

	ShaderHandle GLGPUDevice::_DrvCreateShader( const ShaderCreateInfo & pCreateInfo )
	{
		ShaderHandle glcShader;
		glcShader = GLShader::CreateInstance( *this, pCreateInfo );
	    return glcShader;
	}

	TextureHandle GLGPUDevice::_DrvCreateTexture( const TextureCreateInfo & pCreateInfo )
	{
	    auto glcTexture = GLTexture::CreateDefault( *this, pCreateInfo );
	    Ic3DebugAssert( glcTexture );
	    return glcTexture;
	}

	RenderTargetTextureHandle GLGPUDevice::_DrvCreateRenderTargetTexture(
			const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		auto glcRTTextureView = GLTexture::CreateRenderTargetTextureView( *this, pCreateInfo );
		Ic3DebugAssert( glcRTTextureView );
		return glcRTTextureView;
	}

	GraphicsPipelineStateObjectHandle GLGPUDevice::_DrvCreateGraphicsPipelineStateObject(
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


	GLGPUDeviceCore::GLGPUDeviceCore( GLGPUDriver & pGPUDriver )
	: GLGPUDevice( pGPUDriver, _immutableStateFactoryCore )
	, _immutableStateFactoryCore( *this )
	{}

	GLGPUDeviceCore::~GLGPUDeviceCore() = default;

	bool GLGPUDeviceCore::IsCompatibilityDevice() const noexcept
	{
		return false;
	}


	GLGPUDeviceCompat::GLGPUDeviceCompat( GLGPUDriver & pGPUDriver )
	: GLGPUDevice( pGPUDriver, _immutableStateFactoryCompat )
	, _immutableStateFactoryCompat( *this )
	{}

	GLGPUDeviceCompat::~GLGPUDeviceCompat() = default;

	bool GLGPUDeviceCompat::IsCompatibilityDevice() const noexcept
	{
		return true;
	}


} // namespace Ic3::Graphics::GCI
