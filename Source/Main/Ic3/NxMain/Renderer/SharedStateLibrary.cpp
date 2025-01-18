
#include "gpaSharedStateLibrary.h"
#include "ShaderLibrary.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <Ic3/Graphics/GCI/State/InputAssemblerImmutableStates.h>
#include <Ic3/Graphics/GCI/State/SharedImmutableStateCache.h>

namespace Ic3
{

	GpaSharedStateLibrary::GpaSharedStateLibrary( const CoreEngineState & pCES, ShaderLibraryHandle pShaderLibrary )
	: CoreEngineObject( pCES )
	, mShaderLibrary( pShaderLibrary ? pShaderLibrary : CreateDynamicObject<ShaderLibrary>( pCES ) )
	{}

	GpaSharedStateLibrary::~GpaSharedStateLibrary() = default;

	void GpaSharedStateLibrary::initializeDefaultImmutableBlendStates()
	{
		{
			mCES.mGPUDevice->createCachedImmutableState<GCI::BlendImmutableState>(
					CxDef::GPA_STATE_ID_IA_INPUT_LAYOUT_DEFAULT,
					GCI::defaults::cvPipelineBlendConfigDefault );
		}
	}

	void GpaSharedStateLibrary::initializeDefaultImmutableDepthStencilStates()
	{
		{
			mCES.mGPUDevice->createCachedImmutableState<GCI::DepthStencilImmutableState>(
					CxDef::GPA_STATE_ID_IA_INPUT_LAYOUT_DEFAULT,
					GCI::defaults::cvPipelineDepthStencilConfigDefault );
		}
		{
			mCES.mGPUDevice->createCachedImmutableState<GCI::DepthStencilImmutableState>(
					CxDef::GPA_STATE_ID_DEPTH_STENCIL_DEPTH_TEST_ENABLE,
					GCI::defaults::cvPipelineDepthStencilConfigEnableDepthTest );
		}
	}

	void GpaSharedStateLibrary::initializeDefaultImmutableRasterizerStates()
	{
		{
			GCI::RasterizerConfig rasterizerConfig = GCI::defaults::cvPipelineRasterizerConfigDefault;
			rasterizerConfig.cullMode = GCI::ECullMode::Back;
			rasterizerConfig.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;

			mCES.mGPUDevice->createCachedImmutableState<GCI::RasterizerImmutableState>(
					CxDef::GPA_STATE_ID_RASTERIZER_DEFAULT_CULL_BACK_CCW,
					rasterizerConfig );
		}
		{
			GCI::RasterizerConfig rasterizerConfig = GCI::defaults::cvPipelineRasterizerConfigDefault;
			rasterizerConfig.cullMode = GCI::ECullMode::Back;
			rasterizerConfig.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;

			mCES.mGPUDevice->createCachedImmutableState<GCI::RasterizerImmutableState>(
					CxDef::GPA_STATE_ID_RASTERIZER_DEFAULT_CULL_BACK_CW,
					rasterizerConfig );
		}
		{
			GCI::RasterizerConfig rasterizerConfig = GCI::defaults::cvPipelineRasterizerConfigDefault;
			rasterizerConfig.cullMode = GCI::ECullMode::Front;
			rasterizerConfig.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;

			mCES.mGPUDevice->createCachedImmutableState<GCI::RasterizerImmutableState>(
					CxDef::GPA_STATE_ID_RASTERIZER_DEFAULT_CULL_FRONT_CCW,
					rasterizerConfig );
		}
		{
			GCI::RasterizerConfig rasterizerConfig = GCI::defaults::cvPipelineRasterizerConfigDefault;
			rasterizerConfig.cullMode = GCI::ECullMode::Front;
			rasterizerConfig.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;

			mCES.mGPUDevice->createCachedImmutableState<GCI::RasterizerImmutableState>(
					CxDef::GPA_STATE_ID_RASTERIZER_DEFAULT_CULL_FRONT_CW,
					rasterizerConfig );
		}
	}

	void GpaSharedStateLibrary::initializeDefaultImmutableInputLayoutStates()
	{
		{
			GCI::IAInputLayoutDefinition inputLayoutDefinition;
			inputLayoutDefinition.primitiveTopology = GCI::EPrimitiveTopology::TriangleList;
			inputLayoutDefinition.mActiveAttributesMask =
					GCI::eIAVertexAttributeFlagAttr0Bit |
					GCI::eIAVertexAttributeFlagAttr1Bit |
					GCI::eIAVertexAttributeFlagAttr2Bit |
					GCI::eIAVertexAttributeFlagAttr3Bit;
			inputLayoutDefinition.attributeArray[0] = { 0, "POSITION", 0, Ic3::Graphics::GCI::EVertexAttribFormat::Vec3F32, 0 };
			inputLayoutDefinition.attributeArray[1] = { 0, "COLOR", 0, Ic3::Graphics::GCI::EVertexAttribFormat::Vec4F32, Ic3::Graphics::GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND };
			inputLayoutDefinition.attributeArray[2] = { 0, "NORMAL", 0, Ic3::Graphics::GCI::EVertexAttribFormat::Vec3F32, Ic3::Graphics::GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND };
			inputLayoutDefinition.attributeArray[3] = { 0, "TEXCOORD", 0, Ic3::Graphics::GCI::EVertexAttribFormat::Vec2F32, Ic3::Graphics::GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND };

			const auto shaderPassthroughIADefaultVS = mShaderLibrary->getShader( CxDef::GPA_RESOURCE_ID_SHADER_PASSTHROUGH_IA_DEFAULT_VS );

			mCES.mGPUDevice->createCachedImmutableState<GCI::IAInputLayoutImmutableState>(
					CxDef::GPA_STATE_ID_IA_INPUT_LAYOUT_DEFAULT,
					inputLayoutDefinition,
					*shaderPassthroughIADefaultVS );
		}
		{
			GCI::IAInputLayoutDefinition inputLayoutDefinition;
			inputLayoutDefinition.primitiveTopology = GCI::EPrimitiveTopology::TriangleList;
			inputLayoutDefinition.mActiveAttributesMask =
					GCI::eIAVertexAttributeFlagAttr0Bit |
					GCI::eIAVertexAttributeFlagAttr1Bit |
					GCI::eIAVertexAttributeFlagAttr2Bit |
					GCI::eIAVertexAttributeFlagAttr3Bit;
			inputLayoutDefinition.attributeArray[0] = { 0, "POSITION", 0, Ic3::Graphics::GCI::EVertexAttribFormat::Vec3F32, 0 };
			inputLayoutDefinition.attributeArray[1] = { 0, "COLOR", 0, Ic3::Graphics::GCI::EVertexAttribFormat::Vec4F32, Ic3::Graphics::GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 };
			inputLayoutDefinition.attributeArray[2] = { 0, "NORMAL", 0, Ic3::Graphics::GCI::EVertexAttribFormat::Vec3F32, Ic3::Graphics::GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 };
			inputLayoutDefinition.attributeArray[3] = { 0, "TEXCOORD", 0, Ic3::Graphics::GCI::EVertexAttribFormat::Vec2F32, Ic3::Graphics::GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 };

			const auto shaderPassthroughIADefaultVS = mShaderLibrary->getShader( CxDef::GPA_RESOURCE_ID_SHADER_PASSTHROUGH_IA_DEFAULT_VS );

			mCES.mGPUDevice->createCachedImmutableState<GCI::IAInputLayoutImmutableState>(
					CxDef::GPA_STATE_ID_IA_INPUT_LAYOUT_DEFAULT_16B,
					inputLayoutDefinition,
					*shaderPassthroughIADefaultVS );
		}
	}

	void GpaSharedStateLibrary::initializeDefaultPipelineStateObjects()
	{
	}

}
