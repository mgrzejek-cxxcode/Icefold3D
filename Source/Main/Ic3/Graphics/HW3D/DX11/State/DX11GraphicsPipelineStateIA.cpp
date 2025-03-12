
#include "DX11GraphicsPipelineStateIA.h"
#include <Ic3/Graphics/HW3D/DX11/DX11APITranslationLayer.h>
#include <Ic3/Graphics/HW3D/DX11/DX11gpuDevice.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11GPUBuffer.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11Shader.h>

#include <regex>

namespace Ic3::Graphics::GCI
{

	DX11VertexAttributeLayoutDescriptor::DX11VertexAttributeLayoutDescriptor(
			DX11GPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig,
			ComPtr<ID3D11InputLayout> pD3D11InputLayout,
			D3D11_PRIMITIVE_TOPOLOGY pD3D11PrimitiveTopology )
	: HW3DPipelineStateDescriptor( pGPUDevice, _commonAttributeLayoutConfig )
	, mD3D11InputLayout( pD3D11InputLayout )
	, mD3D11PrimitiveTopology( pD3D11PrimitiveTopology )
	, _commonAttributeLayoutConfig( pCommonAttributeLayoutConfig )
	{}

	DX11VertexAttributeLayoutDescriptor::~DX11VertexAttributeLayoutDescriptor() = default;

	TGfxHandle<DX11VertexAttributeLayoutDescriptor> DX11VertexAttributeLayoutDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutDefinition & pAttributeLayoutDefinition,
			const ShaderBinary & pVertexShaderBinary )
	{
		const auto dx11AttributeLayoutDefinition = GCU::DX11IATranslateVertexAttributeLayoutDefinition( pAttributeLayoutDefinition );

		ComPtr<ID3D11InputLayout> d3d11InputLayout;
		const auto hResult = pGPUDevice.mD3D11Device1->CreateInputLayout(
				dx11AttributeLayoutDefinition.d3d11AttributeArray.data(),
				dx11AttributeLayoutDefinition.GetActiveAttributesNum(),
				pVertexShaderBinary.dataBuffer,
				pVertexShaderBinary.dataSizeInBytes,
				d3d11InputLayout.GetAddressOf() );

		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		const auto stateDescriptor = CreateGfxObject<DX11VertexAttributeLayoutDescriptor>(
				pGPUDevice,
				dx11AttributeLayoutDefinition,
				d3d11InputLayout,
				dx11AttributeLayoutDefinition.d3d11PrimitiveTopology );

		return stateDescriptor;
	}


	DX11VertexSourceBindingDescriptor::DX11VertexSourceBindingDescriptor(
			DX11GPUDevice & pGPUDevice,
			const DX11IAVertexSourceBinding & pDX11VertexSourceBinding )
	: HW3DPipelineStateDescriptor( pGPUDevice, mDX11VertexSourceBinding )
	, mDX11VertexSourceBinding( pDX11VertexSourceBinding )
	{}

	DX11VertexSourceBindingDescriptor::~DX11VertexSourceBindingDescriptor() = default;

	TGfxHandle<DX11VertexSourceBindingDescriptor> DX11VertexSourceBindingDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition )
	{
		const auto dx11SourceBindingDefinition = GCU::DX11IATranslateVertexSourceBindingDefinition( pVertexSourceBindingDefinition );

		const auto stateDescriptor = CreateGfxObject<DX11VertexSourceBindingDescriptor>( pGPUDevice, dx11SourceBindingDefinition );

		return stateDescriptor;
	}

	
	namespace GCU
	{

		D3D11_INPUT_ELEMENT_DESC DX11IATranslateIAVertexAttributeDesc(
				const IAVertexAttributeDesc & pVertexAttributeDesc )
		{
			D3D11_INPUT_ELEMENT_DESC d3D11InputElementDesc;

			d3D11InputElementDesc.AlignedByteOffset = static_cast<UINT>( pVertexAttributeDesc.streamBinding.streamRelativeOffset );
			d3D11InputElementDesc.SemanticName = pVertexAttributeDesc.semantics.semanticName.data();
			d3D11InputElementDesc.SemanticIndex = pVertexAttributeDesc.semantics.semanticIndex;
			d3D11InputElementDesc.Format = ATL::DXTranslateVertexAttribFormat( pVertexAttributeDesc.attribInfo.dataFormat );
			d3D11InputElementDesc.InputSlot = pVertexAttributeDesc.streamBinding.streamSlot;
			d3D11InputElementDesc.InputSlotClass = ATL::DXTranslateVertexAttribDataRate( pVertexAttributeDesc.attribInfo.dataRate );
			d3D11InputElementDesc.InstanceDataStepRate = pVertexAttributeDesc.attribInfo.instanceStepRate;

			return d3D11InputElementDesc;
		}

		DX11IAVertexAttributeLayoutDefinition DX11IATranslateVertexAttributeLayoutDefinition(
				const IAVertexAttributeLayoutDefinition & pVertexAttributeLayoutDefinition )
		{
			DX11IAVertexAttributeLayoutDefinition dx11AttributeLayoutDefinition;

			const auto activeVertexAttributesNum = pVertexAttributeLayoutDefinition.GetActiveAttributesNum();
			for( uint32 inputAttributeIndex = 0, currentVertexAttributesNum = 0; inputAttributeIndex < GCM::kIAMaxVertexAttributesNum; ++inputAttributeIndex )
			{
				const auto attributeBit = CXU::IAMakeVertexAttributeFlag( inputAttributeIndex );
				if( pVertexAttributeLayoutDefinition.activeAttributesMask.is_set( attributeBit ) )
				{
					const auto & inputAttributeDesc = pVertexAttributeLayoutDefinition.attributeArray[inputAttributeIndex];
					auto & d3D11InputElementDesc = dx11AttributeLayoutDefinition.d3d11AttributeArray[currentVertexAttributesNum];

					// Translate the attribute data. This includes the relative offset.
					d3D11InputElementDesc = DX11IATranslateIAVertexAttributeDesc( inputAttributeDesc );

					++currentVertexAttributesNum;

					if( currentVertexAttributesNum == activeVertexAttributesNum )
					{
						break;
					}
				}
			}

			dx11AttributeLayoutDefinition.activeAttributesMask = pVertexAttributeLayoutDefinition.activeAttributesMask;
			dx11AttributeLayoutDefinition.primitiveTopology = pVertexAttributeLayoutDefinition.primitiveTopology;
			dx11AttributeLayoutDefinition.d3d11PrimitiveTopology = ATL::TranslateDX11PrimitiveTopology( pVertexAttributeLayoutDefinition.primitiveTopology );

			return dx11AttributeLayoutDefinition;
		}

		DX11IAVertexSourceBinding DX11IATranslateVertexSourceBindingDefinition(
			const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition )
		{
			DX11IAVertexSourceBinding dx11VertexSourceBinding;
			DX11IAUpdateVertexSourceBindingDefinition( pVertexSourceBindingDefinition, dx11VertexSourceBinding );
			return dx11VertexSourceBinding;
		}

		bool DX11IAUpdateVertexSourceBindingDefinition(
			const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition,
			DX11IAVertexSourceBinding & pOutDX11VertexSourceBinding )
		{

			for( native_uint vertexStreamIndex = 0; vertexStreamIndex < GCM::kIAMaxDataStreamVertexBuffersNum; ++vertexStreamIndex )
			{
				const auto & inputVertexBufferRef = pVertexSourceBindingDefinition.vertexBufferReferences[vertexStreamIndex];
				const auto bufferBindingBit = CXU::IAMakeVertexBufferBindingFlag( vertexStreamIndex );

				if( inputVertexBufferRef && pVertexSourceBindingDefinition.activeStreamsMask.is_set( bufferBindingBit ) )
				{
					const auto * dx11VertexBuffer = inputVertexBufferRef.sourceBuffer->QueryInterface<DX11GPUBuffer>();

					pOutDX11VertexSourceBinding.vertexBufferBindings.bindingData.bufferArray[vertexStreamIndex] =
							dx11VertexBuffer->mD3D11Buffer.Get();

					pOutDX11VertexSourceBinding.vertexBufferBindings.bindingData.offsetArray[vertexStreamIndex] =
							cppx::numeric_cast<UINT>( inputVertexBufferRef.relativeOffset );

					pOutDX11VertexSourceBinding.vertexBufferBindings.bindingData.strideArray[vertexStreamIndex] =
							cppx::numeric_cast<UINT>( inputVertexBufferRef.refParams.vertexStride );
				}
			}

			if( pVertexSourceBindingDefinition.indexBufferReference && pVertexSourceBindingDefinition.activeStreamsMask.is_set( eIAVertexSourceBindingFlagIndexBufferBit ) )
			{
				const auto * dx11IndexBuffer = pVertexSourceBindingDefinition.indexBufferReference.sourceBuffer->QueryInterface<DX11GPUBuffer>();

				pOutDX11VertexSourceBinding.indexBufferBinding.buffer = dx11IndexBuffer->mD3D11Buffer.Get();

				pOutDX11VertexSourceBinding.indexBufferBinding.offset =
						cppx::numeric_cast<UINT>( pVertexSourceBindingDefinition.indexBufferReference.relativeOffset );

				pOutDX11VertexSourceBinding.indexBufferBinding.format =
						ATL::DXTranslateIndexDataFormat( pVertexSourceBindingDefinition.indexBufferReference.refParams.indexFormat );
			}

			pOutDX11VertexSourceBinding.activeStreamsMask = pVertexSourceBindingDefinition.activeStreamsMask;
			pOutDX11VertexSourceBinding.vertexBufferBindings.activeRanges = GCU::IAGenerateActiveVertexBuffersRanges( pVertexSourceBindingDefinition.vertexBufferReferences );

			return !pOutDX11VertexSourceBinding.IsEmpty();
		}

	}

}
