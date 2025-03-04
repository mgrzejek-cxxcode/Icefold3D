
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

	using DXGIGetDebugInterfaceType = HRESULT ( WINAPI * )( const GUID &, void ** );

	namespace ATL
	{

		DXGIGetDebugInterfaceType LoadDXGIDebugLegacyLoader();

		ComPtr<IDXGIDebug> QueryDXGIDebugInterface( cppx::bitmask<EGPUDriverConfigFlags> pDriverConfigFlags );

		ComPtr<IDXGIInfoQueue> QueryDXGIDebugInfoQueue( cppx::bitmask<EGPUDriverConfigFlags> pDriverConfigFlags );

		uint32 GetDXGITextureFormatBPP( DXGI_FORMAT pDXGIFormat );

		const char * GetDXShaderTargetStr( DXShaderTarget pShaderTarget );

		uint32 ComputeDXTextureMemoryByteSize( const TextureDimensions & pTextDimensions, DXGI_FORMAT pFormat );

		cppx::bitmask<UINT> TranslateDXShaderCompileFlags( cppx::bitmask<uint32> pShaderCreateFlags, bool pDebugDevice );

		DXGI_FORMAT TranslateDXShaderCompileFlags( EBaseDataType pBaseDataType );

		DXGI_FORMAT TranslateDXTextureFormat( ETextureFormat pTextureFormat );

		ETextureFormat TranslateDXTextureFormatInv( DXGI_FORMAT pDXGIFormat );

		DXGI_FORMAT TranslateDXVertexAttribFormat( EVertexAttribFormat pVertexAttribFormat );

	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_API_TRANSLATION_LAYER_H__
