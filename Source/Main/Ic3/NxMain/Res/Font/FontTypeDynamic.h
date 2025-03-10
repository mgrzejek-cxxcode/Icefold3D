
#pragma once

#ifndef __IC3_NXMAIN_RES_FONT_TYPE_DYNAMIC_H__
#define __IC3_NXMAIN_RES_FONT_TYPE_DYNAMIC_H__

#include "../Font.h"
#include "../ResourceLoader.h"
#include "FTDFreeTypeCommon.h"
#include <cppx/byteArray.h>
#include <cppx/platform/gds.h>

namespace Ic3
{

	struct DynamicFontDesc : public FontCommonDesc
	{
		uint32 fixedLayersNum = 0;
		uint32 dynamicLayersNum = 0;
	};

	struct DynamicFontCreateInfo
	{
		DynamicFontDesc fontDesc;
		cppx::dynamic_byte_array binaryFontData;
		cxm::vec2u32 fontResolutionHint;
		std::vector<char_code_point_t> preloadGlyphSet;
	};

	class DynamicFont : public Font
	{
	public:
		DynamicFontDesc const mFontDesc;
	};

	class DynamicFontLoader : public ResourceLoader
	{
	public:
		FontHandle createFont( const DynamicFontCreateInfo & pCreateInfo );

	private:
		struct FTInternalFontData;

		FreeTypeFontObject * createFreeTypeFontObject( const DynamicFontCreateInfo & pFontCreateInfo );

		bool loadInternalFontData( FreeTypeFontObject * pFTFontObject, const DynamicFontCreateInfo & pFontCreateInfo, FTInternalFontData & pFontData );

		bool processFontInitData( FreeTypeFontObject * pFTFontObject, const DynamicFontCreateInfo & pFontCreateInfo, FTInternalFontData & pFontData );

		FontTextureCreateInfo setupTextureCreateInfo( const DynamicFontCreateInfo & pFontCreateInfo, FTInternalFontData & pFontData );

	private:
		struct FTFontLoaderPrivateData;
		std::unique_ptr<FTFontLoaderPrivateData> _privateData;
	};

	namespace GDS
	{

		cppx::gds_size_t evalByteSize( const DynamicFontCreateInfo & pValue );
		cppx::gds_size_t serialize( byte * pOutputBuffer, const DynamicFontCreateInfo & pValue );
		cppx::gds_size_t deserialize( const byte * pInputData, DynamicFontCreateInfo & pValue );

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_RES_FONT_TYPE_DYNAMIC_H__
