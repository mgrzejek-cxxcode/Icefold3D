
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_INPUT_ASSEMBLER_H__
#define __IC3_GRAPHICS_HW3D_DX11_INPUT_ASSEMBLER_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIA.h>

namespace Ic3::Graphics::GCI
{

	using DX11IAVertexAttributeDescArray = std::array<D3D11_INPUT_ELEMENT_DESC, GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM>;

	struct DX11IAVertexAttributeLayoutDefinition
	{
		uint32 activeAttributesNum;
		DX11IAVertexAttributeDescArray attributeArray;
		D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
	};

	struct DX11IAIndexBufferBinding
	{
		ID3D11Buffer * buffer;
		uint32 offset;
		DXGI_FORMAT format;

		explicit operator bool() const noexcept
		{
			return buffer != nullptr;
		}
	};

	struct DX11IAVertexBuffersBindings
	{
		struct BindingData
		{
			ID3D11Buffer * bufferArray[GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM];

			UINT offsetArray[GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM];

			UINT strideArray[GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM];
		};

		IADataStreamArrayRangeList activeRanges;

		BindingData bindingData;

		explicit operator bool() const
		{
			return !activeRanges.empty();
		}
	};

	/// @brief
	struct DX11IASourceBindingDefinition
	{
		cppx::bitmask<EIADataStreamBindingFlags> activeStreamsMask;

		DX11IAVertexBuffersBindings vertexBufferBindings;

		DX11IAIndexBufferBinding indexBufferBinding;

	};

	///
	class DX11VertexAttributeLayoutDescriptor : public VertexAttributeLayoutDescriptor
	{
	public:
		ComPtr<ID3D11InputLayout> const mD3D11InputLayout;
		D3D11_PRIMITIVE_TOPOLOGY const mD3D11PrimitiveTopology;

	public:
		DX11VertexAttributeLayoutDescriptor(
				DX11GPUDevice & pGPUDevice,
				const IAVertexAttributeLayoutCommonProperties & pCommonProperties,
				ComPtr<ID3D11InputLayout> pD3D11InputLayout,
				D3D11_PRIMITIVE_TOPOLOGY pD3D11PrimitiveTopology );

		virtual ~DX11VertexAttributeLayoutDescriptor();

		static TGfxHandle<DX11VertexAttributeLayoutDescriptor> CreateInstance(
				DX11GPUDevice & pGPUDevice,
				const IAVertexAttributeLayoutDefinition & pAttributeLayoutDefinition,
				const ShaderBinary & pVertexShaderBinary );
	};

	///
	class DX11VertexSourceBindingDescriptor : public VertexSourceBindingDescriptor
	{
	public:
		DX11IASourceBindingDefinition const mDX11SourceBindingDefinition;

	public:
		DX11VertexSourceBindingDescriptor(
				DX11GPUDevice & pGPUDevice,
				const VertexSourceBindingDescriptorCommonProperties & pCommonProperties,
				const DX11IASourceBindingDefinition & pDX11SourceBindingDefinition );

		virtual ~DX11VertexSourceBindingDescriptor();

		static TGfxHandle<DX11VertexSourceBindingDescriptor> CreateInstance(
				DX11GPUDevice & pGPUDevice,
				const IADataStreamArrayConfiguration & pSourceBindingDefinition );
	};
	
	
	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD D3D11_INPUT_ELEMENT_DESC TranslateDX11IAVertexAttributeInfo(
				const IAVertexAttributeInfo & pAttributeInfo );

		CPPX_ATTR_NO_DISCARD DX11IAVertexAttributeLayoutDefinition TranslateDX11IAVertexAttributeLayoutDefinition(
				const IAVertexAttributeLayoutDefinition & pDefinition );
		
		CPPX_ATTR_NO_DISCARD DX11IASourceBindingDefinition TranslateDX11IASourceBindingDefinition(
				const IADataStreamArrayConfiguration & pDefinition );
		
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_INPUT_ASSEMBLER_H__
