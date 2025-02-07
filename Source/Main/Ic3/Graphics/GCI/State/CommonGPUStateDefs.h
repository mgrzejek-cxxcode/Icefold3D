
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_GPU_STATE_DEFS_H__
#define __IC3_GRAPHICS_GCI_COMMON_GPU_STATE_DEFS_H__

#include "../Memory/CommonGPUMemoryDefs.h"
#include "../Resources/CommonGPUResourceDefs.h"
#include <cppx/hash.h>

namespace Ic3::Graphics::GCI
{

	struct BlendConfig;
	struct DepthStencilConfig;
	struct GraphicsShaderBinding;
	struct IAInputLayoutDefinition;
	struct IAVertexStreamConfiguration;
	struct MultiSamplingConfig;
	struct RasterizerConfig;
	struct RenderPassConfiguration;
	struct ShaderRootSignatureDesc;

	struct ComputePipelineStateObjectCreateInfo;
	struct GraphicsPipelineStateObjectCreateInfo;

	class GraphicsPipelineStateDescriptorFactory;

	Ic3GCIDeclareClassHandle( GPUDeviceChildObject );
	Ic3GCIDeclareClassHandle( PipelineStateObject );
	Ic3GCIDeclareClassHandle( ComputePipelineStateObject );
	Ic3GCIDeclareClassHandle( GraphicsPipelineStateObject );

	Ic3GCIDeclareClassHandle( BlendStateDescriptor );
	Ic3GCIDeclareClassHandle( DepthStencilStateDescriptor );
	Ic3GCIDeclareClassHandle( GraphicsShaderLinkageStateDescriptor );
	Ic3GCIDeclareClassHandle( IAInputLayoutStateDescriptor );
	Ic3GCIDeclareClassHandle( IAVertexStreamStateDescriptor );
	Ic3GCIDeclareClassHandle( MultiSamplingStateDescriptor );
	Ic3GCIDeclareClassHandle( RasterizerStateDescriptor );
	Ic3GCIDeclareClassHandle( RenderPassConfigStateDescriptor );
	Ic3GCIDeclareClassHandle( ShaderRootSignatureStateDescriptor );

	using shader_input_ref_id_t = uint64;
	using shader_input_index_t = uint32;

	using GraphicsShaderArray = std::array<ShaderHandle, GCM::kShaderGraphicsStagesNum>;

	namespace CXU
	{

		/// @brief
		inline constexpr uint32 IAMakeVertexAttributeFlag( native_uint pAttribIndex )
		{
			return ( pAttribIndex < GCM::kIAMaxVertexAttributesNum ) ? ( 1 << pAttribIndex ) : 0u;
		}

		/// @brief Returns
		inline constexpr uint32 IAMakeDataStreamBufferBindingFlag( native_uint pStreamIndex )
		{
			return ( pStreamIndex < GCM::kIAMaxDataStreamCombinedBuffersNum ) ? ( 1 << pStreamIndex ) : 0u;
		}

		/// @brief
		inline constexpr uint32 IAMakeDataStreamVertexBufferBindingFlag( native_uint pStreamIndex )
		{
			return ( pStreamIndex < GCM::kIAMaxDataStreamVertexBuffersNum ) ? ( 1 << pStreamIndex ) : 0u;
		}

		/// @brief Returns
		inline constexpr uint32 IAMakeDataStreamIndexBufferBindingFlag()
		{
			return ( 1 << GCM::kIAMaxDataStreamVertexBuffersNum );
		}

		/// @brief
		inline constexpr uint32 RTOMakeAttachmentFlag( native_uint pAttachmentIndex )
		{
			return ( pAttachmentIndex < GCM::kRTOMaxCombinedAttachmentsNum ) ? ( 1 << pAttachmentIndex ) : 0u;
		}

		/// @brief
		inline constexpr uint32 RTOMakeColorAttachmentFlag( native_uint pColorAttachmentIndex )
		{
			return ( pColorAttachmentIndex < GCM::kRTOMaxColorAttachmentsNum ) ? ( 1 << pColorAttachmentIndex ) : 0u;
		}

		/// @brief
		inline constexpr uint32 RTOMakeDepthStencilAttachmentFlag()
		{
			return ( 1 << GCM::kRTOMaxColorAttachmentsNum );
		}

		/// @brief
		inline constexpr bool IAIsVertexAttributeIndexValid( native_uint pAttribIndex )
		{
			return pAttribIndex < GCM::kIAMaxVertexAttributesNum;
		}

		/// @brief
		inline constexpr bool IAIsDataStreamVertexBufferIndexValid( native_uint pBufferIndex )
		{
			return pBufferIndex < GCM::kIAMaxDataStreamVertexBuffersNum;
		}

		/// @brief
		inline constexpr bool RTOIsAttachmentIndexValid( native_uint pAttachmentIndex )
		{
			return pAttachmentIndex < GCM::kRTOMaxCombinedAttachmentsNum;
		}

		/// @brief
		inline constexpr bool RTOIsColorAttachmentIndexValid( native_uint pColorAttachmentIndex )
		{
			return pColorAttachmentIndex < GCM::kRTOMaxColorAttachmentsNum;
		}

	}

	///
	inline constexpr auto kIAVertexStreamSlotVertexBufferMax = static_cast<native_uint>( GCM::kIAMaxDataStreamVertexBuffersNum - 1 );

	///
	inline constexpr auto kIAVertexStreamSlotIndexBuffer = static_cast<native_uint>( GCM::kIAMaxDataStreamVertexBuffersNum );

	///
	inline constexpr auto kRTOAttachmentIndexDepthStencil = static_cast<native_uint>( GCM::kRTOMaxColorAttachmentsNum );

	enum class EPipelineType : uint32
	{
		Compute,
		Graphics
	};

	/// @brief
	enum EIAVertexStreamBindingFlags : uint32
	{
		eIAVertexStreamBindingFlagVertexBuffer0Bit  = CXU::IAMakeDataStreamVertexBufferBindingFlag( 0 ),
		eIAVertexStreamBindingFlagVertexBuffer1Bit  = CXU::IAMakeDataStreamVertexBufferBindingFlag( 1 ),
		eIAVertexStreamBindingFlagVertexBuffer2Bit  = CXU::IAMakeDataStreamVertexBufferBindingFlag( 2 ),
		eIAVertexStreamBindingFlagVertexBuffer3Bit  = CXU::IAMakeDataStreamVertexBufferBindingFlag( 3 ),
		eIAVertexStreamBindingFlagVertexBuffer4Bit  = CXU::IAMakeDataStreamVertexBufferBindingFlag( 4 ),
		eIAVertexStreamBindingFlagVertexBuffer5Bit  = CXU::IAMakeDataStreamVertexBufferBindingFlag( 5 ),
		eIAVertexStreamBindingFlagVertexBuffer6Bit  = CXU::IAMakeDataStreamVertexBufferBindingFlag( 6 ),
		eIAVertexStreamBindingFlagVertexBuffer7Bit  = CXU::IAMakeDataStreamVertexBufferBindingFlag( 7 ),
		eIAVertexStreamBindingFlagVertexBuffer8Bit  = CXU::IAMakeDataStreamVertexBufferBindingFlag( 8 ),
		eIAVertexStreamBindingFlagIndexBufferBit    = CXU::IAMakeDataStreamIndexBufferBindingFlag(),

		eIAVertexStreamBindingMaskVertexBufferAllBits = cppx::make_lsfb_bitmask<uint32>( GCM::kIAMaxDataStreamVertexBuffersNum ),

		eIAVertexStreamBindingMaskAll = eIAVertexStreamBindingMaskVertexBufferAllBits | eIAVertexStreamBindingFlagIndexBufferBit
	};

	/// @brief
	enum EIAVertexAttributeFlags : uint16
	{
		eIAVertexAttributeFlagAttr0Bit  = CXU::IAMakeVertexAttributeFlag( 0 ),
		eIAVertexAttributeFlagAttr1Bit  = CXU::IAMakeVertexAttributeFlag( 1 ),
		eIAVertexAttributeFlagAttr2Bit  = CXU::IAMakeVertexAttributeFlag( 2 ),
		eIAVertexAttributeFlagAttr3Bit  = CXU::IAMakeVertexAttributeFlag( 3 ),
		eIAVertexAttributeFlagAttr4Bit  = CXU::IAMakeVertexAttributeFlag( 4 ),
		eIAVertexAttributeFlagAttr5Bit  = CXU::IAMakeVertexAttributeFlag( 5 ),
		eIAVertexAttributeFlagAttr6Bit  = CXU::IAMakeVertexAttributeFlag( 6 ),
		eIAVertexAttributeFlagAttr7Bit  = CXU::IAMakeVertexAttributeFlag( 7 ),
		eIAVertexAttributeFlagAttr8Bit  = CXU::IAMakeVertexAttributeFlag( 8 ),
		eIAVertexAttributeMaskAll       = cppx::make_lsfb_bitmask<uint32>( GCM::kIAMaxVertexAttributesNum ),
	};

	enum EPipelineStateDescriptorTypeFlags : uint32
	{
		ePipelineStateDescriptorTypeFlagBlendBit = 0x0001,
		ePipelineStateDescriptorTypeFlagDepthStencilBit = 0x0002,
		ePipelineStateDescriptorTypeFlagRasterizerBit = 0x0004,
		ePipelineStateDescriptorTypeFlagIAInputLayoutBit = 0x0008,
		ePipelineStateDescriptorTypeFlagGraphicsShaderLinkageBit = 0x0010,
		ePipelineStateDescriptorTypeFlagShaderRootSignatureBit = 0x0020,
		ePipelineStateDescriptorTypeFlagMultiSamplingBit = 0x0040,
		ePipelineStateDescriptorTypeFlagRenderTargetLayoutBit = 0x0080,
		ePipelineStateDescriptorTypeMaskAll = 0x00FF,
	};

	/// @brief A set of bit flags representing render target attachments.
	enum ERTAttachmentFlags : uint32
	{
		eRTAttachmentFlagColor0Bit       = CXU::RTOMakeAttachmentFlag( 0 ),
		eRTAttachmentFlagDepthStencilBit = CXU::RTOMakeAttachmentFlag( kRTOAttachmentIndexDepthStencil ),
		eRTAttachmentMaskColorAll        = cppx::make_lsfb_bitmask<uint32>( GCM::kRTOMaxColorAttachmentsNum ),
		eRTAttachmentMaskAll             = eRTAttachmentMaskColorAll | eRTAttachmentFlagDepthStencilBit,
		eRTAttachmentMaskDefaultC0DS     = eRTAttachmentFlagColor0Bit | eRTAttachmentFlagDepthStencilBit,
		eRTAttachmentMaskDefaultDSOnly   = eRTAttachmentFlagDepthStencilBit,
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
		Math::RGBAColorR32Norm colorValue;

		float depthValue = 1.0f;

		uint8 stencilValue = 0;
	};

	struct GraphicsPipelineDynamicState
	{
		// Bitmask which describes what part of the state is active and should be used when the rendering is executed.
		cppx::bitmask<EGraphicsPipelineDynamicStateFlags> activeStateMask = 0;

		// Constant color used for blending. Overrides color specified as a part of the BlendState within a PSO.
		Math::RGBAColorR32Norm blendConstantColor;

		// Global clear values used to clear attachments when a render pass starts. It overrides clear settings
		// for *ALL* attachments for which "Clear" was defined as the "Load" action in a given render pass.
		RenderTargetAttachmentClearConfig commonClearConfig;

		// Ref value used for stencil testing. If this is not set and stencil testing is enabled, 0 is used by default.
		uint8 stencilTestRefValue = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_GPU_STATE_DEFS_H__
