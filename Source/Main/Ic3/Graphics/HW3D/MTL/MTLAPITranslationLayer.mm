
#include "MTLApiTranslationLayer.h"
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>
#include <Ic3/Graphics/GCI/State/SamplerCommon.h>
#include <Ic3/Math/Color.h>

namespace Ic3::Graphics::GCI
{

	MTLCompareFunction ATL::translateMTLCompFunc( ECompFunc pCompFunc )
	{
		switch( pCompFunc )
		{
			ic3CaseReturn( ECompFunc::Never        , MTLCompareFunctionNever        );
			ic3CaseReturn( ECompFunc::Always       , MTLCompareFunctionAlways       );
			ic3CaseReturn( ECompFunc::Equal        , MTLCompareFunctionEqual        );
			ic3CaseReturn( ECompFunc::NotEqual     , MTLCompareFunctionNotEqual     );
			ic3CaseReturn( ECompFunc::Greater      , MTLCompareFunctionGreater      );
			ic3CaseReturn( ECompFunc::GreaterEqual , MTLCompareFunctionGreaterEqual );
			ic3CaseReturn( ECompFunc::Less         , MTLCompareFunctionLess         );
			ic3CaseReturn( ECompFunc::LessEqual    , MTLCompareFunctionLessEqual    );

			default:
				break;
		};

		ic3Throw( 0 );
	}

	MTLStencilOperation ATL::translateMTLStencilOp( EStencilOp pStencilOp )
	{
		switch( pStencilOp )
		{
			ic3CaseReturn( EStencilOp::Zero      , MTLStencilOperationZero           );
			ic3CaseReturn( EStencilOp::Keep      , MTLStencilOperationKeep           );
			ic3CaseReturn( EStencilOp::Replace   , MTLStencilOperationReplace        );
			ic3CaseReturn( EStencilOp::IncrClamp , MTLStencilOperationIncrementClamp );
			ic3CaseReturn( EStencilOp::IncrWrap  , MTLStencilOperationIncrementWrap  );
			ic3CaseReturn( EStencilOp::DecrClamp , MTLStencilOperationDecrementClamp );
			ic3CaseReturn( EStencilOp::DecrWrap  , MTLStencilOperationDecrementWrap  );
			ic3CaseReturn( EStencilOp::Invert    , MTLStencilOperationInvert         );

			default:
				break;
		};

		ic3Throw( 0 );
	}

	MTLSamplerAddressMode ATL::translateMTLTextureAddressMode( ETextureAddressMode pAddressMode )
	{
		switch( pAddressMode )
		{
			ic3CaseReturn( ETextureAddressMode::ClampToColor      , MTLSamplerAddressModeClampToBorderColor );
			ic3CaseReturn( ETextureAddressMode::ClampToEdge       , MTLSamplerAddressModeClampToEdge        );
			ic3CaseReturn( ETextureAddressMode::MirrorClampToEdge , MTLSamplerAddressModeMirrorClampToEdge  );
			ic3CaseReturn( ETextureAddressMode::MirrorRepeat      , MTLSamplerAddressModeMirrorRepeat       );
			ic3CaseReturn( ETextureAddressMode::Repeat            , MTLSamplerAddressModeRepeat             );

			default:
				break;
		};

		ic3Throw( 0 );
	}

	MTLSamplerBorderColor ATL::translateMTLTextureBorderPredefinedColor( ETextureBorderPredefinedColor pColor )
	{
		switch( pColor )
		{
			ic3CaseReturn( ETextureBorderPredefinedColor::OpaqueBlack      , MTLSamplerBorderColorOpaqueBlack      );
			ic3CaseReturn( ETextureBorderPredefinedColor::OpaqueWhite      , MTLSamplerBorderColorOpaqueWhite      );
			ic3CaseReturn( ETextureBorderPredefinedColor::TransparentBlack , MTLSamplerBorderColorTransparentBlack );

			default:
				break;
		};

		ic3Throw( 0 );
	}


	MTLSamplerMinMagFilter ATL::translateMTLTextureFilter( ETextureFilter pFilter )
	{
		switch( pFilter )
		{
			ic3CaseReturn( ETextureFilter::Point       , MTLSamplerMinMagFilterNearest );
			ic3CaseReturn( ETextureFilter::Linear      , MTLSamplerMinMagFilterLinear  );
			ic3CaseReturn( ETextureFilter::Anisotropic , MTLSamplerMinMagFilterLinear  );

			default:
				break;
		};

		ic3Throw( 0 );
	}

	MTLSamplerMipFilter ATL::translateMTLTextureMipMode( ETextureMipMode pMipMode )
	{
		switch( pMipMode )
		{
			ic3CaseReturn( ETextureMipMode::Disable , MTLSamplerMipFilterNotMipmapped );
			ic3CaseReturn( ETextureMipMode::Nearest , MTLSamplerMipFilterNearest  );
			ic3CaseReturn( ETextureMipMode::Linear  , MTLSamplerMipFilterLinear  );

			default:
				break;
		};

		ic3Throw( 0 );
	}

	MTLSamplerBorderColor ATL::selectMTLSamplerBorderColor(
			ETextureBorderPredefinedColor pPredefinedColor,
			const Math::RGBAColorR32Norm & pCustomColor )
	{
		auto selectedColor = pPredefinedColor;

		if( selectedColor == ETextureBorderPredefinedColor::Undefined )
		{
			const auto u8Color = pCustomColor.toU8Color();

			if( u8Color == Ic3::Math::colors::CX_COLOR_BLACK_OPAQUE )
			{
				selectedColor = ETextureBorderPredefinedColor::OpaqueBlack;
			}
			else if( u8Color == Ic3::Math::colors::CX_COLOR_BLACK_TRANSPARENT )
			{
				selectedColor = ETextureBorderPredefinedColor::TransparentBlack;
			}
			else if( u8Color == Ic3::Math::colors::CX_COLOR_WHITE_OPAQUE )
			{
				selectedColor = ETextureBorderPredefinedColor::OpaqueWhite;
			}
		}

		if( selectedColor == ETextureBorderPredefinedColor::Undefined )
		{
			// Custom color which is not any of the supported ones.
			selectedColor = ETextureBorderPredefinedColor::OpaqueWhite;
		}

		return translateMTLTextureBorderPredefinedColor( selectedColor );
	}

}
