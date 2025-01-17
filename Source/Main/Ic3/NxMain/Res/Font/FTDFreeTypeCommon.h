
#pragma once

#ifndef __IC3_NXMAIN_RES_FTD_FREETYPE_FONT_COMMON_H__
#define __IC3_NXMAIN_RES_FTD_FREETYPE_FONT_COMMON_H__

#include "../FontMetrics.h"

namespace Ic3
{

	class FreeTypeFontFace;
	class FreeTypeFontObject;

	using FreeTypeFontFacePtr = std::unique_ptr<FreeTypeFontFace>;
	using FreeTypeFontObjectPtr = std::unique_ptr<FreeTypeFontObject>;

} // namespace Ic3

#endif // __IC3_NXMAIN_RES_FTD_FREETYPE_FONT_COMMON_H__
