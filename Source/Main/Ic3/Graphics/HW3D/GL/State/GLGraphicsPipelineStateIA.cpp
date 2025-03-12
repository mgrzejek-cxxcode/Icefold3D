
#include "GLGraphicsPipelineStateIA.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLVertexArrayObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLGPUBuffer.h>

namespace Ic3::Graphics::GCI
{

	void GLIAVertexAttributeLayout::Reset()
	{
		ResetActiveAttributesMask();
		glcPrimitiveTopology = 0;
		cppx::mem_set_zero( glcAttributeArray );
	}


	void GLIAVertexBufferArrayBindings::InitializeInterleaved()
	{
		bindingType = EGLVertexBufferBindingType::Interleaved;
		interleavedBindings = {};
	}

	void GLIAVertexBufferArrayBindings::InitializeSeparate()
	{
		bindingType = EGLVertexBufferBindingType::Separate;
		separateBindings = {};
	}

	void GLIAVertexBufferArrayBindings::Reset()
	{
		if( bindingType == EGLVertexBufferBindingType::Interleaved )
		{
			cppx::mem_set_zero( interleavedBindings );
		}
		else if( bindingType == EGLVertexBufferBindingType::Separate )
		{
			cppx::mem_set_zero( separateBindings );
		}

		bindingType = EGLVertexBufferBindingType::Undefined;
	}

	GLIAVertexBufferBinding GLIAVertexBufferArrayBindings::GetBinding( native_uint pStreamIndex ) const
	{
		if( bindingType == EGLVertexBufferBindingType::Interleaved )
		{
			return interleavedBindings[pStreamIndex];
		}
		else if( bindingType == EGLVertexBufferBindingType::Separate )
		{
			return {
				separateBindings.handleArray[pStreamIndex],
				separateBindings.offsetArray[pStreamIndex],
				separateBindings.strideArray[pStreamIndex]
			};
		}
		else
		{
			return { 0, 0, 0 };
		}
	}


	void GLIAVertexSourceBinding::Reset()
	{
		activeStreamsMask = 0;
		indexBufferBinding.Reset();
		vertexBufferBindings.Reset();
	}



	GLVertexAttributeLayoutDescriptor::GLVertexAttributeLayoutDescriptor(
			GLGPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig )
	: HW3DPipelineStateDescriptor( pGPUDevice, pCommonAttributeLayoutConfig )
	{}

	GLVertexAttributeLayoutDescriptor::~GLVertexAttributeLayoutDescriptor() = default;


	GLVertexAttributeLayoutDescriptorCore::GLVertexAttributeLayoutDescriptorCore(
			GLGPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig,
			GLVertexArrayObjectHandle pGLVertexArrayObject,
			GLenum pGLPrimitiveTopology )
	: GLVertexAttributeLayoutDescriptor( pGPUDevice, _commonAttributeLayoutConfig )
	, mGLVertexArrayObject( std::move( pGLVertexArrayObject ) )
	, mGLPrimitiveTopology( pGLPrimitiveTopology )
	, _commonAttributeLayoutConfig( pCommonAttributeLayoutConfig )
	{}

	GLVertexAttributeLayoutDescriptorCore::~GLVertexAttributeLayoutDescriptorCore() = default;

	TGfxHandle<GLVertexAttributeLayoutDescriptorCore> GLVertexAttributeLayoutDescriptorCore::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcAttributeLayoutData = GCU::IATranslateVertexAttributeLayoutDefinitionGL( pCreateInfo.layoutDefinition );

		auto vertexArrayObject = GCU::IACreateVertexArrayObjectLayoutOnlyGL( glcAttributeLayoutData );
		if( !vertexArrayObject )
		{
			return nullptr;
		}

		const auto attributeLayoutDescriptor = CreateGfxObject<GLVertexAttributeLayoutDescriptorCore>(
			pGPUDevice,
			glcAttributeLayoutData,
			std::move( vertexArrayObject ),
			glcAttributeLayoutData.glcPrimitiveTopology );

		return attributeLayoutDescriptor;
	}


	GLVertexAttributeLayoutDescriptorCompat::GLVertexAttributeLayoutDescriptorCompat(
			GLGPUDevice & pGPUDevice,
			const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition )
	: GLVertexAttributeLayoutDescriptor( pGPUDevice, pGLAttributeLayoutDefinition )
	, mGLVertexAttributeLayout( pGLAttributeLayoutDefinition )
	{}

	GLVertexAttributeLayoutDescriptorCompat::~GLVertexAttributeLayoutDescriptorCompat() = default;

	TGfxHandle<GLVertexAttributeLayoutDescriptorCompat> GLVertexAttributeLayoutDescriptorCompat::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcAttributeLayoutData = GCU::IATranslateVertexAttributeLayoutDefinitionGL( pCreateInfo.layoutDefinition );
		const auto attributeLayoutDescriptor = CreateGfxObject<GLVertexAttributeLayoutDescriptorCompat>( pGPUDevice, glcAttributeLayoutData );

		return attributeLayoutDescriptor;
	}

	GLVertexSourceBindingDescriptor::GLVertexSourceBindingDescriptor(
			GLGPUDevice & pGPUDevice,
			const GLIAVertexSourceBinding & pGLVertexSourceBinding )
	: HW3DPipelineStateDescriptor( pGPUDevice, pGLVertexSourceBinding )
	, mGLVertexSourceBinding( pGLVertexSourceBinding )
	{}

	GLVertexSourceBindingDescriptor::~GLVertexSourceBindingDescriptor() = default;

	TGfxHandle<GLVertexSourceBindingDescriptor> GLVertexSourceBindingDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcSourceBindingData = GCU::IATranslateVertexSourceBindingDefinitionGL( pCreateInfo.bindingDefinition );
		const auto stateDescriptor = CreateGfxObject<GLVertexSourceBindingDescriptor>( pGPUDevice, glcSourceBindingData );

		return stateDescriptor;
	}


	namespace GCU
	{

		GLIAVertexAttributeInfo IATranslateVertexAttributeDescGL(
				const IAVertexAttributeDesc & pVertexAttributeDesc )
		{
			GLIAVertexAttributeInfo glcAttributeInfo{};

			glcAttributeInfo.attributeIndex = static_cast<GLuint>( pVertexAttributeDesc.attribInfo.attributeSlot );
			glcAttributeInfo.streamIndex = static_cast<GLuint>( pVertexAttributeDesc.streamBinding.streamSlot );
			glcAttributeInfo.instanceRate = ( pVertexAttributeDesc.attribInfo.dataRate == EIAVertexAttributeDataRate::PerInstance ) ? 1 : 0;
			glcAttributeInfo.relativeOffset = static_cast<uint32>( pVertexAttributeDesc.streamBinding.streamRelativeOffset );
			glcAttributeInfo.byteSize = CXU::GetVertexAttribFormatByteSize( pVertexAttributeDesc.attribInfo.dataFormat );

			const auto attributeBaseType = CXU::GetVertexAttribFormatBaseDataType( pVertexAttributeDesc.attribInfo.dataFormat );
			glcAttributeInfo.baseType = ATL::GLTranslateBaseDataType( attributeBaseType );

			const auto attributeComponentsNum = CXU::GetVertexAttribFormatComponentsNum( pVertexAttributeDesc.attribInfo.dataFormat );
			glcAttributeInfo.componentsNum = static_cast<uint32>( attributeComponentsNum );

			const auto attributeFormatFlags = CXU::GetVertexAttribFormatFlags( pVertexAttributeDesc.attribInfo.dataFormat );
			glcAttributeInfo.normalized = attributeFormatFlags.is_set( eGPUDataFormatFlagNormalizedBit ) ? GL_TRUE : GL_FALSE;

			return glcAttributeInfo;
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
					auto & glcAttributeInfo = glcAttributeLayoutData.glcAttributeArray[currentVertexAttributesNum];

					// Translate the attribute data. This includes the relative offset.
					glcAttributeInfo = IATranslateVertexAttributeDescGL( inputAttributeDesc );

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

		GLIAVertexSourceBinding IATranslateVertexSourceBindingDefinitionGL(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition )
		{
			GLIAVertexSourceBinding glcSourceBindingData{};

			IAUpdateVertexSourceBindingDefinitionGL( pVertexSourceBindingDefinition, glcSourceBindingData );

			return glcSourceBindingData;
		}

		void IAUpdateVertexSourceBindingDefinitionGL(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition,
				GLIAVertexSourceBinding & pOutGLVertexSourceBinding)
		{
			if( pVertexSourceBindingDefinition.activeStreamsMask.is_set_any_of( eVertexSourceBindingMaskVertexBufferAllBits ) )
			{
				IAUpdateVertexBufferReferencesGL(
						pVertexSourceBindingDefinition.vertexBufferReferences,
						pVertexSourceBindingDefinition.activeStreamsMask,
						pOutGLVertexSourceBinding.vertexBufferBindings );
			}

			if( pVertexSourceBindingDefinition.activeStreamsMask.is_set( eIAVertexSourceBindingFlagIndexBufferBit ) )
			{
				IAUpdateIndexBufferReferenceGL(
						pVertexSourceBindingDefinition.indexBufferReference,
						pOutGLVertexSourceBinding.indexBufferBinding );
			}

			pOutGLVertexSourceBinding.activeStreamsMask =
					( pVertexSourceBindingDefinition.activeStreamsMask & eVertexSourceBindingMaskAll );

			pOutGLVertexSourceBinding.vertexBufferBindings.activeRanges =
					GCU::IAGenerateActiveVertexBuffersRanges( pVertexSourceBindingDefinition.vertexBufferReferences );
		}

		GLIAVertexBufferArrayBindings IATranslateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVertexBufferReferences,
				cppx::bitmask<EIAVertexSourceBindingFlags> pBindingMask)
		{
			GLIAVertexBufferArrayBindings glcVertexBufferBindings{};

			IAUpdateVertexBufferReferencesGL( pVertexBufferReferences, pBindingMask, glcVertexBufferBindings );

			return glcVertexBufferBindings;
		}

		uint32 IAUpdateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVertexBufferReferences,
				cppx::bitmask<EIAVertexSourceBindingFlags> pBindingMask,
				GLIAVertexBufferArrayBindings & pOutGLBindings )
		{
			uint32 activeBindingsNum = 0;

		#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
			pOutGLBindings.InitializeInterleaved();
		#else
			pOutGLBindings.InitializeSeparate();
		#endif

			for( native_uint vertexStreamIndex = 0; CXU::IAIsDataStreamVertexBufferSlotValid( vertexStreamIndex ) && pBindingMask; ++vertexStreamIndex )
			{
				const auto & inputVertexBufferRef = pVertexBufferReferences[vertexStreamIndex];
				const auto bufferBindingBit = CXU::IAMakeVertexBufferBindingFlag( vertexStreamIndex );

				if( inputVertexBufferRef && pBindingMask.is_set( bufferBindingBit ) )
				{
					const auto * glcVertexBuffer = inputVertexBufferRef.sourceBuffer->QueryInterface<GLGPUBuffer>();

				#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
					pOutGLBindings.interleavedBindings[vertexStreamIndex].handle = glcVertexBuffer->mGLBufferObject->mGLHandle;
					pOutGLBindings.interleavedBindings[vertexStreamIndex].offset = cppx::numeric_cast<GLintptr>( inputVertexBufferRef.DataOffset() );
					pOutGLBindings.interleavedBindings[vertexStreamIndex].stride = cppx::numeric_cast<GLsizei>( inputVertexBufferRef.refParams.vertexStride );
				#else
					pOutGLBindings.separateBindings.handleArray[vertexStreamIndex] = glcVertexBuffer->mGLBufferObject->mGLHandle;
					pOutGLBindings.separateBindings.offsetArray[vertexStreamIndex] = cppx::numeric_cast<GLintptr>( inputVertexBufferRef.DataOffset() );
					pOutGLBindings.separateBindings.strideArray[vertexStreamIndex] = cppx::numeric_cast<GLsizei>( inputVertexBufferRef.refParams.vertexStride );
				#endif

					++activeBindingsNum;
				}
				else
				{
				#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
					pOutGLBindings.interleavedBindings[vertexStreamIndex].handle = 0u;
					pOutGLBindings.interleavedBindings[vertexStreamIndex].offset = 0u;
					pOutGLBindings.interleavedBindings[vertexStreamIndex].stride = 0u;
				#else
					pOutGLBindings.separateBindings.handleArray[vertexStreamIndex] = 0u;
					pOutGLBindings.separateBindings.offsetArray[vertexStreamIndex] = 0u;
					pOutGLBindings.separateBindings.strideArray[vertexStreamIndex] = 0u;
				#endif
				}
			}

			return activeBindingsNum;
		}

		GLIAIndexBufferBinding IATranslateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIndexBufferReference )
		{
			GLIAIndexBufferBinding glcIndexBufferReference{};

			IAUpdateIndexBufferReferenceGL( pIndexBufferReference, glcIndexBufferReference );

			return glcIndexBufferReference;
		}

		bool IAUpdateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIndexBufferReference,
				GLIAIndexBufferBinding & pOutGLBinding )
		{
			if( pIndexBufferReference )
			{
				const auto * glcIndexBuffer = pIndexBufferReference.sourceBuffer->QueryInterface<GLGPUBuffer>();

				pOutGLBinding.handle = glcIndexBuffer->mGLBufferObject->mGLHandle;
				pOutGLBinding.offset = pIndexBufferReference.DataOffset();
				pOutGLBinding.format = ATL::GLTranslateIndexDataFormat( pIndexBufferReference.refParams.indexFormat );
				pOutGLBinding.elementByteSize = CXU::GetIndexDataFormatByteSize( pIndexBufferReference.refParams.indexFormat );

				return true;
			}
			else
			{
				pOutGLBinding.handle = 0;
				pOutGLBinding.offset = 0;
				pOutGLBinding.format = 0;
				pOutGLBinding.elementByteSize = 0;

				return false;
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

				glEnableVertexAttribArray( glcAttribute.attributeIndex );
				Ic3OpenGLHandleLastError();

				glVertexAttribFormat(
					glcAttribute.attributeIndex,
					glcAttribute.componentsNum,
					glcAttribute.baseType,
					glcAttribute.normalized,
					glcAttribute.relativeOffset );
				Ic3OpenGLHandleLastError();

				// NOTE: glVertexAttribDivisor modifies the binding between attribute index and its vertex stream slot.
				// Internally, it does the equivalent of:
				// 1. glVertexBindingDivisor( index, instanceRate );
				// 2. glVertexAttribBinding( index, index );
				// glVertexAttribDivisor( glcAttribute.attributeIndex, glcVertexAttribute.instanceRate );
				// For this reason, we use glVertexBindingDivisor() instead.

				glVertexBindingDivisor( glcAttribute.attributeIndex, glcAttribute.instanceRate );
				Ic3OpenGLHandleLastError();

				// This call has to be executed after any call that implicitly modifies vertex attribute binding.
				// One of the example is glVertexAttribDivisor() above (the actual reason of the old reported crash).
				glVertexAttribBinding( glcAttribute.attributeIndex, glcAttribute.streamIndex );
				Ic3OpenGLHandleLastError();
			}

			glBindVertexArray( 0 );
			Ic3OpenGLHandleLastError();

			return true;
		}

		GLVertexArrayObjectHandle IACreateVertexArrayObjectLayoutStreamCombinedGL(
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
				const GLIAVertexSourceBinding & pSourceBindingDefinition ) noexcept
		{
			if( !pGLAttributeLayoutDefinition.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return nullptr;
			}

			auto vertexArrayObject = GLVertexArrayObject::Create();
			IAUpdateVertexArrayObjectLayoutStreamCombinedGL( *vertexArrayObject, pGLAttributeLayoutDefinition, pSourceBindingDefinition );

			return vertexArrayObject;
		}

		bool IAUpdateVertexArrayObjectLayoutStreamCombinedGL(
				GLVertexArrayObject & pGLVertexArrayObject,
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
				const GLIAVertexSourceBinding & pSourceBindingDefinition ) noexcept
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
				const auto & glcVertexBufferBinding = pSourceBindingDefinition.vertexBufferBindings.GetBinding( glcAttribute.streamIndex );

				Ic3DebugAssert( glcVertexBufferBinding );

				if( currentVertexBufferBinding != glcVertexBufferBinding.handle )
				{
					// glVertexAttribPointer() requires an active binding under GL_ARRAY_BUFFER target.
					// Vertex buffer association is configured implicitly by the GL driver.
					glBindBuffer( GL_ARRAY_BUFFER, glcVertexBufferBinding.handle );
					Ic3OpenGLHandleLastError();

					currentVertexBufferBinding = glcVertexBufferBinding.handle;
				}

				glEnableVertexAttribArray( glcAttribute.attributeIndex );
				Ic3OpenGLHandleLastError();

				// Unlike in IAUpdateGLVertexArrayObjectLayoutOnly we can safely use glVertexAttribDivisor() here,
				// because this is the combined binding+layout setup created using older glVertexAttribPointer().
				// In this approach, vertex buffer binding is fetched from the currently bound GL_ARRAY_BUFFER
				// and the driver creates implicit vertex data stream for that buffer.
				glVertexAttribDivisor( glcAttribute.attributeIndex, glcAttribute.instanceRate );
				Ic3OpenGLHandleLastError();

				glVertexAttribPointer(
					glcAttribute.attributeIndex,
					glcAttribute.componentsNum,
					glcAttribute.baseType,
					glcAttribute.normalized,
					glcVertexBufferBinding.stride,
					reinterpret_cast<const void *>( glcVertexBufferBinding.offset + glcAttribute.relativeOffset ) );
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
