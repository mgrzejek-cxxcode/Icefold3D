
#include "Visual.h"
#include <unordered_map>
#include <cppx/stdHelperAlgo.h>

namespace Ic3::System
{

	// ColorDesc representation of values in EColorFormat enumeration.
	static const ColorDesc sColorDescB8G8R8 {
		Math::RGBAColorU8{ 8,  8,  8,  0 }, 24, EPixelOrder::BGR, EColorSpace::Linear };

	static const ColorDesc sColorDescB8G8R8A8 {
		Math::RGBAColorU8{ 8,  8,  8,  8 }, 32, EPixelOrder::BGR, EColorSpace::Linear };

	static const ColorDesc sColorDescB8G8R8A8SRGB {
		Math::RGBAColorU8{ 8,  8,  8,  8 }, 24, EPixelOrder::BGR, EColorSpace::SRGB };

	static const ColorDesc sColorDescB8G8R8X8 {
		Math::RGBAColorU8{ 8,  8,  8,  0 }, 32, EPixelOrder::BGR, EColorSpace::Linear };

	static const ColorDesc sColorDescR8G8B8A8 {
		Math::RGBAColorU8{ 8,  8,  8,  8 }, 32, EPixelOrder::RGB, EColorSpace::Linear };

	static const ColorDesc sColorDescR8G8B8A8SRGB {
		Math::RGBAColorU8{ 8,  8,  8,  8 }, 32, EPixelOrder::RGB, EColorSpace::SRGB };

	static const ColorDesc sColorDescR8G8B8X8 {
		Math::RGBAColorU8{ 8,  8,  8,  0 }, 32, EPixelOrder::RGB, EColorSpace::Linear };

	static const ColorDesc sColorDescR10G10B10A2 {
		Math::RGBAColorU8{ 10, 10, 10, 2 }, 32, EPixelOrder::RGB, EColorSpace::Linear };

	static const ColorDesc sColorDescUnknown {
			Math::RGBAColorU8{ 0, 0, 0, 0 }, 0, EPixelOrder::Unknown, EColorSpace::Unknown };

	const std::string & VisQueryColorFormatStr( EColorFormat pFormat )
	{
		static const std::unordered_map<EColorFormat, std::string> colorFormatStrMap =
		{
			{ EColorFormat::B8G8R8	     , "B8G8R8"	      },
			{ EColorFormat::B8G8R8A8	 , "B8G8R8A8"	  },
			{ EColorFormat::B8G8R8A8SRGB , "B8G8R8A8SRGB" },
			{ EColorFormat::B8G8R8X8	 , "B8G8R8X8"	  },
			{ EColorFormat::R8G8B8A8	 , "R8G8B8A8"	  },
			{ EColorFormat::R8G8B8A8SRGB , "R8G8B8A8SRGB" },
			{ EColorFormat::R8G8B8X8	 , "R8G8B8X8"	  },
			{ EColorFormat::R10G10B10A2  , "R10G10B10A2"  },
		};
		static const std::string defaultColorFormatStr = "UNKNOWN";
		return cppx::get_map_value_ref_or_default( colorFormatStrMap, pFormat, defaultColorFormatStr );
	}

	const ColorDesc & VisGetDescForColorFormat( EColorFormat pFormat )
	{
		static const std::unordered_map<EColorFormat, ColorDesc> colorDescMap =
		{
			{ EColorFormat::B8G8R8       , sColorDescB8G8R8       },
			{ EColorFormat::B8G8R8A8     , sColorDescB8G8R8A8     },
			{ EColorFormat::B8G8R8A8SRGB , sColorDescB8G8R8A8SRGB },
			{ EColorFormat::B8G8R8X8     , sColorDescB8G8R8X8     },
			{ EColorFormat::R8G8B8A8     , sColorDescR8G8B8A8     },
			{ EColorFormat::R8G8B8A8SRGB , sColorDescR8G8B8A8SRGB },
			{ EColorFormat::R8G8B8X8     , sColorDescR8G8B8X8     },
			{ EColorFormat::R10G10B10A2  , sColorDescR10G10B10A2  },
		};
		return cppx::get_map_value_ref_or_default( colorDescMap, pFormat, sColorDescUnknown );
	}

	// DepthStencilDesc representation of values in EDepthStencilFormat enumeration.
	static const DepthStencilDesc sDepthStencilDescD16     { 16 , 0 };
	static const DepthStencilDesc sDepthStencilDescD24S8   { 24 , 8 };
	static const DepthStencilDesc sDepthStencilDescD24X8   { 24 , 0 };
	static const DepthStencilDesc sDepthStencilDescD32F    { 32 , 0 };
	static const DepthStencilDesc sDepthStencilDescD32FS8  { 32 , 8 };
	static const DepthStencilDesc sDepthStencilDescUnknown { 0  , 0 };

	const DepthStencilDesc & VisGetDescForDepthStencilFormat( EDepthStencilFormat pFormat )
	{
		static const std::unordered_map<EDepthStencilFormat, DepthStencilDesc> depthStencilDescMap =
		{
			{ EDepthStencilFormat::D16    , sDepthStencilDescD16    },
			{ EDepthStencilFormat::D24S8  , sDepthStencilDescD24S8  },
			{ EDepthStencilFormat::D24X8  , sDepthStencilDescD24X8  },
			{ EDepthStencilFormat::D32F   , sDepthStencilDescD32F   },
			{ EDepthStencilFormat::D32FS8 , sDepthStencilDescD32FS8 },
		};
		return cppx::get_map_value_ref_or_default( depthStencilDescMap, pFormat, sDepthStencilDescUnknown );
	}

	// MSAADesc representation of values in EMSAAMode enumeration.
	static const MSAADesc sMSAADescNone { 0, 0  };
	static const MSAADesc sMSAADescX1   { 1, 1  };
	static const MSAADesc sMSAADescX2   { 1, 2  };
	static const MSAADesc sMSAADescX4   { 1, 4  };
	static const MSAADesc sMSAADescX8   { 1, 8  };
	static const MSAADesc sMSAADescX16  { 1, 16 };

	const MSAADesc & VisGetDescForMSAAMode( EMSAAMode pMode )
	{
		static const std::unordered_map<EMSAAMode, MSAADesc> msaaModeMap =
		{
			{ EMSAAMode::X1,  sMSAADescX1  },
			{ EMSAAMode::X2,  sMSAADescX2  },
			{ EMSAAMode::X4,  sMSAADescX4  },
			{ EMSAAMode::X8,  sMSAADescX8  },
			{ EMSAAMode::X16, sMSAADescX16 }
		};
		return cppx::get_map_value_ref_or_default( msaaModeMap, pMode, sMSAADescNone );
	}

	const VisualConfig & VisGetDefaultVisualConfigForSysWindow()
	{
		// Default VisualConfig for creating a system-level window.
		static const VisualConfig sVisualConfigWindowDefault
		{
				sColorDescB8G8R8A8,
				EColorFormat::B8G8R8A8,
				sDepthStencilDescD24S8,
				EDepthStencilFormat::D24S8,
				sMSAADescNone,
				EMSAAMode::Unknown,
				eVisualAttribFlagDoubleBufferBit
		};
		return sVisualConfigWindowDefault;
	}

	bool VisCheckColorFormatCompatibility( EColorFormat pFormat, uint8 pRed, uint8 pGreen, uint8 pBlue, uint8 pAlpha )
	{
		const auto & colorDesc = VisGetDescForColorFormat( pFormat );
		const auto & rgba = colorDesc.rgba;
		// Format matches with a color spec if all channels have the exact same size.
		return ( rgba.u8Red == pRed ) && ( rgba.u8Green == pGreen ) && ( rgba.u8Blue == pBlue ) && ( rgba.u8Alpha == pAlpha );
	}

} // namespace Ic3::System
