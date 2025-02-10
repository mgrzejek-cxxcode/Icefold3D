
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_DEFS_H__
#define __IC3_GRAPHICS_GCI_COMMON_DEFS_H__

namespace Ic3::Graphics::GCI
{

	Ic3EnableCustomExceptionSupport();
	Ic3EnableEnumTypeInfoSupport();

#define Ic3GCIDeclareClassHandle( pClassName ) Ic3DeclareClassHandle( pClassName )
#define Ic3GCIDeclareTypedefHandle( pAliasName, pTypeName ) Ic3DeclareTypedefHandle( pAliasName, pTypeName )

	/// A special constant which can be used for object IDs to indicate that ID should be assigned automatically.
	/// In most cases it is safe to assume that object address will be used as the ID (unless stated otherwise).
	inline constexpr GfxObjectID cxGPUObjectIDAuto { cppx::meta::limits<uint64>::max_value };

	/// An invalid object ID. Such IDs may refer to objects which are either uninitialised, marked for deletion,
	/// or do not yet exist in the object management system. This ID also means "not found" in case of queries.
	inline constexpr GfxObjectID cxGPUObjectIDInvalid { cppx::meta::limits<uint64>::max_value - 1 };

	///
	inline constexpr GfxObjectID cxGPUObjectIDEmpty { 0 };

	/**
	 * GCU Namespace (Constant Expressions Utils).
	 * Contains utilities used to define and analyse compile-time expressions (constants, enum values, flags, etc).
	 */
	namespace CXU
	{}

	namespace CxDef
	{

		// constexpr uint32 GPU_SYSTEM_METRIC_IA_MAX_VERTEX_ATTRIBUTES_NUM = 16;
		// constexpr uint32 GPU_SYSTEM_METRIC_IA_MAX_VERTEX_BUFFER_BINDINGS_NUM = 16;
		// constexpr uint32 GPU_SYSTEM_METRIC_RT_MAX_COLOR_ATTACHMENTS_NUM = 8;
		// constexpr uint32 GPU_SYSTEM_METRIC_RT_MAX_COMBINED_ATTACHMENTS_NUM = 9;
		// constexpr uint32 GPU_SYSTEM_METRIC_SHADER_COMBINED_STAGES_NUM = 6;
		// constexpr uint32 GPU_SYSTEM_METRIC_SHADER_GRAPHICS_STAGES_NUM = 5;
		// constexpr uint32 GPU_SYSTEM_METRIC_IS_MAX_CONSTANT_GROUP_SIZE = 32;
		// constexpr uint32 GPU_SYSTEM_METRIC_IS_MAX_DESCRIPTOR_SET_SIZE = 16;
		// constexpr uint32 GPU_SYSTEM_METRIC_IS_MAX_DESCRIPTOR_SETS_NUM = 4;
		// constexpr uint32 GPU_SYSTEM_METRIC_IS_MAX_DWORD_SIZE = 64;
		// constexpr uint32 GPU_SYSTEM_METRIC_RES_MAX_TEXTURE_UNITS_NUM = 32;
		// constexpr uint32 GPU_SYSTEM_METRIC_TEXTURE_MAX_MIP_LEVELS_NUM = 16;

		///
		inline constexpr bool IsGfxObjectIDValid( GfxObjectID pUniqueID ) noexcept
		{
			return ( pUniqueID != cxGPUObjectIDInvalid ) && ( pUniqueID != cxGPUObjectIDEmpty );
		}

	}

	enum EGPUDriverConfigFlags : uint32
	{
		eGPUDriverConfigFlagEnableDebugLayerBit         = 0x0001,
		eGPUDriverConfigFlagEnableShaderDebugInfoBit    = 0x0002 | eGPUDriverConfigFlagEnableDebugLayerBit,
		eGPUDriverConfigFlagDisableMultiThreadAccessBit = 0x0010,
		eGPUDriverConfigFlagForceCompatibilityBit       = 0x0100,
		eGPUDriverConfigFlagForceCoreProfileBit         = 0x0200,
		eGPUDriverConfigFlagUseReferenceDriverBit       = 0x8000,
		eGPUDriverConfigMaskDefault                     = 0,
		
		eGPU_DRIVER_CONFIG_MASK_DEFAULT
	};

	inline constexpr uint32 kShaderStageIndexGraphicsVertex = 0;
	inline constexpr uint32 kShaderStageIndexGraphicsTessHull = 1;
	inline constexpr uint32 kShaderStageIndexGraphicsTessDomain = 2;
	inline constexpr uint32 kShaderStageIndexGraphicsGeometry = 3;
	inline constexpr uint32 kShaderStageIndexGraphicsPixel = 4;
	inline constexpr uint32 kShaderStageIndexCompute = 5;

	/// Base stage index, i.e. index of the first supported stage. Values below this one are not valid stage indexes.
	/// To compute a zero-based index, subtract this from a valid stage index or use CxDef::GetShaderStageAbsoluteIndex().
	inline constexpr uint32 kShaderStageIndexBase = kShaderStageIndexGraphicsVertex;

	/// Index of the last graphics stage. Used to verify if a specified index is a valid graphics stage index.
	inline constexpr uint32 kShaderStageIndexMaxGraphics = kShaderStageIndexGraphicsPixel;

	/// Index of the last stage (in the whole pipeline). Values above this one are not valid stage indexes.
	inline constexpr uint32 kShaderStageIndexMax = kShaderStageIndexCompute;

	/// An explicitly invalid stage index.
	inline constexpr uint32 kShaderStageIndexInvalid = 0xFFFFu;

	namespace CXU
	{

		CPPX_ATTR_NO_DISCARD inline constexpr bool SHIsShaderStageIndexValid( native_uint pStageIndex )
		{
			return ( pStageIndex >= kShaderStageIndexBase ) && ( pStageIndex <= kShaderStageIndexMax );
		}

		CPPX_ATTR_NO_DISCARD inline constexpr bool SHIsGraphicsShaderStageIndexValid( native_uint pGraphicsStageIndex )
		{
			return ( pGraphicsStageIndex >= kShaderStageIndexBase ) && ( pGraphicsStageIndex <= kShaderStageIndexMaxGraphics );
		}

		/// @brief Returns a 32-bit value which is a bit flag matching any (graphics or compute) shader stage specified using its index.
		/// @return One of eShaderStageFlagXXXBit values for a valid stage index or 0 otherwise, returned as uint32.
		CPPX_ATTR_NO_DISCARD inline constexpr uint32 SHMakeShaderStageBit( native_uint pStageIndex )
		{
			return ( pStageIndex < GCM::kShaderCombinedStagesNum ) ? ( 1 << pStageIndex ) : 0u;
		}

		/// @brief Returns a 32-bit value which is a bit flag matching the graphics shader stage specified using its index.
		/// @return One of eShaderStageFlagGraphicsXXXBit values for a valid graphics stage index or 0 otherwise, returned as uint32.
		CPPX_ATTR_NO_DISCARD inline constexpr uint32 SHMakeGraphicsShaderStageBit( native_uint pGraphicsStageIndex )
		{
			return ( pGraphicsStageIndex < GCM::kShaderGraphicsStagesNum ) ? ( 1 << pGraphicsStageIndex ) : 0u;
		}

	}

	/// @brief Bit flags for all supported shader stages.
	///
	/// Each flag is a value with a single bit set, uniquely identifying the corresponding shader stage.
	/// The position of this bit is the EShaderStageIndex value for the particular stage.
	///
	/// @see EShaderStageIndex
	enum EShaderStageFlags : uint32
	{
		// Note: CxDef::declareShaderType() and other enum utility functions assume, that shader-related bits will fit
		// 16-bit value range and have values up to 0xFFFF (no shifting is performed). In case of any alterations,
		// update that part of the functionality accordingly (see shaderCommon.h).

		eShaderStageFlagGraphicsVertexBit   = CXU::SHMakeShaderStageBit( kShaderStageIndexGraphicsVertex ),
		eShaderStageFlagGraphicsHullBit     = CXU::SHMakeShaderStageBit( kShaderStageIndexGraphicsTessHull ),
		eShaderStageFlagGraphicsDomainBit   = CXU::SHMakeShaderStageBit( kShaderStageIndexGraphicsTessDomain ),
		eShaderStageFlagGraphicsGeometryBit = CXU::SHMakeShaderStageBit( kShaderStageIndexGraphicsGeometry ),
		eShaderStageFlagGraphicsPixelBit    = CXU::SHMakeShaderStageBit( kShaderStageIndexGraphicsPixel ),
		eShaderStageFlagComputeBit          = CXU::SHMakeShaderStageBit( kShaderStageIndexCompute ),

		/// Mask with all graphics stages bits set.
		eShaderStageMaskGraphicsAll = cppx::make_lsfb_bitmask<uint32>( GCM::kShaderGraphicsStagesNum ),

		/// Mask with required stages for vertex-based shader setup (vertex shader and pixel shader).
		eShaderStageMaskGraphicsRequiredVB = eShaderStageFlagGraphicsVertexBit | eShaderStageFlagGraphicsPixelBit,

		/// Mask with all bits for all supported stages (for both graphics and compute) set.
		eShaderStageMaskAll = eShaderStageMaskGraphicsAll | eShaderStageFlagComputeBit,

		/// Mask with no valid bits set (integral value is 0).
		eShaderStageMaskNone = 0u,
	};

	/// @brief A set of index values for supported pipeline stages.
	///
	/// Just like EShaderStageIndex defines indexes for shader stages, this is a set of values for the whole pipeline
	/// (including shader stages as well). Not all of the defined stages may refer to the actual, hardware-reflected
	/// subset of the pipeline. Some of the values may be "pseudo-stages", indicating execution of some host-side commands.
	///
	/// @note
	/// Values for shader stages (ePipelineStageIndexShaderXXX) are guaranteed to be the same as their
	/// EShaderStageIndex counterparts (eShaderIndexShaderXXX). That of course means that are 0-based too.
	enum EPipelineStageIndex : uint32
	{
		ePipelineStageIndexShaderGraphicsVertex   = kShaderStageIndexGraphicsVertex,
		ePipelineStageIndexShaderGraphicsHull     = kShaderStageIndexGraphicsTessHull,
		ePipelineStageIndexShaderGraphicsDomain   = kShaderStageIndexGraphicsTessDomain,
		ePipelineStageIndexShaderGraphicsGeometry = kShaderStageIndexGraphicsGeometry,
		ePipelineStageIndexShaderGraphicsPixel    = kShaderStageIndexGraphicsPixel,
		ePipelineStageIndexShaderCompute          = kShaderStageIndexCompute,
		ePipelineStageIndexShaderMax              = kShaderStageIndexMax,

		ePipelineStageIndexGenericHostAccess,
		ePipelineStageIndexGenericResolve,
		ePipelineStageIndexGenericTransfer,
		ePipelineStageIndexIAIndirectArgumentsRead,
		ePipelineStageIndexIAVertexInputDataRead,
		ePipelineStageIndexFragmentTestEarlyDSOpLoad,
		ePipelineStageIndexFragmentTestLateDSOpStore,
		ePipelineStageIndexRTColorOutput,

		ePipelineStageIndexBase = ePipelineStageIndexShaderGraphicsVertex,
		ePipelineStageIndexMax = ePipelineStageIndexRTColorOutput
	};

	/// @brief Bit flags for all supported shader stages.
	///
	/// Each flag is a value with a single bit set, uniquely identifying the corresponding shader stage.
	/// The position of this bit is the EShaderStageIndex value for the particular stage.
	///
	/// @see EShaderStageIndex
	enum EPipelineStageFlags : uint32
	{
		ePipelineStageFlagShaderGraphicsVertexBit      = eShaderStageFlagGraphicsVertexBit,
		ePipelineStageFlagShaderGraphicsHullBit        = eShaderStageFlagGraphicsHullBit,
		ePipelineStageFlagShaderGraphicsDomainBit      = eShaderStageFlagGraphicsDomainBit,
		ePipelineStageFlagShaderGraphicsGeometryBit    = eShaderStageFlagGraphicsGeometryBit,
		ePipelineStageFlagShaderGraphicsPixelBit       = eShaderStageFlagGraphicsPixelBit,
		ePipelineStageFlagShaderComputeBit             = ePipelineStageIndexShaderCompute,

		ePipelineStageFlagGenericHostAccessBit         = 1 << ePipelineStageIndexGenericHostAccess,
		ePipelineStageFlagGenericResolveBit            = 1 << ePipelineStageIndexGenericResolve,
		ePipelineStageFlagGenericTransferBit           = 1 << ePipelineStageIndexGenericTransfer,
		ePipelineStageFlagIAIndirectArgumentsReadBit   = 1 << ePipelineStageIndexIAIndirectArgumentsRead,
		ePipelineStageFlagIAVertexInputDataReadBit     = 1 << ePipelineStageIndexIAVertexInputDataRead,
		ePipelineStageFlagFragmentTestEarlyDSOpLoadBit = 1 << ePipelineStageIndexFragmentTestEarlyDSOpLoad,
		ePipelineStageFlagFragmentTestLateDSOpStoreBit = 1 << ePipelineStageIndexFragmentTestLateDSOpStore,
		ePipelineStageFlagRTColorOutputBit             = 1 << ePipelineStageIndexRTColorOutput,
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_DEFS_H__
