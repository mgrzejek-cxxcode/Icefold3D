
#include "DX11GpuDevice.h"
#include "DX11GpuDriver.h"
#include "DX11ApiTranslationLayer.h"
#include "DX11CommandList.h"
#include "DX11CommandSystem.h"
#include "Resources/DX11GpuBuffer.h"
#include "Resources/DX11Sampler.h"
#include "Resources/DX11Shader.h"
#include "Resources/DX11Texture.h"
#include "State/DX11pipelineStateObject.h"

namespace Ic3::Graphics::GCI
{

	DX11GpuDevice::DX11GpuDevice(
			DX11GpuDriver & pDriver,
			ComPtr<ID3D11Device1> pD3D11Device1,
			ComPtr<ID3D11Debug> pD3D11Debug )
	: DXGpuDevice( pDriver, ATL::QueryDXGIFactoryForD3D11Device( pD3D11Device1 ) )
	, mD3D11Device1( std::move( pD3D11Device1 ) )
	, mD3D11DebugInterface( std::move( pD3D11Debug ) )
	, _immutableStateFactoryDX11( *this )
	, _immutableStateCache( _immutableStateFactoryDX11 )
	{
		SetImmutableStateCache( _immutableStateCache );
	}

	DX11GpuDevice::~DX11GpuDevice() = default;

	DX11GpuDeviceHandle DX11GpuDevice::Create( DX11GpuDriver & pDriver, const DX11GpuDeviceCreateInfo & pCreateInfo )
	{
		auto driverConfigFlags = pDriver.GetConfigFlags();
		auto deviceCreateFlags = ATL::TranslateDX11GpuDeviceCreateFlags( driverConfigFlags );

		D3D_DRIVER_TYPE deviceDriverType = D3D_DRIVER_TYPE_HARDWARE;
		if( driverConfigFlags.is_set( E_GPU_DRIVER_CONFIG_FLAG_USE_REFERENCE_DRIVER_BIT ) )
		{
			deviceDriverType = D3D_DRIVER_TYPE_REFERENCE;
		}

		auto d3d11Device1 = ATL::CreateD3D11Device( deviceDriverType, deviceCreateFlags );
		ic3DebugAssert( d3d11Device1 );

		auto d3d11DebugInterface = ATL::QueryD3D11DebugInterfaceForD3D11Device( d3d11Device1 );
		ic3DebugAssert( d3d11DebugInterface );

		auto dx11GpuDevice = CreateGfxObject<DX11GpuDevice>( pDriver, d3d11Device1, d3d11DebugInterface );

		return dx11GpuDevice;
	}

	void DX11GpuDevice::WaitForCommandSync( CommandSync & pCommandSync )
	{
		if( pCommandSync )
		{
			auto * dx11CommandSyncData = static_cast<DX11CommandSyncData *>( pCommandSync.syncData );

			UINT64 syncQueryData = FALSE;
			HRESULT hResult = dx11CommandSyncData->d3d11DeviceContext1->GetData( dx11CommandSyncData->d3d11SyncQuery, &syncQueryData, sizeof( UINT64 ), 0 );

			while( ( hResult != S_OK ) && ( syncQueryData != TRUE ) )
			{
				std::this_thread::yield();
				hResult = dx11CommandSyncData->d3d11DeviceContext1->GetData( dx11CommandSyncData->d3d11SyncQuery, &syncQueryData, sizeof( UINT64 ), 0 );
			}

			releaseDX11CommandSyncData( pCommandSync.syncData );
			pCommandSync.syncData = nullptr;
		}
	}

	void DX11GpuDevice::InitializeCommandSystem()
	{
	    ic3DebugAssert( !_commandSystem );
	    _commandSystem = CreateGfxObject<DX11CommandSystem>( *this );
	}

	GpuBufferHandle DX11GpuDevice::_DrvCreateGpuBuffer( const GpuBufferCreateInfo & pCreateInfo )
	{
	    auto dx11Buffer = DX11GpuBuffer::Create( *this, pCreateInfo );
	    ic3DebugAssert( dx11Buffer );
	    return dx11Buffer;
	}

	SamplerHandle DX11GpuDevice::_DrvCreateSampler( const SamplerCreateInfo & pCreateInfo )
	{
	    auto dx11Sampler = DX11Sampler::Create( *this, pCreateInfo );
	    ic3DebugAssert( dx11Sampler );
	    return dx11Sampler;
	}

	ShaderHandle DX11GpuDevice::_DrvCreateShader( const ShaderCreateInfo & pCreateInfo )
	{
	    auto dx11Shader = DX11Shader::Create( *this, pCreateInfo );
	    ic3DebugAssert( dx11Shader );
	    return dx11Shader;
	}

	TextureHandle DX11GpuDevice::_DrvCreateTexture( const TextureCreateInfo & pCreateInfo )
	{
	    auto dx11Texture = DX11Texture::CreateDefault( *this, pCreateInfo );
	    ic3DebugAssert( dx11Texture );
	    return dx11Texture;
	}

	RenderTargetTextureHandle DX11GpuDevice::_DrvCreateRenderTargetTexture(
			const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		auto dx11RTTextureView = DX11Texture::CreateRenderTargetTextureView( *this, pCreateInfo );
		ic3DebugAssert( dx11RTTextureView );
		return dx11RTTextureView;
	}

	GraphicsPipelineStateObjectHandle DX11GpuDevice::_DrvCreateGraphicsPipelineStateObject(
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		auto dx11GraphicsPSO = DX11GraphicsPipelineStateObject::Create( *this, pCreateInfo );
		ic3DebugAssert( dx11GraphicsPSO );
		return dx11GraphicsPSO;
	}

} // namespace Ic3::Graphics::GCI
