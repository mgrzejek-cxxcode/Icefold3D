
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_API_TRANSLATION_LAYER_H__

#include "DXPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	struct TextureDimensions;

	using resource_flags_value_t = uint32;

	enum EGPUDriverConfigFlags : uint32;
	enum ETextureCubeMapFace : uint32;

	enum class EIAVertexAttributeDataRate : uint8;

	using DXGIGetDebugInterfaceType = HRESULT ( WINAPI * )( const GUID &, void ** );

	namespace ATL
	{

		DXGIGetDebugInterfaceType DXGILoadDebugLegacyLoader();

		ComPtr<IDXGIDebug> DXGIQueryDebugInterface( cppx::bitmask<EGPUDriverConfigFlags> pDriverConfigFlags );

		ComPtr<IDXGIInfoQueue> DXGIQueryDebugInfoQueue( cppx::bitmask<EGPUDriverConfigFlags> pDriverConfigFlags );

		uint32 DXGIGetTextureFormatBPP( DXGI_FORMAT pDXGIFormat );

		const char * DXGetShaderTargetStr( DXShaderTarget pShaderTarget );

		uint32 DXComputeTextureMemoryByteSize( const TextureDimensions & pTextDimensions, DXGI_FORMAT pFormat );

		cppx::bitmask<UINT> DXTranslateShaderCompileFlags( cppx::bitmask<uint32> pShaderCreateFlags, bool pDebugDevice );

		DXGI_FORMAT DXTranslateBaseDataType( EBaseDataType pBaseDataType );

		DXGI_FORMAT DXTranslateTextureFormat( ETextureFormat pTextureFormat );

		ETextureFormat DXTranslateTextureFormatInv( DXGI_FORMAT pDXGIFormat );

		DXGI_FORMAT DXTranslateVertexAttribFormat( EVertexAttribFormat pVertexAttribFormat );

		DXGI_FORMAT DXTranslateIndexDataFormat( EIndexDataFormat pIndexAttribFormat );

	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_API_TRANSLATION_LAYER_H__
