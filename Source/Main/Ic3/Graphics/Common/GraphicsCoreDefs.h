
#pragma once

#ifndef __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_DEFS_H__
#define __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_DEFS_H__

#include <Ic3/CoreLib/Utility/HFSIdentifier.h>

namespace Ic3::Graphics
{

	using GfxObjectID = HfsIdentifier;
	using GfxObjectName = std::string;

	template <typename TP>
	using TGfxHandle = TSharedHandle<TP>;

	template <typename TPInput>
	inline GfxObjectID GenerateGfxObjectID( const TPInput & pInput )
	{
		return GenerateHfsIdentifier( pInput );
	}

} // namespace Ic3::Graphics

#endif // __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_DEFS_H__
