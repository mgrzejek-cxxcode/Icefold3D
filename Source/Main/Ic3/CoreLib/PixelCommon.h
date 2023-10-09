
#pragma once

#ifndef __IC3_CORELIB_PIXEL_COMMON_H__
#define __IC3_CORELIB_PIXEL_COMMON_H__

#include "Prerequisites.h"

namespace Ic3
{

	namespace CxDefs
	{

		inline constexpr uint32 declarePixelDataLayout( uint8 pIndex, uint8 pChannelsNum )
		{
			return ( ( ( uint32 )pChannelsNum ) << 8 ) | ( uint32 )pIndex;
		}

	}

	enum class EPixelDataLayout : uint32
	{
		Undefined = 0,
		Alpha     = CxDefs::declarePixelDataLayout( 0x1, 1 ),
		Depth     = CxDefs::declarePixelDataLayout( 0x2, 1 ),
		DS        = CxDefs::declarePixelDataLayout( 0x3, 2 ),
		BGR       = CxDefs::declarePixelDataLayout( 0x4, 3 ),
		BGRA      = CxDefs::declarePixelDataLayout( 0x5, 4 ),
		Red       = CxDefs::declarePixelDataLayout( 0x6, 1 ),
		RG        = CxDefs::declarePixelDataLayout( 0x7, 2 ),
		RGB       = CxDefs::declarePixelDataLayout( 0x8, 3 ),
		RGBA      = CxDefs::declarePixelDataLayout( 0x9, 4 ),
		S3TC      = CxDefs::declarePixelDataLayout( 0xA, 1 ),
	};

	namespace CxDefs
	{

		inline constexpr uint32 getPixelDataLayoutChannelsNum( EPixelDataLayout pPixelDataLayout )
		{
			return ( ( uint32 )pPixelDataLayout >> 8 ) & 0xFF;
		}

	}

} // namespace Ic3

#endif // __IC3_CORELIB_PIXEL_COMMON_H__
