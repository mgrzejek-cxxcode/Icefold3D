
#include "DX11ApiTranslationLayer.h"
#include <Ic3/Graphics/HW3D/DX11/DX11GpuDevice.h>
#include <Ic3/Graphics/GCI/Resources/GpuBufferCommon.h>
#include <Ic3/Graphics/GCI/State/SamplerCommon.h>
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>
#include <Ic3/System/WindowNative.h>
#include <cppx/stdHelperAlgo.h>

namespace Ic3::Graphics::GCI
{

	template <typename TEnum>
	inline const TEnum D3D11_INVALID = static_cast< TEnum >( 0xFFFFFFFF );
	
	template <typename TEnum>
	inline const TEnum D3D11_UNDEFINED = static_cast< TEnum >( 0 );

	ComPtr<ID3D11Device1> ATL::CreateD3D11Device( D3D_DRIVER_TYPE pDeviceType, cppx::bitmask<UINT> pCreateFlags )
	{
		cppx::bitmask<UINT> CreateDeviceFlags = 0;
		if( pCreateFlags.is_set( D3D11_CREATE_DEVICE_DEBUG ) )
		{
			// This one is for creating a debug device, which is able to provide additional diagnostic info.
			CreateDeviceFlags.set( D3D11_CREATE_DEVICE_DEBUG );
			// This one is for shader debugging capability. Usable when using external diagnostic tools.
			CreateDeviceFlags.set( D3D11_CREATE_DEVICE_DEBUGGABLE );
		}
		if( pCreateFlags.is_set( D3D11_CREATE_DEVICE_SINGLETHREADED ) )
		{
			CreateDeviceFlags.set( D3D11_CREATE_DEVICE_SINGLETHREADED );
		}

		const D3D_FEATURE_LEVEL featureLevelArray[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		const D3D_FEATURE_LEVEL * requestedFeatureLevelPtr = &( featureLevelArray[0] );
		UINT requestedFeatureLevelCount = static_cast<UINT>( static_array_size( featureLevelArray ) );

		ComPtr<ID3D11Device> d3d11Device;
		ComPtr<ID3D11DeviceContext> d3d11DeviceContext;
		D3D_FEATURE_LEVEL targetFeatureLevel;

		auto hResult = ::D3D11CreateDevice( nullptr,
		                                    pDeviceType,
		                                    nullptr,
		                                    CreateDeviceFlags,
		                                    requestedFeatureLevelPtr,
		                                    requestedFeatureLevelCount,
		                                    D3D11_SDK_VERSION,
		                                    d3d11Device.GetAddressOf(),
		                                    &targetFeatureLevel,
		                                    d3d11DeviceContext.GetAddressOf() );

		while( hResult != S_OK )
		{
			// E_INVALIDARG is immediately returned if we request 11.1 runtime on a system which does not support it.
			// If that happens, just stick to 11.0. 11.1 is treated as an extension and its absence is not a deal-breaker.
			if( ( hResult == E_INVALIDARG ) && ( requestedFeatureLevelPtr[0] == D3D_FEATURE_LEVEL_11_1 ) )
			{
				// Advance the feature level pointer to a next value (11_0), decrease number of feature levels by 1.
				++requestedFeatureLevelPtr;
				--requestedFeatureLevelCount;
			}
			// D3D11_CREATE_DEVICE_DEBUGGABLE is one of the reasons requires D3D11SDKLayers.dll to be installed on the
			// system *AND* WDDM 1.2 driver + Direct3D 11.1 runtime (>= Windows 8). If this error occurs, drop the flag.
			else if( ( hResult == DXGI_ERROR_UNSUPPORTED ) && CreateDeviceFlags.is_set( D3D11_CREATE_DEVICE_DEBUGGABLE ) )
			{
				CreateDeviceFlags.unset( D3D11_CREATE_DEVICE_DEBUGGABLE );
			}
			// DXGI_ERROR_SDK_COMPONENT_MISSING is returned if D3D11_CREATE_DEVICE_DEBUG is specified and the incorrect
			// version of the debug layer is installed on the system. In such case, we just drop the flag and proceed.
			else if( ( hResult == DXGI_ERROR_SDK_COMPONENT_MISSING ) && CreateDeviceFlags.is_set( D3D11_CREATE_DEVICE_DEBUG ) )
			{
				CreateDeviceFlags.unset( D3D11_CREATE_DEVICE_DEBUG );
			}
			// Doesn't seem to be a known case. Treat as a failure and exit the loop.
			else
			{
				break;
			}

			// Try again.
			hResult = ::D3D11CreateDevice( nullptr,
			                               pDeviceType,
			                               nullptr,
			                               CreateDeviceFlags,
			                               requestedFeatureLevelPtr,
			                               requestedFeatureLevelCount,
			                               D3D11_SDK_VERSION,
			                               d3d11Device.GetAddressOf(),
			                               &targetFeatureLevel,
			                               d3d11DeviceContext.GetAddressOf() );
		}

		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		ComPtr<ID3D11Device1> d3d11Device1 = nullptr;
		hResult = d3d11Device->QueryInterface( IID_PPV_ARGS( &d3d11Device1 ) );
		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		return d3d11Device1;
	}

	ComPtr<ID3D11Debug> ATL::QueryD3D11DebugInterfaceForD3D11Device( const ComPtr<ID3D11Device1> & pD3D11Device1 )
	{
		ComPtr<ID3D11Debug> d3d11DebugInterface;
		auto hResult = pD3D11Device1->QueryInterface( IID_PPV_ARGS( &d3d11DebugInterface ) );
		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		return d3d11DebugInterface;
	}

	ComPtr<IDXGIFactory2> ATL::QueryDXGIFactoryForD3D11Device( const ComPtr<ID3D11Device1> & pD3D11Device1 )
	{
		ComPtr<IDXGIDevice2> dxgiDevice2;
		auto hResult = pD3D11Device1->QueryInterface( IID_PPV_ARGS( &dxgiDevice2 ) );
		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		ComPtr<IDXGIAdapter> dxgiAdapter;
		hResult = dxgiDevice2->GetParent( IID_PPV_ARGS( &dxgiAdapter ) );
		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		ComPtr<IDXGIFactory2> dxgiFactory2;
		hResult = dxgiAdapter->GetParent( IID_PPV_ARGS( &dxgiFactory2 ) );
		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		return dxgiFactory2;
	}

	ComPtr<IDXGISwapChain1> ATL::CreateD3D11SwapChainForSystemWindow( DX11GpuDevice & pDX11GpuDevice, void * pSysWindow )
	{
	    auto * sysWindowPtr = static_cast<System::Window *>( pSysWindow )->QueryInterface<System::Win32Window>();
		auto presentationLayerSize = sysWindowPtr->GetClientAreaSize();

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
		ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC1 ) );
		swapChainDesc.Width = presentationLayerSize.x;
		swapChainDesc.Height = presentationLayerSize.y;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		auto & dxgiFactory = pDX11GpuDevice.mDXGIFactory2;
		ComPtr<IDXGISwapChain1> dxgiSwapChain1;
		auto hResult = dxgiFactory->CreateSwapChainForHwnd( pDX11GpuDevice.mD3D11Device1.Get(),
		                                                    sysWindowPtr->mNativeData.mHWND,
		                                                    &swapChainDesc,
		                                                    nullptr,
		                                                    nullptr,
		                                                    dxgiSwapChain1.GetAddressOf() );
		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		return dxgiSwapChain1;
	}

	UINT ATL::TranslateDX11GpuDeviceCreateFlags( cppx::bitmask<EGpuDriverConfigFlags> pDriverConfigFlags )
	{
		cppx::bitmask<UINT> deviceCreateFlags = 0;
		if( pDriverConfigFlags.is_set( E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT ) )
		{
			deviceCreateFlags.set( D3D11_CREATE_DEVICE_DEBUG );
		}
		if( pDriverConfigFlags.is_set( E_GPU_DRIVER_CONFIG_FLAG_ENABLE_SHADER_DEBUG_INFO_BIT ) )
		{
			deviceCreateFlags.set( D3D11_CREATE_DEVICE_DEBUGGABLE );
		}
		if( pDriverConfigFlags.is_set( E_GPU_DRIVER_CONFIG_FLAG_DISABLE_MULTI_THREAD_ACCESS_BIT ) )
		{
			deviceCreateFlags.set( D3D11_CREATE_DEVICE_SINGLETHREADED );
		}
		return deviceCreateFlags;
	}

	D3D11_BLEND ATL::TranslateDX11BlendFactor( EBlendFactor pBlendFactor )
	{
		switch( pBlendFactor )
		{
			ic3CaseReturn( EBlendFactor::Undefined   , D3D11_UNDEFINED<D3D11_BLEND> );
			ic3CaseReturn( EBlendFactor::Zero        , D3D11_BLEND_ZERO );
			ic3CaseReturn( EBlendFactor::One         , D3D11_BLEND_ONE );
			ic3CaseReturn( EBlendFactor::Const       , D3D11_BLEND_BLEND_FACTOR );
			ic3CaseReturn( EBlendFactor::ConstInv    , D3D11_BLEND_INV_BLEND_FACTOR );
			ic3CaseReturn( EBlendFactor::SrcColor    , D3D11_BLEND_SRC_COLOR );
			ic3CaseReturn( EBlendFactor::SrcAlpha    , D3D11_BLEND_SRC_ALPHA );
			ic3CaseReturn( EBlendFactor::DstColor    , D3D11_BLEND_DEST_COLOR );
			ic3CaseReturn( EBlendFactor::DstAlpha    , D3D11_BLEND_DEST_ALPHA );
			ic3CaseReturn( EBlendFactor::SrcColorInv , D3D11_BLEND_INV_SRC_COLOR );
			ic3CaseReturn( EBlendFactor::SrcAlphaInv , D3D11_BLEND_INV_SRC_ALPHA );
			ic3CaseReturn( EBlendFactor::DstColorInv , D3D11_BLEND_INV_DEST_COLOR );
			ic3CaseReturn( EBlendFactor::DstAlphaInv , D3D11_BLEND_INV_DEST_ALPHA );
		};
		return D3D11_INVALID<D3D11_BLEND>;
	}

	D3D11_BLEND_OP ATL::TranslateDX11BlendOp( EBlendOp pBlendOp )
	{
		switch( pBlendOp )
		{
			ic3CaseReturn( EBlendOp::Undefined   , D3D11_UNDEFINED<D3D11_BLEND_OP> );
			ic3CaseReturn( EBlendOp::Add         , D3D11_BLEND_OP_ADD );
			ic3CaseReturn( EBlendOp::Min         , D3D11_BLEND_OP_MIN );
			ic3CaseReturn( EBlendOp::Max         , D3D11_BLEND_OP_MAX );
			ic3CaseReturn( EBlendOp::Subtract    , D3D11_BLEND_OP_SUBTRACT );
			ic3CaseReturn( EBlendOp::SubtractRev , D3D11_BLEND_OP_REV_SUBTRACT );
		};
		return D3D11_INVALID<D3D11_BLEND_OP>;
	}

	UINT8 ATL::TranslateDX11BlendRenderTargetWriteMask( cppx::bitmask<EBlendWriteMaskFlags> pWriteMask )
	{
		auto d3d11WriteMask = make_bitmask<UINT8>( 0 );

		if( pWriteMask.is_set( E_BLEND_WRITE_MASK_CHANNEL_RED ) )
		{
			d3d11WriteMask.set( D3D11_COLOR_WRITE_ENABLE_RED );
		}
		if( pWriteMask.is_set( E_BLEND_WRITE_MASK_CHANNEL_GREEN ) )
		{
			d3d11WriteMask.set( D3D11_COLOR_WRITE_ENABLE_GREEN );
		}
		if( pWriteMask.is_set( E_BLEND_WRITE_MASK_CHANNEL_BLUE ) )
		{
			d3d11WriteMask.set( D3D11_COLOR_WRITE_ENABLE_BLUE );
		}
		if( pWriteMask.is_set( E_BLEND_WRITE_MASK_CHANNEL_ALPHA ) )
		{
			d3d11WriteMask.set( D3D11_COLOR_WRITE_ENABLE_ALPHA );
		}

		return d3d11WriteMask;
	}

	cppx::bitmask<D3D11_CLEAR_FLAG> ATL::TranslateDX11RTClearDepthStencilFlags( cppx::bitmask<ERenderTargetBufferFlags> pClearFlags )
	{
		cppx::bitmask<D3D11_CLEAR_FLAG> d3d11ClearDSFlags = 0;
		if( pClearFlags.is_set( E_RENDER_TARGET_BUFFER_FLAG_DEPTH_BIT ) )
		{
			d3d11ClearDSFlags.set( D3D11_CLEAR_DEPTH );
		}
		if( pClearFlags.is_set( E_RENDER_TARGET_BUFFER_FLAG_STENCIL_BIT ) )
		{
			d3d11ClearDSFlags.set( D3D11_CLEAR_STENCIL );
		}
		return d3d11ClearDSFlags;
	}

	UINT ATL::TranslateDX11BufferBindFlags( cppx::bitmask<resource_flags_value_t> pBufferFlags )
	{
		cppx::bitmask<UINT> d3d11BindFlags = 0;
		if( pBufferFlags.is_set( E_GPU_BUFFER_BIND_FLAG_CONSTANT_BUFFER_BIT ) )
		{
			d3d11BindFlags.set( D3D11_BIND_CONSTANT_BUFFER );
		}
		if( pBufferFlags.is_set( E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT ) )
		{
			d3d11BindFlags.set( D3D11_BIND_VERTEX_BUFFER );
		}
		if( pBufferFlags.is_set( E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT ) )
		{
			d3d11BindFlags.set( D3D11_BIND_INDEX_BUFFER );
		}
		if( pBufferFlags.is_set( E_GPU_BUFFER_BIND_FLAG_SHADER_INPUT_BUFFER_BIT ) )
		{
			d3d11BindFlags.set( D3D11_BIND_SHADER_RESOURCE );
		}
		if( pBufferFlags.is_set( E_GPU_BUFFER_BIND_FLAG_SHADER_UAV_BUFFER_BIT ) )
		{
			d3d11BindFlags.set( D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS );
		}
		if( pBufferFlags.is_set( E_GPU_BUFFER_BIND_FLAG_STREAM_OUTPUT_BUFFER_BIT ) )
		{
			d3d11BindFlags.set( D3D11_BIND_STREAM_OUTPUT );
		}
		return d3d11BindFlags;
	}

	D3D11_MAP ATL::TranslateDX11BufferMapFlags( EGpuMemoryMapMode pMapMode, cppx::bitmask<EGpuMemoryFlags> /* pMemoryFlags */ )
	{
		switch( pMapMode )
		{
			ic3CaseReturn( EGpuMemoryMapMode::ReadOnly        , D3D11_MAP_READ               );
			ic3CaseReturn( EGpuMemoryMapMode::ReadWrite       , D3D11_MAP_READ_WRITE         );
			ic3CaseReturn( EGpuMemoryMapMode::WriteDefault    , D3D11_MAP_WRITE              );
			ic3CaseReturn( EGpuMemoryMapMode::WriteInvalidate , D3D11_MAP_WRITE_DISCARD      );
			ic3CaseReturn( EGpuMemoryMapMode::WriteAppend     , D3D11_MAP_WRITE_NO_OVERWRITE );
		};
		return static_cast<D3D11_MAP>( 0 );
	}

	D3D11_COMPARISON_FUNC ATL::TranslateDX11CompFunc( ECompFunc pCompFunc )
	{
		switch( pCompFunc )
		{
			ic3CaseReturn( ECompFunc::Undefined    , D3D11_UNDEFINED<D3D11_COMPARISON_FUNC> );
			ic3CaseReturn( ECompFunc::Never        , D3D11_COMPARISON_NEVER );
			ic3CaseReturn( ECompFunc::Always       , D3D11_COMPARISON_ALWAYS );
			ic3CaseReturn( ECompFunc::Equal        , D3D11_COMPARISON_EQUAL );
			ic3CaseReturn( ECompFunc::NotEqual     , D3D11_COMPARISON_NOT_EQUAL );
			ic3CaseReturn( ECompFunc::Greater      , D3D11_COMPARISON_GREATER );
			ic3CaseReturn( ECompFunc::GreaterEqual , D3D11_COMPARISON_GREATER_EQUAL );
			ic3CaseReturn( ECompFunc::Less         , D3D11_COMPARISON_LESS );
			ic3CaseReturn( ECompFunc::LessEqual    , D3D11_COMPARISON_LESS_EQUAL );
		};
		return D3D11_INVALID<D3D11_COMPARISON_FUNC>;
	}

	D3D11_CULL_MODE ATL::TranslateDX11CullMode( ECullMode pCullMode )
	{
		switch( pCullMode )
		{
			ic3CaseReturn( ECullMode::Undefined  , D3D11_UNDEFINED<D3D11_CULL_MODE> );
			ic3CaseReturn( ECullMode::None       , D3D11_CULL_NONE );
			ic3CaseReturn( ECullMode::Back       , D3D11_CULL_BACK );
			ic3CaseReturn( ECullMode::Front      , D3D11_CULL_FRONT );
		};
		return D3D11_INVALID<D3D11_CULL_MODE>;
	}

	D3D11_DEPTH_WRITE_MASK ATL::TranslateDX11DepthWriteMask( EDepthWriteMask pDepthWriteMask )
	{
		if( pDepthWriteMask == EDepthWriteMask::All )
		{
			return D3D11_DEPTH_WRITE_MASK_ALL;
		}
		else
		{
			return D3D11_DEPTH_WRITE_MASK_ZERO;
		}
	}

	D3D11_FILL_MODE ATL::TranslateDX11PrimitiveFillMode( EPrimitiveFillMode pFillMode )
	{
		switch( pFillMode )
		{
			ic3CaseReturn( EPrimitiveFillMode::Undefined , D3D11_UNDEFINED<D3D11_FILL_MODE> );
			ic3CaseReturn( EPrimitiveFillMode::Solid     , D3D11_FILL_SOLID );
			ic3CaseReturn( EPrimitiveFillMode::Wireframe , D3D11_FILL_WIREFRAME );
		};
		return D3D11_INVALID<D3D11_FILL_MODE>;
	}

	D3D11_PRIMITIVE_TOPOLOGY ATL::TranslateDX11PrimitiveTopology( EPrimitiveTopology pTopology )
	{
		switch( pTopology )
		{
			ic3CaseReturn( EPrimitiveTopology::Undefined        , D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED );
			ic3CaseReturn( EPrimitiveTopology::PointList        , D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
			ic3CaseReturn( EPrimitiveTopology::LineList         , D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
			ic3CaseReturn( EPrimitiveTopology::LineListAdj      , D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ );
			ic3CaseReturn( EPrimitiveTopology::LineStrip        , D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );
			ic3CaseReturn( EPrimitiveTopology::LineStripAdj     , D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ );
			ic3CaseReturn( EPrimitiveTopology::TriangleList     , D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			ic3CaseReturn( EPrimitiveTopology::TriangleListAdj  , D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ );
			ic3CaseReturn( EPrimitiveTopology::TriangleStrip    , D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
			ic3CaseReturn( EPrimitiveTopology::TriangleStripAdj , D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ );
			ic3CaseReturn( EPrimitiveTopology::TesselationPatch , D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST );
		};
		return D3D11_INVALID<D3D11_PRIMITIVE_TOPOLOGY>;
	}

	D3D11_SHADER_TYPE ATL::TranslateDX11EShaderType( EShaderType pShaderType )
	{
		switch( pShaderType )
		{
			ic3CaseReturn( EShaderType::Unknown    , D3D11_UNDEFINED<D3D11_SHADER_TYPE> );
			ic3CaseReturn( EShaderType::GSVertex   , D3D11_VERTEX_SHADER );
			ic3CaseReturn( EShaderType::GSHull     , D3D11_HULL_SHADER );
			ic3CaseReturn( EShaderType::GSDomain   , D3D11_DOMAIN_SHADER );
			ic3CaseReturn( EShaderType::GSGeometry , D3D11_GEOMETRY_SHADER );
			ic3CaseReturn( EShaderType::GSPixel    , D3D11_PIXEL_SHADER );
			ic3CaseReturn( EShaderType::CSCompute  , D3D11_COMPUTE_SHADER );
		}
		return D3D11_INVALID<D3D11_SHADER_TYPE>;
	}

	D3D11_STENCIL_OP ATL::TranslateDX11StencilOp( EStencilOp pStencilOp )
	{
		switch( pStencilOp )
		{
			ic3CaseReturn( EStencilOp::Undefined , D3D11_UNDEFINED<D3D11_STENCIL_OP> );
			ic3CaseReturn( EStencilOp::Zero      , D3D11_STENCIL_OP_ZERO );
			ic3CaseReturn( EStencilOp::Keep      , D3D11_STENCIL_OP_KEEP );
			ic3CaseReturn( EStencilOp::Replace   , D3D11_STENCIL_OP_REPLACE );
			ic3CaseReturn( EStencilOp::IncrClamp , D3D11_STENCIL_OP_INCR );
			ic3CaseReturn( EStencilOp::IncrWrap  , D3D11_STENCIL_OP_INCR_SAT );
			ic3CaseReturn( EStencilOp::DecrClamp , D3D11_STENCIL_OP_DECR );
			ic3CaseReturn( EStencilOp::DecrWrap  , D3D11_STENCIL_OP_DECR_SAT );
			ic3CaseReturn( EStencilOp::Invert    , D3D11_STENCIL_OP_INVERT );
		};
		return D3D11_INVALID<D3D11_STENCIL_OP>;
	}

	D3D11_TEXTURE_ADDRESS_MODE ATL::TranslateDX11ETextureAddressMode( ETextureAddressMode pAddressMode )
	{
		switch( pAddressMode )
		{
			ic3CaseReturn( ETextureAddressMode::Undefined         , D3D11_UNDEFINED<D3D11_TEXTURE_ADDRESS_MODE> );
			ic3CaseReturn( ETextureAddressMode::ClampToEdge       , D3D11_TEXTURE_ADDRESS_CLAMP );
			ic3CaseReturn( ETextureAddressMode::MirrorRepeat      , D3D11_TEXTURE_ADDRESS_MIRROR );
			ic3CaseReturn( ETextureAddressMode::Repeat            , D3D11_TEXTURE_ADDRESS_WRAP );
			ic3CaseReturn( ETextureAddressMode::ClampToColor      , D3D11_TEXTURE_ADDRESS_BORDER );
			ic3CaseReturn( ETextureAddressMode::MirrorClampToEdge , D3D11_TEXTURE_ADDRESS_MIRROR_ONCE );
		};
		return D3D11_UNDEFINED<D3D11_TEXTURE_ADDRESS_MODE>;
	}

	UINT ATL::TranslateDX11ETextureBindFlags( cppx::bitmask<resource_flags_value_t> pTextureFlags )
	{
		cppx::bitmask<UINT> d3d11BindFlags = 0;
		if( pTextureFlags.is_set( E_GPU_RESOURCE_USAGE_FLAG_SHADER_INPUT_BIT ) )
		{
			d3d11BindFlags.set( D3D11_BIND_SHADER_RESOURCE );
		}
		if( pTextureFlags.is_set( E_GPU_RESOURCE_USAGE_FLAG_RENDER_TARGET_COLOR_BIT ) )
		{
			d3d11BindFlags.set( D3D11_BIND_RENDER_TARGET );
		}
		if( pTextureFlags.is_set_any_of( E_GPU_RESOURCE_USAGE_MASK_RENDER_TARGET_DEPTH_STENCIL ) )
		{
			d3d11BindFlags.set( D3D11_BIND_DEPTH_STENCIL );
		}
		return d3d11BindFlags;
	}

	D3D11_FILTER ATL::TranslateDX11ETextureFilter( ETextureFilter magFilter, ETextureFilter minFilter, ETextureMipMode mipMode, uint32 anisotropyLevel )
	{
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}

} // namespace Ic3::Graphics::GCI
