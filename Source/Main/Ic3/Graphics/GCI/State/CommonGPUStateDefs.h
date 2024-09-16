
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_STATE_DEFS_H__
#define __IC3_GRAPHICS_GCI_COMMON_STATE_DEFS_H__

#include "../Memory/CommonGPUMemoryDefs.h"
#include "../Resources/CommonGPUResourceDefs.h"
#include <Ic3/Cppx/Hash.h>

namespace Ic3::Graphics::GCI
{

	struct BlendConfig;
	struct DepthStencilConfig;
	struct GraphicsShaderSet;
	struct IAInputLayoutDefinition;
	struct IAVertexStreamDefinition;
	struct RasterizerConfig;
	struct RenderTargetBindingDefinition;
	struct RenderPassConfiguration;
	struct ShaderInputSignature;

	struct ComputePipelineStateObjectCreateInfo;
	struct GraphicsPipelineStateObjectCreateInfo;

	class IAVertexStreamDynamicState;
	class RenderTargetBindingDynamicState;
	class RenderPassConfigurationDynamicState;

	ic3GpaDeclareClassHandle( GPUDeviceChildObject );
	ic3GpaDeclareClassHandle( PipelineStateObject );
	ic3GpaDeclareClassHandle( ComputePipelineStateObject );
	ic3GpaDeclareClassHandle( GraphicsPipelineStateObject );

	ic3GpaDeclareClassHandle( BlendImmutableState );
	ic3GpaDeclareClassHandle( DepthStencilImmutableState );
	ic3GpaDeclareClassHandle( GraphicsShaderLinkageImmutableState );
	ic3GpaDeclareClassHandle( IAInputLayoutImmutableState );
	ic3GpaDeclareClassHandle( IAVertexStreamImmutableState );
	ic3GpaDeclareClassHandle( RasterizerImmutableState );
	ic3GpaDeclareClassHandle( RenderTargetBindingImmutableState );
	ic3GpaDeclareClassHandle( RenderPassConfigurationImmutableState );

	using pipeline_internal_state_hash_t = uint64;
	using render_target_index_t = uint16;
	using shader_input_ref_id_t = uint64;
	using shader_input_index_t = uint32;

	using GraphicsShaderArray = std::array<ShaderHandle, GCM::cxShaderGraphicsStagesNum>;

	///
	constexpr auto cxPipelineInternalStateIDInvalid = 0u;

	///
	constexpr auto cxRRAttachmentMSAALevelInvalid = QLimits<uint32>::sMaxValue;

	///
	constexpr auto cxPipelineImmutableStateTypesNum = 8u;

	namespace CxDef
	{

		/// @brief
		inline constexpr uint32 makeRTAttachmentFlag( native_uint pAttachmentIndex )
		{
			return ( pAttachmentIndex < GCM::cxRTMaxCombinedAttachmentsNum ) ? ( 1 << static_cast<render_target_index_t>( pAttachmentIndex ) ) : 0u;
		}

		/// @brief
		inline constexpr bool isRTAttachmentIndexValid( native_uint pIndex )
		{
			return pIndex < GCM::cxRTMaxCombinedAttachmentsNum;
		}

		/// @brief
		inline constexpr bool isRTColorAttachmentIndexValid( native_uint pIndex )
		{
			return pIndex < GCM::cxRTMaxColorAttachmentsNum;
		}

	}

	enum EPipelineImmutableStateTypeFlags : uint32
	{
		ePipelineImmutableStateTypeFlagBlendBit = 0x0001,
		ePipelineImmutableStateTypeFlagDepthStencilBit = 0x0002,
		ePipelineImmutableStateTypeFlagGraphicsShaderLinkageBit = 0x0008,
		ePipelineImmutableStateTypeFlagIAInputLayoutBit = 0x0010,
		ePipelineImmutableStateTypeFlagIAVertexStreamBit = 0x0020,
		ePipelineImmutableStateTypeFlagRasterizerBit = 0x0004,
		ePipelineImmutableStateTypeFlagRenderTargetBindingBit = 0x0040,
		ePipelineImmutableStateTypeFlagRenderPassBit = 0x0080,
		ePipelineImmutableStateTypeMaskAll = 0x00FF,
	};

	/// @brief
	enum ERTAttachmentIndex : render_target_index_t
	{
		eRTAttachmentIndexColor0,
		eRTAttachmentIndexColor1,
		eRTAttachmentIndexColor2,
		eRTAttachmentIndexColor3,
		eRTAttachmentIndexColor4,
		eRTAttachmentIndexColor5,
		eRTAttachmentIndexColor6,
		eRTAttachmentIndexColor7,
		eRTAttachmentIndexDepthStencil
	};

	/// @brief A set of bit flags representing render target attachments.
	enum ERTAttachmentFlags : uint32
	{
		eRTAttachmentFlagColor0Bit       = CxDef::makeRTAttachmentFlag( eRTAttachmentIndexColor0 ),
		eRTAttachmentFlagColor1Bit       = CxDef::makeRTAttachmentFlag( eRTAttachmentIndexColor1 ),
		eRTAttachmentFlagColor2Bit       = CxDef::makeRTAttachmentFlag( eRTAttachmentIndexColor2 ),
		eRTAttachmentFlagColor3Bit       = CxDef::makeRTAttachmentFlag( eRTAttachmentIndexColor3 ),
		eRTAttachmentFlagColor4Bit       = CxDef::makeRTAttachmentFlag( eRTAttachmentIndexColor4 ),
		eRTAttachmentFlagColor5Bit       = CxDef::makeRTAttachmentFlag( eRTAttachmentIndexColor5 ),
		eRTAttachmentFlagColor6Bit       = CxDef::makeRTAttachmentFlag( eRTAttachmentIndexColor6 ),
		eRTAttachmentFlagColor7Bit       = CxDef::makeRTAttachmentFlag( eRTAttachmentIndexColor7 ),
		eRtAttachmentFlagDepthStencilBit = CxDef::makeRTAttachmentFlag( eRTAttachmentIndexDepthStencil ),

		eRTAttachmentMaskColorAll      = Cppx::makeLSFBitmask<uint32>( GCM::cxRTMaxColorAttachmentsNum ),
		eRTAttachmentMaskAll           = eRTAttachmentMaskColorAll | eRtAttachmentFlagDepthStencilBit,
		eRTAttachmentMaskDefaultC0DS   = eRTAttachmentFlagColor0Bit | eRtAttachmentFlagDepthStencilBit,
		eRTAttachmentMaskDefaultDSOnly = eRTAttachmentFlagColor0Bit | eRtAttachmentFlagDepthStencilBit,
	};

	enum EGraphicsPipelineDynamicStateFlags : uint32
	{
		eGraphicsPipelineDynamicStateFlagBlendConstantColorBit = 0x01,
		eGraphicsPipelineDynamicStateFlagCommonClearConfigBit = 0x02,
		eGraphicsPipelineDynamicStateFlagStencilRefValueBit = 0x04,
	};

	enum class EPrimitiveFillMode : uint16
	{
		Undefined,
		Solid,
		Wireframe
	};

	enum class EPrimitiveTopology : uint16
	{
		Undefined,
		PointList,
		LineList,
		LineListAdj,
		LineStrip,
		LineStripAdj,
		TriangleList,
		TriangleListAdj,
		TriangleStrip,
		TriangleStripAdj,
		TesselationPatch,
	};

	enum class ETriangleVerticesOrder : uint16
	{
		Clockwise,
		CounterClockwise
	};

	struct RenderTargetAttachmentClearConfig
	{
		Math::RGBAColorR32Norm mColorValue;

		float mDepthValue = 1.0f;

		uint8 mStencilValue = 0;
	};

	struct GraphicsPipelineDynamicState
	{
		// Bitmask which describes what part of the state is active and should be used when the rendering is executed.
		TBitmask<EGraphicsPipelineDynamicStateFlags> mActiveStateMask = 0;

		// Constant color used for blending. Overrides color specified as a part of the BlendState within a PSO.
		Math::RGBAColorR32Norm mBlendConstantColor;

		// Global clear values used to clear attachments when a render pass starts. It overrides clear settings
		// for *ALL* attachments for which "Clear" was defined as the "Load" action in a given render pass.
		RenderTargetAttachmentClearConfig mCommonClearConfig;

		// Ref value used for stencil testing. If this is not set and stencil testing is enabled, 0 is used by default.
		uint8 mStencilTestRefValue = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_STATE_DEFS_H__
