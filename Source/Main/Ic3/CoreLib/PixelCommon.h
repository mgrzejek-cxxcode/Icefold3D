
#pragma once

#ifndef __IC3_CORELIB_PIXEL_COMMON_H__
#define __IC3_CORELIB_PIXEL_COMMON_H__

#include "Prerequisites.h"

namespace Ic3
{

	namespace CXU
	{

		inline constexpr uint16 declarePixelDataLayout( uint8 pIndex, uint8 pChannelsNum )
		{
			return ( ( ( uint16 )pChannelsNum ) << 8 ) | ( uint16 )pIndex;
		}

	}

	enum class EPixelDataLayout : uint16
	{
		Undefined = 0,
		Alpha     = CXU::declarePixelDataLayout( 0x1, 1 ),
		Depth     = CXU::declarePixelDataLayout( 0x2, 1 ),
		DS        = CXU::declarePixelDataLayout( 0x3, 2 ),
		BGR       = CXU::declarePixelDataLayout( 0x4, 3 ),
		BGRA      = CXU::declarePixelDataLayout( 0x5, 4 ),
		Red       = CXU::declarePixelDataLayout( 0x6, 1 ),
		RG        = CXU::declarePixelDataLayout( 0x7, 2 ),
		RGB       = CXU::declarePixelDataLayout( 0x8, 3 ),
		RGBA      = CXU::declarePixelDataLayout( 0x9, 4 ),
		S3TC      = CXU::declarePixelDataLayout( 0xA, 1 ),
	};

	namespace CXU
	{

		inline constexpr uint16 getPixelDataLayoutChannelsNum( EPixelDataLayout pPixelDataLayout )
		{
			return ( ( uint16 )pPixelDataLayout >> 8 ) & 0xFF;
		}

	}

} // namespace Ic3

#endif // __IC3_CORELIB_PIXEL_COMMON_H__
