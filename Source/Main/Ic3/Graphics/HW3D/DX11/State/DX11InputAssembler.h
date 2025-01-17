
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_INPUT_ASSEMBLER_H__
#define __IC3_GRAPHICS_HW3D_DX11_INPUT_ASSEMBLER_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/Graphics/GCI/State/InputAssemblerImmutableStates.h>

namespace Ic3::Graphics::GCI
{

	using DX11IAVertexAttributeInfoArray = std::array<D3D11_INPUT_ELEMENT_DESC, GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM>;

	struct DX11IAInputLayoutDefinition
	{
		uint32 activeAttributesNum;
		DX11IAVertexAttributeInfoArray attributeArray;
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

		IAVertexBufferRangeList activeRanges;

		BindingData bindingData;

		explicit operator bool() const
		{
			return !activeRanges.empty();
		}
	};

	/// @brief
	struct DX11IAVertexStreamDefinition
	{
		cppx::bitmask<EIAVertexStreamBindingFlags> activeBindingsMask;

		DX11IAVertexBuffersBindings vertexBufferBindings;

		DX11IAIndexBufferBinding indexBufferBinding;

	};

	///
	class DX11IAInputLayoutImmutableState : public IAInputLayoutImmutableState
	{
	public:
		ComPtr<ID3D11InputLayout> const mD3D11InputLayout;
		D3D11_PRIMITIVE_TOPOLOGY const mD3D11PrimitiveTopology;

	public:
		DX11IAInputLayoutImmutableState(
				DX11GpuDevice & pGpuDevice,
				const IAInputLayoutStateCommonProperties & pCommonProperties,
				ComPtr<ID3D11InputLayout> pD3D11InputLayout,
				D3D11_PRIMITIVE_TOPOLOGY pD3D11PrimitiveTopology );

		virtual ~DX11IAInputLayoutImmutableState();

		static GpaHandle<DX11IAInputLayoutImmutableState> CreateInstance(
				DX11GpuDevice & pGpuDevice,
				const IAInputLayoutDefinition & pInputLayoutDefinition,
				const ShaderBinary & pVertexShaderBinary );
	};

	///
	class DX11IAVertexStreamImmutableState : public IAVertexStreamImmutableState
	{
	public:
		DX11IAVertexStreamDefinition const mDX11VertexStreamDefinition;

	public:
		DX11IAVertexStreamImmutableState(
				DX11GpuDevice & pGpuDevice,
				const IAVertexStreamStateCommonProperties & pCommonProperties,
				const DX11IAVertexStreamDefinition & pDX11VertexStreamDefinition );

		virtual ~DX11IAVertexStreamImmutableState();

		static GpaHandle<DX11IAVertexStreamImmutableState> CreateInstance(
				DX11GpuDevice & pGpuDevice,
				const IAVertexStreamDefinition & pVertexStreamDefinition );
	};
	
	
	namespace SMU
	{

		CPPX_ATTR_NO_DISCARD D3D11_INPUT_ELEMENT_DESC TranslateDX11IAVertexAttributeInfo(
				const IAVertexAttributeInfo & pAttributeInfo );

		CPPX_ATTR_NO_DISCARD DX11IAInputLayoutDefinition TranslateDX11IAInputLayoutDefinition(
				const IAInputLayoutDefinition & pDefinition );
		
		CPPX_ATTR_NO_DISCARD DX11IAVertexStreamDefinition TranslateDX11IAVertexStreamDefinition(
				const IAVertexStreamDefinition & pDefinition );
		
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_INPUT_ASSEMBLER_H__
