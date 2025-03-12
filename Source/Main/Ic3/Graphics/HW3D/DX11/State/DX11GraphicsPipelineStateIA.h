
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_IA_H__
#define __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_IA_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIA.h>

namespace Ic3::Graphics::GCI
{

	using DX11IAVertexAttributeDescArray = std::array<D3D11_INPUT_ELEMENT_DESC, GCM::kIAMaxVertexAttributesNum>;

	struct DX11IAVertexAttributeLayoutDefinition : public IAVertexAttributeLayoutCommonConfig
	{
		DX11IAVertexAttributeDescArray d3d11AttributeArray;
		D3D11_PRIMITIVE_TOPOLOGY d3d11PrimitiveTopology;
	};

	struct DX11IAIndexBufferBinding
	{
		ID3D11Buffer * buffer;
		UINT offset;
		DXGI_FORMAT format;

		explicit operator bool() const noexcept
		{
			return buffer != nullptr;
		}
	};

	struct DX11IAVertexBufferArrayBindings
	{
		struct BindingData
		{
			ID3D11Buffer * bufferArray[GCM::kIAMaxDataStreamVertexBuffersNum];
			UINT offsetArray[GCM::kIAMaxDataStreamVertexBuffersNum];
			UINT strideArray[GCM::kIAMaxDataStreamVertexBuffersNum];
		};

		VertexSourceBindingRageList activeRanges;

		BindingData bindingData;

		explicit operator bool() const
		{
			return !activeRanges.empty();
		}
	};

	/// @brief
	struct DX11IAVertexSourceBinding : public IAVertexSourceBindingCommonConfig
	{
		DX11IAVertexBufferArrayBindings vertexBufferBindings;

		DX11IAIndexBufferBinding indexBufferBinding;
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
				const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig,
				ComPtr<ID3D11InputLayout> pD3D11InputLayout,
				D3D11_PRIMITIVE_TOPOLOGY pD3D11PrimitiveTopology );

		virtual ~DX11VertexAttributeLayoutDescriptor();

		static TGfxHandle<DX11VertexAttributeLayoutDescriptor> CreateInstance(
				DX11GPUDevice & pGPUDevice,
				const IAVertexAttributeLayoutDefinition & pAttributeLayoutDefinition,
				const ShaderBinary & pVertexShaderBinary );

	private:
		IAVertexAttributeLayoutCommonConfig _commonAttributeLayoutConfig;
	};

	///
	class IC3_GX_DX11_CLASS DX11VertexSourceBindingDescriptor : public HW3DPipelineStateDescriptor<VertexSourceBindingDescriptor>
	{
	public:
		DX11IAVertexSourceBinding const mDX11VertexSourceBinding;

	public:
		DX11VertexSourceBindingDescriptor(
				DX11GPUDevice & pGPUDevice,
				const DX11IAVertexSourceBinding & pDX11VertexSourceBinding );

		virtual ~DX11VertexSourceBindingDescriptor();

		static TGfxHandle<DX11VertexSourceBindingDescriptor> CreateInstance(
				DX11GPUDevice & pGPUDevice,
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition );
	};
	
	
	namespace GCU
	{

		// VertexAttributeLayout

		CPPX_ATTR_NO_DISCARD D3D11_INPUT_ELEMENT_DESC DX11IATranslateIAVertexAttributeDesc(
				const IAVertexAttributeDesc & pVertexAttributeDesc );

		CPPX_ATTR_NO_DISCARD DX11IAVertexAttributeLayoutDefinition DX11IATranslateVertexAttributeLayoutDefinition(
				const IAVertexAttributeLayoutDefinition & pVertexAttributeLayoutDefinition );

		// VertexSourceBinding

		CPPX_ATTR_NO_DISCARD DX11IAVertexSourceBinding DX11IATranslateVertexSourceBindingDefinition(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition );

		CPPX_ATTR_NO_DISCARD bool DX11IAUpdateVertexSourceBindingDefinition(
				const IAVertexSourceBindingDefinition & pVertexSourceBindingDefinition,
				DX11IAVertexSourceBinding & pOutDX11VertexSourceBinding );
		
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_IA_H__
