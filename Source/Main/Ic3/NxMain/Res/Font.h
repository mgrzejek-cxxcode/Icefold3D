
#pragma once

#ifndef __IC3_NXMAIN_RES_FONT_H__
#define __IC3_NXMAIN_RES_FONT_H__

#include "fontMetrics.h"
#include "fontTexture.h"
#include <Ic3/Cppx/sortedArray.h>
#include <unordered_map>

namespace Ic3
{

	struct FontCommonDesc
	{
		EFontBaseType baseType = EFontBaseType::Unknown;
		std::string fontID;
		font_size_t fontSize = 0;
		Math::Vec2u32 textureDimensions = { 0, 0 };
		GCI::ETextureFormat textureFormat = GCI::ETextureFormat::UNKNOWN;
		Bitmask<EFontPropertyFlags> propertyFlags = 0;

	};

	class Font
	{
	public:
		const FontCommonDesc * const mFontCommonDesc;

	protected:
		Font( const FontCommonDesc & pFontCommonDesc );

	public:
		virtual ~Font();

		virtual const FontGlyph * loadCharacterGlyph( char_code_point_t pCharCP );

		virtual char_kerning_value_t loadCharacterKerning( const CharCodePointPair & pCharCPPair );

		const FontGlyph * getCharacterGlyph( char_code_point_t pCharCP ) const;

		char_kerning_value_t getCharacterKerning( const CharCodePointPair & pCharCPPair ) const;

	protected:
		using GlyphMap = std::unordered_map<char_code_point_t, FontGlyph>;
		using KerningInfoArray = SortedArray<CharKerningInfo>;

		void addGlyph( const FontGlyph & pGlyph );

		void addKerningInfo( const CharKerningInfo & pKerningInfo );

		void setGlyphMap( GlyphMap pGlyphMap );

		void setKerningData( std::vector<CharKerningInfo> pKerningData );
		void setKerningInfoArray( KerningInfoArray pKerningInfoArray );

	private:
		GlyphMap _glyphMap;
		KerningInfoArray _kerningInfoArray;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_RES_FONT_H__
