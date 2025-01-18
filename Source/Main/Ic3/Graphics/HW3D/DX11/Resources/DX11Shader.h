
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_SHADER_INTERFACE_H__
#define __IC3_GRAPHICS_HW3D_DX11_SHADER_INTERFACE_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/Shader.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( DX11Shader );

	class  DX11Shader : public Shader
	{
	public:
		union
		{
			ID3D11VertexShader * const mD3D11VertexShader;
			ID3D11HullShader * const mD3D11HullShader;
			ID3D11DomainShader * const mD3D11DomainShader;
			ID3D11GeometryShader * const mD3D11GeometryShader;
			ID3D11PixelShader * const mD3D11PixelShader;
			ID3D11ComputeShader * const mD3D11ComputeShader;
		};
		ComPtr<ID3D11DeviceChild> const mD3D11ShaderReference;

		DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11VertexShader> pD3D11VertexShader, std::unique_ptr<ShaderBinary> pShaderBinary );
		DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11HullShader> pD3D11HullShader, std::unique_ptr<ShaderBinary> pShaderBinary );
		DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11DomainShader> pD3D11DomainShader, std::unique_ptr<ShaderBinary> pShaderBinary );
		DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11GeometryShader> pD3D11GeometryShader, std::unique_ptr<ShaderBinary> pShaderBinary );
		DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11PixelShader> pD3D11PixelShader, std::unique_ptr<ShaderBinary> pShaderBinary );
		DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11ComputeShader> pD3D11ComputeShader, std::unique_ptr<ShaderBinary> pShaderBinary );
		virtual ~DX11Shader();

		static DX11ShaderHandle Create( DX11GPUDevice & pDX11GPUDevice, const ShaderCreateInfo & pCreateInfo );
	};


	namespace RCU
	{

		CPPX_ATTR_NO_DISCARD std::unique_ptr<ShaderBinary> CompileShader(
				const void * pCode,
				size_t pCodeLength,
				const char * pEntryPoint,
				DXShaderTarget pShaderTarget,
				cppx::bitmask<UINT> pCompileFlags );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_SHADER_INTERFACE_H__
