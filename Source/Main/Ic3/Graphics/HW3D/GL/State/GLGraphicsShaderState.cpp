
#include "GLGraphicsShaderState.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLShader.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLShaderPipelineObject.h>

namespace Ic3::Graphics::GCI
{

	GLGraphicsShaderLinkageImmutableState::GLGraphicsShaderLinkageImmutableState(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderLinkageCommonProperties & pCommonProperties )
	: GraphicsShaderLinkageImmutableState( pGPUDevice, pCommonProperties )
	{}

	GLGraphicsShaderLinkageImmutableState::~GLGraphicsShaderLinkageImmutableState() = default;


	GLGraphicsShaderLinkageImmutableStateCore::GLGraphicsShaderLinkageImmutableStateCore(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderLinkageCommonProperties & pCommonProperties,
			GLShaderPipelineObjectHandle pGLShaderPipelineObject )
	: GLGraphicsShaderLinkageImmutableState( pGPUDevice, pCommonProperties )
	, mGLShaderPipelineObject( std::move( pGLShaderPipelineObject ) )
	{}

	GLGraphicsShaderLinkageImmutableStateCore::~GLGraphicsShaderLinkageImmutableStateCore() = default;

	TGfxHandle<GLGraphicsShaderLinkageImmutableStateCore> GLGraphicsShaderLinkageImmutableStateCore::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderSet & pShaderSet )
	{
		const auto & commonProperties = SMU::GetGraphicsShaderLinkageCommonPropertiesForShaderSet( pShaderSet );

		auto shaderPipelineObject = SMU::CreateGraphicsShaderPipelineObjectGL( pShaderSet );
		if( !shaderPipelineObject )
		{
			return nullptr;
		}

		auto immutableState = CreateGfxObject<GLGraphicsShaderLinkageImmutableStateCore>(
				pGPUDevice,
				commonProperties,
				std::move( shaderPipelineObject ) );

		return immutableState;
	}


	GLGraphicsShaderLinkageImmutableStateCompat::GLGraphicsShaderLinkageImmutableStateCompat(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderLinkageCommonProperties & pCommonProperties,
			GLShaderProgramObjectHandle pGLShaderProgramObject )
	: GLGraphicsShaderLinkageImmutableState( pGPUDevice, pCommonProperties )
	, mGLShaderProgramObject( std::move( pGLShaderProgramObject ) )
	{}

	GLGraphicsShaderLinkageImmutableStateCompat::~GLGraphicsShaderLinkageImmutableStateCompat() = default;

	TGfxHandle<GLGraphicsShaderLinkageImmutableStateCompat> GLGraphicsShaderLinkageImmutableStateCompat::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderSet & pShaderSet )
	{
		const auto & commonProperties = SMU::GetGraphicsShaderLinkageCommonPropertiesForShaderSet( pShaderSet );

		auto shaderProgramObject = SMU::CreateGraphicsShaderProgramObjectGL( pShaderSet );
		if( !shaderProgramObject )
		{
			return nullptr;
		}

		auto immutableState = CreateGfxObject<GLGraphicsShaderLinkageImmutableStateCompat>(
				pGPUDevice,
				commonProperties,
				std::move( shaderProgramObject ) );

		return immutableState;
	}


	namespace SMU
	{

		GLShaderPipelineObjectHandle CreateGraphicsShaderPipelineObjectGL( const GraphicsShaderSet & pShaderSet )
		{
			auto shaderPipelineObject = GLShaderPipelineObject::Create();
			if( !shaderPipelineObject )
			{
				return nullptr;
			}

			glBindProgramPipeline( shaderPipelineObject->mGLHandle );
			Ic3OpenGLHandleLastError();

			for( auto & shaderHandle : pShaderSet.commonShaderArray )
			{
				if( shaderHandle )
				{
					auto * openglShader = shaderHandle->QueryInterface<GLShader>();
					Ic3DebugAssert( openglShader->mGLShaderProgramObject );
					shaderPipelineObject->AttachProgram( *( openglShader->mGLShaderProgramObject ) );
				}
			}

			glBindProgramPipeline( 0 );
			Ic3OpenGLHandleLastError();

			return shaderPipelineObject;
		}

		GLShaderProgramObjectHandle CreateGraphicsShaderProgramObjectGL( const GraphicsShaderSet & pShaderSet )
		{
			auto shaderProgramObject = GLShaderProgramObject::Create( GLShaderProgramType::Combined );
			if( !shaderProgramObject )
			{
				return nullptr;
			}

			bool useShaderLayoutMaps = false;

			for( auto & shaderHandle : pShaderSet.commonShaderArray )
			{
				if( shaderHandle )
				{
					auto * openglShader = shaderHandle->QueryInterface<GLShader>();
					Ic3DebugAssert( openglShader->mGLShaderObject );
					shaderProgramObject->AttachShader( *( openglShader->mGLShaderObject ) );

					if( const auto * shaderLayoutMap = openglShader->mGLShaderObject->GetDataLayoutMap() )
					{
						GLShaderProgramObject::SetProgramPreLinkBindings( *shaderProgramObject, *shaderLayoutMap );
						useShaderLayoutMaps = true;
					}
				}
			}

			if( !shaderProgramObject->Link() )
			{
				Ic3DebugInterrupt();
				return nullptr;
			}

			if( useShaderLayoutMaps )
			{
				for( auto & shaderHandle : pShaderSet.commonShaderArray )
				{
					if( shaderHandle )
					{
						auto * openglShader = shaderHandle->QueryInterface<GLShader>();
						if( const auto * shaderLayoutMap = openglShader->mGLShaderObject->GetDataLayoutMap() )
						{
							GLShaderProgramObject::SetProgramPostLinkBindings( *shaderProgramObject, *shaderLayoutMap );
						}
					}
				}
			}

			shaderProgramObject->DetachAllShaders();

			return shaderProgramObject;
		}

		void UpdateUniformDataCurrentGL(
				GLShaderPipelineObject & pShaderPipeline,
				uint32 pUniformIndex,
				EBaseDataType pBaseType,
				uint32 pLength,
				const void * pData )
		{
			if( ( pLength == 0 ) || !pData )
			{
				return;
			}

			const auto currentPipelineHandle = GLShaderPipelineObject::QueryCurrentShaderPipelineBinding();
			if( currentPipelineHandle != pShaderPipeline.mGLHandle )
			{
				Ic3Throw( 0 );
			}

			if( pBaseType == EBaseDataType::Float32 )
			{
				const auto * floatData = reinterpret_cast<const float *>( pData );
				switch( pLength )
				{
					case 1: glUniform1fv( pUniformIndex, 1, floatData ); Ic3OpenGLHandleLastError(); break;
					case 2: glUniform2fv( pUniformIndex, 1, floatData ); Ic3OpenGLHandleLastError(); break;
					case 3: glUniform3fv( pUniformIndex, 1, floatData ); Ic3OpenGLHandleLastError(); break;
					case 4: glUniform4fv( pUniformIndex, 1, floatData ); Ic3OpenGLHandleLastError(); break;
				}
			}
			else if( pBaseType == EBaseDataType::Int32 )
			{
				const auto * intData = reinterpret_cast<const GLint *>( pData );
				switch( pLength )
				{
					case 1: glUniform1iv( pUniformIndex, 1, intData ); Ic3OpenGLHandleLastError(); break;
					case 2: glUniform2iv( pUniformIndex, 1, intData ); Ic3OpenGLHandleLastError(); break;
					case 3: glUniform3iv( pUniformIndex, 1, intData ); Ic3OpenGLHandleLastError(); break;
					case 4: glUniform4iv( pUniformIndex, 1, intData ); Ic3OpenGLHandleLastError(); break;
				}
			}
			else if( pBaseType == EBaseDataType::Uint32 )
			{
				const auto * uintData = reinterpret_cast<const GLuint *>( pData );
				switch( pLength )
				{
					case 1: glUniform1uiv( pUniformIndex, 1, uintData ); Ic3OpenGLHandleLastError(); break;
					case 2: glUniform2uiv( pUniformIndex, 1, uintData ); Ic3OpenGLHandleLastError(); break;
					case 3: glUniform3uiv( pUniformIndex, 1, uintData ); Ic3OpenGLHandleLastError(); break;
					case 4: glUniform4uiv( pUniformIndex, 1, uintData ); Ic3OpenGLHandleLastError(); break;
				}
			}
		}

		void UpdateUniformDataExplicitGL(
				GLShaderProgramObject & pShaderProgram,
				uint32 pUniformIndex,
				EBaseDataType pBaseType,
				uint32 pLength,
				const void * pData )
		{
			if( ( pLength == 0 ) || !pData )
			{
				return;
			}

			const auto currentProgramHandle = GLShaderProgramObject::QueryCurrentShaderProgramBinding();
			if( currentProgramHandle != pShaderProgram.mGLHandle )
			{
				Ic3Throw( 0 );
			}

			if( pBaseType == EBaseDataType::Float32 )
			{
				const auto * floatData = reinterpret_cast<const float *>( pData );
				switch( pLength )
				{
					case 1: glProgramUniform1fv( pShaderProgram.mGLHandle, pUniformIndex, 1, floatData ); Ic3OpenGLHandleLastError(); break;
					case 2: glProgramUniform2fv( pShaderProgram.mGLHandle, pUniformIndex, 1, floatData ); Ic3OpenGLHandleLastError(); break;
					case 3: glProgramUniform3fv( pShaderProgram.mGLHandle, pUniformIndex, 1, floatData ); Ic3OpenGLHandleLastError(); break;
					case 4: glProgramUniform4fv( pShaderProgram.mGLHandle, pUniformIndex, 1, floatData ); Ic3OpenGLHandleLastError(); break;
				}
			}
			else if( pBaseType == EBaseDataType::Int32 )
			{
				const auto * intData = reinterpret_cast<const GLint *>( pData );
				switch( pLength )
				{
					case 1: glProgramUniform1iv( pShaderProgram.mGLHandle, pUniformIndex, 1, intData ); Ic3OpenGLHandleLastError(); break;
					case 2: glProgramUniform2iv( pShaderProgram.mGLHandle, pUniformIndex, 1, intData ); Ic3OpenGLHandleLastError(); break;
					case 3: glProgramUniform3iv( pShaderProgram.mGLHandle, pUniformIndex, 1, intData ); Ic3OpenGLHandleLastError(); break;
					case 4: glProgramUniform4iv( pShaderProgram.mGLHandle, pUniformIndex, 1, intData ); Ic3OpenGLHandleLastError(); break;
				}
			}
			else if( pBaseType == EBaseDataType::Uint32 )
			{
				const auto * uintData = reinterpret_cast<const GLuint *>( pData );
				switch( pLength )
				{
					case 1: glProgramUniform1uiv( pShaderProgram.mGLHandle, pUniformIndex, 1, uintData ); Ic3OpenGLHandleLastError(); break;
					case 2: glProgramUniform2uiv( pShaderProgram.mGLHandle, pUniformIndex, 1, uintData ); Ic3OpenGLHandleLastError(); break;
					case 3: glProgramUniform3uiv( pShaderProgram.mGLHandle, pUniformIndex, 1, uintData ); Ic3OpenGLHandleLastError(); break;
					case 4: glProgramUniform4uiv( pShaderProgram.mGLHandle, pUniformIndex, 1, uintData ); Ic3OpenGLHandleLastError(); break;
				}
			}
		}

	}

}
