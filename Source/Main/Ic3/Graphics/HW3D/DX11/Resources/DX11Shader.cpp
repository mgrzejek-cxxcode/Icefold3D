
#include "DX11Shader.h"
#include <Ic3/Graphics/HW3D/DX11/DX11APITranslationLayer.h>
#include <Ic3/Graphics/HW3D/DX11/DX11GPUDevice.h>
#include <cppx/memory.h>

namespace Ic3::Graphics::GCI
{

	DX11Shader::DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11VertexShader> pD3D11VertexShader, std::unique_ptr<ShaderBinary> pShaderBinary )
	: Shader( pGPUDevice, EShaderType::GSVertex, std::move( pShaderBinary ) )
	, mD3D11VertexShader( pD3D11VertexShader.Get() )
	, mD3D11ShaderReference( pD3D11VertexShader )
	{}

	DX11Shader::DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11HullShader> pD3D11HullShader, std::unique_ptr<ShaderBinary> pShaderBinary )
	: Shader( pGPUDevice, EShaderType::GSHull, std::move( pShaderBinary ) )
	, mD3D11HullShader( pD3D11HullShader.Get() )
	, mD3D11ShaderReference( pD3D11HullShader )
	{}

	DX11Shader::DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11DomainShader> pD3D11DomainShader, std::unique_ptr<ShaderBinary> pShaderBinary )
	: Shader( pGPUDevice, EShaderType::GSDomain, std::move( pShaderBinary ) )
	, mD3D11DomainShader( pD3D11DomainShader.Get() )
	, mD3D11ShaderReference( pD3D11DomainShader )
	{}

	DX11Shader::DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11GeometryShader> pD3D11GeometryShader, std::unique_ptr<ShaderBinary> pShaderBinary )
	: Shader( pGPUDevice, EShaderType::GSGeometry, std::move( pShaderBinary ) )
	, mD3D11GeometryShader( pD3D11GeometryShader.Get() )
	, mD3D11ShaderReference( pD3D11GeometryShader )
	{}

	DX11Shader::DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11PixelShader> pD3D11PixelShader, std::unique_ptr<ShaderBinary> pShaderBinary )
	: Shader( pGPUDevice, EShaderType::GSPixel, std::move( pShaderBinary ) )
	, mD3D11PixelShader( pD3D11PixelShader.Get() )
	, mD3D11ShaderReference( pD3D11PixelShader )
	{}

	DX11Shader::DX11Shader( DX11GPUDevice & pGPUDevice, ComPtr<ID3D11ComputeShader> pD3D11ComputeShader, std::unique_ptr<ShaderBinary> pShaderBinary )
	: Shader( pGPUDevice, EShaderType::CSCompute, std::move( pShaderBinary ) )
	, mD3D11ComputeShader( pD3D11ComputeShader.Get() )
	, mD3D11ShaderReference( pD3D11ComputeShader )
	{}

	DX11Shader::~DX11Shader() = default;

	DX11ShaderHandle DX11Shader::Create( DX11GPUDevice & pDX11GPUDevice, const ShaderCreateInfo & pCreateInfo )
	{
		const char * entryPoint = pCreateInfo.entryPointName ? pCreateInfo.entryPointName : "main";

		auto compileFlags = ATL::TranslateDXShaderCompileFlags( pCreateInfo.createFlags, pDX11GPUDevice.IsDebugDevice() );
		auto shaderSourceView = pCreateInfo.shaderSourceView;

		if( !shaderSourceView )
		{
			shaderSourceView = bind_memory_view( pCreateInfo.shaderSource.data(), pCreateInfo.shaderSource.size() );
			if( !shaderSourceView )
			{
				return nullptr;
			}
		}

		DX11ShaderHandle dx11Shader;

		if( pCreateInfo.shaderType == EShaderType::GSVertex )
		{
			if( auto shaderBinary = RCU::CompileShader( shaderSourceView.data(), shaderSourceView.size(), entryPoint, DXShaderTarget::SM_5_0_VS, compileFlags ) )
			{
				ComPtr<ID3D11VertexShader> d3d11VertexShader;
				auto hResult = pDX11GPUDevice.mD3D11Device1->CreateVertexShader(
						shaderBinary->dataBuffer,
						shaderBinary->dataSizeInBytes,
						nullptr,
						d3d11VertexShader.GetAddressOf() );

				if( SUCCEEDED( hResult ) )
				{
					dx11Shader = CreateDynamicObject<DX11Shader>( pDX11GPUDevice, d3d11VertexShader, std::move( shaderBinary ) );
				}
			}
		}
		else if( pCreateInfo.shaderType == EShaderType::GSHull )
		{
			if( auto shaderBinary = RCU::CompileShader( shaderSourceView.data(), shaderSourceView.size(), entryPoint, DXShaderTarget::SM_5_0_HS, compileFlags ) )
			{
				ComPtr<ID3D11HullShader> d3d11HullShader;
				auto hResult = pDX11GPUDevice.mD3D11Device1->CreateHullShader(
						shaderBinary->dataBuffer,
						shaderBinary->dataSizeInBytes,
						nullptr,
						d3d11HullShader.GetAddressOf() );

				if( SUCCEEDED( hResult ) )
				{
					dx11Shader = CreateDynamicObject<DX11Shader>( pDX11GPUDevice, d3d11HullShader, std::move( shaderBinary ) );
				}
			}
		}
		else if( pCreateInfo.shaderType == EShaderType::GSDomain )
		{
			if( auto shaderBinary = RCU::CompileShader( shaderSourceView.data(), shaderSourceView.size(), entryPoint, DXShaderTarget::SM_5_0_DS, compileFlags ) )
			{
				ComPtr<ID3D11DomainShader> d3d11DomainShader;
				auto hResult = pDX11GPUDevice.mD3D11Device1->CreateDomainShader(
						shaderBinary->dataBuffer,
						shaderBinary->dataSizeInBytes,
						nullptr,
						d3d11DomainShader.GetAddressOf() );

				if( SUCCEEDED( hResult ) )
				{
					dx11Shader = CreateDynamicObject<DX11Shader>( pDX11GPUDevice, d3d11DomainShader, std::move( shaderBinary ) );
				}
			}
		}
		else if( pCreateInfo.shaderType == EShaderType::GSGeometry )
		{
			if( auto shaderBinary = RCU::CompileShader( shaderSourceView.data(), shaderSourceView.size(), entryPoint, DXShaderTarget::SM_5_0_GS, compileFlags ) )
			{
				ComPtr<ID3D11GeometryShader> d3d11GeometryShader;
				auto hResult = pDX11GPUDevice.mD3D11Device1->CreateGeometryShader(
						shaderBinary->dataBuffer,
						shaderBinary->dataSizeInBytes,
						nullptr,
						d3d11GeometryShader.GetAddressOf() );

				if( SUCCEEDED( hResult ) )
				{
					dx11Shader = CreateDynamicObject<DX11Shader>( pDX11GPUDevice, d3d11GeometryShader, std::move( shaderBinary ) );
				}
			}
		}
		else if( pCreateInfo.shaderType == EShaderType::GSPixel )
		{
			if( auto shaderBinary = RCU::CompileShader( shaderSourceView.data(), shaderSourceView.size(), entryPoint, DXShaderTarget::SM_5_0_PS, compileFlags ) )
			{
				ComPtr<ID3D11PixelShader> d3d11PixelShader;
				auto hResult = pDX11GPUDevice.mD3D11Device1->CreatePixelShader(
						shaderBinary->dataBuffer,
						shaderBinary->dataSizeInBytes,
						nullptr,
						d3d11PixelShader.GetAddressOf() );

				if( SUCCEEDED( hResult ) )
				{
					dx11Shader = CreateDynamicObject<DX11Shader>( pDX11GPUDevice, d3d11PixelShader, std::move( shaderBinary ) );
				}
			}
		}
		else if( pCreateInfo.shaderType == EShaderType::CSCompute )
		{
			if( auto shaderBinary = RCU::CompileShader( shaderSourceView.data(), shaderSourceView.size(), entryPoint, DXShaderTarget::SM_5_0_CS, compileFlags ) )
			{
				ComPtr<ID3D11ComputeShader> d3d11ComputeShader;
				auto hResult = pDX11GPUDevice.mD3D11Device1->CreateComputeShader(
						shaderBinary->dataBuffer,
						shaderBinary->dataSizeInBytes,
						nullptr,
						d3d11ComputeShader.GetAddressOf() );

				if( SUCCEEDED( hResult ) )
				{
					dx11Shader = CreateDynamicObject<DX11Shader>( pDX11GPUDevice, d3d11ComputeShader, std::move( shaderBinary ) );
				}
			}
		}

		return dx11Shader;
	}


	namespace RCU
	{

		std::unique_ptr<ShaderBinary> CompileShader(
				const void * pCode,
				size_t pCodeLength,
				const char * pEntryPoint,
				DXShaderTarget pShaderTarget,
				cppx::bitmask<UINT> pCompileFlags )
		{
			const char * shaderTargetStr = ATL::GetDXShaderTargetStr( pShaderTarget );

			ComPtr<ID3DBlob> compiledBinaryBuffer;
			ComPtr<ID3DBlob> errorMessagesBuffer;

			auto hResult = D3DCompile(
					pCode,
					pCodeLength,
					nullptr,
					nullptr,
					nullptr,
					pEntryPoint,
					shaderTargetStr,
					pCompileFlags,
					0,
					compiledBinaryBuffer.GetAddressOf(),
					errorMessagesBuffer.GetAddressOf() );

			if( FAILED( hResult ) )
			{
				if( errorMessagesBuffer )
				{
					auto * errorStr = (const char *)errorMessagesBuffer->GetBufferPointer();
					std::string err{ errorStr };;
					Ic3DebugOutput( errorStr );
					Ic3DebugInterrupt();
				}

				return nullptr;
			}

			const auto binarySize = compiledBinaryBuffer->GetBufferSize();
			if( binarySize == 0 )
			{
				return nullptr;
			}

			auto shaderBinary = ShaderBinary::Create( binarySize );
			shaderBinary->driverSpecificID = 0;
			shaderBinary->driverSpecificType = static_cast<uint64>( pShaderTarget );
			shaderBinary->dataSizeInBytes = cppx::numeric_cast<uint32>( binarySize );
			mem_copy( shaderBinary->dataBuffer, shaderBinary->dataSizeInBytes, compiledBinaryBuffer->GetBufferPointer(), binarySize );

			return shaderBinary;
		}

	}

} // namespace Ic3::Graphics::GCI
