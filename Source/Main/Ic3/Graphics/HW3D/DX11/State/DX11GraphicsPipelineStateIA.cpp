
#include "DX11GraphicsPipelineStateIA.h"
#include <Ic3/Graphics/HW3D/DX11/DX11APITranslationLayer.h>
#include <Ic3/Graphics/HW3D/DX11/DX11GPUDevice.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11GPUBuffer.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11Shader.h>

#include <regex>

namespace Ic3::Graphics::GCI
{

	DX11VertexAttributeLayoutDescriptor::DX11VertexAttributeLayoutDescriptor(
			DX11GPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutMetaData & pAttributeLayoutMetaData,
			ComPtr<ID3D11InputLayout> pD3D11InputLayout,
			D3D11_PRIMITIVE_TOPOLOGY pD3D11PrimitiveTopology )
	: HW3DPipelineStateDescriptor( pGPUDevice, _attributeLayoutMetaData )
	, mD3D11InputLayout( pD3D11InputLayout )
	, mD3D11PrimitiveTopology( pD3D11PrimitiveTopology )
	, _attributeLayoutMetaData( pAttributeLayoutMetaData )
	{}

	DX11VertexAttributeLayoutDescriptor::~DX11VertexAttributeLayoutDescriptor() = default;

	TGfxHandle<DX11VertexAttributeLayoutDescriptor> DX11VertexAttributeLayoutDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutDefinition & pAttributeLayoutDefinition,
			const ShaderBinary & pVertexShaderBinary )
	{
		const auto dx11AttributeLayoutDefinition = Utilities::DX11IATranslateVertexAttributeLayoutDefinition( pAttributeLayoutDefinition );

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
			DX11IAVertexSourceBindingPtr pDX11VertexSourceBinding )
	: HW3DPipelineStateDescriptor( pGPUDevice, *pDX11VertexSourceBinding )
	, mDX11VertexSourceBinding( std::move( pDX11VertexSourceBinding ) )
	{}

	DX11VertexSourceBindingDescriptor::~DX11VertexSourceBindingDescriptor() = default;

	TGfxHandle<DX11VertexSourceBindingDescriptor> DX11VertexSourceBindingDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition )
	{
		auto dx11VertexSourceBinding = Utilities::DX11IACreateVertexSourceBinding( pVertexSourceBindingDefinition );
		auto dx11VertexSourceDescriptor = CreateGfxObject<DX11VertexSourceBindingDescriptor>( pGPUDevice, std::move( dx11VertexSourceBinding ) );

		return dx11VertexSourceDescriptor;
	}

	
	namespace Utilities
	{

		D3D11_INPUT_ELEMENT_DESC DX11IATranslateVertexAttributeDesc(
				const IAVertexAttributeDesc & pVertexAttributeDesc )
		{
			D3D11_INPUT_ELEMENT_DESC d3D11InputElementDesc;

			d3D11InputElementDesc.AlignedByteOffset = static_cast<UINT>( pVertexAttributeDesc.vertexStreamBinding.streamRelativeOffset );
			d3D11InputElementDesc.SemanticName = pVertexAttributeDesc.semantics.semanticName.data();
			d3D11InputElementDesc.SemanticIndex = pVertexAttributeDesc.semantics.semanticIndex;
			d3D11InputElementDesc.Format = ATL::DXTranslateVertexAttribFormat( pVertexAttributeDesc.attribInfo.dataFormat );
			d3D11InputElementDesc.InputSlot = pVertexAttributeDesc.vertexStreamBinding.streamSlot;
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
					d3D11InputElementDesc = DX11IATranslateVertexAttributeDesc( inputAttributeDesc );

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

		DX11IAVertexSourceBindingPtr DX11IACreateVertexSourceBinding(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition )
		{
			const auto activeVertexBuffersNum = pVertexSourceBindingDefinition.GetActiveVertexBuffersNum();
			auto dx11VertexSourceBinding = MakeUniqueDynamicallySizedTemplate<DX11IAVertexSourceBinding>( activeVertexBuffersNum );

			DX11IAUpdateVertexSourceBindingDefinition( pVertexSourceBindingDefinition, *dx11VertexSourceBinding );

			return dx11VertexSourceBinding;
		}

		void DX11IAUpdateVertexSourceBindingDefinition(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition,
				DX11IAVertexSourceBindingBase & pOutDX11VertexSourceBinding )
		{
			uint32 activeVertexBuffersNum = 0;

			for( native_uint vertexStreamIndex = 0; CXU::IAIsDataStreamVertexBufferSlotValid( vertexStreamIndex ); ++vertexStreamIndex )
			{
				const auto bufferBindingBit = CXU::IAMakeVertexBufferBindingFlag( vertexStreamIndex );

				const auto & inputVertexBufferRef = pVertexSourceBindingDefinition.vertexBufferReferences[vertexStreamIndex];
				if( inputVertexBufferRef && pVertexSourceBindingDefinition.IsVertexBufferActive( vertexStreamIndex ) )
				{
					const auto * dx11VertexBuffer = inputVertexBufferRef.sourceBuffer->QueryInterface<DX11GPUBuffer>();

					pOutDX11VertexSourceBinding.SetVertexStreamIndexMapping( vertexStreamIndex, activeVertexBuffersNum );

					pOutDX11VertexSourceBinding.vertexBufferBindings[activeVertexBuffersNum].buffer = dx11VertexBuffer->mD3D11Buffer.Get();
					pOutDX11VertexSourceBinding.vertexBufferBindings[activeVertexBuffersNum].offset = cppx::numeric_cast<UINT>( inputVertexBufferRef.relativeOffset );
					pOutDX11VertexSourceBinding.vertexBufferBindings[activeVertexBuffersNum].stride = cppx::numeric_cast<UINT>( inputVertexBufferRef.refParams.vertexStride );

					pOutDX11VertexSourceBinding.activeStreamsMask.set( bufferBindingBit );
					
					++activeVertexBuffersNum;
				}
				else
				{
					// This stream is inactive (no vertex buffer referenced). Set this special value to indicate this.
					pOutDX11VertexSourceBinding.SetVertexStreamIndexInactive( vertexStreamIndex );
					pOutDX11VertexSourceBinding.activeStreamsMask.unset( bufferBindingBit );
				}
			}

			const auto & inputIndexBufferRef = pVertexSourceBindingDefinition.indexBufferReference;
			if( inputIndexBufferRef && pVertexSourceBindingDefinition.IsIndexBufferActive() )
			{
				const auto * dx11IndexBuffer = pVertexSourceBindingDefinition.indexBufferReference.sourceBuffer->QueryInterface<DX11GPUBuffer>();

				pOutDX11VertexSourceBinding.indexBufferBinding.buffer = dx11IndexBuffer->mD3D11Buffer.Get();
				pOutDX11VertexSourceBinding.indexBufferBinding.offset = cppx::numeric_cast<UINT>( inputIndexBufferRef.relativeOffset );
				pOutDX11VertexSourceBinding.indexBufferBinding.format = ATL::DXTranslateIndexDataFormat( inputIndexBufferRef.refParams.indexFormat );

				pOutDX11VertexSourceBinding.activeStreamsMask.set( eIAVertexSourceBindingFlagIndexBufferBit );
			}
			else
			{
				const auto * dx11IndexBuffer = pVertexSourceBindingDefinition.indexBufferReference.sourceBuffer->QueryInterface<DX11GPUBuffer>();

				pOutDX11VertexSourceBinding.indexBufferBinding.buffer = nullptr;
				pOutDX11VertexSourceBinding.indexBufferBinding.offset = 0;
				pOutDX11VertexSourceBinding.indexBufferBinding.format = DXGI_FORMAT_UNKNOWN;

				pOutDX11VertexSourceBinding.activeStreamsMask.unset( eIAVertexSourceBindingFlagIndexBufferBit );
			}
		}

	}

}
