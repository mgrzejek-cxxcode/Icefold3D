
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_GPU_STATE_DEFS_H__
#define __IC3_GRAPHICS_GCI_COMMON_GPU_STATE_DEFS_H__

#include "../Memory/CommonGPUMemoryDefs.h"
#include "../Resources/CommonGPUResourceDefs.h"
#include <cppx/hash.h>

namespace Ic3::Graphics::GCI
{

	struct BlendSettings;
	struct DepthStencilSettings;
	struct RasterizerSettings;
	struct GraphicsShaderBinding;
	struct IAVertexAttributeLayoutDefinition;
	struct ShaderRootSignatureDesc;
	struct IAVertexStreamBindingDefinition;
	struct RenderPassConfiguration;

	struct BlendStateDescriptorCreateInfo;
	struct DepthStencilStateDescriptorCreateInfo;
	struct RasterizerStateDescriptorCreateInfo;
	struct GraphicsShaderLinkageStateDescriptorCreateInfo;
	struct IAVertexAttributeLayoutStateDescriptorCreateInfo;
	struct ShaderRootSignatureStateDescriptorCreateInfo;
	struct IAVertexStreamBindingStateDescriptorCreateInfo;
	struct RenderPassConfigurationStateDescriptorCreateInfo;

	struct ComputePipelineStateObjectCreateInfo;
	struct GraphicsPipelineStateObjectCreateInfo;

	class PipelineStateDescriptor;
	class PipelineStateDescriptorFactory;
	class GraphicsPipelineStateDescriptorCache;

	Ic3GCIDeclareClassHandle( GPUDeviceChildObject );
	Ic3GCIDeclareClassHandle( PipelineStateObject );
	Ic3GCIDeclareClassHandle( ComputePipelineStateObject );
	Ic3GCIDeclareClassHandle( GraphicsPipelineStateObject );

	Ic3GCIDeclareClassHandle( BlendStateDescriptor );
	Ic3GCIDeclareClassHandle( DepthStencilStateDescriptor );
	Ic3GCIDeclareClassHandle( RasterizerStateDescriptor );
	Ic3GCIDeclareClassHandle( GraphicsShaderLinkageStateDescriptor );
	Ic3GCIDeclareClassHandle( IAVertexAttributeLayoutStateDescriptor );
	Ic3GCIDeclareClassHandle( ShaderRootSignatureStateDescriptor );
	Ic3GCIDeclareClassHandle( IAVertexStreamBindingStateDescriptor );
	Ic3GCIDeclareClassHandle( RenderPassConfigurationStateDescriptor );

	using pipeline_config_hash_t = cppx::hash_object<cppx::hash_algo::fnv1a64>;
	using pipeline_config_hash_value_t = pipeline_config_hash_t::value_type;
	using pipeline_state_descriptor_id_t = uint16;
	using pipeline_state_object_id_t = uint64;

	using shader_input_ref_id_t = uint64;
	using shader_input_index_t = uint32;

	using GraphicsShaderArray = std::array<ShaderHandle, GCM::kShaderGraphicsStagesNum>;

	inline constexpr auto kPipelineStateDescriptorIDAuto = static_cast<pipeline_state_descriptor_id_t>( 0u );

	inline constexpr auto kPipelineStateDescriptorIDInvalid = cppx::meta::limits<pipeline_state_descriptor_id_t>::max_value;

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
		ePipelineStateDescriptorTypeFlagIAVertexAttributeLayoutBit = 0x0008,
		ePipelineStateDescriptorTypeFlagGraphicsShaderLinkageBit = 0x0010,
		ePipelineStateDescriptorTypeFlagShaderRootSignatureBit = 0x0020,
		ePipelineStateDescriptorTypeFlagRenderPassConfigurationBit = 0x0080,
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

	/**
	 * Flags used to control the creation process of pipeline state descriptors. These values are specified inside the
	 * 'flags' field of XxxStateDescriptorCreateInfo (defined in PipelineStateDescriptorCreateInfoBase).
	 * @see PipelineStateDescriptorCreateInfoBase
	 */
	enum EPipelineStateDescriptorCreateFlags : uint32
	{
		/// Enables automatic caching: whether a descriptor is saved to the cache or not depends on the specified ID:
		/// - If a valid ID is specified, the descriptor is saved in the internal PipelineStateManager cache if that ID
		///   has not been taken already. A valid ID is either an ID defined via CXU::DeclarePipelineStateDescriptorID()
		///   or a user-defined ID that meets the compatibility requirements. See documentation of pipeline_state_descriptor_id_t
		///   for details.
		/// - If a non-empty name is specified, the descriptor is saved in the internal PipelineStateManager cache if
		///   that name has not been taken already.
		/// - If both ID and name are specified for a descriptor, both of the above must be true for that descriptor to be cached.
		/// @see pipeline_state_descriptor_id_t
		ePipelineStateDescriptorCreateFlagCacheStoreAuto = 0x0001,

		/// Disables descriptor caching. This flag also disables ID validation, so it allows to
		/// create multiple descriptors sharing the same ID/name.
		ePipelineStateDescriptorCreateFlagCacheStoreDisable = 0x0002,

		/// Forces the cache to store a new descriptor, regardless of the current cache contents and correctness of the ID.
		/// This essentially means that the new descriptor will overwrite the existing one(s) in case either
		/// the ID or the name are already present in the cache. This is a destructive action: existing descriptors
		/// referenced by the specified identifiers will be removed from the cache (thus, if those are the only
		/// active references this essentially means the descriptors will be destroyed).
		///
		/// @example
		/// Consider this pseudo-code (IDs are part of CreateInfo, here shown for clarity):
		/// @code
		/// CreateDescriptor( CreateInfo1, DescriptorID1 ); // unnamed descriptor with ID, cached
		/// CreateDescriptor( CreateInfo2, DescriptorID2, "MY_MAIN_DESC" ); // named descriptor with ID, cached
		/// CreateDescriptor( CreateInfo3, DescriptorID1, "MY_MAIN_DESC" ); // named descriptor, both ID and name reused
		/// @endcode
		/// In this example, a new descriptor is created with both ID and name already taken. If CreateInfo3 specifies
		/// ePipelineStateDescriptorCreateFlagCacheStoreForce, descriptors referenced by DescriptorID1 and DescriptorID2
		/// will be removed from the cache. This behaviour is necessary, because otherwise the descriptor originally
		/// created with DescriptorID1 would not be accessible after its ID is re-assigned.
		///
		/// @note
		/// The described behaviour does not apply to config hash - it is left intact unless a dedicated flag is set
		/// (ePipelineStateDescriptorCreateFlagCachePolicyOverwriteConfig).
		/// @see ePipelineStateDescriptorCreateFlagCachePolicyOverwriteConfig
		ePipelineStateDescriptorCreateFlagCacheStoreForce = 0x0004,

		/// Controls the overwrite policy for cached descriptors.
		/// When a new descriptor of a given type ends up having the same config hash, this hash is mapped
		/// to that new descriptor. This means that the created descriptor will be returned if the cache
		/// is queried for a config-compatible descriptor later on.
		ePipelineStateDescriptorCreateFlagCachePolicyOverwriteConfig = 0x0008,

		/// Default mask used for creation process.
		ePipelineStateDescriptorCreateMaskDefault = ePipelineStateDescriptorCreateFlagCacheStoreAuto
	};

	enum class EPipelineStateDescriptorType : uint16
	{
		Invalid = 0xFF,
		Unknown = 0,

		// Descriptors embedded within the PSO, created solely on the HW3D API level.

		///
		DTBlend,
		DTDepthStencil,
		DTRasterizer,
		DTGraphicsShaderLinkage,
		DTIAVertexAttributeLayout,
		DTShaderRootSignature,

		// Descriptors not included in the PSO, can be either API-native or dynamic.

		///
		DTIAVertexStreamBinding,
		DTRenderPassConfiguration,
	};

	inline constexpr auto kPipelineStateDescriptorTypeMaxValue = static_cast<uint16>( EPipelineStateDescriptorType::DTRenderPassConfiguration );

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

	/**
	 * Part of the graphics pipeline configuration which can be set dynamically, i.e. without specifying at the
	 * descriptor/PSO level. Dynamic state overrides its static counterpart.
	 */
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

	/**
	 *
	 */
	struct PipelineStateDescriptorCreateInfoBase
	{
		/// Descriptor ID used to reference the created descriptor.
		pipeline_state_descriptor_id_t descriptorID = kPipelineStateDescriptorIDAuto;

		/// Flags that control the creation process.
		cppx::bitmask<EPipelineStateDescriptorCreateFlags> flags = ePipelineStateDescriptorCreateMaskDefault;
	};

	template <EPipelineStateDescriptorType>
	struct PipelineStateDescriptorClass;

#define Ic3DefinePipelineStateDescriptorClassMapping( pDescriptorTypeEnum, pDescriptorClass ) \
	template <> struct PipelineStateDescriptorClass<pDescriptorTypeEnum> { using Type = pDescriptorClass; };

	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTBlend, BlendStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTDepthStencil, DepthStencilStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTRasterizer, RasterizerStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTGraphicsShaderLinkage, GraphicsShaderLinkageStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTIAVertexAttributeLayout, IAVertexAttributeLayoutStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTShaderRootSignature, ShaderRootSignatureStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTIAVertexStreamBinding, IAVertexStreamBindingStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTRenderPassConfiguration, RenderPassConfigurationStateDescriptor );

	template <EPipelineStateDescriptorType tpDescriptorType>
	using PipelineStateDecriptorClass = typename PipelineStateDescriptorClass<tpDescriptorType>::Type;

	namespace CXU
	{

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType pDescriptorType, uint16 pUserIDComponent )
		{
			return ( ( static_cast<uint16>( pDescriptorType ) << 12 ) | ( pUserIDComponent & 0x00FFFFFF ) );
		}

		inline constexpr EPipelineStateDescriptorType GetPipelineStateDescriptorIDTypeComponent( pipeline_state_descriptor_id_t pDescriptorID )
		{
			const auto descriptorTypeComponent = ( ( pDescriptorID & 0xFF000000 ) >> 12 );

			return ( descriptorTypeComponent <= kPipelineStateDescriptorTypeMaxValue ) ?
			       static_cast<EPipelineStateDescriptorType>( descriptorTypeComponent ) :
				   EPipelineStateDescriptorType::Invalid;
		}

		inline constexpr uint16 GetPipelineStateDescriptorIDUserIDComponent( pipeline_state_descriptor_id_t pDescriptorID )
		{
			return ( pDescriptorID & 0x00FFFFFF );
		}

		inline constexpr bool IsPipelineStateDescriptorIDValid( pipeline_state_descriptor_id_t pDescriptorID )
		{
			const auto typeComponent = GetPipelineStateDescriptorIDTypeComponent( pDescriptorID );
			const auto userIDComponent = GetPipelineStateDescriptorIDUserIDComponent( pDescriptorID );

			return ( typeComponent != EPipelineStateDescriptorType::Invalid ) && ( userIDComponent > 0 );
		}

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_GPU_STATE_DEFS_H__
