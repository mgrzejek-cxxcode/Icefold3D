
#include "DX11APITranslationLayer.h"
#include <Ic3/Graphics/HW3D/DX11/DX11GPUDevice.h>
#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>
#include <Ic3/Graphics/GCI/State/SamplerCommon.h>
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>
#include <Ic3/System/Core/WindowNative.h>
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
		UINT requestedFeatureLevelCount = static_cast<UINT>( cppx::static_array_size( featureLevelArray ) );

		ComPtr<ID3D11Device> d3d11Device;
		ComPtr<ID3D11DeviceContext> d3d11DeviceContext;
		D3D_FEATURE_LEVEL targetFeatureLevel;

		auto hResult = ::D3D11CreateDevice(
				nullptr,
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
			hResult = ::D3D11CreateDevice(
					nullptr,
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

	ComPtr<IDXGISwapChain1> ATL::CreateD3D11SwapChainForSystemWindow( DX11GPUDevice & pDX11GPUDevice, void * pSysWindow )
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

		auto & dxgiFactory = pDX11GPUDevice.mDXGIFactory2;

		ComPtr<IDXGISwapChain1> dxgiSwapChain1;
		auto hResult = dxgiFactory->CreateSwapChainForHwnd(
				pDX11GPUDevice.mD3D11Device1.Get(),
				sysWindowPtr->mNativeData.hwnd,
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

	UINT ATL::TranslateDX11GPUDeviceCreateFlags( cppx::bitmask<EGPUDriverConfigFlags> pDriverConfigFlags )
	{
		cppx::bitmask<UINT> deviceCreateFlags = 0;
		if( pDriverConfigFlags.is_set( eGPUDriverConfigFlagEnableShaderDebugInfoBit ) )
		{
			deviceCreateFlags.set( D3D11_CREATE_DEVICE_DEBUGGABLE );
		}
		if( pDriverConfigFlags.is_set( eGPUDriverConfigFlagEnableDebugLayerBit ) )
		{
			deviceCreateFlags.set( D3D11_CREATE_DEVICE_DEBUG );
		}
		if( pDriverConfigFlags.is_set( eGPUDriverConfigFlagDisableMultiThreadAccessBit ) )
		{
			deviceCreateFlags.set( D3D11_CREATE_DEVICE_SINGLETHREADED );
		}
		return deviceCreateFlags;
	}

	D3D11_BLEND ATL::TranslateDX11BlendFactor( EBlendFactor pBlendFactor )
	{
		switch( pBlendFactor )
		{
			Ic3CaseReturn( EBlendFactor::Undefined   , D3D11_UNDEFINED<D3D11_BLEND> );
			Ic3CaseReturn( EBlendFactor::Zero        , D3D11_BLEND_ZERO );
			Ic3CaseReturn( EBlendFactor::One         , D3D11_BLEND_ONE );
			Ic3CaseReturn( EBlendFactor::Const       , D3D11_BLEND_BLEND_FACTOR );
			Ic3CaseReturn( EBlendFactor::ConstInv    , D3D11_BLEND_INV_BLEND_FACTOR );
			Ic3CaseReturn( EBlendFactor::SrcColor    , D3D11_BLEND_SRC_COLOR );
			Ic3CaseReturn( EBlendFactor::SrcAlpha    , D3D11_BLEND_SRC_ALPHA );
			Ic3CaseReturn( EBlendFactor::DstColor    , D3D11_BLEND_DEST_COLOR );
			Ic3CaseReturn( EBlendFactor::DstAlpha    , D3D11_BLEND_DEST_ALPHA );
			Ic3CaseReturn( EBlendFactor::SrcColorInv , D3D11_BLEND_INV_SRC_COLOR );
			Ic3CaseReturn( EBlendFactor::SrcAlphaInv , D3D11_BLEND_INV_SRC_ALPHA );
			Ic3CaseReturn( EBlendFactor::DstColorInv , D3D11_BLEND_INV_DEST_COLOR );
			Ic3CaseReturn( EBlendFactor::DstAlphaInv , D3D11_BLEND_INV_DEST_ALPHA );
		};
		return D3D11_INVALID<D3D11_BLEND>;
	}

	D3D11_BLEND_OP ATL::TranslateDX11BlendOp( EBlendOp pBlendOp )
	{
		switch( pBlendOp )
		{
			Ic3CaseReturn( EBlendOp::Undefined   , D3D11_UNDEFINED<D3D11_BLEND_OP> );
			Ic3CaseReturn( EBlendOp::Add         , D3D11_BLEND_OP_ADD );
			Ic3CaseReturn( EBlendOp::Min         , D3D11_BLEND_OP_MIN );
			Ic3CaseReturn( EBlendOp::Max         , D3D11_BLEND_OP_MAX );
			Ic3CaseReturn( EBlendOp::Subtract    , D3D11_BLEND_OP_SUBTRACT );
			Ic3CaseReturn( EBlendOp::SubtractRev , D3D11_BLEND_OP_REV_SUBTRACT );
		};
		return D3D11_INVALID<D3D11_BLEND_OP>;
	}

	UINT8 ATL::TranslateDX11BlendRenderTargetWriteMask( cppx::bitmask<EBlendWriteMaskFlags> pWriteMask )
	{
		auto d3d11WriteMask = cppx::make_bitmask<UINT8>( 0 );

		if( pWriteMask.is_set( eBlendWriteMaskChannelRed ) )
		{
			d3d11WriteMask.set( D3D11_COLOR_WRITE_ENABLE_RED );
		}
		if( pWriteMask.is_set( eBlendWriteMaskChannelGreen ) )
		{
			d3d11WriteMask.set( D3D11_COLOR_WRITE_ENABLE_GREEN );
		}
		if( pWriteMask.is_set( eBlendWriteMaskChannelBlue ) )
		{
			d3d11WriteMask.set( D3D11_COLOR_WRITE_ENABLE_BLUE );
		}
		if( pWriteMask.is_set( eBlendWriteMaskChannelAlpha ) )
		{
			d3d11WriteMask.set( D3D11_COLOR_WRITE_ENABLE_ALPHA );
		}

		return d3d11WriteMask;
	}

	cppx::bitmask<D3D11_CLEAR_FLAG> ATL::TranslateDX11RTClearDepthStencilFlags( cppx::bitmask<ERenderTargetBufferFlags> pClearFlags )
	{
		cppx::bitmask<D3D11_CLEAR_FLAG> d3d11ClearDSFlags = 0;
		if( pClearFlags.is_set( eRenderTargetBufferFlagDepthBit ) )
		{
			d3d11ClearDSFlags.set( D3D11_CLEAR_DEPTH );
		}
		if( pClearFlags.is_set( eRenderTargetBufferFlagStencilBit ) )
		{
			d3d11ClearDSFlags.set( D3D11_CLEAR_STENCIL );
		}
		return d3d11ClearDSFlags;
	}

	UINT ATL::TranslateDX11BufferBindFlags( cppx::bitmask<resource_flags_value_t> pBufferFlags )
	{
		cppx::bitmask<UINT> d3d11BindFlags = 0;
		if( pBufferFlags.is_set( eGPUBufferBindFlagConstantBufferBit ) )
		{
			d3d11BindFlags.set( D3D11_BIND_CONSTANT_BUFFER );
		}
		if( pBufferFlags.is_set( eGPUBufferBindFlagVertexBufferBit ) )
		{
			d3d11BindFlags.set( D3D11_BIND_VERTEX_BUFFER );
		}
		if( pBufferFlags.is_set( eGPUBufferBindFlagIndexBufferBit ) )
		{
			d3d11BindFlags.set( D3D11_BIND_INDEX_BUFFER );
		}
		if( pBufferFlags.is_set( eGPUBufferBindFlagShaderInputBufferBit ) )
		{
			d3d11BindFlags.set( D3D11_BIND_SHADER_RESOURCE );
		}
		if( pBufferFlags.is_set( eGPUBufferBindFlagShaderUAVBufferBit ) )
		{
			d3d11BindFlags.set( D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS );
		}
		if( pBufferFlags.is_set( eGPUBufferBindFlagStreamOutputBufferBit ) )
		{
			d3d11BindFlags.set( D3D11_BIND_STREAM_OUTPUT );
		}
		return d3d11BindFlags;
	}

	D3D11_MAP ATL::TranslateDX11BufferMapFlags( EGPUMemoryMapMode pMapMode, cppx::bitmask<EGPUMemoryFlags> /* pMemoryFlags */ )
	{
		switch( pMapMode )
		{
			Ic3CaseReturn( EGPUMemoryMapMode::ReadOnly        , D3D11_MAP_READ               );
			Ic3CaseReturn( EGPUMemoryMapMode::ReadWrite       , D3D11_MAP_READ_WRITE         );
			Ic3CaseReturn( EGPUMemoryMapMode::WriteDefault    , D3D11_MAP_WRITE              );
			Ic3CaseReturn( EGPUMemoryMapMode::WriteInvalidate , D3D11_MAP_WRITE_DISCARD      );
			Ic3CaseReturn( EGPUMemoryMapMode::WriteAppend     , D3D11_MAP_WRITE_NO_OVERWRITE );
		};
		return static_cast<D3D11_MAP>( 0 );
	}

	D3D11_COMPARISON_FUNC ATL::TranslateDX11CompFunc( ECompFunc pCompFunc )
	{
		switch( pCompFunc )
		{
			Ic3CaseReturn( ECompFunc::Undefined    , D3D11_UNDEFINED<D3D11_COMPARISON_FUNC> );
			Ic3CaseReturn( ECompFunc::Never        , D3D11_COMPARISON_NEVER );
			Ic3CaseReturn( ECompFunc::Always       , D3D11_COMPARISON_ALWAYS );
			Ic3CaseReturn( ECompFunc::Equal        , D3D11_COMPARISON_EQUAL );
			Ic3CaseReturn( ECompFunc::NotEqual     , D3D11_COMPARISON_NOT_EQUAL );
			Ic3CaseReturn( ECompFunc::Greater      , D3D11_COMPARISON_GREATER );
			Ic3CaseReturn( ECompFunc::GreaterEqual , D3D11_COMPARISON_GREATER_EQUAL );
			Ic3CaseReturn( ECompFunc::Less         , D3D11_COMPARISON_LESS );
			Ic3CaseReturn( ECompFunc::LessEqual    , D3D11_COMPARISON_LESS_EQUAL );
		};
		return D3D11_INVALID<D3D11_COMPARISON_FUNC>;
	}

	D3D11_CULL_MODE ATL::TranslateDX11CullMode( ECullMode pCullMode )
	{
		switch( pCullMode )
		{
			Ic3CaseReturn( ECullMode::Undefined  , D3D11_UNDEFINED<D3D11_CULL_MODE> );
			Ic3CaseReturn( ECullMode::None       , D3D11_CULL_NONE );
			Ic3CaseReturn( ECullMode::Back       , D3D11_CULL_BACK );
			Ic3CaseReturn( ECullMode::Front      , D3D11_CULL_FRONT );
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
			Ic3CaseReturn( EPrimitiveFillMode::Undefined , D3D11_UNDEFINED<D3D11_FILL_MODE> );
			Ic3CaseReturn( EPrimitiveFillMode::Solid     , D3D11_FILL_SOLID );
			Ic3CaseReturn( EPrimitiveFillMode::Wireframe , D3D11_FILL_WIREFRAME );
		};
		return D3D11_INVALID<D3D11_FILL_MODE>;
	}

	D3D11_PRIMITIVE_TOPOLOGY ATL::TranslateDX11PrimitiveTopology( EPrimitiveTopology pTopology )
	{
		switch( pTopology )
		{
			Ic3CaseReturn( EPrimitiveTopology::Undefined        , D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED );
			Ic3CaseReturn( EPrimitiveTopology::PointList        , D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
			Ic3CaseReturn( EPrimitiveTopology::LineList         , D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
			Ic3CaseReturn( EPrimitiveTopology::LineListAdj      , D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ );
			Ic3CaseReturn( EPrimitiveTopology::LineStrip        , D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );
			Ic3CaseReturn( EPrimitiveTopology::LineStripAdj     , D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ );
			Ic3CaseReturn( EPrimitiveTopology::TriangleList     , D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			Ic3CaseReturn( EPrimitiveTopology::TriangleListAdj  , D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ );
			Ic3CaseReturn( EPrimitiveTopology::TriangleStrip    , D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
			Ic3CaseReturn( EPrimitiveTopology::TriangleStripAdj , D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ );
			Ic3CaseReturn( EPrimitiveTopology::TesselationPatch , D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST );
		};
		return D3D11_INVALID<D3D11_PRIMITIVE_TOPOLOGY>;
	}

	D3D11_SHADER_TYPE ATL::TranslateDX11EShaderType( EShaderType pShaderType )
	{
		switch( pShaderType )
		{
			Ic3CaseReturn( EShaderType::Unknown      , D3D11_UNDEFINED<D3D11_SHADER_TYPE> );
			Ic3CaseReturn( EShaderType::GSVertex     , D3D11_VERTEX_SHADER );
			Ic3CaseReturn( EShaderType::GSTessHull   , D3D11_HULL_SHADER );
			Ic3CaseReturn( EShaderType::GSTessDomain , D3D11_DOMAIN_SHADER );
			Ic3CaseReturn( EShaderType::GSGeometry   , D3D11_GEOMETRY_SHADER );
			Ic3CaseReturn( EShaderType::GSPixel      , D3D11_PIXEL_SHADER );
			Ic3CaseReturn( EShaderType::CSCompute    , D3D11_COMPUTE_SHADER );
		}
		return D3D11_INVALID<D3D11_SHADER_TYPE>;
	}

	D3D11_STENCIL_OP ATL::TranslateDX11StencilOp( EStencilOp pStencilOp )
	{
		switch( pStencilOp )
		{
			Ic3CaseReturn( EStencilOp::Undefined , D3D11_UNDEFINED<D3D11_STENCIL_OP> );
			Ic3CaseReturn( EStencilOp::Zero      , D3D11_STENCIL_OP_ZERO );
			Ic3CaseReturn( EStencilOp::Keep      , D3D11_STENCIL_OP_KEEP );
			Ic3CaseReturn( EStencilOp::Replace   , D3D11_STENCIL_OP_REPLACE );
			Ic3CaseReturn( EStencilOp::IncrClamp , D3D11_STENCIL_OP_INCR );
			Ic3CaseReturn( EStencilOp::IncrWrap  , D3D11_STENCIL_OP_INCR_SAT );
			Ic3CaseReturn( EStencilOp::DecrClamp , D3D11_STENCIL_OP_DECR );
			Ic3CaseReturn( EStencilOp::DecrWrap  , D3D11_STENCIL_OP_DECR_SAT );
			Ic3CaseReturn( EStencilOp::Invert    , D3D11_STENCIL_OP_INVERT );
		};
		return D3D11_INVALID<D3D11_STENCIL_OP>;
	}

	D3D11_TEXTURE_ADDRESS_MODE ATL::TranslateDX11ETextureAddressMode( ETextureAddressMode pAddressMode )
	{
		switch( pAddressMode )
		{
			Ic3CaseReturn( ETextureAddressMode::Undefined         , D3D11_UNDEFINED<D3D11_TEXTURE_ADDRESS_MODE> );
			Ic3CaseReturn( ETextureAddressMode::ClampToEdge       , D3D11_TEXTURE_ADDRESS_CLAMP );
			Ic3CaseReturn( ETextureAddressMode::MirrorRepeat      , D3D11_TEXTURE_ADDRESS_MIRROR );
			Ic3CaseReturn( ETextureAddressMode::Repeat            , D3D11_TEXTURE_ADDRESS_WRAP );
			Ic3CaseReturn( ETextureAddressMode::ClampToColor      , D3D11_TEXTURE_ADDRESS_BORDER );
			Ic3CaseReturn( ETextureAddressMode::MirrorClampToEdge , D3D11_TEXTURE_ADDRESS_MIRROR_ONCE );
		};
		return D3D11_UNDEFINED<D3D11_TEXTURE_ADDRESS_MODE>;
	}

	UINT ATL::TranslateDX11ETextureBindFlags( cppx::bitmask<resource_flags_value_t> pTextureFlags )
	{
		cppx::bitmask<UINT> d3d11BindFlags = 0;
		if( pTextureFlags.is_set( eGPUResourceUsageFlagShaderInputBit ) )
		{
			d3d11BindFlags.set( D3D11_BIND_SHADER_RESOURCE );
		}
		if( pTextureFlags.is_set( eGPUResourceUsageFlagRenderTargetColorBit ) )
		{
			d3d11BindFlags.set( D3D11_BIND_RENDER_TARGET );
		}
		if( pTextureFlags.is_set_any_of( eGPUResourceUsageMaskRenderTargetDepthStencil ) )
		{
			d3d11BindFlags.set( D3D11_BIND_DEPTH_STENCIL );
		}
		return d3d11BindFlags;
	}

	D3D11_FILTER ATL::TranslateDX11ETextureFilter( ETextureFilter magFilter, ETextureFilter minFilter, ETextureMipMode mipMode, uint32 anisotropyLevel )
	{
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}

	D3D11_INPUT_CLASSIFICATION ATL::DXTranslateVertexAttribDataRate( EIAVertexAttributeDataRate pDataRate )
	{
		if( pDataRate == EIAVertexAttributeDataRate::PerInstance )
		{
			return D3D11_INPUT_PER_INSTANCE_DATA;
		}
		else
		{
			return D3D11_INPUT_PER_VERTEX_DATA;
		}
		return static_cast< D3D11_INPUT_CLASSIFICATION >( cppx::meta::limits<uint32>::max_value );
	}

} // namespace Ic3::Graphics::GCI
