
#include "GLShader.h"
#include "../GLGPUDevice.h"
#include "../Objects/GLShaderObject.h"
#include "../Objects/GLShaderProgramObject.h"

namespace Ic3::Graphics::GCI
{

	GLShader::GLShader( GLGPUDevice & pGPUDevice, EShaderType pShaderType, GLShaderObjectHandle pGLShaderObject )
	: Shader( pGPUDevice, pShaderType )
	, mGLShaderObject( std::move( pGLShaderObject ) )
	{}

	GLShader::GLShader( GLGPUDevice & pGPUDevice, EShaderType pShaderType, GLShaderProgramObjectHandle pGLShaderProgramObject )
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

	GLShaderHandle GLShader::CreateInstanceFromSource( GLGPUDevice & pGPUDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength )
	{
		const auto openglShaderType = ATL::translateShaderType( pShaderType );
		const auto runtimeVersion = pGPUDevice.mSysGLSupportInfo.apiVersion;

		GLShaderHandle shaderObject = nullptr;

		if( pGPUDevice.IsCompatibilityDevice() )
		{
			GLShaderDataLayoutMap shaderLayoutMap{};
			std::string shaderSource{ reinterpret_cast<const char *>( pSource ), pSourceLength };

			RCU::ProcessGLShaderSourceExplicitLayoutQualifiers( runtimeVersion, shaderSource, shaderLayoutMap );

			auto openglShaderObject = GLShaderObject::CreateWithSource( openglShaderType, shaderSource.data(), shaderSource.length() );
			if( !openglShaderObject )
			{
				return nullptr;
			}

			openglShaderObject->SetDataLayoutMap( std::move( shaderLayoutMap ) );

			shaderObject = std::make_unique<GLShader>( pGPUDevice, pShaderType, std::move( openglShaderObject ) );
		}
		else
		{
			auto openglShaderObject = GLShaderObject::CreateWithSource( openglShaderType, pSource, pSourceLength );
			if( !openglShaderObject )
			{
				return nullptr;
			}

			auto openglProgramObject = GLShaderProgramObject::CreateSeparableModule( *openglShaderObject );
			if( !openglProgramObject )
			{
				return nullptr;
			}

			shaderObject = std::make_unique<GLShader>( pGPUDevice, pShaderType, std::move( openglProgramObject ) );
		}

		return shaderObject;
	}

} // namespace Ic3::Graphics::GCI
