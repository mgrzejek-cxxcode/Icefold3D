
#include "GLGraphicsPipelineStateIA.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLVertexArrayObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLGPUBuffer.h>

namespace Ic3::Graphics::GCI
{

	void GLIAVertexAttributeLayout::Reset()
	{
		ResetActiveAttributesInfo();
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
			pipeline_state_descriptor_id_t pDescriptorID,
			const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig )
	: PIM::VertexAttributeLayoutDescriptorNative( pGPUDevice, pDescriptorID, pCommonAttributeLayoutConfig )
	{}

	GLVertexAttributeLayoutDescriptor::~GLVertexAttributeLayoutDescriptor() = default;


	GLVertexAttributeLayoutDescriptorCore::GLVertexAttributeLayoutDescriptorCore(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig,
			GLVertexArrayObjectHandle pGLVertexArrayObject,
			GLenum pGLPrimitiveTopology )
	: GLVertexAttributeLayoutDescriptor( pGPUDevice, pDescriptorID, _commonAttributeLayoutConfig )
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

		auto attributeLayoutDescriptor = CreateGfxObject<GLVertexAttributeLayoutDescriptorCore>(
			pGPUDevice,
			pCreateInfo.descriptorID,
			glcAttributeLayoutData,
			std::move( vertexArrayObject ),
			glcAttributeLayoutData.glcPrimitiveTopology );

		return attributeLayoutDescriptor;
	}


	GLVertexAttributeLayoutDescriptorCompat::GLVertexAttributeLayoutDescriptorCompat(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const GLIAVertexAttributeLayout & pGLVertexAttributeLayout )
	: GLVertexAttributeLayoutDescriptor( pGPUDevice, pDescriptorID, pGLVertexAttributeLayout )
	, mGLVertexAttributeLayout( pGLVertexAttributeLayout )
	{}

	GLVertexAttributeLayoutDescriptorCompat::~GLVertexAttributeLayoutDescriptorCompat() = default;

	TGfxHandle<GLVertexAttributeLayoutDescriptorCompat> GLVertexAttributeLayoutDescriptorCompat::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcAttributeLayoutData = GCU::IATranslateVertexAttributeLayoutDefinitionGL( pCreateInfo.layoutDefinition );

		auto attributeLayoutDescriptor = CreateGfxObject<GLVertexAttributeLayoutDescriptorCompat>(
			pGPUDevice,
			pCreateInfo.descriptorID,
			glcAttributeLayoutData );

		return attributeLayoutDescriptor;
	}

	GLVertexSourceBindingDescriptor::GLVertexSourceBindingDescriptor(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const GLIAVertexSourceBinding & pGLVertexSourceBinding )
	: PIM::VertexSourceBindingDescriptorNative( pGPUDevice, pDescriptorID, pGLVertexSourceBinding )
	, mGLVertexSourceBinding( pGLVertexSourceBinding )
	{}

	GLVertexSourceBindingDescriptor::~GLVertexSourceBindingDescriptor() = default;

	TGfxHandle<GLVertexSourceBindingDescriptor> GLVertexSourceBindingDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcSourceBindingData = GCU::IATranslateVertexSourceBindingDefinitionGL( pCreateInfo.bindingDefinition );

		auto stateDescriptor = CreateGfxObject<GLVertexSourceBindingDescriptor>(
			pGPUDevice,
			pCreateInfo.descriptorID,
			glcSourceBindingData );

		return stateDescriptor;
	}


	namespace GCU
	{

		GLIAVertexAttributeInfo IATranslateVertexAttributeInfoGL(
				const IAVertexAttributeDesc & pVertexAttributeDesc )
		{
			GLIAVertexAttributeInfo glcAttributeInfo{};

			glcAttributeInfo.streamIndex = static_cast<GLuint>( pVertexAttributeDesc.streamBinding.streamSlot );
			glcAttributeInfo.instanceRate = ( pVertexAttributeDesc.attribInfo.dataRate == EIAVertexAttributeDataRate::PerInstance ) ? 1 : 0;
			glcAttributeInfo.relativeOffset = static_cast<uint32>( pVertexAttributeDesc.streamBinding.streamRelativeOffset );
			glcAttributeInfo.byteSize = CXU::GetVertexAttribFormatByteSize( pVertexAttributeDesc.attribInfo.dataFormat );

			const auto attributeBaseType = CXU::GetVertexAttribFormatBaseDataType( pVertexAttributeDesc.attribInfo.dataFormat );
			glcAttributeInfo.baseType = ATL::TranslateGLBaseDataType( attributeBaseType );

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

			const auto activeVertexAttributesNum = pVertexAttributeLayoutDefinition.activeAttributesNum;

			uint32 currentVertexAttributesNum = 0;

			for( uint32 attributeIndex = 0; attributeIndex < GCM::kIAMaxVertexAttributesNum; ++attributeIndex )
			{
				const auto attributeBit = CXU::IAMakeVertexAttributeFlag( attributeIndex );
				if( pVertexAttributeLayoutDefinition.activeAttributesMask.is_set( attributeBit ) )
				{
					const auto & inputAttributeDesc = pVertexAttributeLayoutDefinition.attributeArray[attributeIndex];
					auto & glcAttributeInfo = glcAttributeLayoutData.glcAttributeArray[attributeIndex];

					// Translate the attribute data. This includes the relative offset.
					glcAttributeInfo = IATranslateVertexAttributeInfoGL( inputAttributeDesc );

					++currentVertexAttributesNum;

					if( currentVertexAttributesNum == activeVertexAttributesNum )
					{
						break;
					}
				}
			}

			glcAttributeLayoutData.activeAttributesMask = pVertexAttributeLayoutDefinition.activeAttributesMask;
			glcAttributeLayoutData.activeAttributesNum = pVertexAttributeLayoutDefinition.activeAttributesNum;
			glcAttributeLayoutData.primitiveTopology = pVertexAttributeLayoutDefinition.primitiveTopology;
			glcAttributeLayoutData.glcPrimitiveTopology = ATL::TranslateGLPrimitiveTopology( pVertexAttributeLayoutDefinition.primitiveTopology );

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

			if( pVertexSourceBindingDefinition.activeStreamsMask.is_set( eVertexSourceBindingFlagIndexBufferBit ) )
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
				cppx::bitmask<EVertexSourceBindingFlags> pBindingMask)
		{
			GLIAVertexBufferArrayBindings glcVertexBufferBindings{};

			IAUpdateVertexBufferReferencesGL( pVertexBufferReferences, pBindingMask, glcVertexBufferBindings );

			return glcVertexBufferBindings;
		}

		uint32 IAUpdateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVertexBufferReferences,
				cppx::bitmask<EVertexSourceBindingFlags> pBindingMask,
				GLIAVertexBufferArrayBindings & pOutGLBindings)
		{
			uint32 activeBindingsNum = 0;

		#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
			pOutGLBindings.InitializeInterleaved();
		#else
			pOutGLBindings.InitializeSeparate();
		#endif

			for( native_uint streamIndex = 0; streamIndex < GCM::kIAMaxDataStreamVertexBuffersNum; ++streamIndex )
			{
				const auto & inputVertexBufferRef = pVertexBufferReferences[streamIndex];
				const auto bufferBindingBit = CXU::IAMakeVertexSourceVertexBufferBindingFlag( streamIndex );

				if( inputVertexBufferRef && pBindingMask.is_set( bufferBindingBit ) )
				{
					const auto * glcVertexBuffer = inputVertexBufferRef.sourceBuffer->QueryInterface<GLGPUBuffer>();

				#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
					pOutGLBindings.interleavedBindings[streamIndex].handle = glcVertexBuffer->mGLBufferObject->mGLHandle;
					pOutGLBindings.interleavedBindings[streamIndex].offset = cppx::numeric_cast<GLintptr>( inputVertexBufferRef.DataOffset() );
					pOutGLBindings.interleavedBindings[streamIndex].stride = cppx::numeric_cast<GLsizei>( inputVertexBufferRef.refParams.vertexStride );
				#else
					pOutGLBindings.separateBindings.handleArray[streamIndex] = glcVertexBuffer->mGLBufferObject->mGLHandle;
					pOutGLBindings.separateBindings.offsetArray[streamIndex] = cppx::numeric_cast<GLintptr>( inputVertexBufferRef.DataOffset() );
					pOutGLBindings.separateBindings.strideArray[streamIndex] = cppx::numeric_cast<GLsizei>( inputVertexBufferRef.refParams.vertexStride );
				#endif

					++activeBindingsNum;
				}
				else
				{
				#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
					pOutGLBindings.interleavedBindings[streamIndex].handle = 0u;
					pOutGLBindings.interleavedBindings[streamIndex].offset = 0u;
					pOutGLBindings.interleavedBindings[streamIndex].stride = 0u;
				#else
					pOutGLBindings.separateBindings.handleArray[streamIndex] = 0u;
					pOutGLBindings.separateBindings.offsetArray[streamIndex] = 0u;
					pOutGLBindings.separateBindings.strideArray[streamIndex] = 0u;
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
				pOutGLBinding.format = ATL::TranslateGLIndexDataFormat( pIndexBufferReference.refParams.indexFormat );
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
				const GLIAVertexAttributeLayout & pGLVertexAttributeLayout ) noexcept
		{
			if( !pGLVertexAttributeLayout.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return nullptr;
			}

			auto vertexArrayObject = GLVertexArrayObject::Create();
			IAUpdateGLVertexArrayObjectLayoutOnly( *vertexArrayObject, pGLVertexAttributeLayout );

			return vertexArrayObject;
		}

		bool IAUpdateGLVertexArrayObjectLayoutOnly(
				GLVertexArrayObject & pGLVertexArrayObject,
				const GLIAVertexAttributeLayout & pGLVertexAttributeLayout ) noexcept
		{
			if( !pGLVertexAttributeLayout.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return false;
			}

			glBindVertexArray( pGLVertexArrayObject.mGLHandle );
			Ic3OpenGLHandleLastError();

			for( uint32 attributeIndex = 0; attributeIndex < GCM::kIAMaxVertexAttributesNum; ++attributeIndex )
			{
				const auto attributeBit = CXU::IAMakeVertexAttributeFlag( attributeIndex );
				if( pGLVertexAttributeLayout.activeAttributesMask.is_set( attributeBit ) )
				{
					const auto & glcAttribute = pGLVertexAttributeLayout.glcAttributeArray[attributeIndex];

					glEnableVertexAttribArray( attributeIndex );
					Ic3OpenGLHandleLastError();

					glVertexAttribFormat(
						attributeIndex,
						glcAttribute.componentsNum,
						glcAttribute.baseType,
						glcAttribute.normalized,
						glcAttribute.relativeOffset );
					Ic3OpenGLHandleLastError();

					// NOTE: glVertexAttribDivisor modifies the binding between attribute index and its vertex stream slot.
					// Internally, it does the equivalent of:
					// 1. glVertexBindingDivisor( attributeIndex, instanceRate );
					// 2. glVertexAttribBinding( attributeIndex, attributeIndex );
					// glVertexAttribDivisor( attributeIndex, glcVertexAttribute.instanceRate );
					// For this reason, we use glVertexBindingDivisor() instead.

					glVertexBindingDivisor( attributeIndex, glcAttribute.instanceRate );
					Ic3OpenGLHandleLastError();

					// This call has to be executed after any call that implicitly modifies vertex attribute binding.
					// One of the example is glVertexAttribDivisor() above (the actual reason of the old reported crash).
					glVertexAttribBinding( attributeIndex, glcAttribute.streamIndex );
					Ic3OpenGLHandleLastError();
				}
			}

			glBindVertexArray( 0 );
			Ic3OpenGLHandleLastError();

			return true;
		}

		GLVertexArrayObjectHandle IACreateVertexArrayObjectLayoutStreamCombinedGL(
				const GLIAVertexAttributeLayout & pGLVertexAttributeLayout,
				const GLIAVertexSourceBinding & pSourceBindingDefinition ) noexcept
		{
			if( !pGLVertexAttributeLayout.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return nullptr;
			}

			auto vertexArrayObject = GLVertexArrayObject::Create();
			IAUpdateVertexArrayObjectLayoutStreamCombinedGL( *vertexArrayObject, pGLVertexAttributeLayout, pSourceBindingDefinition );

			return vertexArrayObject;
		}

		bool IAUpdateVertexArrayObjectLayoutStreamCombinedGL(
				GLVertexArrayObject & pGLVertexArrayObject,
				const GLIAVertexAttributeLayout & pGLVertexAttributeLayout,
				const GLIAVertexSourceBinding & pSourceBindingDefinition ) noexcept
		{
			if( !pGLVertexAttributeLayout.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return false;
			}

			glBindVertexArray( pGLVertexArrayObject.mGLHandle );
			Ic3OpenGLHandleLastError();

			GLuint currentVertexBufferBinding = 0;

			for( uint32 attributeIndex = 0; attributeIndex < GCM::kIAMaxVertexAttributesNum; ++attributeIndex )
			{
				const auto attributeBit = CXU::IAMakeVertexAttributeFlag( attributeIndex );
				if( pGLVertexAttributeLayout.activeAttributesMask.is_set( attributeBit ) )
				{
					const auto & glcAttribute = pGLVertexAttributeLayout.glcAttributeArray[attributeIndex];
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

					glEnableVertexAttribArray( attributeIndex );
					Ic3OpenGLHandleLastError();

					// Unlike in IAUpdateGLVertexArrayObjectLayoutOnly can safely use glVertexAttribDivisor() here,
					// because this is the combined binding+layout setup created using older glVertexAttribPointer().
					// In this approach, vertex buffer binding is fetched from the currently bound GL_ARRAY_BUFFER
					// and the driver creates implicit vertex data stream for that buffer.
					glVertexAttribDivisor( attributeIndex, glcAttribute.instanceRate );
					Ic3OpenGLHandleLastError();

					glVertexAttribPointer(
						attributeIndex,
						glcAttribute.componentsNum,
						glcAttribute.baseType,
						glcAttribute.normalized,
						glcVertexBufferBinding.stride,
						reinterpret_cast<const void *>( glcVertexBufferBinding.offset + glcAttribute.relativeOffset ) );
					Ic3OpenGLHandleLastError();
				}
			}

			glBindBuffer( GL_ARRAY_BUFFER, 0u );
			Ic3OpenGLHandleLastError();

			glBindVertexArray( 0 );
			Ic3OpenGLHandleLastError();

			return true;
		}

	}

}
