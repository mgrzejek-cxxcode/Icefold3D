
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

	GpaHandle<GLGraphicsShaderLinkageImmutableStateCore> GLGraphicsShaderLinkageImmutableStateCore::createInstance(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderSet & pShaderSet )
	{
		const auto & commonProperties = smutil::getGraphicsShaderLinkageCommonPropertiesForShaderSet( pShaderSet );

		auto shaderPipelineObject = smutil::createGraphicsShaderPipelineObjectGL( pShaderSet );
		if( !shaderPipelineObject )
		{
			return nullptr;
		}

		auto immutableState = createGPUAPIObject<GLGraphicsShaderLinkageImmutableStateCore>(
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

	GpaHandle<GLGraphicsShaderLinkageImmutableStateCompat> GLGraphicsShaderLinkageImmutableStateCompat::createInstance(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderSet & pShaderSet )
	{
		const auto & commonProperties = smutil::getGraphicsShaderLinkageCommonPropertiesForShaderSet( pShaderSet );

		auto shaderProgramObject = smutil::createGraphicsShaderProgramObjectGL( pShaderSet );
		if( !shaderProgramObject )
		{
			return nullptr;
		}

		auto immutableState = createGPUAPIObject<GLGraphicsShaderLinkageImmutableStateCompat>(
				pGPUDevice,
				commonProperties,
				std::move( shaderProgramObject ) );

		return immutableState;
	}


	namespace smutil
	{

		GLShaderPipelineObjectHandle createGraphicsShaderPipelineObjectGL( const GraphicsShaderSet & pShaderSet )
		{
			auto shaderPipelineObject = GLShaderPipelineObject::create();
			if( !shaderPipelineObject )
			{
				return nullptr;
			}

			glBindProgramPipeline( shaderPipelineObject->mGLHandle );
			ic3OpenGLHandleLastError();

			for( auto & shaderHandle : pShaderSet.commonShaderArray )
			{
				if( shaderHandle )
				{
					auto * openglShader = shaderHandle->queryInterface<GLShader>();
					ic3DebugAssert( openglShader->mGLShaderProgramObject );
					shaderPipelineObject->attachProgram( *( openglShader->mGLShaderProgramObject ) );
				}
			}

			glBindProgramPipeline( 0 );
			ic3OpenGLHandleLastError();

			return shaderPipelineObject;
		}

		GLShaderProgramObjectHandle createGraphicsShaderProgramObjectGL( const GraphicsShaderSet & pShaderSet )
		{
			auto shaderProgramObject = GLShaderProgramObject::create( GLShaderProgramType::Combined );
			if( !shaderProgramObject )
			{
				return nullptr;
			}

			bool useShaderLayoutMaps = false;

			for( auto & shaderHandle : pShaderSet.commonShaderArray )
			{
				if( shaderHandle )
				{
					auto * openglShader = shaderHandle->queryInterface<GLShader>();
					ic3DebugAssert( openglShader->mGLShaderObject );
					shaderProgramObject->attachShader( *( openglShader->mGLShaderObject ) );

					if( const auto * shaderLayoutMap = openglShader->mGLShaderObject->getDataLayoutMap() )
					{
						GLShaderProgramObject::setProgramPreLinkBindings( *shaderProgramObject, *shaderLayoutMap );
						useShaderLayoutMaps = true;
					}
				}
			}

			if( !shaderProgramObject->link() )
			{
				ic3DebugInterrupt();
				return nullptr;
			}

			if( useShaderLayoutMaps )
			{
				for( auto & shaderHandle : pShaderSet.commonShaderArray )
				{
					if( shaderHandle )
					{
						auto * openglShader = shaderHandle->queryInterface<GLShader>();
						if( const auto * shaderLayoutMap = openglShader->mGLShaderObject->getDataLayoutMap() )
						{
							GLShaderProgramObject::setProgramPostLinkBindings( *shaderProgramObject, *shaderLayoutMap );
						}
					}
				}
			}

			shaderProgramObject->detachAllShaders();

			return shaderProgramObject;
		}

		void updateUniformDataCurrentGL(
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

			const auto currentPipelineHandle = GLShaderPipelineObject::queryCurrentShaderPipelineBinding();
			if( currentPipelineHandle != pShaderPipeline.mGLHandle )
			{
				ic3Throw( 0 );
			}

			if( pBaseType == EBaseDataType::Float32 )
			{
				const auto * floatData = reinterpret_cast<const float *>( pData );
				switch( pLength )
				{
					case 1: glUniform1fv( pUniformIndex, 1, floatData ); ic3OpenGLHandleLastError(); break;
					case 2: glUniform2fv( pUniformIndex, 1, floatData ); ic3OpenGLHandleLastError(); break;
					case 3: glUniform3fv( pUniformIndex, 1, floatData ); ic3OpenGLHandleLastError(); break;
					case 4: glUniform4fv( pUniformIndex, 1, floatData ); ic3OpenGLHandleLastError(); break;
				}
			}
			else if( pBaseType == EBaseDataType::Int32 )
			{
				const auto * intData = reinterpret_cast<const GLint *>( pData );
				switch( pLength )
				{
					case 1: glUniform1iv( pUniformIndex, 1, intData ); ic3OpenGLHandleLastError(); break;
					case 2: glUniform2iv( pUniformIndex, 1, intData ); ic3OpenGLHandleLastError(); break;
					case 3: glUniform3iv( pUniformIndex, 1, intData ); ic3OpenGLHandleLastError(); break;
					case 4: glUniform4iv( pUniformIndex, 1, intData ); ic3OpenGLHandleLastError(); break;
				}
			}
			else if( pBaseType == EBaseDataType::Uint32 )
			{
				const auto * uintData = reinterpret_cast<const GLuint *>( pData );
				switch( pLength )
				{
					case 1: glUniform1uiv( pUniformIndex, 1, uintData ); ic3OpenGLHandleLastError(); break;
					case 2: glUniform2uiv( pUniformIndex, 1, uintData ); ic3OpenGLHandleLastError(); break;
					case 3: glUniform3uiv( pUniformIndex, 1, uintData ); ic3OpenGLHandleLastError(); break;
					case 4: glUniform4uiv( pUniformIndex, 1, uintData ); ic3OpenGLHandleLastError(); break;
				}
			}
		}

		void updateUniformDataExplicitGL(
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

			const auto currentProgramHandle = GLShaderProgramObject::queryCurrentShaderProgramBinding();
			if( currentProgramHandle != pShaderProgram.mGLHandle )
			{
				ic3Throw( 0 );
			}

			if( pBaseType == EBaseDataType::Float32 )
			{
				const auto * floatData = reinterpret_cast<const float *>( pData );
				switch( pLength )
				{
					case 1: glProgramUniform1fv( pShaderProgram.mGLHandle, pUniformIndex, 1, floatData ); ic3OpenGLHandleLastError(); break;
					case 2: glProgramUniform2fv( pShaderProgram.mGLHandle, pUniformIndex, 1, floatData ); ic3OpenGLHandleLastError(); break;
					case 3: glProgramUniform3fv( pShaderProgram.mGLHandle, pUniformIndex, 1, floatData ); ic3OpenGLHandleLastError(); break;
					case 4: glProgramUniform4fv( pShaderProgram.mGLHandle, pUniformIndex, 1, floatData ); ic3OpenGLHandleLastError(); break;
				}
			}
			else if( pBaseType == EBaseDataType::Int32 )
			{
				const auto * intData = reinterpret_cast<const GLint *>( pData );
				switch( pLength )
				{
					case 1: glProgramUniform1iv( pShaderProgram.mGLHandle, pUniformIndex, 1, intData ); ic3OpenGLHandleLastError(); break;
					case 2: glProgramUniform2iv( pShaderProgram.mGLHandle, pUniformIndex, 1, intData ); ic3OpenGLHandleLastError(); break;
					case 3: glProgramUniform3iv( pShaderProgram.mGLHandle, pUniformIndex, 1, intData ); ic3OpenGLHandleLastError(); break;
					case 4: glProgramUniform4iv( pShaderProgram.mGLHandle, pUniformIndex, 1, intData ); ic3OpenGLHandleLastError(); break;
				}
			}
			else if( pBaseType == EBaseDataType::Uint32 )
			{
				const auto * uintData = reinterpret_cast<const GLuint *>( pData );
				switch( pLength )
				{
					case 1: glProgramUniform1uiv( pShaderProgram.mGLHandle, pUniformIndex, 1, uintData ); ic3OpenGLHandleLastError(); break;
					case 2: glProgramUniform2uiv( pShaderProgram.mGLHandle, pUniformIndex, 1, uintData ); ic3OpenGLHandleLastError(); break;
					case 3: glProgramUniform3uiv( pShaderProgram.mGLHandle, pUniformIndex, 1, uintData ); ic3OpenGLHandleLastError(); break;
					case 4: glProgramUniform4uiv( pShaderProgram.mGLHandle, pUniformIndex, 1, uintData ); ic3OpenGLHandleLastError(); break;
				}
			}
		}

	}

}
