
#include "DX11inputAssembler.h"
#include "../DX11ApiTranslationLayer.h"
#include "../DX11gpuDevice.h"
#include "../Resources/DX11GpuBuffer.h"
#include "../Resources/DX11Shader.h"

namespace Ic3::Graphics::GCI
{

	DX11IAInputLayoutImmutableState::DX11IAInputLayoutImmutableState(
			DX11GpuDevice & pGpuDevice,
			const IAInputLayoutStateCommonProperties & pCommonProperties,
			ComPtr<ID3D11InputLayout> pD3D11InputLayout,
			D3D11_PRIMITIVE_TOPOLOGY pD3D11PrimitiveTopology )
	: IAInputLayoutImmutableState( pGpuDevice, pCommonProperties )
	, mD3D11InputLayout( pD3D11InputLayout )
	, mD3D11PrimitiveTopology( pD3D11PrimitiveTopology )
	{}

	DX11IAInputLayoutImmutableState::~DX11IAInputLayoutImmutableState() = default;

	GpaHandle<DX11IAInputLayoutImmutableState> DX11IAInputLayoutImmutableState::CreateInstance(
			DX11GpuDevice & pGpuDevice,
			const IAInputLayoutDefinition & pInputLayoutDefinition,
			const ShaderBinary & pVertexShaderBinary )
	{
		const auto inputLayoutCommonProperties = SMU::GetIAInputLayoutStateCommonProperties( pInputLayoutDefinition );
		const auto dx11InputLayoutDefinition = SMU::TranslateDX11IAInputLayoutDefinition( pInputLayoutDefinition );

		ComPtr<ID3D11InputLayout> d3d11InputLayout;
		const auto hResult = pGpuDevice.mD3D11Device1->CreateInputLayout(
				dx11InputLayoutDefinition.attributeArray.data(),
				dx11InputLayoutDefinition.mActiveAttributesNum,
				pVertexShaderBinary.dataBuffer,
				pVertexShaderBinary.dataSizeInBytes,
				d3d11InputLayout.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			ic3DebugInterrupt();
			return nullptr;
		}

		auto immutableState = CreateGfxObject<DX11IAInputLayoutImmutableState>(
				pGpuDevice,
				inputLayoutCommonProperties,
				d3d11InputLayout,
				dx11InputLayoutDefinition.primitiveTopology );

		return immutableState;
	}


	DX11IAVertexStreamImmutableState::DX11IAVertexStreamImmutableState(
			DX11GpuDevice & pGpuDevice,
			const IAVertexStreamStateCommonProperties & pCommonProperties,
			const DX11IAVertexStreamDefinition & pDX11VertexStreamDefinition )
	: IAVertexStreamImmutableState( pGpuDevice, pCommonProperties )
	, mDX11VertexStreamDefinition( pDX11VertexStreamDefinition )
	{}

	DX11IAVertexStreamImmutableState::~DX11IAVertexStreamImmutableState() = default;

	GpaHandle<DX11IAVertexStreamImmutableState> DX11IAVertexStreamImmutableState::CreateInstance(
			DX11GpuDevice & pGpuDevice,
			const IAVertexStreamDefinition & pVertexStreamDefinition )
	{
		const auto vertexStreamCommonProperties = SMU::GetIAVertexStreamStateCommonProperties( pVertexStreamDefinition );
		const auto dx11VertexStreamDefinition = SMU::TranslateDX11IAVertexStreamDefinition( pVertexStreamDefinition );

		auto immutableState = CreateGfxObject<DX11IAVertexStreamImmutableState>(
				pGpuDevice,
				vertexStreamCommonProperties,
				dx11VertexStreamDefinition );

		return immutableState;
	}

	
	namespace SMU
	{

		D3D11_INPUT_ELEMENT_DESC TranslateDX11IAVertexAttributeInfo(
				const IAVertexAttributeInfo & pAttributeInfo )
		{
			D3D11_INPUT_ELEMENT_DESC d3D11InputElementDesc;

			d3D11InputElementDesc.AlignedByteOffset = static_cast<UINT>( pAttributeInfo.relativeOffset );
			d3D11InputElementDesc.SemanticName = pAttributeInfo.semanticName.c_str();
			d3D11InputElementDesc.SemanticIndex = pAttributeInfo.semanticIndex;
			d3D11InputElementDesc.Format = ATL::TranslateDXVertexAttribFormat( pAttributeInfo.format );
			d3D11InputElementDesc.InputSlot = pAttributeInfo.streamIndex;
			d3D11InputElementDesc.InputSlotClass = ( pAttributeInfo.instanceRate == 0 ) ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
			d3D11InputElementDesc.InstanceDataStepRate = pAttributeInfo.instanceRate;

			return d3D11InputElementDesc;
		}

		DX11IAInputLayoutDefinition TranslateDX11IAInputLayoutDefinition(
				const IAInputLayoutDefinition & pDefinition )
		{
			DX11IAInputLayoutDefinition dx11InputLayoutDefinition;

			const auto definedVertexAttributesNum = pop_count( pDefinition.mActiveAttributesMask );

			uint32 currentVertexAttributesNum = 0;
			uint64 currentAttributePackedRelativeOffset = 0;

			for( uint32 attributeIndex = 0; attributeIndex < GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM; ++attributeIndex )
			{
				const auto attributeBit = CxDef::makeIAVertexAttributeFlag( attributeIndex );
				if( pDefinition.mActiveAttributesMask.is_set( attributeBit ) )
				{
					const auto & inputAttributeInfo = pDefinition.attributeArray[attributeIndex];
					auto & dx11AttributeInfo = dx11InputLayoutDefinition.attributeArray[currentVertexAttributesNum];

					// Translate the attribute data. This includes the relative offset.
					dx11AttributeInfo = TranslateDX11IAVertexAttributeInfo( inputAttributeInfo );

					if( inputAttributeInfo.relativeOffset == CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND )
					{
						// If the offset is APPEND, update it with the current packed offset calculated.
						dx11AttributeInfo.AlignedByteOffset = cppx::numeric_cast<uint32>( mem_get_aligned_value( currentAttributePackedRelativeOffset, 4 ) );
					}
					else if( inputAttributeInfo.relativeOffset == CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 )
					{
						// If the offset is APPEND, update it with the current packed offset calculated.
						dx11AttributeInfo.AlignedByteOffset = cppx::numeric_cast<uint32>( mem_get_aligned_value( currentAttributePackedRelativeOffset, 16 ) );
					}

					// Update the current packed offset.
					const auto attributeByteSize = CxDef::GetVertexAttribFormatByteSize( inputAttributeInfo.format );
					currentAttributePackedRelativeOffset = dx11AttributeInfo.AlignedByteOffset + attributeByteSize;

					++currentVertexAttributesNum;

					if( currentVertexAttributesNum == definedVertexAttributesNum )
					{
						break;
					}
				}
			}

			dx11InputLayoutDefinition.mActiveAttributesNum = currentVertexAttributesNum;
			dx11InputLayoutDefinition.primitiveTopology = ATL::TranslateDX11PrimitiveTopology( pDefinition.primitiveTopology );

			return dx11InputLayoutDefinition;
		}

		DX11IAVertexStreamDefinition TranslateDX11IAVertexStreamDefinition(
				const IAVertexStreamDefinition & pDefinition )
		{
			DX11IAVertexStreamDefinition dx11IAVertexStreamDefinition;

			if( pDefinition.indexBufferReference )
			{
				auto * dx11IndexBuffer = pDefinition.indexBufferReference.sourceBuffer->QueryInterface<DX11GpuBuffer>();

				dx11IAVertexStreamDefinition.indexBufferBinding.buffer = dx11IndexBuffer->mD3D11Buffer.Get();

				dx11IAVertexStreamDefinition.indexBufferBinding.offset =
						cppx::numeric_cast<uint32>( pDefinition.indexBufferReference.relativeOffset );

				dx11IAVertexStreamDefinition.indexBufferBinding.format =
						ATL::TranslateDXShaderCompileFlags( static_cast<EBaseDataType>( pDefinition.indexBufferReference.indexFormat ) );
			}

			for( native_uint vertexInputStreamIndex = 0; vertexInputStreamIndex < GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM; ++vertexInputStreamIndex )
			{
				if( const auto & vertexBufferReference = pDefinition.vertexBufferReferences[vertexInputStreamIndex] )
				{
					auto * dx11VertexBuffer = vertexBufferReference.sourceBuffer->QueryInterface<DX11GpuBuffer>();

					dx11IAVertexStreamDefinition.vertexBufferBindings.bindingData.bufferArray[vertexInputStreamIndex] =
							dx11VertexBuffer->mD3D11Buffer.Get();

					dx11IAVertexStreamDefinition.vertexBufferBindings.bindingData.offsetArray[vertexInputStreamIndex] =
							cppx::numeric_cast<UINT>( vertexBufferReference.relativeOffset );

					dx11IAVertexStreamDefinition.vertexBufferBindings.bindingData.strideArray[vertexInputStreamIndex] =
							cppx::numeric_cast<UINT>( vertexBufferReference.vertexStride );
				}
			}

			dx11IAVertexStreamDefinition.activeBindingsMask = pDefinition.activeBindingsMask;
			dx11IAVertexStreamDefinition.vertexBufferBindings.activeRanges = SMU::GenerateActiveVertexBufferRanges( pDefinition.vertexBufferReferences );

			return dx11IAVertexStreamDefinition;
		}

	}

}
