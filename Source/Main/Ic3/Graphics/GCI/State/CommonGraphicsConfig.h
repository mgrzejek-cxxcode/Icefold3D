
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_H__
#define __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_H__

#include "CommonGpuStateDefs.h"

namespace Ic3::Graphics::GCI
{

	enum EBlendConfigFlags : uint16
	{
		eBlendConfigFlagEnableAlphaToCoverageBit        = 0x01,
		eBlendConfigFlagEnableMultiRenderTargetBit      = 0x02,
		eBlendConfigFlagEnableMRTIndependentBlendingBit = 0x04,
		eBlendConfigFlagSetFixedBlendConstantsBit       = 0x08,

		eBlendConfigMaskDefault = 0,
		eBlendConfigMaskAll     = 0x07,
	};

	enum EBlendWriteMaskFlags : uint16
	{
		eBlendWriteMaskChannelRed   = 0x1,
		eBlendWriteMaskChannelGreen = 0x2,
		eBlendWriteMaskChannelBlue  = 0x4,
		eBlendWriteMaskChannelAlpha = 0x8,

		eBlendWriteMaskNone = 0,
		eBlendWriteMaskAll  = 0xF,
	};

	enum EDepthStencilConfigFlags : uint16
	{
		eDepthStencilConfigFlagEnableDepthTestBit   = 0x01,
		eDepthStencilConfigFlagEnableStencilTestBit = 0x02,

		eDepthStencilConfigMaskNone = 0,
		eDepthStencilConfigMaskAll  = 0x03,
	};

	enum ERasterizerConfigFlags : uint16
	{
		eRasterizerConfigFlagEnableScissorTestBit = 0x01,

		eRasterizerConfigMaskNone = 0,
		eRasterizerConfigMaskAll  = 0x01,
	};

	enum class EBlendFactor : uint16
	{
		Undefined,
		Zero,
		One,
		Const,
		ConstInv,
		SrcColor,
		SrcAlpha,
		DstColor,
		DstAlpha,
		SrcColorInv,
		SrcAlphaInv,
		DstColorInv,
		DstAlphaInv
	};

	enum class EBlendOp : uint16
	{
		Undefined,
		Add,
		Min,
		Max,
		Subtract,
		SubtractRev
	};

	enum class ECompFunc : uint16
	{
		Undefined,
		Never,
		Always,
		Equal,
		NotEqual,
		Greater,
		GreaterEqual,
		Less,
		LessEqual
	};

	enum class ECullMode : uint16
	{
		Undefined,
		None,
		Back,
		Front
	};

	enum class EDepthWriteMask : uint16
	{
		All = cppx::meta::limits<uint16>::max_value,
		None = 0
	};

	enum class EStencilOp : uint16
	{
		Undefined,
		Zero,
		Keep,
		Replace,
		IncrClamp,
		IncrWrap,
		DecrClamp,
		DecrWrap,
		Invert,
	};

	/// @brief Blend configuration for a single render target attachment (texture/Render buffer).
	/// @see BlendConfig
	struct RTColorAttachmentBlendSettings
	{
		EBlendFactor factorSrcColor;
		EBlendFactor factorSrcAlpha;
		EBlendFactor factorDstColor;
		EBlendFactor factorDstAlpha;
		EBlendOp opColor;
		EBlendOp opAlpha;
		cppx::bitmask<EBlendWriteMaskFlags> writeMask;
	};

	/// @brief A configuration of the depth test for the depth-stencil stage.
	/// Together with the StencilTestSettings struct, it forms DepthStencilConfig used for depth-stencil configuration.
	/// @see StencilTestSettings
	/// @see DepthStencilConfig
	struct DepthTestSettings
	{
		ECompFunc depthCompFunc;
		EDepthWriteMask depthWriteMask;
	};

	/// @brief
	struct StencilFaceOp
	{
		ECompFunc compFunc;
		EStencilOp opFail;
		EStencilOp opPassDepthFail;
		EStencilOp opPassDepthPass;
	};
	
	/// @brief A configuration of the stencil test for the depth-stencil stage.
	/// Together with the DepthTestSettings struct, it forms DepthStencilConfig used for depth-stencil configuration.
	/// @see DepthTestSettings
	/// @see DepthStencilConfig
	struct StencilTestSettings
	{
		StencilFaceOp frontFace;
		StencilFaceOp backFace;
		uint8 readMask;
		uint8 writeMask;
	};

	/// @brief Represents blend configuration for the graphics pipeline. Used to Create BlendStateDescriptor.
	struct BlendConfig
	{
		/// A cppx::bitmask with RT attachments for which the blending is enabled.
		/// Used to Validate the blend configuration against the RT layout.
		cppx::bitmask<ERTAttachmentFlags> attachmentsMask = 0;

		/// Blending flags.
		cppx::bitmask<EBlendConfigFlags> flags = 0;

		/// An array of blend settings for each RT attachment.
		/// If eBlendConfigFlagEnableMRTIndependentBlendingBit is set, each active target uses its corresponding entry.
		/// Otherwise, attachments[0] is used for all targets and rest of the array is ignored.
		/// @see EBlendConfigFlags
		RTColorAttachmentBlendSettings attachments[GCM::cxRTMaxColorAttachmentsNum];

		Math::RGBAColorR32Norm constantColor;
	};

	/// @brief
	struct DepthStencilConfig
	{
		cppx::bitmask<EDepthStencilConfigFlags> commonFlags = 0;
		DepthTestSettings depthTestSettings;
		StencilTestSettings stencilTestSettings;
	};

	/// @brief
	struct RasterizerConfig
	{
		cppx::bitmask<ERasterizerConfigFlags> flags = 0;
		ECullMode cullMode;
		EPrimitiveFillMode primitiveFillMode;
		ETriangleVerticesOrder frontFaceVerticesOrder;
	};

	namespace defaults
	{

		/// @brief A default blend configuration for an RT attachment.
		/// This default config represents default set of options used for blending. The config is as follows:
		/// - constantColor:        {0,0,0,0}
		/// - factorSrcColor:        EBlendFactor::One
		/// - factorSrcAlpha:        EBlendFactor::One
		/// - factorDstColor:        EBlendFactor::Zero
		/// - factorDstAlpha:        EBlendFactor::Zero
		/// - opColor:               EBlendOp::Add
		/// - opAlpha:               EBlendOp::Add
		/// - renderTargetWriteMask: E_BLEND_WRITE_MASK_ALL
		/// @see RTAttachmentBlendConfig
		IC3_GRAPHICS_GCI_OBJ const RTColorAttachmentBlendSettings cvCommonRTColorAttachmentBlendSettingsDefault;

		/// @brief A default configuration for the depth test.
		/// This default config represents default set of options used for depth testing. The config is as follows:
		/// - depthCompFunc:  ECompFunc::Less
		/// - depthWriteMask: EDepthWriteMask::All
		IC3_GRAPHICS_GCI_OBJ const DepthTestSettings cvCommonDepthTestSettingsDefault;

		/// @brief A default configuration for the stencil test.
		/// This default config represents default set of options used for stencil testing. The config is as follows:
		/// - frontFace.compFunc:        ECompFunc::Always
		/// - frontFace.opFail:          EStencilOp::Keep
		/// - frontFace.opPassDepthFail: EStencilOp::Keep
		/// - frontFace.opPassDepthPass: EStencilOp::Keep
		/// - backFace.compFunc:         ECompFunc::Always
		/// - backFace.opFail:           EStencilOp::Keep
		/// - backFace.opPassDepthFail:  EStencilOp::Keep
		/// - backFace.opPassDepthPass:  EStencilOp::Keep
		/// - refValue:                  0
		/// - readMask:                  E_STENCIL_MASK_READ_DEFAULT
		/// - writeMask:                 E_STENCIL_MASK_WRITE_DEFAULT
		IC3_GRAPHICS_GCI_OBJ const StencilTestSettings cvCommonStencilTestSettingsDefault;

		///
		IC3_GRAPHICS_GCI_OBJ const BlendConfig cvPipelineBlendConfigDefault;

		///
		IC3_GRAPHICS_GCI_OBJ const DepthStencilConfig cvPipelineDepthStencilConfigDefault;

		///
		IC3_GRAPHICS_GCI_OBJ const DepthStencilConfig cvPipelineDepthStencilConfigEnableDepthTest;

		///
		IC3_GRAPHICS_GCI_OBJ const RasterizerConfig cvPipelineRasterizerConfigDefault;

	}

	namespace SMU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetBlendActiveAttachmentMask( const BlendConfig & pBlendConfig );

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 GetBlendActiveAttachmentsNum( const BlendConfig & pBlendConfig );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_H__
