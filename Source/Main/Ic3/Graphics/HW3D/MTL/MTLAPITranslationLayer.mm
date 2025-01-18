
#include "MTLAPITranslationLayer.h"
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>
#include <Ic3/Graphics/GCI/State/SamplerCommon.h>
#include <Ic3/Math/Color.h>

namespace Ic3::Graphics::GCI
{

	MTLCompareFunction ATL::translateMTLCompFunc( ECompFunc pCompFunc )
	{
		switch( pCompFunc )
		{
			Ic3CaseReturn( ECompFunc::Never        , MTLCompareFunctionNever        );
			Ic3CaseReturn( ECompFunc::Always       , MTLCompareFunctionAlways       );
			Ic3CaseReturn( ECompFunc::Equal        , MTLCompareFunctionEqual        );
			Ic3CaseReturn( ECompFunc::NotEqual     , MTLCompareFunctionNotEqual     );
			Ic3CaseReturn( ECompFunc::Greater      , MTLCompareFunctionGreater      );
			Ic3CaseReturn( ECompFunc::GreaterEqual , MTLCompareFunctionGreaterEqual );
			Ic3CaseReturn( ECompFunc::Less         , MTLCompareFunctionLess         );
			Ic3CaseReturn( ECompFunc::LessEqual    , MTLCompareFunctionLessEqual    );

			default:
				break;
		};

		Ic3Throw( 0 );
	}

	MTLStencilOperation ATL::translateMTLStencilOp( EStencilOp pStencilOp )
	{
		switch( pStencilOp )
		{
			Ic3CaseReturn( EStencilOp::Zero      , MTLStencilOperationZero           );
			Ic3CaseReturn( EStencilOp::Keep      , MTLStencilOperationKeep           );
			Ic3CaseReturn( EStencilOp::Replace   , MTLStencilOperationReplace        );
			Ic3CaseReturn( EStencilOp::IncrClamp , MTLStencilOperationIncrementClamp );
			Ic3CaseReturn( EStencilOp::IncrWrap  , MTLStencilOperationIncrementWrap  );
			Ic3CaseReturn( EStencilOp::DecrClamp , MTLStencilOperationDecrementClamp );
			Ic3CaseReturn( EStencilOp::DecrWrap  , MTLStencilOperationDecrementWrap  );
			Ic3CaseReturn( EStencilOp::Invert    , MTLStencilOperationInvert         );

			default:
				break;
		};

		Ic3Throw( 0 );
	}

	MTLSamplerAddressMode ATL::translateMTLTextureAddressMode( ETextureAddressMode pAddressMode )
	{
		switch( pAddressMode )
		{
			Ic3CaseReturn( ETextureAddressMode::ClampToColor      , MTLSamplerAddressModeClampToBorderColor );
			Ic3CaseReturn( ETextureAddressMode::ClampToEdge       , MTLSamplerAddressModeClampToEdge        );
			Ic3CaseReturn( ETextureAddressMode::MirrorClampToEdge , MTLSamplerAddressModeMirrorClampToEdge  );
			Ic3CaseReturn( ETextureAddressMode::MirrorRepeat      , MTLSamplerAddressModeMirrorRepeat       );
			Ic3CaseReturn( ETextureAddressMode::Repeat            , MTLSamplerAddressModeRepeat             );

			default:
				break;
		};

		Ic3Throw( 0 );
	}

	MTLSamplerBorderColor ATL::translateMTLTextureBorderPredefinedColor( ETextureBorderPredefinedColor pColor )
	{
		switch( pColor )
		{
			Ic3CaseReturn( ETextureBorderPredefinedColor::OpaqueBlack      , MTLSamplerBorderColorOpaqueBlack      );
			Ic3CaseReturn( ETextureBorderPredefinedColor::OpaqueWhite      , MTLSamplerBorderColorOpaqueWhite      );
			Ic3CaseReturn( ETextureBorderPredefinedColor::TransparentBlack , MTLSamplerBorderColorTransparentBlack );

			default:
				break;
		};

		Ic3Throw( 0 );
	}


	MTLSamplerMinMagFilter ATL::translateMTLTextureFilter( ETextureFilter pFilter )
	{
		switch( pFilter )
		{
			Ic3CaseReturn( ETextureFilter::Point       , MTLSamplerMinMagFilterNearest );
			Ic3CaseReturn( ETextureFilter::Linear      , MTLSamplerMinMagFilterLinear  );
			Ic3CaseReturn( ETextureFilter::Anisotropic , MTLSamplerMinMagFilterLinear  );

			default:
				break;
		};

		Ic3Throw( 0 );
	}

	MTLSamplerMipFilter ATL::translateMTLTextureMipMode( ETextureMipMode pMipMode )
	{
		switch( pMipMode )
		{
			Ic3CaseReturn( ETextureMipMode::Disable , MTLSamplerMipFilterNotMipmapped );
			Ic3CaseReturn( ETextureMipMode::Nearest , MTLSamplerMipFilterNearest  );
			Ic3CaseReturn( ETextureMipMode::Linear  , MTLSamplerMipFilterLinear  );

			default:
				break;
		};

		Ic3Throw( 0 );
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
