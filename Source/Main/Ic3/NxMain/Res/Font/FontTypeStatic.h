
#pragma once

#ifndef __IC3_NXMAIN_RES_FONT_TYPE_STATIC_H__
#define __IC3_NXMAIN_RES_FONT_TYPE_STATIC_H__

namespace Ic3
{

	namespace rcdata
	{

		struct FontBMFBaseMetrics
		{
			uint16 lineHeight;
			uint16 base;
			uint32 pagesNum;
			cxm::vec4u16 padding;
			cxm::vec2u16 spacing;
			cxm::vec2u16 scale;
			cxm::rgba_color_u8 pixelFormat;
			std::string faceName;
		};

		struct FontBMFPageDesc
		{
			uint32 id;
			cxm::vec2u32 textureDimensions = { 0, 0 };
			dynamic_memory_buffer imageData;
		};

		struct FontBMFCharDesc
		{
			char_code_point_t codePoint;
			cxm::vec2u32 position;
			cxm::vec2u16 size;
			cxm::vec2u16 offset;
			uint16 xadvance;
			uint16 pageID;
		};

		struct FontBMFRCData
		{
			FontBaseInfo baseInfo;
			FontBMFBaseMetrics bmfMetrics;
			std::vector<FontBMFPageDesc> bmfPages;
			std::vector<FontBMFCharDesc> bmfCharArray;
			std::vector<CharKerningInfo> bmfKerningArray;
		};

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_RES_FONT_TYPE_STATIC_H__
