
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PREREQUISITES_H__
#define __IC3_GRAPHICS_GCI_PREREQUISITES_H__

#include <Ic3/CoreLib/Exception.h>
#include <Ic3/CoreLib/PixelCommon.h>
#include <Ic3/CoreLib/MathImports.h>
#include <Ic3/CoreLib/Utility/HFSIdentifier.h>
#include <cppx/hash.h>
#include <cppx/utilities.h>

#include <Ic3/Graphics/Common/GraphicsCoreMetrics.h>

#include <memory>

#if( IC3_BUILD_STATIC )
#  define IC3_GRAPHICS_GCI_API
#  define IC3_GRAPHICS_GCI_CLASS
#  define IC3_GRAPHICS_GCI_OBJ    extern
#else
#  if( IC3_GRAPHICS_GCI_BUILD )
#    define IC3_GRAPHICS_GCI_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GRAPHICS_GCI_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GRAPHICS_GCI_OBJ    PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_GRAPHICS_GCI_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GRAPHICS_GCI_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GRAPHICS_GCI_OBJ    PCL_ATTR_DLL_IMPORT
#  endif
#endif

#define IC3_GRAPHICS_GCI_API_NO_DISCARD \
	IC3_GRAPHICS_GCI_API CPPX_ATTR_NO_DISCARD

#define Ic3driverApi( access ) access

#include "Prerequisites/CommonDefs.h"
#include "Prerequisites/CommonTypes.h"
#include "Prerequisites/CoreInterfaceDefs.h"
#include "Prerequisites/GPUDataFormats.h"
#include "Prerequisites/VertexAttribFormatUtils.h"

namespace Ic3::Graphics
{

	using cppx::hash_object;

}

namespace Ic3::Graphics::GCI
{

	/// @namespace CxDef
	/// @brief GPUAPI-level constant expressions and utility functions used by those.

	/// @namespace defaults
	/// @brief GPUAPI-level default/pre-defined values/instances that can be used whenever a common case is implemented.

	/// @namespace RCU
	/// @brief Resource Utilities, used to interact with resources and provide additional, common functionalities.

	/// @namespace SMU
	/// @brief State Management Utilities, used to provide helper methods related to GPU state management.

	// Same for all drivers. A top-level interface for querying capabilities and
	// creating some system-level things like a display manager or a swap chain.
	class GPUDriver;

	// OpenGL:
	// GLES: --//--
	// DX11: IDXGIAdapter + ID3D11Device
	// DX12: IDXGIAdapter + ID3D12CommandQueue
	// Vulkan: VkInstance + VkPhysicalDevice
	class GPUDevice;

	// OpenGL: context + extensions, specific version
	// GLES: --//--
	// DX11: ID3D11DeviceContext (immediate or deferred)
	// DX12: ID3D12CommandList
	// Vulkan: VkCommandBuffer (primary or secondary)
	class CommandContext;

	// Same for all drivers in terms of functionality: enumerating adapters and outputs,
	// querying display topology and supported modes. Implemented with different APIs:
	// OpenGL: Ic3::DisplayDriver (Win32 API/X11+XRandR/etc.)
	// GLES: --//--
	// DX11&12: Ic3::DisplayDriverDXGI (native data access)
	// Vulkan: Ic3::DisplayDriverVulkan
	class DisplayManager;

	// OpenGL: window + surface, xxxSwapBuffers()
	// GLES: EGL surface + context, eglSwapBuffers()
	// DX11&12: HWND + IDXGISwapChain, Present()
	// Vulkan: native window + KHR swap chain + KHR Present
	class PresentationLayer;

	Ic3GCIDeclareClassHandle( CommandContext );
	Ic3GCIDeclareClassHandle( CommandSystem );
	Ic3GCIDeclareClassHandle( DisplayManager );
	Ic3GCIDeclareClassHandle( GPUDevice );
	Ic3GCIDeclareClassHandle( GPUDriver );
	Ic3GCIDeclareClassHandle( PresentationLayer );

	using resource_flags_value_t = uint32;

	enum EBlendWriteMaskFlags : uint16;
	enum EGPUDriverConfigFlags : uint32;
	enum ERenderTargetAttachmentFlags : uint32;
	enum ERenderTargetBufferFlags : uint32;
	enum ETextureCubeMapFace : uint32;

	enum class EBlendFactor : uint16;
	enum class EBlendOp : uint16;
	enum class ECompFunc : uint16;
	enum class ECullMode : uint16;
	enum class EDepthWriteMask : uint16;
	enum class EStencilOp : uint16;
	enum class EPrimitiveFillMode : uint16;
	enum class EPrimitiveTopology : uint16;
	enum class ETriangleVerticesOrder : uint16;

	enum class EGPUBufferTarget : enum_default_value_t;
	enum class EIndexDataFormat : base_data_type_value_t;
	enum class EShaderType : uint32;
	enum class ETextureAddressMode : enum_default_value_t;
	enum class ETextureClass : enum_default_value_t;
	enum class ETextureFilter : enum_default_value_t;
	enum class ETextureFormat : texture_format_value_t;
	enum class ETextureMipMode : enum_default_value_t;

	enum class EGPUDriverAPI : uint32
	{
		DirectX = 0xD1,
		Metal = 0xAA,
		OpenGL = 0x77,
		Vulkan = 0xCC,
		Unknown = 0x00
	};

	namespace CxDef
	{

		inline constexpr uint32 makeGPUDriverID( EGPUDriverAPI pDriverAPI, uint32 pAPISubVersion )
		{
			return ( ( static_cast<uint32>( pDriverAPI ) & 0xFF ) << 8 ) | ( pAPISubVersion & 0xFF );
		}

	}

	enum class EGPUDriverID : uint32
	{
		GDIDirectX11      = CxDef::makeGPUDriverID( EGPUDriverAPI::DirectX, 0x11 ),
		GDIDirectX12      = CxDef::makeGPUDriverID( EGPUDriverAPI::DirectX, 0x12 ),
		GDIMetal1         = CxDef::makeGPUDriverID( EGPUDriverAPI::Metal,   0x01 ),
		GDIOpenGLDesktop4 = CxDef::makeGPUDriverID( EGPUDriverAPI::OpenGL,  0xD4 ),
		GDIOpenGLES3      = CxDef::makeGPUDriverID( EGPUDriverAPI::OpenGL,  0xE3 ),
		GDIVulkan10       = CxDef::makeGPUDriverID( EGPUDriverAPI::Vulkan,  0x10 ),
		GDINull           = CxDef::makeGPUDriverID( EGPUDriverAPI::Unknown, 0xFF ),
		GDIUnknown        = CxDef::makeGPUDriverID( EGPUDriverAPI::Unknown, 0x00 )
	};

} // namespace Ic3::Graphics::GCI

namespace Ic3
{

	namespace GFX = Graphics;
	namespace GCI = Graphics::GCI;
	namespace GCM = Graphics::GCM;

}

#endif // __IC3_GRAPHICS_GCI_PREREQUISITES_H__
