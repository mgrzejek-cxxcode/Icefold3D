
#include "GLGraphicsPipelineStateIA.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLVertexArrayObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLGPUBuffer.h>

namespace Ic3::Graphics::GCI
{

	namespace Utilities
	{

		GLIAVertexAttributeDesc IATranslateVertexAttributeDescGL(
				const IAVertexAttributeDesc & pVertexAttributeDesc )
		{
			GLIAVertexAttributeDesc glcAttributeDesc{};

			glcAttributeDesc.attributeSlot = pVertexAttributeDesc.attribInfo.attributeSlot;
			glcAttributeDesc.vertexStreamSlot = pVertexAttributeDesc.vertexStreamBinding.streamSlot;
			glcAttributeDesc.instanceRate = pVertexAttributeDesc.attribInfo.instanceStepRate;
			glcAttributeDesc.vertexStreamRelativeOffset = pVertexAttributeDesc.vertexStreamBinding.streamRelativeOffset;
			glcAttributeDesc.byteSize = CXU::GetVertexAttribFormatByteSize( pVertexAttributeDesc.attribInfo.dataFormat );

			const auto attributeBaseType = CXU::GetVertexAttribFormatBaseDataType( pVertexAttributeDesc.attribInfo.dataFormat );
			glcAttributeDesc.baseType = ATL::GLTranslateBaseDataType( attributeBaseType );

			const auto attributeComponentsNum = CXU::GetVertexAttribFormatComponentsNum( pVertexAttributeDesc.attribInfo.dataFormat );
			glcAttributeDesc.componentsNum = static_cast<uint32>( attributeComponentsNum );

			const auto attributeFormatFlags = CXU::GetVertexAttribFormatFlags( pVertexAttributeDesc.attribInfo.dataFormat );
			glcAttributeDesc.normalized = attributeFormatFlags.is_set( eGPUDataFormatFlagNormalizedBit ) ? GL_TRUE : GL_FALSE;

			return glcAttributeDesc;
		}

		GLIAVertexAttributeLayout IATranslateVertexAttributeLayoutDefinitionGL(
				const IAVertexAttributeLayoutDefinition & pVertexAttributeLayoutDefinition )
		{
			GLIAVertexAttributeLayout glcAttributeLayoutData{};

			const auto activeVertexAttributesNum = pVertexAttributeLayoutDefinition.GetActiveAttributesNum();
			for( uint32 attributeIndex = 0, currentVertexAttributesNum = 0; attributeIndex < GCM::kIAMaxVertexAttributesNum; ++attributeIndex )
			{
				const auto attributeBit = CXU::IAMakeVertexAttributeFlag( attributeIndex );
				if( pVertexAttributeLayoutDefinition.activeAttributesMask.is_set( attributeBit ) )
				{
					const auto & inputAttributeDesc = pVertexAttributeLayoutDefinition.attributeArray[attributeIndex];
					auto & glcAttributeDesc = glcAttributeLayoutData.glcAttributeArray[currentVertexAttributesNum];

					// Translate the attribute data. This includes the relative offset.
					glcAttributeDesc = IATranslateVertexAttributeDescGL( inputAttributeDesc );

					++currentVertexAttributesNum;

					if( currentVertexAttributesNum == activeVertexAttributesNum )
					{
						break;
					}
				}
			}

			glcAttributeLayoutData.activeAttributesMask = pVertexAttributeLayoutDefinition.activeAttributesMask;
			glcAttributeLayoutData.primitiveTopology = pVertexAttributeLayoutDefinition.primitiveTopology;
			glcAttributeLayoutData.glcPrimitiveTopology = ATL::GLTranslatePrimitiveTopology( pVertexAttributeLayoutDefinition.primitiveTopology );

			return glcAttributeLayoutData;
		}

		GLIAVertexSourceBindingPtr IATranslateVertexSourceBindingDefinitionGL(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition )
		{
			const auto activeVertexBuffersNum = pVertexSourceBindingDefinition.GetActiveVertexBuffersNum();
			auto glcVertexSourceBinding = MakeUniqueDynamicallySizedTemplate<GLIAVertexSourceBinding>( activeVertexBuffersNum );

			IAUpdateVertexSourceBindingDefinitionGL( pVertexSourceBindingDefinition, *glcVertexSourceBinding );

			return glcVertexSourceBinding;
		}

		void IAUpdateVertexSourceBindingDefinitionGL(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition,
				GLIAVertexSourceBindingBase & pOutGLVertexSourceBinding )
		{
			if( pVertexSourceBindingDefinition.GetActiveVertexBuffersNum() > 0 )
			{
				uint32 activeVertexBuffersNum = 0;

				for( uint32 vertexStreamIndex = 0; CXU::IAIsDataStreamVertexBufferSlotValid( vertexStreamIndex ); ++vertexStreamIndex )
				{
					const auto bufferBindingBit = CXU::IAMakeVertexBufferBindingFlag( vertexStreamIndex );

					const auto & inputVertexBufferRef = pVertexSourceBindingDefinition.vertexBufferReferences[vertexStreamIndex];
					if( inputVertexBufferRef && pVertexSourceBindingDefinition.IsVertexBufferActive( vertexStreamIndex ) )
					{
						const auto * glcVertexBuffer = inputVertexBufferRef.sourceBuffer->QueryInterface<GLGPUBuffer>();

						// Currently, we 'activeVertexBuffersNum' occupied slots in the binding data array. This is also the next free index we can use.
						// This means that vertex stream with index 'vertexStreamIndex' will reference 'activeVertexBuffersNum' index in the data array.
						pOutGLVertexSourceBinding.SetVertexStreamIndexMapping( vertexStreamIndex, activeVertexBuffersNum );

						// Save everything in the data arrays.
						pOutGLVertexSourceBinding.vertexBufferBindings[activeVertexBuffersNum].streamIndex = vertexStreamIndex;
						pOutGLVertexSourceBinding.vertexBufferBindings[activeVertexBuffersNum].handle = glcVertexBuffer->mGLBufferObject->mGLHandle;
						pOutGLVertexSourceBinding.vertexBufferBindings[activeVertexBuffersNum].offset = cppx::numeric_cast<GLintptr>( inputVertexBufferRef.DataOffset() );
						pOutGLVertexSourceBinding.vertexBufferBindings[activeVertexBuffersNum].stride = cppx::numeric_cast<GLsizei>( inputVertexBufferRef.refParams.vertexStride );
						pOutGLVertexSourceBinding.activeStreamsMask.set( bufferBindingBit );

						++activeVertexBuffersNum;
					}
					else
					{
						// This stream is inactive (no vertex buffer referenced). Set this special value to indicate this.
						pOutGLVertexSourceBinding.SetVertexStreamIndexInactive( vertexStreamIndex );
						pOutGLVertexSourceBinding.activeStreamsMask.unset( bufferBindingBit );
					}
				}

				// This should never trigger. If it does, it means the GLIAVertexSourceBinding has been created
				// with an invalid size parameter - pOutGLVertexSourceBinding.activeVertexBuffersNum is always
				// set to the number of active vertex buffers in the input binding definition.
				Ic3DebugAssert( activeVertexBuffersNum == pOutGLVertexSourceBinding.GetActiveVertexBuffersNum() );
			}

			const auto & inputIndexBufferRef = pVertexSourceBindingDefinition.indexBufferReference;
			if( inputIndexBufferRef && pVertexSourceBindingDefinition.IsIndexBufferActive() )
			{
				const auto * glcIndexBuffer = inputIndexBufferRef.sourceBuffer->QueryInterface<GLGPUBuffer>();
				pOutGLVertexSourceBinding.indexBufferBinding.handle = glcIndexBuffer->mGLBufferObject->mGLHandle;
				pOutGLVertexSourceBinding.indexBufferBinding.offset = inputIndexBufferRef.DataOffset();
				pOutGLVertexSourceBinding.indexBufferBinding.format = ATL::GLTranslateIndexDataFormat( inputIndexBufferRef.refParams.indexFormat );
				pOutGLVertexSourceBinding.indexBufferBinding.elementByteSize = CXU::GetIndexDataFormatByteSize( inputIndexBufferRef.refParams.indexFormat );

				pOutGLVertexSourceBinding.activeStreamsMask.set( eIAVertexSourceBindingFlagIndexBufferBit );
			}
			else
			{
				pOutGLVertexSourceBinding.indexBufferBinding.handle = 0;
				pOutGLVertexSourceBinding.indexBufferBinding.offset = 0;
				pOutGLVertexSourceBinding.indexBufferBinding.format = 0;
				pOutGLVertexSourceBinding.indexBufferBinding.elementByteSize = 0;

				pOutGLVertexSourceBinding.activeStreamsMask.unset( eIAVertexSourceBindingFlagIndexBufferBit );
			}
		}

		GLVertexArrayObjectHandle IACreateVertexArrayObjectLayoutOnlyGL(
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition ) noexcept
		{
			if( !pGLAttributeLayoutDefinition.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return nullptr;
			}

			auto vertexArrayObject = GLVertexArrayObject::Create();
			IAUpdateGLVertexArrayObjectLayoutOnly( *vertexArrayObject, pGLAttributeLayoutDefinition );

			return vertexArrayObject;
		}

		bool IAUpdateGLVertexArrayObjectLayoutOnly(
				GLVertexArrayObject & pGLVertexArrayObject,
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition ) noexcept
		{
			if( !pGLAttributeLayoutDefinition.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return false;
			}

			glBindVertexArray( pGLVertexArrayObject.mGLHandle );
			Ic3OpenGLHandleLastError();

			// Note: in GLIAVertexAttributeLayout glcAttributeArray[index] does not describe slot 'index' of the input assembler.
			// Instead, all active N attributes are placed in the first N slots of glcAttributeArray, making it easier to iterate.
			const auto activeVertexAttributesNum = pGLAttributeLayoutDefinition.GetActiveAttributesNum();
			for( uint32 attributeIndex = 0; attributeIndex < activeVertexAttributesNum; ++attributeIndex )
			{
				const auto & glcAttribute = pGLAttributeLayoutDefinition.glcAttributeArray[attributeIndex];

				glEnableVertexAttribArray( glcAttribute.attributeSlot );
				Ic3OpenGLHandleLastError();

				glVertexAttribFormat(
					glcAttribute.attributeSlot,
					glcAttribute.componentsNum,
					glcAttribute.baseType,
					glcAttribute.normalized,
					glcAttribute.vertexStreamRelativeOffset );
				Ic3OpenGLHandleLastError();

				// NOTE: glVertexAttribDivisor modifies the binding between attribute index and its vertex stream slot.
				// Internally, it does the equivalent of:
				// 1. glVertexBindingDivisor( index, instanceRate );
				// 2. glVertexAttribBinding( index, index );
				// For this reason, we use glVertexBindingDivisor() instead.

				glVertexBindingDivisor( glcAttribute.attributeSlot, glcAttribute.instanceRate );
				Ic3OpenGLHandleLastError();

				// This call has to be executed after any call that implicitly modifies vertex attribute binding.
				// One of the example is glVertexAttribDivisor() above (the actual reason of the old reported crash).
				glVertexAttribBinding( glcAttribute.attributeSlot, glcAttribute.vertexStreamSlot );
				Ic3OpenGLHandleLastError();
			}

			glBindVertexArray( 0 );
			Ic3OpenGLHandleLastError();

			return true;
		}

		GLVertexArrayObjectHandle IACreateVertexArrayObjectLayoutStreamCombinedGL(
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
				const GLIAVertexSourceBindingBase & pGLVertexSourceBinding ) noexcept
		{
			if( !pGLAttributeLayoutDefinition.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return nullptr;
			}

			auto vertexArrayObject = GLVertexArrayObject::Create();
			IAUpdateVertexArrayObjectLayoutStreamCombinedGL( *vertexArrayObject, pGLAttributeLayoutDefinition, pGLVertexSourceBinding );

			return vertexArrayObject;
		}

		bool IAUpdateVertexArrayObjectLayoutStreamCombinedGL(
				GLVertexArrayObject & pGLVertexArrayObject,
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
				const GLIAVertexSourceBindingBase & pGLVertexSourceBinding ) noexcept
		{
			if( !pGLAttributeLayoutDefinition.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return false;
			}

			glBindVertexArray( pGLVertexArrayObject.mGLHandle );
			Ic3OpenGLHandleLastError();

			GLuint currentVertexBufferBinding = 0;

			// Note: in GLIAVertexAttributeLayout glcAttributeArray[index] does not describe slot 'index' of the input assembler.
			// Instead, all active N attributes are placed in the first N slots of glcAttributeArray, making it easier to iterate.
			const auto activeVertexAttributesNum = pGLAttributeLayoutDefinition.GetActiveAttributesNum();
			for( uint32 attributeIndex = 0; attributeIndex < activeVertexAttributesNum; ++attributeIndex )
			{
				const auto & glcAttribute = pGLAttributeLayoutDefinition.glcAttributeArray[attributeIndex];
				const auto * glcVertexBufferBinding = pGLVertexSourceBinding.GetBindingForVertexStream( glcAttribute.vertexStreamSlot );

				Ic3DebugAssert( glcVertexBufferBinding );

				if( currentVertexBufferBinding != glcVertexBufferBinding->handle )
				{
					// glVertexAttribPointer() requires an active binding under GL_ARRAY_BUFFER target.
					// Vertex buffer association is configured implicitly by the GL driver.
					glBindBuffer( GL_ARRAY_BUFFER, glcVertexBufferBinding->handle );
					Ic3OpenGLHandleLastError();

					currentVertexBufferBinding = glcVertexBufferBinding->handle;
				}

				glEnableVertexAttribArray( glcAttribute.attributeSlot );
				Ic3OpenGLHandleLastError();

				// Unlike in IAUpdateGLVertexArrayObjectLayoutOnly we can safely use glVertexAttribDivisor() here,
				// because this is the combined binding+layout setup created using older glVertexAttribPointer().
				// In this approach, vertex buffer binding is fetched from the currently bound GL_ARRAY_BUFFER
				// and the driver creates implicit vertex data stream for that buffer.
				glVertexAttribDivisor( glcAttribute.attributeSlot, glcAttribute.instanceRate );
				Ic3OpenGLHandleLastError();

				glVertexAttribPointer(
					glcAttribute.attributeSlot,
					glcAttribute.componentsNum,
					glcAttribute.baseType,
					glcAttribute.normalized,
					glcVertexBufferBinding->stride,
					reinterpret_cast< const void * >( glcVertexBufferBinding->offset + glcAttribute.vertexStreamRelativeOffset ) );
				Ic3OpenGLHandleLastError();
			}

			glBindBuffer( GL_ARRAY_BUFFER, 0u );
			Ic3OpenGLHandleLastError();

			glBindVertexArray( 0 );
			Ic3OpenGLHandleLastError();

			return true;
		}

	}

}
