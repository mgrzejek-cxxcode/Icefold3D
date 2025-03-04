
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
	struct RootSignatureDesc;
	struct IAVertexSourceBindingDefinition;

	struct BlendStateDescriptorCreateInfo;
	struct DepthStencilStateDescriptorCreateInfo;
	struct RasterizerStateDescriptorCreateInfo;
	struct GraphicsShaderLinkageDescriptorCreateInfo;
	struct VertexAttributeLayoutDescriptorCreateInfo;
	struct RootSignatureDescriptorCreateInfo;

	struct RenderPassDescriptorCreateInfo;
	struct RenderTargetDescriptorCreateInfo;
	struct VertexSourceBindingDescriptorCreateInfo;

	struct ComputePipelineStateObjectCreateInfo;
	struct GraphicsPipelineStateObjectCreateInfo;

	class PipelineStateDescriptor;
	class PipelineStateDescriptorFactory;

	class BlendStateDescriptor;
	class DepthStencilStateDescriptor;
	class RasterizerStateDescriptor;
	class GraphicsShaderLinkageDescriptor;
	class VertexAttributeLayoutDescriptor;
	class RootSignatureDescriptor;
	class RenderPassDescriptor;
	class RenderTargetDescriptor;
	class VertexSourceBindingDescriptor;

	class RenderPassDescriptorDynamic;
	class RenderTargetDescriptorDynamic;
	class VertexSourceBindingDescriptorDynamic;

	Ic3GCIDeclareClassHandle( GPUDeviceChildObject );
	Ic3GCIDeclareClassHandle( PipelineStateDescriptor );
	Ic3GCIDeclareClassHandle( PipelineStateObject );
	Ic3GCIDeclareClassHandle( ComputePipelineStateObject );
	Ic3GCIDeclareClassHandle( GraphicsPipelineStateObject );

	using ia_vertex_stream_binding_flag_value_t = uint32;

	using pipeline_config_hash_t = cppx::hash_object<cppx::hash_algo::fnv1a64>;
	using pipeline_config_hash_value_t = pipeline_config_hash_t::value_type;
	using pipeline_state_descriptor_id_t = uint16;
	using pipeline_state_object_id_t = uint64;

	using shader_input_ref_id_t = uint64;
	using shader_input_index_t = uint32;

	using GraphicsShaderArray = std::array<ShaderHandle, GCM::kShaderGraphicsStagesNum>;

	inline constexpr auto kPipelineStateDescriptorIDAuto = static_cast<pipeline_state_descriptor_id_t>( 0u );

	inline constexpr auto kPipelineStateDescriptorIDInvalid = cppx::meta::limits<pipeline_state_descriptor_id_t>::max_value;

	inline constexpr auto kPipelineStateObjectIDAuto = static_cast<pipeline_state_object_id_t>( 0u );

	inline constexpr auto kPipelineStateObjectIDInvalid = cppx::meta::limits<pipeline_state_object_id_t>::max_value;

	namespace CXU
	{

		inline constexpr bool IAIsVertexAttributeComponentsNumValid( native_uint pComponentsNum ) noexcept
		{
			return ( pComponentsNum > 0 ) && ( pComponentsNum <= GCM::kIAMaxVertexAttributeComponentsNum );
		}

		inline constexpr bool IAIsVertexAttributeSemanticGroupSizeValid( native_uint pSemanticGroupSize ) noexcept
		{
			return ( pSemanticGroupSize > 0 ) && ( pSemanticGroupSize <= GCM::kIAMaxVertexAttributeSemanticGroupSize );
		}

		/// @brief
		inline constexpr uint32 IAMakeVertexAttributeFlag( native_uint pAttribIndex )
		{
			return ( pAttribIndex < GCM::kIAMaxVertexAttributesNum ) ? ( 1 << pAttribIndex ) : 0u;
		}

		/// @brief Returns
		inline constexpr uint32 IAMakeVertexSourceBufferBindingFlag( native_uint pStreamIndex )
		{
			return ( pStreamIndex < GCM::kIAMaxDataStreamCombinedBuffersNum ) ? ( 1 << pStreamIndex ) : 0u;
		}

		/// @brief
		inline constexpr uint32 IAMakeVertexSourceVertexBufferBindingFlag( native_uint pStreamIndex )
		{
			return ( pStreamIndex < GCM::kIAMaxDataStreamVertexBuffersNum ) ? ( 1 << pStreamIndex ) : 0u;
		}

		/// @brief Returns
		inline constexpr uint32 IAMakeVertexSourceIndexBufferBindingFlag()
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
		inline constexpr bool IAIsVertexAttributeSlotValid( native_uint pAttributeSlot )
		{
			return pAttributeSlot < GCM::kIAMaxVertexAttributesNum;
		}

		/// @brief
		inline constexpr bool IAIsDataStreamVertexBufferSlotValid( native_uint pVertexBufferSlor )
		{
			return pVertexBufferSlor < GCM::kIAMaxDataStreamVertexBuffersNum;
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
	enum EVertexSourceBindingFlags : uint32
	{
		eVertexSourceBindingFlagVertexBuffer0Bit  = CXU::IAMakeVertexSourceVertexBufferBindingFlag( 0 ),
		eVertexSourceBindingFlagVertexBuffer1Bit  = CXU::IAMakeVertexSourceVertexBufferBindingFlag( 1 ),
		eVertexSourceBindingFlagVertexBuffer2Bit  = CXU::IAMakeVertexSourceVertexBufferBindingFlag( 2 ),
		eVertexSourceBindingFlagVertexBuffer3Bit  = CXU::IAMakeVertexSourceVertexBufferBindingFlag( 3 ),
		eVertexSourceBindingFlagVertexBuffer4Bit  = CXU::IAMakeVertexSourceVertexBufferBindingFlag( 4 ),
		eVertexSourceBindingFlagVertexBuffer5Bit  = CXU::IAMakeVertexSourceVertexBufferBindingFlag( 5 ),
		eVertexSourceBindingFlagVertexBuffer6Bit  = CXU::IAMakeVertexSourceVertexBufferBindingFlag( 6 ),
		eVertexSourceBindingFlagVertexBuffer7Bit  = CXU::IAMakeVertexSourceVertexBufferBindingFlag( 7 ),
		eVertexSourceBindingFlagVertexBuffer8Bit  = CXU::IAMakeVertexSourceVertexBufferBindingFlag( 8 ),
		eVertexSourceBindingFlagIndexBufferBit    = CXU::IAMakeVertexSourceIndexBufferBindingFlag(),

		eVertexSourceBindingMaskVertexBufferAllBits = cppx::make_lsfb_bitmask<uint32>( GCM::kIAMaxDataStreamVertexBuffersNum ),

		eVertexSourceBindingMaskAll = eVertexSourceBindingMaskVertexBufferAllBits | eVertexSourceBindingFlagIndexBufferBit
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
		ePipelineStateDescriptorTypeFlagRootSignatureBit = 0x0020,
		ePipelineStateDescriptorTypeFlagRenderPassConfigurationBit = 0x0080,
		ePipelineStateDescriptorTypeMaskAll = 0x00FF,
	};

	/// @brief A set of bit flags representing render target attachments.
	enum ERTAttachmentFlags : uint16
	{
		eRTAttachmentFlagColor0Bit       = CXU::RTOMakeAttachmentFlag( 0 ),
		eRTAttachmentFlagDepthStencilBit = CXU::RTOMakeAttachmentFlag( kRTOAttachmentIndexDepthStencil ),
		eRTAttachmentMaskColorAll        = cppx::make_lsfb_bitmask<uint32>( GCM::kRTOMaxColorAttachmentsNum ),
		eRTAttachmentMaskAll             = eRTAttachmentMaskColorAll | eRTAttachmentFlagDepthStencilBit,
		eRTAttachmentMaskDefaultC0DS     = eRTAttachmentFlagColor0Bit | eRTAttachmentFlagDepthStencilBit,
		eRTAttachmentMaskDefaultDSOnly   = eRTAttachmentFlagDepthStencilBit,
	};

	enum EGraphicsPipelineDynamicConfigFlags : uint16
	{
		eGraphicsPipelineDynamicConfigFlagBlendConstantColorBit = 0x01,
		eGraphicsPipelineDynamicConfigFlagRenderTargetClearConfigBit = 0x02,
		eGraphicsPipelineDynamicConfigFlagStencilRefValueBit = 0x04,
		eGraphicsPipelineDynamicConfigMaskAll = 0x07
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
		ePipelineStateDescriptorCreateMaskDefault = 0
	};

	enum class EPipelineStateDescriptorType : uint16
	{
		Invalid = 0xF,
		Unknown = 0,

		// Descriptors embedded within the PSO, created solely on the HW3D API level.

		///
		DTBlendState,
		DTDepthStencilState,
		DTRasterizerState,
		DTGraphicsShaderLinkage,
		DTVertexAttributeLayout,
		DTRootSignature,

		// Descriptors not included in the PSO, can be either API-native or dynamic.

		///
		DTRenderPass,
		DTRenderTarget,
		DTVertexSourceBinding,

		_Reserved
	};

	inline constexpr auto kPipelineStateDescriptorTypeMaxValue = static_cast<uint16>( EPipelineStateDescriptorType::_Reserved ) - 1;

	static_assert( kPipelineStateDescriptorTypeMaxValue <= cppx::meta::limits<uint8>::max_value );

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
	struct GraphicsPipelineDynamicConfig
	{
		// Bitmask which describes what part of the state is active and should be used when the rendering is executed.
		cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> activeStateMask = 0;

		// Constant color used for blending. Overrides color specified as a part of the BlendState within a PSO.
		Math::RGBAColorR32Norm blendConstantColor;

		// Global clear values used to clear attachments when a render pass starts. It overrides clear settings
		// for *ALL* attachments for which "Clear" was defined as the "Load" action in a given render pass.
		RenderTargetAttachmentClearConfig renderTargetClearConfig;

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

		virtual ~PipelineStateDescriptorCreateInfoBase() = default;

		CPPX_ATTR_NO_DISCARD virtual bool Validate() const noexcept
		{
			return true;
		}
	};

	template <EPipelineStateDescriptorType>
	struct TPipelineStateDescriptorClassProxy;

#define Ic3DefinePipelineStateDescriptorClassMapping( pDescriptorTypeEnum, pDescriptorClass ) \
	template <> struct TPipelineStateDescriptorClassProxy<pDescriptorTypeEnum> { using Type = pDescriptorClass; };

	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTBlendState, BlendStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTDepthStencilState, DepthStencilStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTRasterizerState, RasterizerStateDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTGraphicsShaderLinkage, GraphicsShaderLinkageDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTVertexAttributeLayout, VertexAttributeLayoutDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTRootSignature, RootSignatureDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTRenderPass, RenderPassDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTRenderTarget, RenderTargetDescriptor );
	Ic3DefinePipelineStateDescriptorClassMapping( EPipelineStateDescriptorType::DTVertexSourceBinding, VertexSourceBindingDescriptor );

	template <EPipelineStateDescriptorType tpDescriptorType>
	using PipelineStateDescriptorClass = typename TPipelineStateDescriptorClassProxy<tpDescriptorType>::Type;

	namespace CXU
	{

		inline constexpr uint16 MakePipelineStateDescriptorIDUserComponent( native_uint pUserIDComponent )
		{
			return ( static_cast<uint16>( pUserIDComponent ) & 0x0FFF );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType pDescriptorType, uint16 pUserIDComponent )
		{
			return ( ( static_cast<uint16>( pDescriptorType ) << 12 ) | MakePipelineStateDescriptorIDUserComponent( pUserIDComponent ) );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorIDBlendState( uint16 pUserIDComponent )
		{
			return DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTBlendState, pUserIDComponent );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorIDDepthStencilState( uint16 pUserIDComponent )
		{
			return DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTDepthStencilState, pUserIDComponent );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorIDRasterizerState( uint16 pUserIDComponent )
		{
			return DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTRasterizerState, pUserIDComponent );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorIDGraphicsShaderLinkage( uint16 pUserIDComponent )
		{
			return DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTGraphicsShaderLinkage, pUserIDComponent );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorIDVertexAttributeLayout( uint16 pUserIDComponent )
		{
			return DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTVertexAttributeLayout, pUserIDComponent );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorIDRootSignature( uint16 pUserIDComponent )
		{
			return DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTRootSignature, pUserIDComponent );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorIDRenderPass( uint16 pUserIDComponent )
		{
			return DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTRenderPass, pUserIDComponent );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorIDRenderTarget( uint16 pUserIDComponent )
		{
			return DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTRenderTarget, pUserIDComponent );
		}

		inline constexpr pipeline_state_descriptor_id_t DeclarePipelineStateDescriptorIDVertexSourceBinding( uint16 pUserIDComponent )
		{
			return DeclarePipelineStateDescriptorID( EPipelineStateDescriptorType::DTVertexSourceBinding, pUserIDComponent );
		}

		inline constexpr EPipelineStateDescriptorType GetPipelineStateDescriptorIDTypeComponent( pipeline_state_descriptor_id_t pDescriptorID )
		{
			const auto descriptorTypeComponent = ( ( pDescriptorID & 0xF000 ) >> 12 );

			return ( descriptorTypeComponent <= kPipelineStateDescriptorTypeMaxValue ) ?
			       static_cast<EPipelineStateDescriptorType>( descriptorTypeComponent ) :
				   EPipelineStateDescriptorType::Invalid;
		}

		inline constexpr uint16 GetPipelineStateDescriptorIDUserIDComponent( pipeline_state_descriptor_id_t pDescriptorID )
		{
			return ( pDescriptorID & 0x0FFF );
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
