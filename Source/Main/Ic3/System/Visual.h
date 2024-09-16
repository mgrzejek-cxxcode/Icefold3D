
#ifndef __IC3_SYSTEM_VISUAL_H__
#define __IC3_SYSTEM_VISUAL_H__

#include "Prerequisites.h"
#include <Ic3/Math/Color.h>

namespace Ic3::System
{

	enum class EColorFormat : uint32
	{
		Unknown,
		B8G8R8,
		B8G8R8A8,
		B8G8R8A8SRGB,
		B8G8R8X8,
		R8G8B8A8,
		R8G8B8A8SRGB,
		R8G8B8X8,
		R10G10B10A2,

		// Sentinel value, used to determine the number of possible values.
		// New color formats should be all added above this line so that the
		// implementation can automatically adjust to the new definitions.
		_Reserved,
	};

	inline constexpr auto CX_ENUM_COLOR_FORMAT_COUNT = static_cast<uint32>( EColorFormat::_Reserved ) - 1;

	enum class EColorSpace : uint32
	{
		Unknown,
		Linear,
		SRGB
	};

	enum class EPixelOrder : uint32
	{
		Unknown,
		BGR,
		RGB
	};

	enum class EDepthStencilFormat : uint16
	{
		Unknown,
		D16,
		D24S8,
		D24X8,
		D32F,
		D32FS8
	};

	enum class EMSAAMode : uint16
	{
		Unknown,
		X1,
		X2,
		X4,
		X8,
		X16,
		Best = static_cast< uint16 >( X16 ),
	};

	/// @brief Flags representing attributes (properties) of visuals.
	///
	/// Note: some flags are mutually exclusive. If more than one is set (e.g. inside visual
	/// configuration for a surface), they are chosen by the following priority rules:
	/// - Buffering: TRIPLE_BUFFER > DOUBLE_BUFFER > SINGLE_BUFFER.
	/// - If StereoDisplay and MonoDisplay are both set, MonoDisplay is used. If none is set, MonoDisplay is used.
	enum EVisualAttribFlags : uint32
	{
		/// Visual supports legacy formats. Used forcreation of temporary GL surfaces.
		eVisualAttribFlagLegacyBit = 0x0001,

		/// Visual supports double buffering.
		eVisualAttribFlagDoubleBufferBit = 0x0100,

		/// Visual does not support double buffering.
		eVisualAttribFlagSingleBufferBit = 0x0200,

		/// Visual supports triple buffering.
		eVisualAttribFlagTripleBufferBit = 0x0400,

		/// Visual is sRGB-capable.
		eVisualAttribFlagSRGBCapableBit = 0x1000,

		/// Visual has the stereoscopic display mode.
		eVisualAttribFlagStereoDisplayBit = 0x0400,

		/// Visual has the classic, monoscopic display mode.
		eVisualAttribFlagMonoDisplayBit = 0x0800,
	};

	struct ColorDesc
	{
		Math::RGBAColorU8 mRGBA;
		uint32 mSize = 0;
		EPixelOrder mPixelOrder = EPixelOrder::Unknown;
		EColorSpace mColorSpace = EColorSpace::Unknown;
	};

	struct DepthStencilDesc
	{
		uint8 mDepthBufferSize;
		uint8 mStencilBufferSize;
	};

	struct MSAADesc
	{
		uint8 mBufferCount;
		uint8 mQuality;
	};

	/// @brief
	struct VisualConfig
	{
		// Struct representation of the color format.
		ColorDesc mColorDesc;

		// Enum ID of the color format.
		EColorFormat mColorFormat;

		// Struct representation of a depth/stencil buffer description.
		DepthStencilDesc mDepthStencilDesc;

		// Enum ID of the depth/stencil buffer format.
		EDepthStencilFormat mDepthStencilFormat;

		// Struct representation of the MSAA mode.
		MSAADesc mMSAADesc;

		// Enum ID of the MSAA mode.
		EMSAAMode mMSAAMode;

		// Additional visual config flags.
		TBitmask<EVisualAttribFlags> mFlags;
	};

	/// @brief Returns a ColorDesc structure representation of a specified EColorFormat.
	IC3_SYSTEM_API const std::string & vsxQueryColorFormatStr( EColorFormat pFormat );

	/// @brief Returns a ColorDesc structure representation of a specified EColorFormat.
	IC3_SYSTEM_API const ColorDesc & vsxGetDescForColorFormat( EColorFormat pFormat );

	/// @brief Returns a DepthStencilDesc structure representation of a specified EDepthStencilFormat.
	IC3_SYSTEM_API const DepthStencilDesc & vsxGetDescForDepthStencilFormat( EDepthStencilFormat pFormat );

	/// @brief Returns an MSAADesc structure representation of a specified EMSAAMode.
	IC3_SYSTEM_API const MSAADesc & vsxGetDescForMSAAMode( EMSAAMode pMode );

	/// @brief Returns a ColorDesc structure representation of a specified EColorFormat.
	IC3_SYSTEM_API const VisualConfig & vsxGetDefaultVisualConfigForSysWindow();

	///
	IC3_SYSTEM_API bool vsxCheckColorFormatCompatibility( EColorFormat pFormat, uint8 pRed, uint8 pGreen, uint8 pBlue, uint8 pAlpha );

} // namespace Ic3::System

#endif // __IC3_SYSTEM_VISUAL_H__
