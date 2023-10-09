
#pragma once

#ifndef __IC3_NXMAIN_RES_FONT_COMMON_H__
#define __IC3_NXMAIN_RES_FONT_COMMON_H__

#include "../Prerequisites.h"

namespace Ic3
{

	class Font;

	using char_code_point_t = uint32;
	using font_size_t = uint32;
	using font_type_t = uint64;

	enum EFontBaseType : uint32
	{
		Unknown,
		DynamicFont,
		StaticFont
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_RES_FONT_COMMON_H__
