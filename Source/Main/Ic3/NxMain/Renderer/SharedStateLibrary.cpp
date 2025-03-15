
#include "SharedStateLibrary.h"
#include "ShaderLibrary.h"
#include "../GCI/VertexAttributeKey.h"

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
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDBlendDefault = descriptor->GetDescriptorID();
		}
	}

	void GCISharedStateLibrary::InitializeDefaultDepthStencilStateDescriptors()
	{
		GCI::DepthStencilStateDescriptorCreateInfo descriptorCreateInfo{};
		{
			descriptorCreateInfo.depthStencilSettings = mCES.mGPUDevice->GetDefaultDepthStencilSettings();
			const auto descriptor = mCES.mGPUDevice->CreateDepthStencilStateDescriptor( descriptorCreateInfo );
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDDepthStencilDefault = descriptor->GetDescriptorID();
		}
		{
			descriptorCreateInfo.depthStencilSettings = mCES.mGPUDevice->GetDefaultDepthStencilSettingsWithDepthTestEnabled();
			const auto descriptor = mCES.mGPUDevice->CreateDepthStencilStateDescriptor( descriptorCreateInfo );
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDDepthStencilDepthTestEnable = descriptor->GetDescriptorID();
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
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerSolidCullBackCCW = descriptor->GetDescriptorID();
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::Back;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerSolidCullBackCW = descriptor->GetDescriptorID();
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerSolidCullNoneCCW = descriptor->GetDescriptorID();
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Solid;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerSolidCullNoneCW = descriptor->GetDescriptorID();
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Wireframe;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerWireframeCCW = descriptor->GetDescriptorID();
		}
		{
			descriptorCreateInfo.rasterizerSettings.cullMode = GCI::ECullMode::None;
			descriptorCreateInfo.rasterizerSettings.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
			descriptorCreateInfo.rasterizerSettings.primitiveFillMode = GCI::EPrimitiveFillMode::Wireframe;
			const auto descriptor = mCES.mGPUDevice->CreateRasterizerStateDescriptor( descriptorCreateInfo );
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDRasterizerWireframeCW = descriptor->GetDescriptorID();
		}
	}

	void GCISharedStateLibrary::InitializeDefaultInputLayoutStateDescriptors()
	{
		auto shaderPassthroughIADefaultVS = mShaderLibrary->GetShader( "SID_DEFAULT_PASSTHROUGH_VS" );
		if( !shaderPassthroughIADefaultVS )
		{
			Ic3DebugOutputNx(
					"[GCISharedStateLibrary] Shader %s is missing from the ShaderLibrary, default IA state will not be created.",
					GID::kShaderNamePassthroughIADefaultVS.str() );
			return;
		}

		GCI::VertexAttributeLayoutDescriptorCreateInfo descriptorCreateInfo{};
		descriptorCreateInfo.vertexShaderWithBinary = shaderPassthroughIADefaultVS.get();

		{

			std::array<VertexInputAttributeDefinition, 4> inpuDefs{
				VertexFormat::MakeAttributeDefinition( 0, kVertexAttributeKeySysPosition ),
				VertexFormat::MakeAttributeDefinition( 0, kVertexAttributeKeySysFixedColorU8N ),
				VertexFormat::MakeAttributeDefinition( 0, kVertexAttributeKeySysNormal ),
				VertexFormat::MakeAttributeDefinition( 0, kVertexAttributeKeySysTexCoord2D0 )
			};

			descriptorCreateInfo.layoutDefinition = VertexFormat::GCICreateVertexAttributeLayoutDefinition( cppx::bind_array_view( inpuDefs ) );
			descriptorCreateInfo.layoutDefinition.primitiveTopology = GCI::EPrimitiveTopology::TriangleList;
			const auto descriptor = mCES.mGPUDevice->CreateVertexAttributeLayoutDescriptor( descriptorCreateInfo );
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDIAVertexAttributeLayoutDefault = descriptor->GetDescriptorID();
		}
		{
			std::array<VertexInputAttributeDefinition, 4> inpuDefs {
				VertexFormat::MakeAttributeDefinition( 0, kVertexAttributeKeySysPosition, kVertexAttributePaddingAlign16 ),
				VertexFormat::MakeAttributeDefinition( 0, kVertexAttributeKeySysFixedColorU8N, kVertexAttributePaddingAlign16 ),
				VertexFormat::MakeAttributeDefinition( 0, kVertexAttributeKeySysNormal, kVertexAttributePaddingAlign16 ),
				VertexFormat::MakeAttributeDefinition( 0, kVertexAttributeKeySysTexCoord2D0, kVertexAttributePaddingAlign16 )
			};

			descriptorCreateInfo.layoutDefinition = VertexFormat::GCICreateVertexAttributeLayoutDefinition( cppx::bind_array_view( inpuDefs ) );
			descriptorCreateInfo.layoutDefinition.primitiveTopology = GCI::EPrimitiveTopology::TriangleList;
			const auto descriptor = mCES.mGPUDevice->CreateVertexAttributeLayoutDescriptor( descriptorCreateInfo );
			Ic3DebugAssert( descriptor );
			sSharedDescriptorsGlobalStore.descriptorIDIAVertexAttributeLayoutDefault16B = descriptor->GetDescriptorID();
		}
	}

	void GCISharedStateLibrary::InitializeDefaultPipelineStateObjects()
	{
	}

}
