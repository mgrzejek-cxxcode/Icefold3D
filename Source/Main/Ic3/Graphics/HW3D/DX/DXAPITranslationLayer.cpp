
#include "DXApiTranslationLayer.h"
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>
#include <cppx/stdHelperAlgo.h>
#include <cppx/utilities.h>

namespace Ic3::Graphics::GCI
{

	DXGIGetDebugInterfaceType ATL::LoadDXGIDebugLegacyLoader()
	{
		HMODULE dxgiDebugLib = ::LoadLibraryExA( "dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32 );
		if( !dxgiDebugLib )
		{
			return nullptr;
		}

		auto dxgiProcAddress = ::GetProcAddress( dxgiDebugLib, "DXGIGetDebugInterface" );
		if( !dxgiProcAddress )
		{
			return nullptr;
		}

		return reinterpret_cast<DXGIGetDebugInterfaceType>( dxgiProcAddress );
	}

	ComPtr<IDXGIDebug> ATL::QueryDXGIDebugInterface( cppx::bitmask<EGpuDriverConfigFlags> pDriverConfigFlags )
	{
		if( !pDriverConfigFlags.is_set( E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT ) )
		{
			return nullptr;
		}

		ComPtr<IDXGIDebug> dxgiDebugInterface;

	#if( IC3_DRIVER_GRAPHICS_HW3D_DX12_BUILD )
		ComPtr<IDXGIDebug1> dxgiDebugInterface1;
		auto hResult = DXGIGetDebugInterface1( 0, IID_PPV_ARGS( &dxgiDebugInterface1 ) );
		if( FAILED( hResult ) )
		{
			// TODO: warning
		}
		dxgiDebugInterface = std::move( dxgiDebugInterface1 );
	#elif( IC3_DRIVER_GRAPHICS_HW3D_DX11_BUILD )
		auto DXGIGetDebugInterface = LoadDXGIDebugLegacyLoader();
		auto hResult = DXGIGetDebugInterface( IID_PPV_ARGS( &dxgiDebugInterface ) );
		if( FAILED( hResult ) )
		{
			// TODO: warning
		}
	#endif

		return dxgiDebugInterface;
	}

	ComPtr<IDXGIInfoQueue> ATL::QueryDXGIDebugInfoQueue( cppx::bitmask<EGpuDriverConfigFlags> pDriverConfigFlags )
	{
		if( !pDriverConfigFlags.is_set( E_GPU_DRIVER_CONFIG_FLAG_ENABLE_DEBUG_LAYER_BIT ) )
		{
			return nullptr;
		}

		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;

	#if( IC3_DRIVER_GRAPHICS_HW3D_DX12_BUILD )
		auto hResult = DXGIGetDebugInterface1( 0, IID_PPV_ARGS( &dxgiInfoQueue ) );
		if( FAILED( hResult ) )
		{
			// TODO: warning
		}
	#elif( IC3_DRIVER_GRAPHICS_HW3D_DX11_BUILD )
		auto DXGIGetDebugInterface = LoadDXGIDebugLegacyLoader();
		auto hResult = DXGIGetDebugInterface( IID_PPV_ARGS( &dxgiInfoQueue ) );
		if( FAILED( hResult ) )
		{
			// TODO: warning
		}
	#endif

		return dxgiInfoQueue;
	}

	uint32 ATL::GetDXGITextureFormatBPP( DXGI_FORMAT pDXGIFormat )
	{
	    switch( pDXGIFormat )
		{
			ic3CaseReturn( DXGI_FORMAT_UNKNOWN                    , 0   );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32A32_TYPELESS      , 128 );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32A32_FLOAT         , 128 );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32A32_UINT          , 128 );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32A32_SINT          , 128 );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32_TYPELESS         , 128 );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32_FLOAT            , 96  );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32_UINT             , 96  );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32_SINT             , 96  );
			ic3CaseReturn( DXGI_FORMAT_R16G16B16A16_TYPELESS      , 64  );
			ic3CaseReturn( DXGI_FORMAT_R16G16B16A16_FLOAT         , 64  );
			ic3CaseReturn( DXGI_FORMAT_R16G16B16A16_UNORM         , 64  );
			ic3CaseReturn( DXGI_FORMAT_R16G16B16A16_UINT          , 64  );
			ic3CaseReturn( DXGI_FORMAT_R16G16B16A16_SNORM         , 64  );
			ic3CaseReturn( DXGI_FORMAT_R16G16B16A16_SINT          , 64  );
			ic3CaseReturn( DXGI_FORMAT_R32G32_TYPELESS            , 64  );
			ic3CaseReturn( DXGI_FORMAT_R32G32_FLOAT               , 64  );
			ic3CaseReturn( DXGI_FORMAT_R32G32_UINT                , 64  );
			ic3CaseReturn( DXGI_FORMAT_R32G32_SINT                , 64  );
			ic3CaseReturn( DXGI_FORMAT_R32G8X24_TYPELESS          , 64  );
			ic3CaseReturn( DXGI_FORMAT_D32_FLOAT_S8X24_UINT       , 64  );
			ic3CaseReturn( DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS   , 64  );
			ic3CaseReturn( DXGI_FORMAT_X32_TYPELESS_G8X24_UINT    , 64  );
			ic3CaseReturn( DXGI_FORMAT_R10G10B10A2_TYPELESS       , 32  );
			ic3CaseReturn( DXGI_FORMAT_R10G10B10A2_UNORM          , 32  );
			ic3CaseReturn( DXGI_FORMAT_R10G10B10A2_UINT           , 32  );
			ic3CaseReturn( DXGI_FORMAT_R11G11B10_FLOAT            , 32  );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_TYPELESS          , 32  );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_UNORM             , 32  );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_UNORM_SRGB        , 32  );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_UINT              , 32  );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_SNORM             , 32  );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_SINT              , 32  );
			ic3CaseReturn( DXGI_FORMAT_R16G16_TYPELESS            , 32  );
			ic3CaseReturn( DXGI_FORMAT_R16G16_FLOAT               , 32  );
			ic3CaseReturn( DXGI_FORMAT_R16G16_UNORM               , 32  );
			ic3CaseReturn( DXGI_FORMAT_R16G16_UINT                , 32  );
			ic3CaseReturn( DXGI_FORMAT_R16G16_SNORM               , 32  );
			ic3CaseReturn( DXGI_FORMAT_R16G16_SINT                , 32  );
			ic3CaseReturn( DXGI_FORMAT_R32_TYPELESS               , 32  );
			ic3CaseReturn( DXGI_FORMAT_D32_FLOAT                  , 32  );
			ic3CaseReturn( DXGI_FORMAT_R32_FLOAT                  , 32  );
			ic3CaseReturn( DXGI_FORMAT_R32_UINT                   , 32  );
			ic3CaseReturn( DXGI_FORMAT_R32_SINT                   , 32  );
			ic3CaseReturn( DXGI_FORMAT_R24G8_TYPELESS             , 32  );
			ic3CaseReturn( DXGI_FORMAT_D24_UNORM_S8_UINT          , 32  );
			ic3CaseReturn( DXGI_FORMAT_R24_UNORM_X8_TYPELESS      , 32  );
			ic3CaseReturn( DXGI_FORMAT_X24_TYPELESS_G8_UINT       , 32  );
			ic3CaseReturn( DXGI_FORMAT_R8G8_TYPELESS              , 16  );
			ic3CaseReturn( DXGI_FORMAT_R8G8_UNORM                 , 16  );
			ic3CaseReturn( DXGI_FORMAT_R8G8_UINT                  , 16  );
			ic3CaseReturn( DXGI_FORMAT_R8G8_SNORM                 , 16  );
			ic3CaseReturn( DXGI_FORMAT_R8G8_SINT                  , 16  );
			ic3CaseReturn( DXGI_FORMAT_R16_TYPELESS               , 16  );
			ic3CaseReturn( DXGI_FORMAT_R16_FLOAT                  , 16  );
			ic3CaseReturn( DXGI_FORMAT_D16_UNORM                  , 16  );
			ic3CaseReturn( DXGI_FORMAT_R16_UNORM                  , 16  );
			ic3CaseReturn( DXGI_FORMAT_R16_UINT                   , 16  );
			ic3CaseReturn( DXGI_FORMAT_R16_SNORM                  , 16  );
			ic3CaseReturn( DXGI_FORMAT_R16_SINT                   , 16  );
			ic3CaseReturn( DXGI_FORMAT_R8_TYPELESS                , 8   );
			ic3CaseReturn( DXGI_FORMAT_R8_UNORM                   , 8   );
			ic3CaseReturn( DXGI_FORMAT_R8_UINT                    , 8   );
			ic3CaseReturn( DXGI_FORMAT_R8_SNORM                   , 8   );
			ic3CaseReturn( DXGI_FORMAT_R8_SINT                    , 8   );
			ic3CaseReturn( DXGI_FORMAT_A8_UNORM                   , 8   );
			ic3CaseReturn( DXGI_FORMAT_R1_UNORM                   , 8   );
			ic3CaseReturn( DXGI_FORMAT_R9G9B9E5_SHAREDEXP         , 32  );
			ic3CaseReturn( DXGI_FORMAT_R8G8_B8G8_UNORM            , 32  );
			ic3CaseReturn( DXGI_FORMAT_G8R8_G8B8_UNORM            , 32  );
			ic3CaseReturn( DXGI_FORMAT_BC1_TYPELESS               , 4   );
			ic3CaseReturn( DXGI_FORMAT_BC1_UNORM                  , 4   );
			ic3CaseReturn( DXGI_FORMAT_BC1_UNORM_SRGB             , 4   );
			ic3CaseReturn( DXGI_FORMAT_BC2_TYPELESS               , 8   );
			ic3CaseReturn( DXGI_FORMAT_BC2_UNORM                  , 8   );
			ic3CaseReturn( DXGI_FORMAT_BC2_UNORM_SRGB             , 8   );
			ic3CaseReturn( DXGI_FORMAT_BC3_TYPELESS               , 8   );
			ic3CaseReturn( DXGI_FORMAT_BC3_UNORM                  , 8   );
			ic3CaseReturn( DXGI_FORMAT_BC3_UNORM_SRGB             , 8   );
			ic3CaseReturn( DXGI_FORMAT_BC4_TYPELESS               , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC4_UNORM                  , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC4_SNORM                  , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC5_TYPELESS               , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC5_UNORM                  , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC5_SNORM                  , 0   );
			ic3CaseReturn( DXGI_FORMAT_B5G6R5_UNORM               , 16  );
			ic3CaseReturn( DXGI_FORMAT_B5G5R5A1_UNORM             , 16  );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8A8_UNORM             , 32  );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8X8_UNORM             , 32  );
			ic3CaseReturn( DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM , 32  );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8A8_TYPELESS          , 32  );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8A8_UNORM_SRGB        , 32  );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8X8_TYPELESS          , 32  );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8X8_UNORM_SRGB        , 32  );
			ic3CaseReturn( DXGI_FORMAT_BC6H_TYPELESS              , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC6H_UF16                  , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC6H_SF16                  , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC7_TYPELESS               , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC7_UNORM                  , 0   );
			ic3CaseReturn( DXGI_FORMAT_BC7_UNORM_SRGB             , 0   );
		};
		return DXGI_FORMAT_UNKNOWN;
	}

	const char * ATL::GetDXShaderTargetStr( DXShaderTarget pShaderTarget )
	{
		static const char * const shaderTargetStrArray[] =
		{
			"vs_4_1",
			"gs_4_1",
			"ps_4_1",
			"cs_4_1",
			"vs_5_0",
			"hs_5_0",
			"ds_5_0",
			"gs_5_0",
			"ps_5_0",
			"cs_5_0",
		};
		return static_array_element( shaderTargetStrArray, pShaderTarget );
	}

	uint32 ATL::ComputeDXTextureMemoryByteSize( const TextureDimensions & pTextDimensions, DXGI_FORMAT pFormat )
	{
		const uint64 formatBitSize = GetDXGITextureFormatBPP( pFormat );
		const uint64 totalPixelCount = pTextDimensions.width * pTextDimensions.height * pTextDimensions.depth * pTextDimensions.arraySize;

		uint64 totalBaseBitSize = totalPixelCount * formatBitSize;
		uint64 byteSizeDivisor = 8;

		if( pTextDimensions.mipLevelsNum > 1 )
		{
			totalBaseBitSize *= 4;
			byteSizeDivisor *= 3;
		}

		return static_cast<uint32>( ( double )totalBaseBitSize / ( double )byteSizeDivisor );
	}

	cppx::bitmask<UINT> ATL::TranslateDXShaderCompileFlags( cppx::bitmask<uint32> pShaderCreateFlags, bool pDebugDevice )
	{
		cppx::bitmask<UINT> compileFlags = 0;
		if( pDebugDevice || pShaderCreateFlags.is_set( E_SHADER_CREATE_FLAG_DEBUG_BIT ) )
		{
			compileFlags.set( D3DCOMPILE_DEBUG );
		}
		if( pDebugDevice || pShaderCreateFlags.is_set( E_SHADER_CREATE_FLAG_OPTIMIZATION_DISABLE_BIT ) )
		{
			compileFlags.set( D3DCOMPILE_SKIP_OPTIMIZATION );
		}
		if( pShaderCreateFlags.is_set( E_SHADER_CREATE_FLAG_OPTIMIZATION_L0_BIT ) )
		{
			compileFlags.set( D3DCOMPILE_OPTIMIZATION_LEVEL0 );
		}
		if( !pDebugDevice || pShaderCreateFlags.is_set( E_SHADER_CREATE_FLAG_OPTIMIZATION_L1_BIT ) )
		{
			compileFlags.set( D3DCOMPILE_OPTIMIZATION_LEVEL3 );
		}
		return compileFlags;
	}

	DXGI_FORMAT ATL::TranslateDXShaderCompileFlags( EBaseDataType pBaseDataType )
	{
		switch( pBaseDataType )
		{
			ic3CaseReturn( EBaseDataType::Undefined , DXGI_FORMAT_UNKNOWN           );
			ic3CaseReturn( EBaseDataType::Byte      , DXGI_FORMAT_R8_SINT           );
			ic3CaseReturn( EBaseDataType::Ubyte     , DXGI_FORMAT_R8_UINT           );
			ic3CaseReturn( EBaseDataType::Int16     , DXGI_FORMAT_R16_SINT          );
			ic3CaseReturn( EBaseDataType::Uint16    , DXGI_FORMAT_R16_UINT          );
			ic3CaseReturn( EBaseDataType::Int32     , DXGI_FORMAT_R32_SINT          );
			ic3CaseReturn( EBaseDataType::Uint32    , DXGI_FORMAT_R32_UINT          );
			ic3CaseReturn( EBaseDataType::Float16   , DXGI_FORMAT_R16_FLOAT         );
			ic3CaseReturn( EBaseDataType::Float32   , DXGI_FORMAT_R32_FLOAT         );
			ic3CaseReturn( EBaseDataType::Uint24S8  , DXGI_FORMAT_D24_UNORM_S8_UINT );
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	DXGI_FORMAT ATL::TranslateDXTextureFormat( ETextureFormat pTextureFormat )
	{
	    switch( pTextureFormat )
		{
            ic3CaseReturn( ETextureFormat::UNKNOWN    , DXGI_FORMAT_UNKNOWN              );
			ic3CaseReturn( ETextureFormat::R32F       , DXGI_FORMAT_R32_FLOAT            );
			ic3CaseReturn( ETextureFormat::R32I       , DXGI_FORMAT_R32_SINT             );
			ic3CaseReturn( ETextureFormat::R32U       , DXGI_FORMAT_R32_UINT             );
			ic3CaseReturn( ETextureFormat::RG32F      , DXGI_FORMAT_R32G32_FLOAT         );
			ic3CaseReturn( ETextureFormat::RG32I      , DXGI_FORMAT_R32G32_SINT          );
			ic3CaseReturn( ETextureFormat::RG32U      , DXGI_FORMAT_R32G32_UINT          );
			ic3CaseReturn( ETextureFormat::RGB32F     , DXGI_FORMAT_R32G32B32_FLOAT      );
			ic3CaseReturn( ETextureFormat::RGB32I     , DXGI_FORMAT_R32G32B32_SINT       );
			ic3CaseReturn( ETextureFormat::RGB32U     , DXGI_FORMAT_R32G32B32_UINT       );
			ic3CaseReturn( ETextureFormat::RGBA32F    , DXGI_FORMAT_R32G32B32A32_FLOAT   );
			ic3CaseReturn( ETextureFormat::RGBA32I    , DXGI_FORMAT_R32G32B32A32_SINT    );
			ic3CaseReturn( ETextureFormat::RGBA32U    , DXGI_FORMAT_R32G32B32A32_UINT    );
			ic3CaseReturn( ETextureFormat::R16F       , DXGI_FORMAT_R16_FLOAT            );
			ic3CaseReturn( ETextureFormat::R16I       , DXGI_FORMAT_R16_SINT             );
			ic3CaseReturn( ETextureFormat::R16U       , DXGI_FORMAT_R16_UINT             );
			ic3CaseReturn( ETextureFormat::RG16F      , DXGI_FORMAT_R16G16_FLOAT         );
			ic3CaseReturn( ETextureFormat::RG16I      , DXGI_FORMAT_R16G16_SINT          );
			ic3CaseReturn( ETextureFormat::RG16U      , DXGI_FORMAT_R16G16_UINT          );
			ic3CaseReturn( ETextureFormat::RGBA16F    , DXGI_FORMAT_R16G16B16A16_FLOAT   );
			ic3CaseReturn( ETextureFormat::RGBA16I    , DXGI_FORMAT_R16G16B16A16_SINT    );
			ic3CaseReturn( ETextureFormat::RGBA16U    , DXGI_FORMAT_R16G16B16A16_UINT    );
			ic3CaseReturn( ETextureFormat::R8I        , DXGI_FORMAT_R8_SINT              );
			ic3CaseReturn( ETextureFormat::R8U        , DXGI_FORMAT_R8_UINT              );
			ic3CaseReturn( ETextureFormat::R8IN       , DXGI_FORMAT_R8_SNORM             );
			ic3CaseReturn( ETextureFormat::R8UN       , DXGI_FORMAT_R8_UNORM             );
			ic3CaseReturn( ETextureFormat::RG8I       , DXGI_FORMAT_R8G8_SINT            );
			ic3CaseReturn( ETextureFormat::RG8U       , DXGI_FORMAT_R8G8_UINT            );
			ic3CaseReturn( ETextureFormat::RG8IN      , DXGI_FORMAT_R8G8_SNORM           );
			ic3CaseReturn( ETextureFormat::RG8UN      , DXGI_FORMAT_R8G8_UNORM           );
            ic3CaseReturn( ETextureFormat::BGRX8UN    , DXGI_FORMAT_B8G8R8X8_UNORM       );
            ic3CaseReturn( ETextureFormat::BGRX8SRGB  , DXGI_FORMAT_B8G8R8X8_UNORM_SRGB  );
            ic3CaseReturn( ETextureFormat::BGRA8UN    , DXGI_FORMAT_B8G8R8A8_UNORM       );
            ic3CaseReturn( ETextureFormat::BGRA8SRGB  , DXGI_FORMAT_B8G8R8A8_UNORM_SRGB  );
            ic3CaseReturn( ETextureFormat::RGBA8I     , DXGI_FORMAT_R8G8B8A8_SINT        );
            ic3CaseReturn( ETextureFormat::RGBA8U     , DXGI_FORMAT_R8G8B8A8_UINT        );
            ic3CaseReturn( ETextureFormat::RGBA8IN    , DXGI_FORMAT_R8G8B8A8_SNORM       );
            ic3CaseReturn( ETextureFormat::RGBA8UN    , DXGI_FORMAT_R8G8B8A8_UNORM       );
            ic3CaseReturn( ETextureFormat::RGBA8SRGB  , DXGI_FORMAT_R8G8B8A8_UNORM_SRGB  );

			ic3CaseReturn( ETextureFormat::R5G5B5A1   , DXGI_FORMAT_B5G5R5A1_UNORM     );
			ic3CaseReturn( ETextureFormat::R5G6B5     , DXGI_FORMAT_B5G6R5_UNORM       );
			ic3CaseReturn( ETextureFormat::R9G9B9E5   , DXGI_FORMAT_R9G9B9E5_SHAREDEXP );
			ic3CaseReturn( ETextureFormat::RGB10A2U   , DXGI_FORMAT_R10G10B10A2_UINT   );
			ic3CaseReturn( ETextureFormat::RGB10A2UN  , DXGI_FORMAT_R10G10B10A2_UNORM  );
			ic3CaseReturn( ETextureFormat::R11G11B10F , DXGI_FORMAT_R11G11B10_FLOAT    );
			ic3CaseReturn( ETextureFormat::D16UN      , DXGI_FORMAT_D16_UNORM          );
			ic3CaseReturn( ETextureFormat::D24UNS8U   , DXGI_FORMAT_D24_UNORM_S8_UINT  );
			ic3CaseReturn( ETextureFormat::D24UNX8    , DXGI_FORMAT_D24_UNORM_S8_UINT  );
			ic3CaseReturn( ETextureFormat::D32F       , DXGI_FORMAT_D32_FLOAT          );

			ic3CaseReturn( ETextureFormat::BC1        , DXGI_FORMAT_BC1_UNORM      );
			ic3CaseReturn( ETextureFormat::BC1SRGB    , DXGI_FORMAT_BC1_UNORM_SRGB );
			ic3CaseReturn( ETextureFormat::BC2        , DXGI_FORMAT_BC2_UNORM      );
			ic3CaseReturn( ETextureFormat::BC2SRGB    , DXGI_FORMAT_BC2_UNORM_SRGB );
			ic3CaseReturn( ETextureFormat::BC3        , DXGI_FORMAT_BC3_UNORM      );
			ic3CaseReturn( ETextureFormat::BC3SRGB    , DXGI_FORMAT_BC3_UNORM_SRGB );
			ic3CaseReturn( ETextureFormat::BC4IN      , DXGI_FORMAT_BC4_SNORM      );
			ic3CaseReturn( ETextureFormat::BC4UN      , DXGI_FORMAT_BC4_UNORM      );
			ic3CaseReturn( ETextureFormat::BC5IN      , DXGI_FORMAT_BC5_SNORM      );
			ic3CaseReturn( ETextureFormat::BC5UN      , DXGI_FORMAT_BC5_UNORM      );
			ic3CaseReturn( ETextureFormat::BC6HSF     , DXGI_FORMAT_BC6H_SF16      );
			ic3CaseReturn( ETextureFormat::BC6HUF     , DXGI_FORMAT_BC6H_UF16      );
			ic3CaseReturn( ETextureFormat::BC7        , DXGI_FORMAT_BC7_UNORM      );
			ic3CaseReturn( ETextureFormat::BC7SRGB    , DXGI_FORMAT_BC7_UNORM_SRGB );
		};
		return DXGI_FORMAT_UNKNOWN;
	}

	ETextureFormat ATL::TranslateDXTextureFormatInv( DXGI_FORMAT pDXGIFormat )
	{
		switch( pDXGIFormat )
		{
			ic3CaseReturn( DXGI_FORMAT_UNKNOWN             , ETextureFormat::UNKNOWN    );
			ic3CaseReturn( DXGI_FORMAT_R32_FLOAT           , ETextureFormat::R32F       );
			ic3CaseReturn( DXGI_FORMAT_R32_SINT            , ETextureFormat::R32I       );
			ic3CaseReturn( DXGI_FORMAT_R32_UINT            , ETextureFormat::R32U       );
			ic3CaseReturn( DXGI_FORMAT_R32G32_FLOAT        , ETextureFormat::RG32F      );
			ic3CaseReturn( DXGI_FORMAT_R32G32_SINT         , ETextureFormat::RG32I      );
			ic3CaseReturn( DXGI_FORMAT_R32G32_UINT         , ETextureFormat::RG32U      );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32_FLOAT     , ETextureFormat::RGB32F     );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32_SINT      , ETextureFormat::RGB32I     );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32_UINT      , ETextureFormat::RGB32U     );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32A32_FLOAT  , ETextureFormat::RGBA32F    );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32A32_SINT   , ETextureFormat::RGBA32I    );
			ic3CaseReturn( DXGI_FORMAT_R32G32B32A32_UINT   , ETextureFormat::RGBA32U    );
			ic3CaseReturn( DXGI_FORMAT_R16_FLOAT           , ETextureFormat::R16F       );
			ic3CaseReturn( DXGI_FORMAT_R16_SINT            , ETextureFormat::R16I       );
			ic3CaseReturn( DXGI_FORMAT_R16_UINT            , ETextureFormat::R16U       );
			ic3CaseReturn( DXGI_FORMAT_R16G16_FLOAT        , ETextureFormat::RG16F      );
			ic3CaseReturn( DXGI_FORMAT_R16G16_SINT         , ETextureFormat::RG16I      );
			ic3CaseReturn( DXGI_FORMAT_R16G16_UINT         , ETextureFormat::RG16U      );
			ic3CaseReturn( DXGI_FORMAT_R16G16B16A16_FLOAT  , ETextureFormat::RGBA16F    );
			ic3CaseReturn( DXGI_FORMAT_R16G16B16A16_SINT   , ETextureFormat::RGBA16I    );
			ic3CaseReturn( DXGI_FORMAT_R16G16B16A16_UINT   , ETextureFormat::RGBA16U    );
			ic3CaseReturn( DXGI_FORMAT_R8_SINT             , ETextureFormat::R8I        );
			ic3CaseReturn( DXGI_FORMAT_R8_UINT             , ETextureFormat::R8U        );
			ic3CaseReturn( DXGI_FORMAT_R8_SNORM            , ETextureFormat::R8IN       );
			ic3CaseReturn( DXGI_FORMAT_R8_UNORM            , ETextureFormat::R8UN       );
			ic3CaseReturn( DXGI_FORMAT_R8G8_SINT           , ETextureFormat::RG8I       );
			ic3CaseReturn( DXGI_FORMAT_R8G8_UINT           , ETextureFormat::RG8U       );
			ic3CaseReturn( DXGI_FORMAT_R8G8_SNORM          , ETextureFormat::RG8IN      );
			ic3CaseReturn( DXGI_FORMAT_R8G8_UNORM          , ETextureFormat::RG8UN      );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8X8_UNORM      , ETextureFormat::BGRX8UN    );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8X8_UNORM_SRGB , ETextureFormat::BGRX8SRGB  );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8A8_UNORM      , ETextureFormat::BGRA8UN    );
			ic3CaseReturn( DXGI_FORMAT_B8G8R8A8_UNORM_SRGB , ETextureFormat::BGRA8SRGB  );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_SINT       , ETextureFormat::RGBA8I     );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_UINT       , ETextureFormat::RGBA8U     );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_SNORM      , ETextureFormat::RGBA8IN    );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_UNORM      , ETextureFormat::RGBA8UN    );
			ic3CaseReturn( DXGI_FORMAT_R8G8B8A8_UNORM_SRGB , ETextureFormat::RGBA8SRGB  );
			ic3CaseReturn( DXGI_FORMAT_B5G5R5A1_UNORM      , ETextureFormat::R5G5B5A1   );
			ic3CaseReturn( DXGI_FORMAT_B5G6R5_UNORM        , ETextureFormat::R5G6B5     );
			ic3CaseReturn( DXGI_FORMAT_R9G9B9E5_SHAREDEXP  , ETextureFormat::R9G9B9E5   );
			ic3CaseReturn( DXGI_FORMAT_R10G10B10A2_UINT    , ETextureFormat::RGB10A2U   );
			ic3CaseReturn( DXGI_FORMAT_R10G10B10A2_UNORM   , ETextureFormat::RGB10A2UN  );
			ic3CaseReturn( DXGI_FORMAT_R11G11B10_FLOAT     , ETextureFormat::R11G11B10F );
			ic3CaseReturn( DXGI_FORMAT_D16_UNORM           , ETextureFormat::D16UN      );
			ic3CaseReturn( DXGI_FORMAT_D24_UNORM_S8_UINT   , ETextureFormat::D24UNS8U   );
			ic3CaseReturn( DXGI_FORMAT_D32_FLOAT           , ETextureFormat::D32F       );
			ic3CaseReturn( DXGI_FORMAT_BC1_UNORM           , ETextureFormat::BC1        );
			ic3CaseReturn( DXGI_FORMAT_BC1_UNORM_SRGB      , ETextureFormat::BC1SRGB    );
			ic3CaseReturn( DXGI_FORMAT_BC2_UNORM           , ETextureFormat::BC2        );
			ic3CaseReturn( DXGI_FORMAT_BC2_UNORM_SRGB      , ETextureFormat::BC2SRGB    );
			ic3CaseReturn( DXGI_FORMAT_BC3_UNORM           , ETextureFormat::BC3        );
			ic3CaseReturn( DXGI_FORMAT_BC3_UNORM_SRGB      , ETextureFormat::BC3SRGB    );
			ic3CaseReturn( DXGI_FORMAT_BC4_SNORM           , ETextureFormat::BC4IN      );
			ic3CaseReturn( DXGI_FORMAT_BC4_UNORM           , ETextureFormat::BC4UN      );
			ic3CaseReturn( DXGI_FORMAT_BC5_SNORM           , ETextureFormat::BC5IN      );
			ic3CaseReturn( DXGI_FORMAT_BC5_UNORM           , ETextureFormat::BC5UN      );
			ic3CaseReturn( DXGI_FORMAT_BC6H_SF16           , ETextureFormat::BC6HSF     );
			ic3CaseReturn( DXGI_FORMAT_BC6H_UF16           , ETextureFormat::BC6HUF     );
			ic3CaseReturn( DXGI_FORMAT_BC7_UNORM           , ETextureFormat::BC7        );
			ic3CaseReturn( DXGI_FORMAT_BC7_UNORM_SRGB      , ETextureFormat::BC7SRGB    );
		};
		return ETextureFormat::UNKNOWN;
	}

	DXGI_FORMAT ATL::TranslateDXVertexAttribFormat( EVertexAttribFormat pVertexAttribFormat )
	{
	    switch( pVertexAttribFormat )
		{
			ic3CaseReturn( EVertexAttribFormat::F16      , DXGI_FORMAT_R16_FLOAT          );
			ic3CaseReturn( EVertexAttribFormat::F32      , DXGI_FORMAT_R32_FLOAT          );
			ic3CaseReturn( EVertexAttribFormat::I8       , DXGI_FORMAT_R8_SINT            );
			ic3CaseReturn( EVertexAttribFormat::I16      , DXGI_FORMAT_R16_SINT           );
			ic3CaseReturn( EVertexAttribFormat::I32      , DXGI_FORMAT_R32_SINT           );
			ic3CaseReturn( EVertexAttribFormat::U8       , DXGI_FORMAT_R8_UINT            );
			ic3CaseReturn( EVertexAttribFormat::U16      , DXGI_FORMAT_R16_UINT           );
			ic3CaseReturn( EVertexAttribFormat::U32      , DXGI_FORMAT_R32_UINT           );
			ic3CaseReturn( EVertexAttribFormat::I8N      , DXGI_FORMAT_R8_SNORM           );
			ic3CaseReturn( EVertexAttribFormat::I16N     , DXGI_FORMAT_R16_SNORM          );
			ic3CaseReturn( EVertexAttribFormat::U8N      , DXGI_FORMAT_R8_UNORM           );
			ic3CaseReturn( EVertexAttribFormat::U16N     , DXGI_FORMAT_R16_UNORM          );
			ic3CaseReturn( EVertexAttribFormat::Vec2F16  , DXGI_FORMAT_R16G16_FLOAT       );
			ic3CaseReturn( EVertexAttribFormat::Vec2F32  , DXGI_FORMAT_R32G32_FLOAT       );
			ic3CaseReturn( EVertexAttribFormat::Vec2I8   , DXGI_FORMAT_R8G8_SINT          );
			ic3CaseReturn( EVertexAttribFormat::Vec2I16  , DXGI_FORMAT_R16G16_SINT        );
			ic3CaseReturn( EVertexAttribFormat::Vec2I32  , DXGI_FORMAT_R32G32_SINT        );
			ic3CaseReturn( EVertexAttribFormat::Vec2U8   , DXGI_FORMAT_R8G8_UINT          );
			ic3CaseReturn( EVertexAttribFormat::Vec2U16  , DXGI_FORMAT_R16G16_UINT        );
			ic3CaseReturn( EVertexAttribFormat::Vec2U32  , DXGI_FORMAT_R32G32_UINT        );
			ic3CaseReturn( EVertexAttribFormat::Vec2I8N  , DXGI_FORMAT_R8G8_SNORM         );
			ic3CaseReturn( EVertexAttribFormat::Vec2I16N , DXGI_FORMAT_R16G16_SNORM       );
			ic3CaseReturn( EVertexAttribFormat::Vec2U8N  , DXGI_FORMAT_R8G8_UNORM         );
			ic3CaseReturn( EVertexAttribFormat::Vec2U16N , DXGI_FORMAT_R16G16_UNORM       );
			ic3CaseReturn( EVertexAttribFormat::Vec3F32  , DXGI_FORMAT_R32G32B32_FLOAT    );
			ic3CaseReturn( EVertexAttribFormat::Vec3I32  , DXGI_FORMAT_R32G32B32_SINT     );
			ic3CaseReturn( EVertexAttribFormat::Vec3U32  , DXGI_FORMAT_R32G32B32_UINT     );
			ic3CaseReturn( EVertexAttribFormat::Vec4F16  , DXGI_FORMAT_R16G16B16A16_FLOAT );
			ic3CaseReturn( EVertexAttribFormat::Vec4F32  , DXGI_FORMAT_R32G32B32A32_FLOAT );
			ic3CaseReturn( EVertexAttribFormat::Vec4I8   , DXGI_FORMAT_R8G8B8A8_SINT      );
			ic3CaseReturn( EVertexAttribFormat::Vec4I16  , DXGI_FORMAT_R16G16B16A16_SINT  );
			ic3CaseReturn( EVertexAttribFormat::Vec4I32  , DXGI_FORMAT_R32G32B32A32_SINT  );
			ic3CaseReturn( EVertexAttribFormat::Vec4U8   , DXGI_FORMAT_R8G8B8A8_UINT      );
			ic3CaseReturn( EVertexAttribFormat::Vec4U16  , DXGI_FORMAT_R16G16B16A16_UINT  );
			ic3CaseReturn( EVertexAttribFormat::Vec4U32  , DXGI_FORMAT_R32G32B32A32_UINT  );
			ic3CaseReturn( EVertexAttribFormat::Vec4I8N  , DXGI_FORMAT_R8G8B8A8_SNORM     );
			ic3CaseReturn( EVertexAttribFormat::Vec4I16N , DXGI_FORMAT_R16G16B16A16_SNORM );
			ic3CaseReturn( EVertexAttribFormat::Vec4U8N  , DXGI_FORMAT_R8G8B8A8_UNORM     );
			ic3CaseReturn( EVertexAttribFormat::Vec4U16N , DXGI_FORMAT_R16G16B16A16_UNORM );
		};
	    return DXGI_FORMAT_UNKNOWN;
	}

} // namespace Ic3::Graphics::GCI
