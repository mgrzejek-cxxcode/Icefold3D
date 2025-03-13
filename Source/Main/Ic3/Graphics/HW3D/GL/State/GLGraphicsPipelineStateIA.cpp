
#include "GLGraphicsPipelineStateIA.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLVertexArrayObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLGPUBuffer.h>

namespace Ic3::Graphics::GCI
{

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
			GLIAVertexSourceBindingPtr pGLVertexSourceBinding )
	: HW3DPipelineStateDescriptor( pGPUDevice, *pGLVertexSourceBinding )
	, mGLVertexSourceBinding( std::move( pGLVertexSourceBinding ) )
	{}

	GLVertexSourceBindingDescriptor::~GLVertexSourceBindingDescriptor() = default;

	TGfxHandle<GLVertexSourceBindingDescriptor> GLVertexSourceBindingDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		auto glcVertexSourceBindingPtr = GCU::IATranslateVertexSourceBindingDefinitionGL( pCreateInfo.bindingDefinition );
		const auto stateDescriptor = CreateGfxObject<GLVertexSourceBindingDescriptor>( pGPUDevice, std::move( glcVertexSourceBindingPtr ) );

		return stateDescriptor;
	}

}
