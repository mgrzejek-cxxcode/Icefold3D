
#include "DX11inputAssembler.h"
#include "../DX11APITranslationLayer.h"
#include "../DX11gpuDevice.h"
#include "../Resources/DX11GPUBuffer.h"
#include "../Resources/DX11Shader.h"

namespace Ic3::Graphics::GCI
{

	DX11VertexAttributeLayoutDescriptor::DX11VertexAttributeLayoutDescriptor(
			DX11GPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutCommonProperties & pCommonProperties,
			ComPtr<ID3D11InputLayout> pD3D11InputLayout,
			D3D11_PRIMITIVE_TOPOLOGY pD3D11PrimitiveTopology )
	: VertexAttributeLayoutDescriptor( pGPUDevice, pCommonProperties )
	, mD3D11InputLayout( pD3D11InputLayout )
	, mD3D11PrimitiveTopology( pD3D11PrimitiveTopology )
	{}

	DX11VertexAttributeLayoutDescriptor::~DX11VertexAttributeLayoutDescriptor() = default;

	TGfxHandle<DX11VertexAttributeLayoutDescriptor> DX11VertexAttributeLayoutDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutDefinition & pAttributeLayoutDefinition,
			const ShaderBinary & pVertexShaderBinary )
	{
		const auto vertexAttributeLayoutCommonProperties = GCU::GetIAVertexAttributeLayoutCommonProperties( pAttributeLayoutDefinition );
		const auto dx11AttributeLayoutDefinition = GCU::TranslateDX11IAVertexAttributeLayoutDefinition( pAttributeLayoutDefinition );

		ComPtr<ID3D11InputLayout> d3d11InputLayout;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateInputLayout(
				dx11AttributeLayoutDefinition.attributeArray.data(),
				dx11AttributeLayoutDefinition.mActiveAttributesNum,
				pVertexShaderBinary.dataBuffer,
				pVertexShaderBinary.dataSizeInBytes,
				d3d11InputLayout.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto stateDescriptor = CreateGfxObject<DX11VertexAttributeLayoutDescriptor>(
				pGPUDevice,
				vertexAttributeLayoutCommonProperties,
				d3d11InputLayout,
				dx11AttributeLayoutDefinition.primitiveTopology );

		return stateDescriptor;
	}


	DX11VertexSourceBindingDescriptor::DX11VertexSourceBindingDescriptor(
			DX11GPUDevice & pGPUDevice,
			const VertexSourceBindingDescriptorCommonProperties & pCommonProperties,
			const DX11IASourceBindingDefinition & pDX11SourceBindingDefinition )
	: VertexSourceBindingDescriptor( pGPUDevice, pCommonProperties )
	, mDX11SourceBindingDefinition( pDX11SourceBindingDefinition )
	{}

	DX11VertexSourceBindingDescriptor::~DX11VertexSourceBindingDescriptor() = default;

	TGfxHandle<DX11VertexSourceBindingDescriptor> DX11VertexSourceBindingDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const IADataStreamArrayConfiguration & pSourceBindingDefinition )
	{
		const auto vertexStreamCommonProperties = GCU::GetVertexSourceBindingDescriptorCommonProperties( pSourceBindingDefinition );
		const auto dx11SourceBindingDefinition = GCU::TranslateDX11IASourceBindingDefinition( pSourceBindingDefinition );

		auto stateDescriptor = CreateGfxObject<DX11VertexSourceBindingDescriptor>(
				pGPUDevice,
				vertexStreamCommonProperties,
				dx11SourceBindingDefinition );

		return stateDescriptor;
	}

	
	namespace GCU
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

		DX11IAVertexAttributeLayoutDefinition TranslateDX11IAVertexAttributeLayoutDefinition(
				const IAVertexAttributeLayoutDefinition & pDefinition )
		{
			DX11IAVertexAttributeLayoutDefinition dx11AttributeLayoutDefinition;

			const auto definedVertexAttributesNum = pop_count( pDefinition.mActiveAttributesMask );

			uint32 currentVertexAttributesNum = 0;
			uint64 currentAttributePackedRelativeOffset = 0;

			for( uint32 attributeIndex = 0; attributeIndex < GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM; ++attributeIndex )
			{
				const auto attributeBit = CXU::MakeIAVertexAttributeFlag( attributeIndex );
				if( pDefinition.mActiveAttributesMask.is_set( attributeBit ) )
				{
					const auto & inputAttributeInfo = pDefinition.attributeArray[attributeIndex];
					auto & dx11AttributeInfo = dx11AttributeLayoutDefinition.attributeArray[currentVertexAttributesNum];

					// Translate the attribute data. This includes the relative offset.
					dx11AttributeInfo = TranslateDX11IAVertexAttributeInfo( inputAttributeInfo );

					if( inputAttributeInfo.relativeOffset == CXU::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND )
					{
						// If the offset is APPEND, update it with the current packed offset calculated.
						dx11AttributeInfo.AlignedByteOffset = cppx::numeric_cast<uint32>( mem_get_aligned_value( currentAttributePackedRelativeOffset, 4 ) );
					}
					else if( inputAttributeInfo.relativeOffset == CXU::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 )
					{
						// If the offset is APPEND, update it with the current packed offset calculated.
						dx11AttributeInfo.AlignedByteOffset = cppx::numeric_cast<uint32>( mem_get_aligned_value( currentAttributePackedRelativeOffset, 16 ) );
					}

					// Update the current packed offset.
					const auto attributeByteSize = CXU::GetVertexAttribFormatByteSize( inputAttributeInfo.format );
					currentAttributePackedRelativeOffset = dx11AttributeInfo.AlignedByteOffset + attributeByteSize;

					++currentVertexAttributesNum;

					if( currentVertexAttributesNum == definedVertexAttributesNum )
					{
						break;
					}
				}
			}

			dx11AttributeLayoutDefinition.mActiveAttributesNum = currentVertexAttributesNum;
			dx11AttributeLayoutDefinition.primitiveTopology = ATL::TranslateDX11PrimitiveTopology( pDefinition.primitiveTopology );

			return dx11AttributeLayoutDefinition;
		}

		DX11IASourceBindingDefinition TranslateDX11IASourceBindingDefinition(
				const IADataStreamArrayConfiguration & pDefinition )
		{
			DX11IASourceBindingDefinition dx11IASourceBindingDefinition;

			if( pDefinition.indexBufferReference )
			{
				auto * dx11IndexBuffer = pDefinition.indexBufferReference.sourceBuffer->QueryInterface<DX11GPUBuffer>();

				dx11IASourceBindingDefinition.indexBufferBinding.buffer = dx11IndexBuffer->mD3D11Buffer.Get();

				dx11IASourceBindingDefinition.indexBufferBinding.offset =
						cppx::numeric_cast<uint32>( pDefinition.indexBufferReference.relativeOffset );

				dx11IASourceBindingDefinition.indexBufferBinding.format =
						ATL::TranslateDXShaderCompileFlags( static_cast<EBaseDataType>( pDefinition.indexBufferReference.indexFormat ) );
			}

			for( native_uint vertexInputStreamIndex = 0; vertexInputStreamIndex < GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM; ++vertexInputStreamIndex )
			{
				if( const auto & vertexBufferReference = pDefinition.vertexBufferReferences[vertexInputStreamIndex] )
				{
					auto * dx11VertexBuffer = vertexBufferReference.sourceBuffer->QueryInterface<DX11GPUBuffer>();

					dx11IASourceBindingDefinition.vertexBufferBindings.bindingData.bufferArray[vertexInputStreamIndex] =
							dx11VertexBuffer->mD3D11Buffer.Get();

					dx11IASourceBindingDefinition.vertexBufferBindings.bindingData.offsetArray[vertexInputStreamIndex] =
							cppx::numeric_cast<UINT>( vertexBufferReference.relativeOffset );

					dx11IASourceBindingDefinition.vertexBufferBindings.bindingData.strideArray[vertexInputStreamIndex] =
							cppx::numeric_cast<UINT>( vertexBufferReference.vertexStride );
				}
			}

			dx11IASourceBindingDefinition.activeStreamsMask = pDefinition.activeStreamsMask;
			dx11IASourceBindingDefinition.vertexBufferBindings.activeRanges = GCU::GenerateActiveVertexBufferRanges( pDefinition.vertexBufferReferences );

			return dx11IASourceBindingDefinition;
		}

	}

}
