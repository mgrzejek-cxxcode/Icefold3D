
#include "GLGPUDevice.h"
#include "GLGPUDriver.h"
#include "GLCommandSystem.h"
#include "GLPresentationLayer.h"
#include "Resources/GLGPUBuffer.h"
#include "Resources/GLSampler.h"
#include "Resources/GLShader.h"
#include "Resources/GLTexture.h"
#include "State/GLGraphicsPipelineStateObject.h"
#include "State/GLGraphicsPipelineStateController.h"

namespace Ic3::Graphics::GCI
{

	GLGPUDevice::GLGPUDevice( GLGPUDriver & pGPUDriver, GLPipelineStateDescriptorFactory & pCompiledDescriptorFactory )
	: GPUDevice( pGPUDriver, _glcDeviceFeatureQueryInterface )
	, mSysGLDriver( pGPUDriver.mSysGLDriver )
	, mSysGLSupportInfo( mSysGLDriver->GetVersionSupportInfo() )
	, mGLRuntimeSupportFlags( QueryGLRuntimeSupportFlags( mSysGLSupportInfo ) )
	, _glcPipelineStateDescriptorFactory( &pCompiledDescriptorFactory )
	{}

	GLGPUDevice::~GLGPUDevice() = default;

	GLDebugOutput * GLGPUDevice::GetDebugOutputInterface() const
	{
		return _glcDebugOutput.get();
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
		if( !_glcDebugOutput )
		{
			auto glcDebugOutput = GLDebugOutput::CreateInterface( GLDebugOutputVersion::ARBExt );
			if( glcDebugOutput )
			{
				_glcDebugOutput = std::move( glcDebugOutput );
			}
		}
		return _glcDebugOutput ? true : false;
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
		auto glcGraphicsPSO = GLGraphicsPipelineStateObject::Create( *this, pCreateInfo );
		Ic3DebugAssert( glcGraphicsPSO );

		return glcGraphicsPSO;
	}


	GLGPUDeviceCore::GLGPUDeviceCore( GLGPUDriver & pGPUDriver )
	: GLGPUDevice( pGPUDriver, _immutableDescriptorFactoryCore )
	, _immutableDescriptorFactoryCore( *this )
	{}

	GLGPUDeviceCore::~GLGPUDeviceCore() = default;

	bool GLGPUDeviceCore::IsCompatibilityDevice() const noexcept
	{
		return false;
	}


	GLGPUDeviceCompat::GLGPUDeviceCompat( GLGPUDriver & pGPUDriver )
	: GLGPUDevice( pGPUDriver, _immutableDescriptorFactoryCompat )
	, _immutableDescriptorFactoryCompat( *this )
	{}

	GLGPUDeviceCompat::~GLGPUDeviceCompat() = default;

	bool GLGPUDeviceCompat::IsCompatibilityDevice() const noexcept
	{
		return true;
	}


	MultiSamplingSettingsList GLGPUDeviceFeatureQuery::EnumSupportedMultisamplingConfigs( ETextureFormat pFormat ) const noexcept
	{
		const auto textureFormatFlags = CXU::GetTextureFormatFlags( pFormat );

		GLint maxSamplesNum = 0;
		if( !textureFormatFlags.is_set( eGPUDataFormatFlagCompressedBit ) )
		{
		#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_DESKTOP )
			if( textureFormatFlags.is_set_any_of( eGPUDataFormatMaskDepthStencilBit ) )
			{
				glGetIntegerv( GL_MAX_DEPTH_TEXTURE_SAMPLES, &maxSamplesNum );
				Ic3OpenGLHandleLastError();
			}
			else
			{
				glGetIntegerv( GL_MAX_COLOR_TEXTURE_SAMPLES, &maxSamplesNum );
				Ic3OpenGLHandleLastError();
			}
		#endif

		#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
			glGetIntegerv( GL_MAX_SAMPLES, &maxSamplesNum );
			Ic3OpenGLHandleLastError();
		#endif
		}

		MultiSamplingSettingsList msSettingsList{};

		const auto multiSamplingConfigsNum = ( maxSamplesNum > 1 ) ? cppx::pop_count( static_cast<uint32>( maxSamplesNum - 1 ) ) : 0u;
		if( multiSamplingConfigsNum > 0 )
		{
			msSettingsList.reserve( multiSamplingConfigsNum );
			for( GLint samplesNum = 2; samplesNum <= maxSamplesNum; samplesNum <<= 2 )
			{
				auto & multiSamplingSettings = msSettingsList.emplace_back();
				multiSamplingSettings.sampleCount = static_cast<uint16>( samplesNum );
				multiSamplingSettings.sampleQuality = 1;
			}
		}

		return msSettingsList;
	}


} // namespace Ic3::Graphics::GCI
