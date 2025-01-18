
#pragma once

#ifndef __IC3_NXMAIN_RES_FTD_FREETYPE_FONT_OBJECT_H__
#define __IC3_NXMAIN_RES_FTD_FREETYPE_FONT_OBJECT_H__

#include "FTDFreeTypeCommon.h"
#include <cppx/byteArray.h>
#include <unordered_map>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftbitmap.h>
#include <freetype/ftglyph.h>

namespace Ic3
{

	class FreeTypeFontObject
	{
	public:
		FT_Library const mFTLibrary;

		FreeTypeFontObject( std::nullptr_t );
		FreeTypeFontObject( FT_Library pFTLibrary, cppx::dynamic_byte_array pFontData );
		~FreeTypeFontObject();

		explicit operator bool() const
		{
			return _ftLibraryInstance != nullptr;
		}

		FreeTypeFontFace * loadFace( uint32 pFontSize );

		bool SetActiveFace( uint32 pFontSize, const Math::Vec2u32 & pFontResolution );

		bool loadGlyph( char_code_point_t pCodePoint, FontGlyph * pOutGlyph, FontImageData * pOutGlyphImage );

		bool loadKerning( const CharCodePointPair & pCharCPPair, char_kerning_value_t * pOutKerning );

		void ResetActiveGlyphCache();

		FreeTypeFontFace * getFace( uint32 pFontSize ) const;

		uint32 getLineHeight() const;

		static FreeTypeFontObjectPtr createFromMemory( const void * pFontData, size_t pFontDataSize );

	private:
		using FreeTypeFontFaceMap = std::unordered_map<font_size_t, FreeTypeFontFacePtr>;

		// Currently selected face.
		FreeTypeFontFace * _activeFace;

		// Instance of the FT library.
		FT_Library _ftLibraryInstance;

		// Loaded font data (from its font file). Used by FT_New_Memory_Face() when a new face is created.
		cppx::dynamic_byte_array _ftFontData;

		// All faces created by this font, mapped by their size.
		FreeTypeFontFaceMap _ftFontFaceMap;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_RES_FTD_FREETYPE_FONT_OBJECT_H__
