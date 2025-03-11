
#include "GLGraphicsPipelineStateShader.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLShader.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLShaderPipelineObject.h>

namespace Ic3::Graphics::GCI
{

	GLGraphicsShaderLinkageDescriptor::GLGraphicsShaderLinkageDescriptor(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig )
	: GraphicsShaderLinkageDescriptorNative( pGPUDevice, _commonShaderBindingConfig )
	, _commonShaderBindingConfig( pCommonShaderBindingConfig )
	{}

	GLGraphicsShaderLinkageDescriptor::~GLGraphicsShaderLinkageDescriptor() = default;


	GLGraphicsShaderLinkageDescriptorCore::GLGraphicsShaderLinkageDescriptorCore(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig,
			GLShaderPipelineObjectHandle pGLShaderPipelineObject )
	: GLGraphicsShaderLinkageDescriptor( pGPUDevice, pCommonShaderBindingConfig )
	, mGLShaderPipelineObject( std::move( pGLShaderPipelineObject ) )
	{}

	GLGraphicsShaderLinkageDescriptorCore::~GLGraphicsShaderLinkageDescriptorCore() = default;

	TGfxHandle<GLGraphicsShaderLinkageDescriptorCore> GLGraphicsShaderLinkageDescriptorCore::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		auto shaderPipelineObject = GCU::CreateGraphicsShaderPipelineObjectGL( pCreateInfo.shaderBinding );
		if( !shaderPipelineObject )
		{
			return nullptr;
		}

		auto stateDescriptor = CreateGfxObject<GLGraphicsShaderLinkageDescriptorCore>(
				pGPUDevice,
				pCreateInfo.shaderBinding,
				std::move( shaderPipelineObject ) );

		return stateDescriptor;
	}


	GLGraphicsShaderLinkageDescriptorCompat::GLGraphicsShaderLinkageDescriptorCompat(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig,
			GLShaderProgramObjectHandle pGLShaderProgramObject )
	: GLGraphicsShaderLinkageDescriptor( pGPUDevice, pCommonShaderBindingConfig )
	, mGLShaderProgramObject( std::move( pGLShaderProgramObject ) )
	{}

	GLGraphicsShaderLinkageDescriptorCompat::~GLGraphicsShaderLinkageDescriptorCompat() = default;

	TGfxHandle<GLGraphicsShaderLinkageDescriptorCompat> GLGraphicsShaderLinkageDescriptorCompat::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		auto shaderProgramObject = GCU::CreateGraphicsShaderProgramObjectGL( pCreateInfo.shaderBinding );
		if( !shaderProgramObject )
		{
			return nullptr;
		}

		auto stateDescriptor = CreateGfxObject<GLGraphicsShaderLinkageDescriptorCompat>(
				pGPUDevice,
				pCreateInfo.shaderBinding,
				std::move( shaderProgramObject ) );

		return stateDescriptor;
	}


	namespace GCU
	{

		GLShaderPipelineObjectHandle CreateGraphicsShaderPipelineObjectGL( const GraphicsShaderBinding & pBindingConfiguration )
		{
			auto shaderPipelineObject = GLShaderPipelineObject::Create();
			if( !shaderPipelineObject )
			{
				return nullptr;
			}

			glBindProgramPipeline( shaderPipelineObject->mGLHandle );
			Ic3OpenGLHandleLastError();

			for( auto & shaderHandle : pBindingConfiguration.commonShaderArray )
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

		GLShaderProgramObjectHandle CreateGraphicsShaderProgramObjectGL( const GraphicsShaderBinding & pBindingConfiguration )
		{
			auto shaderProgramObject = GLShaderProgramObject::Create( GLShaderProgramType::Combined );
			if( !shaderProgramObject )
			{
				return nullptr;
			}

			bool applyExplicitShaderBindings = false;

			for( auto & shaderHandle : pBindingConfiguration.commonShaderArray )
			{
				if( shaderHandle )
				{
					auto * openglShader = shaderHandle->QueryInterface<GLShader>();
					Ic3DebugAssert( openglShader->mGLShaderObject );
					shaderProgramObject->AttachShader( *( openglShader->mGLShaderObject ) );

					if( const auto * shaderBindingLayout = openglShader->mGLShaderObject->GetBindingLayout() )
					{
						GLShaderProgramObject::SetProgramPreLinkBindings( *shaderProgramObject, *shaderBindingLayout );
						applyExplicitShaderBindings = true;
					}
				}
			}

			if( !shaderProgramObject->Link() )
			{
				Ic3DebugInterrupt();
				return nullptr;
			}

			if( applyExplicitShaderBindings )
			{
				for( auto & shaderHandle : pBindingConfiguration.commonShaderArray )
				{
					if( shaderHandle )
					{
						auto * openglShader = shaderHandle->QueryInterface<GLShader>();
						if( const auto * shaderBindingLayout = openglShader->mGLShaderObject->GetBindingLayout() )
						{
							GLShaderProgramObject::SetProgramPostLinkBindings( *shaderProgramObject, *shaderBindingLayout );
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
