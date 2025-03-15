
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_IA_H__
#define __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_IA_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIA.h>
#include <Ic3/Graphics/GCI/Utilities/DSTVertexSourceBinding.h>

namespace Ic3::Graphics::GCI
{

	using DX11IAVertexAttributeDescArray = std::array<D3D11_INPUT_ELEMENT_DESC, GCM::kIAMaxVertexAttributesNum>;

	struct DX11IAVertexAttributeLayoutDefinition : public IAVertexAttributeLayoutMetaData
	{
		DX11IAVertexAttributeDescArray d3d11AttributeArray;
		D3D11_PRIMITIVE_TOPOLOGY d3d11PrimitiveTopology;
	};

	struct DX11IBufferBinding
	{
		ID3D11Buffer * buffer;
		UINT offset;

		explicit operator bool() const noexcept
		{
			return buffer != nullptr;
		}
	};

	struct DX11IAVertexBufferBinding : public DX11IBufferBinding
	{
		UINT index;
		UINT stride;
	};

	struct DX11IAIndexBufferBinding : public DX11IBufferBinding
	{
		DXGI_FORMAT format;
	};

	using DX11IAVertexSourceBindingBase = DSTVertexSourceBindingBase<DX11IAVertexBufferBinding, DX11IAIndexBufferBinding>;

	template <native_uint tpActiveVertexBuffersNum>
	using DX11IAVertexSourceBinding = DSTVertexSourceBinding<DX11IAVertexSourceBindingBase, tpActiveVertexBuffersNum>;

	using DX11IAVertexSourceBindingPtr = std::unique_ptr<DX11IAVertexSourceBindingBase>;

	struct DX11UnwrappedVertexBufferBindings
	{
		std::array<ID3D11Buffer *, GCM::kIAMaxDataStreamVertexBuffersNum> vertexBufferArray;
		std::array<UINT, GCM::kIAMaxDataStreamVertexBuffersNum> vertexBufferOffsetArray;
		std::array<UINT, GCM::kIAMaxDataStreamVertexBuffersNum> vertexBufferStrideArray;

		void SetBindings( const DX11IAVertexSourceBindingBase & pDX11IAVertexSourceBinding )
		{
			for( native_uint vertexBufferIndex = 0; vertexBufferIndex < GCM::kIAMaxDataStreamVertexBuffersNum; ++vertexBufferIndex )
			{
				if( const auto * dx11VertexBufferBinding = pDX11IAVertexSourceBinding.GetBindingForVertexStream( vertexBufferIndex ) )
				{
					vertexBufferArray[vertexBufferIndex] = dx11VertexBufferBinding->buffer;
					vertexBufferOffsetArray[vertexBufferIndex] = dx11VertexBufferBinding->offset;
					vertexBufferStrideArray[vertexBufferIndex] = dx11VertexBufferBinding->stride;
				}
				else
				{
					vertexBufferArray[vertexBufferIndex] = nullptr;
					vertexBufferOffsetArray[vertexBufferIndex] = 0;
					vertexBufferStrideArray[vertexBufferIndex] = 0;
				}
			}
		}
	};

	///
	class IC3_GX_DX11_CLASS DX11VertexAttributeLayoutDescriptor : public HW3DPipelineStateDescriptor<VertexAttributeLayoutDescriptor>
	{
	public:
		ComPtr<ID3D11InputLayout> const mD3D11InputLayout;

		D3D11_PRIMITIVE_TOPOLOGY const mD3D11PrimitiveTopology;

	public:
		DX11VertexAttributeLayoutDescriptor(
				DX11GPUDevice & pGPUDevice,
				const IAVertexAttributeLayoutMetaData & pAttributeLayoutMetaData,
				ComPtr<ID3D11InputLayout> pD3D11InputLayout,
				D3D11_PRIMITIVE_TOPOLOGY pD3D11PrimitiveTopology );

		virtual ~DX11VertexAttributeLayoutDescriptor();

		static TGfxHandle<DX11VertexAttributeLayoutDescriptor> CreateInstance(
				DX11GPUDevice & pGPUDevice,
				const IAVertexAttributeLayoutDefinition & pAttributeLayoutDefinition,
				const ShaderBinary & pVertexShaderBinary );

	private:
		IAVertexAttributeLayoutMetaData _attributeLayoutMetaData;
	};

	///
	class IC3_GX_DX11_CLASS DX11VertexSourceBindingDescriptor : public HW3DPipelineStateDescriptor<VertexSourceBindingDescriptor>
	{
	public:
		DX11IAVertexSourceBindingPtr const mDX11VertexSourceBinding;

	public:
		DX11VertexSourceBindingDescriptor(
				DX11GPUDevice & pGPUDevice,
				DX11IAVertexSourceBindingPtr pDX11VertexSourceBinding );

		virtual ~DX11VertexSourceBindingDescriptor();

		static TGfxHandle<DX11VertexSourceBindingDescriptor> CreateInstance(
				DX11GPUDevice & pGPUDevice,
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition );
	};
	
	
	namespace Utilities
	{

		// VertexAttributeLayout

		CPPX_ATTR_NO_DISCARD D3D11_INPUT_ELEMENT_DESC DX11IATranslateVertexAttributeDesc(
				const IAVertexAttributeDesc & pVertexAttributeDesc );

		CPPX_ATTR_NO_DISCARD DX11IAVertexAttributeLayoutDefinition DX11IATranslateVertexAttributeLayoutDefinition(
				const IAVertexAttributeLayoutDefinition & pVertexAttributeLayoutDefinition );

		// VertexSourceBinding

		CPPX_ATTR_NO_DISCARD DX11IAVertexSourceBindingPtr DX11IACreateVertexSourceBinding(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition );

		CPPX_ATTR_NO_DISCARD void DX11IAUpdateVertexSourceBindingDefinition(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition,
				DX11IAVertexSourceBindingBase & pOutDX11VertexSourceBinding );
		
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_IA_H__
