
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_API_TRANSLATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_API_TRANSLATION_LAYER_H__

#include "DXPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	struct TextureDimensions;

    using resource_flags_value_t = uint32;

    enum EGPUDriverConfigFlags : uint32;
    enum ERenderTargetAttachmentFlags : uint32;
    enum ETextureCubeMapFace : uint32;

	using DXGIGetDebugInterfaceType = HRESULT ( WINAPI * )( const GUID &, void ** );

	namespace ATL
	{

		DXGIGetDebugInterfaceType loadDXGIDebugLegacyLoader();

		ComPtr<IDXGIDebug> queryDXGIDebugInterface( Bitmask<EGPUDriverConfigFlags> pDriverConfigFlags );

		ComPtr<IDXGIInfoQueue> queryDXGIDebugInfoQueue( Bitmask<EGPUDriverConfigFlags> pDriverConfigFlags );

		uint32 getDXGITextureFormatBPP( DXGI_FORMAT pDXGIFormat );

		const char * getDXShaderTargetStr( DXShaderTarget pShaderTarget );

		uint32 computeDXTextureMemoryByteSize( const TextureDimensions & pTextDimensions, DXGI_FORMAT pFormat );

		Bitmask<UINT> translateShaderCompileFlagsDX( Bitmask<uint32> pShaderCreateFlags, bool pDebugDevice );

		DXGI_FORMAT translateBaseDataTypeDX( EBaseDataType pBaseDataType );

		DXGI_FORMAT translateTextureFormatDX( ETextureFormat pTextureFormat );

		ETextureFormat translateTextureFormatInvDX( DXGI_FORMAT pDXGIFormat );

		DXGI_FORMAT translateVertexAttribFormatDX( EVertexAttribFormat pVertexAttribFormat );

	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_API_TRANSLATION_LAYER_H__
