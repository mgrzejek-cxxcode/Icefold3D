
#include "GLInputAssembler.h"
#include <Ic3/Graphics/HW3D/GL/GLApiTranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGpuDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLVertexArrayObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLGpuBuffer.h>

namespace Ic3::Graphics::GCI
{

	void GLIAInputLayoutDefinition::reset()
	{
		activeAttributesMask = 0;
		primitiveTopology = 0;
		cppx::mem_set_zero( attributeArray );
	}


	void GLIAVertexBuffersBindings::initializeInterleaved()
	{
		bindingType = EGLVertexBufferBindingType::Interleaved;
		interleavedBindings = {};
	}

	void GLIAVertexBuffersBindings::initializeSeparate()
	{
		bindingType = EGLVertexBufferBindingType::Separate;
		separateBindings = {};
	}

	void GLIAVertexBuffersBindings::reset()
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

	GLIAVertexBufferBinding GLIAVertexBuffersBindings::GetBinding( native_uint pStreamIndex ) const
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


	void GLIAVertexStreamDefinition::reset()
	{
		activeBindingsMask = 0;
		indexBufferBinding.reset();
		vertexBufferBindings.reset();
	}


	GLIAVertexStreamImmutableState::GLIAVertexStreamImmutableState(
			GLGpuDevice & pGpuDevice,
			const IAVertexStreamStateCommonProperties & pCommonProperties,
			const GLIAVertexStreamDefinition & pGLVertexStreamDefinition )
	: IAVertexStreamImmutableState( pGpuDevice, pCommonProperties )
	, mGLVertexStreamDefinition( pGLVertexStreamDefinition )
	{}

	GLIAVertexStreamImmutableState::~GLIAVertexStreamImmutableState() = default;

	TGfxHandle<GLIAVertexStreamImmutableState> GLIAVertexStreamImmutableState::CreateInstance(
			GLGpuDevice & pGpuDevice,
			const IAVertexStreamDefinition & pVertexStreamDefinition )
	{
		const auto vertexStreamCommonProperties = SMU::GetIAVertexStreamStateCommonProperties( pVertexStreamDefinition );
		const auto glcVertexStreamDefinition = SMU::TranslateIAVertexStreamDefinitionGL( pVertexStreamDefinition );

		auto immutableState = CreateGfxObject<GLIAVertexStreamImmutableState>(
			pGpuDevice,
			vertexStreamCommonProperties,
			glcVertexStreamDefinition );

		return immutableState;
	}


	GLIAInputLayoutImmutableState::GLIAInputLayoutImmutableState(
			GLGpuDevice & pGpuDevice,
			const IAInputLayoutStateCommonProperties & pCommonProperties )
	: IAInputLayoutImmutableState( pGpuDevice, pCommonProperties )
	{}

	GLIAInputLayoutImmutableState::~GLIAInputLayoutImmutableState() = default;


	GLIAInputLayoutImmutableStateCore::GLIAInputLayoutImmutableStateCore(
			GLGpuDevice & pGpuDevice,
			const IAInputLayoutStateCommonProperties & pCommonProperties,
			GLVertexArrayObjectHandle pVertexArrayObject,
			GLenum pGLPrimitiveTopology )
	: GLIAInputLayoutImmutableState( pGpuDevice, pCommonProperties )
	, mVertexArrayObject( std::move( pVertexArrayObject ) )
	, mGLPrimitiveTopology( pGLPrimitiveTopology )
	{}

	GLIAInputLayoutImmutableStateCore::~GLIAInputLayoutImmutableStateCore() = default;

	TGfxHandle<GLIAInputLayoutImmutableStateCore> GLIAInputLayoutImmutableStateCore::CreateInstance(
			GLGpuDevice & pGpuDevice,
			const IAInputLayoutDefinition & pInputLayoutDefinition )
	{
		const auto inputLayoutCommonProperties = SMU::GetIAInputLayoutStateCommonProperties( pInputLayoutDefinition );
		const auto glcInputLayoutDefinition = SMU::TranslateIAInputLayoutDefinitionGL( pInputLayoutDefinition );

		auto vertexArrayObject = SMU::CreateGLVertexArrayObjectLayoutOnlyGL( glcInputLayoutDefinition );
		if( !vertexArrayObject )
		{
			return nullptr;
		}

		auto immutableState = CreateGfxObject<GLIAInputLayoutImmutableStateCore>(
			pGpuDevice,
			inputLayoutCommonProperties,
			std::move( vertexArrayObject ),
			glcInputLayoutDefinition.primitiveTopology);

		return immutableState;
	}


	GLIAInputLayoutImmutableStateCompat::GLIAInputLayoutImmutableStateCompat(
			GLGpuDevice & pGpuDevice,
			const IAInputLayoutStateCommonProperties & pCommonProperties,
			const GLIAInputLayoutDefinition & pGLInputLayoutDefinition )
	: GLIAInputLayoutImmutableState( pGpuDevice, pCommonProperties )
	, mGLInputLayoutDefinition( pGLInputLayoutDefinition )
	{}

	GLIAInputLayoutImmutableStateCompat::~GLIAInputLayoutImmutableStateCompat() = default;

	TGfxHandle<GLIAInputLayoutImmutableStateCompat> GLIAInputLayoutImmutableStateCompat::CreateInstance(
			GLGpuDevice & pGpuDevice,
			const IAInputLayoutDefinition & pInputLayoutDefinition )
	{
		const auto inputLayoutCommonProperties = SMU::GetIAInputLayoutStateCommonProperties( pInputLayoutDefinition );
		const auto glcInputLayoutDefinition = SMU::TranslateIAInputLayoutDefinitionGL( pInputLayoutDefinition );

		auto immutableState = CreateGfxObject<GLIAInputLayoutImmutableStateCompat>(
			pGpuDevice,
			inputLayoutCommonProperties,
			glcInputLayoutDefinition );

		return immutableState;
	}


	namespace SMU
	{

		GLIAVertexAttributeInfo TranslateIAVertexAttributeInfoGL(
				const IAVertexAttributeInfo & pAttributeInfo )
		{
			GLIAVertexAttributeInfo glcAttributeInfo{};

			glcAttributeInfo.streamIndex = static_cast<GLuint>( pAttributeInfo.streamIndex );
			glcAttributeInfo.instanceRate = pAttributeInfo.instanceRate;
			glcAttributeInfo.relativeOffset = static_cast<uint32>( pAttributeInfo.relativeOffset );
			glcAttributeInfo.byteSize = CxDef::GetVertexAttribFormatByteSize( pAttributeInfo.format );

			const auto attributeBaseType = CxDef::GetVertexAttribFormatBaseDataType( pAttributeInfo.format );
			glcAttributeInfo.baseType = ATL::TranslateGLBaseDataType( attributeBaseType );

			const auto attributeComponentsNum = CxDef::GetVertexAttribFormatComponentsNum( pAttributeInfo.format );
			glcAttributeInfo.componentsNum = static_cast<uint32>( attributeComponentsNum );

			const auto attributeFormatFlags = CxDef::GetVertexAttribFormatFlags( pAttributeInfo.format );
			glcAttributeInfo.normalized = attributeFormatFlags.is_set( eGpuDataFormatFlagNormalizedBit ) ? GL_TRUE : GL_FALSE;

			return glcAttributeInfo;
		}

		GLIAInputLayoutDefinition TranslateIAInputLayoutDefinitionGL(
				const IAInputLayoutDefinition & pDefinition )
		{
			GLIAInputLayoutDefinition glcInputLayoutDefinition{};

			const auto definedVertexAttributesNum = pop_count( pDefinition.activeAttributesMask );

			uint32 currentVertexAttributesNum = 0;
			uint64 currentAttributePackedRelativeOffset = 0;

			for( uint32 attributeIndex = 0; attributeIndex < GCM::cxIAMaxVertexAttributesNum; ++attributeIndex )
			{
				const auto attributeBit = CxDef::makeIAVertexAttributeFlag( attributeIndex );
				if( pDefinition.activeAttributesMask.is_set( attributeBit ) )
				{
					const auto & inputAttributeInfo = pDefinition.attributeArray[attributeIndex];
					auto & glcAttributeInfo = glcInputLayoutDefinition.attributeArray[attributeIndex];

					// Translate the attribute data. This includes the relative offset.
					glcAttributeInfo = TranslateIAVertexAttributeInfoGL( inputAttributeInfo );

					if( inputAttributeInfo.relativeOffset == cxIAVertexAttributeOffsetAppend )
					{
						// If the offset is APPEND, update it with the current packed offset calculated.
						glcAttributeInfo.relativeOffset = cppx::numeric_cast<uint32>( currentAttributePackedRelativeOffset );
					}
					else if( inputAttributeInfo.relativeOffset == cxIAVertexAttributeOffsetAppend16 )
					{
						// If the offset is APPEND, update it with the current packed offset calculated.
						glcAttributeInfo.relativeOffset = cppx::numeric_cast<uint32>( cppx::mem_get_aligned_value( currentAttributePackedRelativeOffset, 16 ) );
					}

					// Update the current packed offset.
					currentAttributePackedRelativeOffset = glcAttributeInfo.relativeOffset + glcAttributeInfo.byteSize;

					++currentVertexAttributesNum;

					if( currentVertexAttributesNum == definedVertexAttributesNum )
					{
						break;
					}
				}
			}

			glcInputLayoutDefinition.activeAttributesMask = pDefinition.activeAttributesMask;
			glcInputLayoutDefinition.primitiveTopology = ATL::TranslateGLPrimitiveTopology( pDefinition.primitiveTopology );

			return glcInputLayoutDefinition;
		}

		GLIAVertexStreamDefinition TranslateIAVertexStreamDefinitionGL(
				const IAVertexStreamDefinition & pDefinition )
		{
			GLIAVertexStreamDefinition glcVertexStreamDefinition{};

			if( pDefinition.activeBindingsMask.is_set_any_of( eIAVertexStreamBindingMaskVertexBufferAllBits ) )
			{
				TranslateVertexBufferReferencesGL(
					pDefinition.vertexBufferReferences,
					pDefinition.activeBindingsMask,
					glcVertexStreamDefinition.vertexBufferBindings );
			}

			if( pDefinition.activeBindingsMask.is_set( eIAVertexStreamBindingFlagIndexBufferBit ) )
			{
				TranslateIndexBufferReferenceGL(
					pDefinition.indexBufferReference,
					glcVertexStreamDefinition.indexBufferBinding );
			}

			glcVertexStreamDefinition.activeBindingsMask = (pDefinition.activeBindingsMask & eIAVertexStreamBindingMaskAll );
			glcVertexStreamDefinition.vertexBufferBindings.activeRanges = SMU::GenerateActiveVertexBufferRanges( pDefinition.vertexBufferReferences );

			return glcVertexStreamDefinition;
		}

		uint32 TranslateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVBReferences,
				cppx::bitmask<EIAVertexStreamBindingFlags> pBindingMask,
				GLIAVertexBuffersBindings & pOutGLBindings)
		{
			uint32 activeBindingsNum = 0;

		#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
			pOutGLBindings.initializeInterleaved();
		#else
			pOutGLBindings.initializeSeparate();
		#endif

			for( native_uint streamIndex = 0; streamIndex < GCM::cxIAMaxVertexBufferBindingsNum; ++streamIndex )
			{
				const auto & inputVertexBufferRef = pVBReferences[streamIndex];
				const auto vbBindingFlag = CxDef::makeIAVertexBufferFlag( streamIndex );

				if( inputVertexBufferRef && pBindingMask.is_set( vbBindingFlag ) )
				{
					const auto * glcVertexBuffer = inputVertexBufferRef.sourceBuffer->QueryInterface<GLGpuBuffer>();

				#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
					pOutGLBindings.interleavedBindings[streamIndex].handle = glcVertexBuffer->mGLBufferObject->mGLHandle;
					pOutGLBindings.interleavedBindings[streamIndex].offset = cppx::numeric_cast<GLintptr>( inputVertexBufferRef.dataOffset() );
					pOutGLBindings.interleavedBindings[streamIndex].stride = cppx::numeric_cast<GLsizei>( inputVertexBufferRef.vertexStride );
				#else
					pOutGLBindings.separateBindings.handleArray[streamIndex] = glcVertexBuffer->mGLBufferObject->mGLHandle;
					pOutGLBindings.separateBindings.offsetArray[streamIndex] = cppx::numeric_cast<GLintptr>( inputVertexBufferRef.DataOffset() );
					pOutGLBindings.separateBindings.strideArray[streamIndex] = cppx::numeric_cast<GLsizei>( inputVertexBufferRef.vertexStride );
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

		bool TranslateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIBReference,
				GLIAIndexBufferBinding & pOutGLBinding )
		{
			if( pIBReference )
			{
				const auto * glcIndexBuffer = pIBReference.sourceBuffer->QueryInterface<GLGpuBuffer>();

				pOutGLBinding.handle = glcIndexBuffer->mGLBufferObject->mGLHandle;
				pOutGLBinding.offset = pIBReference.DataOffset();
				pOutGLBinding.format = ATL::TranslateGLIndexDataFormat( pIBReference.indexFormat );
				pOutGLBinding.elementByteSize = CxDef::GetIndexDataFormatByteSize( pIBReference.indexFormat );

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

		GLIAVertexBuffersBindings TranslateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVBReferences,
				cppx::bitmask<EIAVertexStreamBindingFlags> pBindingMask )
		{
			GLIAVertexBuffersBindings glcVertexBufferBindings{};
			TranslateVertexBufferReferencesGL( pVBReferences, pBindingMask, glcVertexBufferBindings );
			return glcVertexBufferBindings;
		}

		GLIAIndexBufferBinding TranslateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIBReference )
		{
			GLIAIndexBufferBinding glcIndexBufferBinding{};
			TranslateIndexBufferReferenceGL( pIBReference, glcIndexBufferBinding );
			return glcIndexBufferBinding;
		}

		GLVertexArrayObjectHandle CreateGLVertexArrayObjectLayoutOnlyGL(
				const GLIAInputLayoutDefinition & pInputLayoutDefinition ) noexcept
		{
			if( !pInputLayoutDefinition.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return nullptr;
			}

			auto vertexArrayObject = GLVertexArrayObject::Create();
			UpdateGLVertexArrayObjectLayoutOnlyGL( *vertexArrayObject, pInputLayoutDefinition );

			return vertexArrayObject;
		}

		bool UpdateGLVertexArrayObjectLayoutOnlyGL(
				GLVertexArrayObject & pVertexArrayObject,
				const GLIAInputLayoutDefinition & pInputLayoutDefinition ) noexcept
		{
			if( !pInputLayoutDefinition.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return false;
			}

			glBindVertexArray( pVertexArrayObject.mGLHandle );
			ic3OpenGLHandleLastError();

			for( uint32 attributeIndex = 0; attributeIndex < GCM::cxIAMaxVertexAttributesNum; ++attributeIndex )
			{
				const auto attributeBit = CxDef::makeIAVertexAttributeFlag( attributeIndex );
				if( pInputLayoutDefinition.activeAttributesMask.is_set( attributeBit ) )
				{
					const auto & glcAttribute = pInputLayoutDefinition.attributeArray[attributeIndex];

					glEnableVertexAttribArray( attributeIndex );
					ic3OpenGLHandleLastError();

					glVertexAttribFormat(
						attributeIndex,
						glcAttribute.componentsNum,
						glcAttribute.baseType,
						glcAttribute.normalized,
						glcAttribute.relativeOffset );
					ic3OpenGLHandleLastError();

					// NOTE: glVertexAttribDivisor modifies the binding between attribute index and its vertex stream slot.
					// Internally, it does the equivalent of:
					// 1. glVertexBindingDivisor( attributeIndex, instanceRate );
					// 2. glVertexAttribBinding( attributeIndex, attributeIndex );
					// glVertexAttribDivisor( attributeIndex, glcVertexAttribute.instanceRate );
					// For this reason, we use glVertexBindingDivisor() instead.

					glVertexBindingDivisor( attributeIndex, glcAttribute.instanceRate );
					ic3OpenGLHandleLastError();

					// This call has to be executed after any call that implicitly modifies vertex attribute binding.
					// One of the example is glVertexAttribDivisor() above (the actual reason of the old reported crash).
					glVertexAttribBinding( attributeIndex, glcAttribute.streamIndex );
					ic3OpenGLHandleLastError();
				}
			}

			glBindVertexArray( 0 );
			ic3OpenGLHandleLastError();

			return true;
		}

		GLVertexArrayObjectHandle CreateGLVertexArrayObjectLayoutStreamCombinedGL(
				const GLIAInputLayoutDefinition & pInputLayoutDefinition,
				const GLIAVertexStreamDefinition & pVertexStreamDefinition ) noexcept
		{
			if( !pInputLayoutDefinition.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return nullptr;
			}

			auto vertexArrayObject = GLVertexArrayObject::Create();
			UpdateGLVertexArrayObjectLayoutStreamCombinedGL( *vertexArrayObject, pInputLayoutDefinition, pVertexStreamDefinition );

			return vertexArrayObject;
		}

		bool UpdateGLVertexArrayObjectLayoutStreamCombinedGL(
				GLVertexArrayObject & pVertexArrayObject,
				const GLIAInputLayoutDefinition & pInputLayoutDefinition,
				const GLIAVertexStreamDefinition & pVertexStreamDefinition ) noexcept
		{
			if( !pInputLayoutDefinition.activeAttributesMask.is_set_any_of( eIAVertexAttributeMaskAll ) )
			{
				return false;
			}

			glBindVertexArray( pVertexArrayObject.mGLHandle );
			ic3OpenGLHandleLastError();

			GLuint currentVBBinding = 0;

			for( uint32 attributeIndex = 0; attributeIndex < GCM::cxIAMaxVertexAttributesNum; ++attributeIndex )
			{
				const auto attributeBit = CxDef::makeIAVertexAttributeFlag( attributeIndex );
				if( pInputLayoutDefinition.activeAttributesMask.is_set( attributeBit ) )
				{
					const auto & glcAttribute = pInputLayoutDefinition.attributeArray[attributeIndex];
					const auto & glcVertexBufferBinding = pVertexStreamDefinition.vertexBufferBindings.GetBinding( glcAttribute.streamIndex );

					ic3DebugAssert( glcVertexBufferBinding );

					if( currentVBBinding != glcVertexBufferBinding.handle )
					{
						glBindBuffer( GL_ARRAY_BUFFER, glcVertexBufferBinding.handle );
						ic3OpenGLHandleLastError();

						currentVBBinding = glcVertexBufferBinding.handle;
					}

					glEnableVertexAttribArray( attributeIndex );
					ic3OpenGLHandleLastError();

					glVertexAttribDivisor( attributeIndex, glcAttribute.instanceRate );
					ic3OpenGLHandleLastError();

					glVertexAttribPointer(
						attributeIndex,
						glcAttribute.componentsNum,
						glcAttribute.baseType,
						glcAttribute.normalized,
						glcVertexBufferBinding.stride,
						reinterpret_cast<const void *>( glcVertexBufferBinding.offset + glcAttribute.relativeOffset ) );
					ic3OpenGLHandleLastError();
				}
			}

			glBindBuffer( GL_ARRAY_BUFFER, 0u );
			ic3OpenGLHandleLastError();

			glBindVertexArray( 0 );
			ic3OpenGLHandleLastError();

			return true;
		}

	}

}
