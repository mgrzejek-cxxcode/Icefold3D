
#include "SharedStateLibrary.h"
#include "ShaderLibrary.h"
#include "../GCI/VertexFormatUtils.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3
{

	GCISharedStateLibrary::GCISharedStateLibrary( ShaderLibrary & pShaderLibrary )
	: CoreEngineObject( pShaderLibrary.mCES )
	, mShaderLibrary( pShaderLibrary.GetHandle<ShaderLibrary>() )
	{}

	GCISharedStateLibrary::~GCISharedStateLibrary() = default;

	GCI::ShaderHandle GCISharedStateLibrary::GetShader( GfxObjectID pShaderID ) const noexcept
	{
		return mShaderLibrary->GetShader( pShaderID );
	}

	GCI::ShaderHandle GCISharedStateLibrary::GetShader( const GfxObjectName & pShaderName ) const noexcept
	{
		return mShaderLibrary->GetShader( pShaderName );
	}

	void GCISharedStateLibrary::Initialize()
	{
		CreateDefaultShaders();
		InitializeDefaultBlendStateDescriptors();
		InitializeDefaultDepthStencilStateDescriptors();
		InitializeDefaultRasterizerStateDescriptors();
		InitializeDefaultInputLayoutStateDescriptors();
		InitializeDefaultPipelineStateObjects();
	}

	void GCISharedStateLibrary::CreateDefaultShaders()
	{
	}

	void GCISharedStateLibrary::InitializeDefaultBlendStateDescriptors()
	{
		GCI::BlendStateDescriptorCreateInfo descriptorCreateInfo{};
		descriptorCreateInfo.flags = GCI::ePipelineStateDescriptorCreateMaskDefault;
		descriptorCreateInfo.blendSettings = mCES.mGPUDevice->GetDefaultBlendSettings();
		{
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorBlendDefault;
			mCES.mGPUDevice->CreateBlendStateDescriptor( descriptorCreateInfo );
		}
	}

	void GCISharedStateLibrary::InitializeDefaultDepthStencilStateDescriptors()
	{
		GCI::DepthStencilStateDescriptorCreateInfo descriptorCreateInfo{};
		descriptorCreateInfo.flags = GCI::ePipelineStateDescriptorCreateMaskDefault;
		{
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorDepthStencilDefault;
			descriptorCreateInfo.depthStencilSettings = mCES.mGPUDevice->GetDefaultDepthStencilSettings();
			mCES.mGPUDevice->CreateDepthStencilStateDescriptor( descriptorCreateInfo );
		}
		{
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorDepthStencilDepthTestEnable;
			descriptorCreateInfo.depthStencilSettings = mCES.mGPUDevice->GetDefaultDepthStencilSettingsWithDepthTestEnabled();
			mCES.mGPUDevice->CreateDepthStencilStateDescriptor( descriptorCreateInfo );
		}
	}

	void GCISharedStateLibrary::InitializeDefaultRasterizerStateDescriptors()
	{
		GCI::RasterizerStateDescriptorCreateInfo descriptorCreateInfo{};
		descriptorCreateInfo.flags = GCI::ePipelineStateDescriptorCreateMaskDefault;
		{
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorRasterizerSolidCullBackCCW;
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::Back;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
		}
		{
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorRasterizerSolidCullBackCW;
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::Back;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
		}
		{
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorRasterizerSolidCullNoneCCW;
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
		}
		{
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorRasterizerSolidCullNoneCW;
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
		}
		{
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorRasterizerWireframeCCW;
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Wireframe;
			mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
		}
		{
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorRasterizerWireframeCW;
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Wireframe;
			mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
		}
	}

	void GCISharedStateLibrary::InitializeDefaultInputLayoutStateDescriptors()
	{
		auto shaderPassthroughIADefaultVS = mShaderLibrary->GetShader( "SID_DEFAULT_PASSTHROUGH_VS" );
		if( !shaderPassthroughIADefaultVS )
		{
			Ic3DebugOutputFmt(
					"[GCISharedStateLibrary] Shader %s is missing from the ShaderLibrary, default IA state will not be created.",
					GID::kShaderNamePassthroughIADefaultVS.str() );
			return;
		}

		GCI::VertexAttributeLayoutDescriptorCreateInfo descriptorCreateInfo{};
		descriptorCreateInfo.flags = GCI::ePipelineStateDescriptorCreateMaskDefault;
		descriptorCreateInfo.vertexShaderWithBinary = shaderPassthroughIADefaultVS.get();

        {
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorIAVertexAttributeLayoutDefault;
        	GCIVertexAttributeLayoutBuilder gciAttributeLayoutBuilder{};
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysPosition );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysFixedColorU8N );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysNormal );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysTexCoord2D0 );
            descriptorCreateInfo.layoutDefinition =
                gciAttributeLayoutBuilder.CreateGCIVertexAttributeLayoutDefinition( GCI::EPrimitiveTopology::TriangleList );

	        auto vtxAttrLayoutSD = mCES.mGPUDevice->CreateVertexAttributeLayoutDescriptor( descriptorCreateInfo );
	        vtxAttrLayoutSD.reset();
        }
        {
			descriptorCreateInfo.descriptorID = GID::kGfxIDStateDescriptorIAVertexAttributeLayoutDefault16B;
        	GCIVertexAttributeLayoutBuilder gciAttributeLayoutBuilder{};
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysPosition, GCI::kIAVertexAttributePaddingAlign16 );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysFixedColorU8N, GCI::kIAVertexAttributePaddingAlign16 );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysNormal, GCI::kIAVertexAttributePaddingAlign16 );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysTexCoord2D0, GCI::kIAVertexAttributePaddingAlign16 );
            descriptorCreateInfo.layoutDefinition =
                gciAttributeLayoutBuilder.CreateGCIVertexAttributeLayoutDefinition( GCI::EPrimitiveTopology::TriangleList );

	        auto vtxAttrLayoutSD = mCES.mGPUDevice->CreateVertexAttributeLayoutDescriptor( descriptorCreateInfo );
	        vtxAttrLayoutSD.reset();
        }
	}

	void GCISharedStateLibrary::InitializeDefaultPipelineStateObjects()
	{
	}

}
