
#include "GLShader.h"
#include "../GLGPUDevice.h"
#include "../Objects/GLShaderObject.h"
#include "../Objects/GLShaderProgramObject.h"

namespace Ic3::Graphics::GCI
{

	GLShader::GLShader(
			GLGPUDevice & pGPUDevice,
			EShaderType pShaderType,
			GLShaderObjectHandle pGLShaderObject )
	: Shader( pGPUDevice, pShaderType )
	, mGLShaderObject( std::move( pGLShaderObject ) )
	{}

	GLShader::GLShader(
			GLGPUDevice & pGPUDevice,
			EShaderType pShaderType,
			GLShaderProgramObjectHandle pGLShaderProgramObject )
	: Shader( pGPUDevice, pShaderType )
	, mGLShaderProgramObject( std::move( pGLShaderProgramObject ) )
	{}

	GLShader::~GLShader() = default;

	GLShaderHandle GLShader::CreateInstance( GLGPUDevice & pGPUDevice, const ShaderCreateInfo & pCreateInfo )
	{
		return CreateInstanceFromSource(
				pGPUDevice,
				pCreateInfo.shaderType,
				pCreateInfo.shaderSourceView.data(),
				pCreateInfo.shaderSourceView.size() );
	}

	GLShaderHandle GLShader::CreateInstanceFromSource(
			GLGPUDevice & pGPUDevice,
			EShaderType pShaderType,
			const void * pSource,
			size_t pSourceLength )
	{
		const auto openglShaderType = ATL::GLTranslateShaderType( pShaderType );
		const auto runtimeVersion = pGPUDevice.mSysGLSupportInfo.apiVersion;

		std::string shaderSource{ reinterpret_cast<const char *>( pSource ), pSourceLength };

		GLShaderHandle shaderObject = nullptr;

		if( pGPUDevice.IsCompatibilityDevice() )
		{
			auto shaderBindingLayout = std::make_unique<GLShaderBindingLayout>();
			auto shaderInputSignature = std::make_unique<ShaderInputSignature>();

			RCU::ProcessGLShaderSource(
					runtimeVersion,
					shaderSource,
					*shaderInputSignature,
					shaderBindingLayout.get() );

			auto openglShaderObject = GLShaderObject::CreateWithSource( openglShaderType, shaderSource.data(), shaderSource.length() );
			if( !openglShaderObject )
			{
				return nullptr;
			}

			openglShaderObject->SetBindingLayout( std::move( shaderBindingLayout ) );

			shaderObject = std::make_unique<GLShader>( pGPUDevice, pShaderType, std::move( openglShaderObject ) );
			shaderObject->SetShaderInputSignature( std::move( shaderInputSignature ) );
		}
		else
		{
			auto shaderInputSignature = std::make_unique<ShaderInputSignature>();

			RCU::ProcessGLShaderSource(
					runtimeVersion,
					shaderSource,
					*shaderInputSignature );

			auto openglShaderObject = GLShaderObject::CreateWithSource( openglShaderType, shaderSource.data(), shaderSource.length() );
			if( !openglShaderObject )
			{
				return nullptr;
			}

			auto openglProgramObject = GLShaderProgramObject::CreateSeparableModule( *openglShaderObject );
			if( !openglProgramObject )
			{
				return nullptr;
			}

			const auto programBinarySize = openglProgramObject->QueryParameter( GL_PROGRAM_BINARY_LENGTH );

			shaderObject = std::make_unique<GLShader>( pGPUDevice, pShaderType, std::move( openglProgramObject ) );
			shaderObject->SetShaderInputSignature( std::move( shaderInputSignature ) );

			auto * shaderBinary = shaderObject->InitializeShaderBinaryStorage( programBinarySize );
			shaderObject->mGLShaderProgramObject->GetBinary( *shaderBinary );
		}

		return shaderObject;
	}

} // namespace Ic3::Graphics::GCI
