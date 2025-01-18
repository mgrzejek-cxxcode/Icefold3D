
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
			Math::Vec4u16 padding;
			Math::Vec2u16 spacing;
			Math::Vec2u16 scale;
			Math::RGBAColorU8 pixelFormat;
			std::string faceName;
		};

		struct FontBMFPageDesc
		{
			uint32 id;
			Math::Vec2u32 textureDimensions = { 0, 0 };
			dynamic_memory_buffer imageData;
		};

		struct FontBMFCharDesc
		{
			char_code_point_t codePoint;
			Math::Vec2u32 position;
			Math::Vec2u16 size;
			Math::Vec2u16 offset;
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
