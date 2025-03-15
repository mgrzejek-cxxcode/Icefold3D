
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_H__
#define __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_H__

#include "CommonGPUStateDefs.h"

namespace Ic3::Graphics::GCI
{

	inline constexpr uint8 kPipelineConfigDefaultStencilTestRefValue = 0u;

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

	/**
	 * Blend configuration for a single render target attachment (texture/Render buffer).
	 * @see BlendSettings
	 */
	struct RenderTargetColorAttachmentBlendSettings
	{
		EBlendFactor factorSrcColor;
		EBlendFactor factorDstColor;
		EBlendOp opColor;
		EBlendFactor factorSrcAlpha;
		EBlendFactor factorDstAlpha;
		EBlendOp opAlpha;
		cppx::bitmask<EBlendWriteMaskFlags> writeMask;
	};

	/**
	 * A configuration of the depth test for the depth-stencil stage.
	 * Together with the StencilTestSettings struct, it forms DepthStencilSettings used for depth-stencil configuration.
	 * @see StencilTestSettings
	 * @see DepthStencilSettings
	 */
	struct DepthTestSettings
	{
		ECompFunc depthCompFunc;
		EDepthWriteMask depthWriteMask;
	};

	/**
	 *
	 */
	struct StencilFaceOp
	{
		ECompFunc compFunc;
		EStencilOp opFail;
		EStencilOp opPassDepthFail;
		EStencilOp opPassDepthPass;
	};

	/**
	 * A configuration of the stencil test for the depth-stencil stage.
	 * Together with the DepthTestSettings struct, it forms DepthStencilSettings used for depth-stencil configuration.
	 * @see DepthTestSettings
	 * @see DepthStencilSettings
	 */
	struct StencilTestSettings
	{
		StencilFaceOp frontFace;
		StencilFaceOp backFace;
		uint8 readMask;
		uint8 writeMask;
	};

	/**
	 * Represents blend configuration for the graphics pipeline. Used to Create BlendStateDescriptor.
	 */
	struct BlendSettings
	{
		/// A cppx::bitmask with RT attachments for which the blending is enabled.
		/// Used to Validate the blend configuration against the RT layout.
		cppx::bitmask<ERTAttachmentFlags> attachmentsMask = 0;

		/// Blending flags.
		cppx::bitmask<EBlendConfigFlags> flags = 0;

		cxm::rgba_color_r32_norm constantColor;

		/// An array of blend settings for each RT attachment.
		/// If eBlendConfigFlagEnableMRTIndependentBlendingBit is set, each active target uses its corresponding entry.
		/// Otherwise, attachments[0] is used for all targets and rest of the array is ignored.
		/// @see EBlendConfigFlags
		RenderTargetColorAttachmentBlendSettings attachments[GCM::kRTOMaxColorAttachmentsNum];
	};

	/**
	 *
	 */
	struct DepthStencilSettings
	{
		cppx::bitmask<EDepthStencilConfigFlags> commonFlags = 0;
		DepthTestSettings depthTest;
		StencilTestSettings stencilTest;
	};

	/**
	 *
	 */
	struct RasterizerSettings
	{
		cppx::bitmask<ERasterizerConfigFlags> flags = 0;
		ECullMode cullMode;
		EPrimitiveFillMode primitiveFillMode;
		ETriangleVerticesOrder frontFaceVerticesOrder;
	};

	/**
	 *
	 */
	struct MultiSamplingSettings
	{
		uint16 sampleCount = 1;
		uint16 sampleQuality = 0;
	};

	struct BlendStateDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		BlendSettings blendSettings;

		CPPX_ATTR_NO_DISCARD pipeline_config_hash_t GetConfigHash() const noexcept
		{
			return cppx::hash_compute<pipeline_config_hash_t::hash_algo>( blendSettings );
		}
	};

	struct DepthStencilStateDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		DepthStencilSettings depthStencilSettings;

		CPPX_ATTR_NO_DISCARD pipeline_config_hash_t GetConfigHash() const noexcept
		{
			return cppx::hash_compute<pipeline_config_hash_t::hash_algo>( depthStencilSettings );
		}
	};

	struct RasterizerStateDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		RasterizerSettings rasterizerSettings;

		CPPX_ATTR_NO_DISCARD pipeline_config_hash_t GetConfigHash() const noexcept
		{
			return cppx::hash_compute<pipeline_config_hash_t::hash_algo>( rasterizerSettings );
		}
	};


	namespace Utilities
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetBlendActiveAttachmentMask( const BlendSettings & pBlendSettings );

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 GetBlendActiveAttachmentsNum( const BlendSettings & pBlendSettings );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_H__
