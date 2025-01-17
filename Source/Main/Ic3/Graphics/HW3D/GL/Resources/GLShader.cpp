
#include "GLShader.h"
#include "../GLGpuDevice.h"
#include "../Objects/GLShaderObject.h"
#include "../Objects/GLShaderProgramObject.h"

namespace Ic3::Graphics::GCI
{

	GLShader::GLShader( GLGpuDevice & pGpuDevice, EShaderType pShaderType, GLShaderObjectHandle pGLShaderObject )
	: Shader( pGpuDevice, pShaderType )
	, mGLShaderObject( std::move( pGLShaderObject ) )
	{}

	GLShader::GLShader( GLGpuDevice & pGpuDevice, EShaderType pShaderType, GLShaderProgramObjectHandle pGLShaderProgramObject )
	: Shader( pGpuDevice, pShaderType )
	, mGLShaderProgramObject( std::move( pGLShaderProgramObject ) )
	{}

	GLShader::~GLShader() = default;

	GLShaderHandle GLShader::CreateInstance( GLGpuDevice & pGpuDevice, const ShaderCreateInfo & pCreateInfo )
	{
		return CreateInstanceFromSource(
			pGpuDevice,
			pCreateInfo.shaderType,
			pCreateInfo.shaderSourceView.data(),
			pCreateInfo.shaderSourceView.size() );
	}

	GLShaderHandle GLShader::CreateInstanceFromSource( GLGpuDevice & pGpuDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength )
	{
		const auto openglShaderType = ATL::translateShaderType( pShaderType );
		const auto runtimeVersion = pGpuDevice.mSysGLSupportInfo.apiVersion;

		GLShaderHandle shaderObject = nullptr;

		if( pGpuDevice.IsCompatibilityDevice() )
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

			shaderObject = std::make_unique<GLShader>( pGpuDevice, pShaderType, std::move( openglShaderObject ) );
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

			shaderObject = std::make_unique<GLShader>( pGpuDevice, pShaderType, std::move( openglProgramObject ) );
		}

		return shaderObject;
	}

} // namespace Ic3::Graphics::GCI
