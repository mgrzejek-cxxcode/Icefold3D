
#pragma once

#ifndef __IC3_CORELIB_PIXEL_COMMON_H__
#define __IC3_CORELIB_PIXEL_COMMON_H__

#include "Prerequisites.h"

namespace Ic3
{

	namespace CXU
	{

		inline constexpr uint16 DeclarePixelDataLayout( uint8 pIndex, uint8 pChannelsNum )
		{
			return ( ( ( uint16 )pChannelsNum ) << 8 ) | ( uint16 )pIndex;
		}

	}

	enum class EPixelDataLayout : uint16
	{
		Undefined = 0,
		Alpha     = CXU::DeclarePixelDataLayout( 0x1, 1 ),
		Depth     = CXU::DeclarePixelDataLayout( 0x2, 1 ),
		DS        = CXU::DeclarePixelDataLayout( 0x3, 2 ),
		BGR       = CXU::DeclarePixelDataLayout( 0x4, 3 ),
		BGRA      = CXU::DeclarePixelDataLayout( 0x5, 4 ),
		Red       = CXU::DeclarePixelDataLayout( 0x6, 1 ),
		RG        = CXU::DeclarePixelDataLayout( 0x7, 2 ),
		RGB       = CXU::DeclarePixelDataLayout( 0x8, 3 ),
		RGBA      = CXU::DeclarePixelDataLayout( 0x9, 4 ),
		S3TC      = CXU::DeclarePixelDataLayout( 0xA, 1 ),
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
