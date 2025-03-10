
#include "SharedStateLibrary.h"
#include "ShaderLibrary.h"
#include "../GCI/VertexFormatUtils.h"

#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorCommon.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIA.h>

namespace Ic3
{

	static GCISharedDescriptorsIDSet sSharedDescriptorsGlobalStore;

	const GCISharedDescriptorsIDSet & GCISharedStateLibrary::sSharedDescriptors = sSharedDescriptorsGlobalStore;


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
		descriptorCreateInfo.blendSettings = mCES.mGPUDevice->GetDefaultBlendSettings();
		{
			const auto descriptor = mCES.mGPUDevice->CreateBlendStateDescriptor( descriptorCreateInfo );
			sSharedDescriptorsGlobalStore.descriptorIDBlendDefault = descriptor->mDescriptorID;
		}
	}

	void GCISharedStateLibrary::InitializeDefaultDepthStencilStateDescriptors()
	{
		GCI::DepthStencilStateDescriptorCreateInfo descriptorCreateInfo{};
		{
			descriptorCreateInfo.depthStencilSettings = mCES.mGPUDevice->GetDefaultDepthStencilSettings();
			const auto descriptor = mCES.mGPUDevice->CreateDepthStencilStateDescriptor( descriptorCreateInfo );
			sSharedDescriptorsGlobalStore.descriptorIDDepthStencilDefault = descriptor->mDescriptorID;
		}
		{
			descriptorCreateInfo.depthStencilSettings = mCES.mGPUDevice->GetDefaultDepthStencilSettingsWithDepthTestEnabled();
			const auto descriptor = mCES.mGPUDevice->CreateDepthStencilStateDescriptor( descriptorCreateInfo );
			sSharedDescriptorsGlobalStore.descriptorIDDepthStencilDepthTestEnable = descriptor->mDescriptorID;
		}
	}

	void GCISharedStateLibrary::InitializeDefaultRasterizerStateDescriptors()
	{
		GCI::RasterizerStateDescriptorCreateInfo descriptorCreateInfo{};
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::Back;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerSolidCullBackCCW = descriptor->mDescriptorID;
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::Back;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerSolidCullBackCW = descriptor->mDescriptorID;
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerSolidCullNoneCCW = descriptor->mDescriptorID;
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerSolidCullNoneCW = descriptor->mDescriptorID;
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Wireframe;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerWireframeCCW = descriptor->mDescriptorID;
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Wireframe;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerWireframeCW = descriptor->mDescriptorID;
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
		descriptorCreateInfo.vertexShaderWithBinary = shaderPassthroughIADefaultVS.get();

        {
        	GCIVertexAttributeLayoutBuilder gciAttributeLayoutBuilder{};
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysPosition );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysFixedColorU8N );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysNormal );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysTexCoord2D0 );
            descriptorCreateInfo.layoutDefinition =
                gciAttributeLayoutBuilder.CreateGCIVertexAttributeLayoutDefinition( GCI::EPrimitiveTopology::TriangleList );
	        const auto descriptor = mCES.mGPUDevice->CreateVertexAttributeLayoutDescriptor( descriptorCreateInfo );
	        sSharedDescriptorsGlobalStore.descriptorIDIAVertexAttributeLayoutDefault = descriptor->mDescriptorID;
        }
        {
        	GCIVertexAttributeLayoutBuilder gciAttributeLayoutBuilder{};
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysPosition, GCI::kIAVertexAttributePaddingAlign16 );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysFixedColorF32, GCI::kIAVertexAttributePaddingAlign16 );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysNormal, GCI::kIAVertexAttributePaddingAlign16 );
            gciAttributeLayoutBuilder.AddAttribute( 0, kVertexAttributeKeySysTexCoord2D0, GCI::kIAVertexAttributePaddingAlign16 );
            descriptorCreateInfo.layoutDefinition =
                gciAttributeLayoutBuilder.CreateGCIVertexAttributeLayoutDefinition( GCI::EPrimitiveTopology::TriangleList );
	        const auto descriptor = mCES.mGPUDevice->CreateVertexAttributeLayoutDescriptor( descriptorCreateInfo );
	        sSharedDescriptorsGlobalStore.descriptorIDIAVertexAttributeLayoutDefault16B = descriptor->mDescriptorID;
        }
	}

	void GCISharedStateLibrary::InitializeDefaultPipelineStateObjects()
	{
	}

}
