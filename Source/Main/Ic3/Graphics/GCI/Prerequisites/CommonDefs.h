
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_DEFS_H__
#define __IC3_GRAPHICS_GCI_COMMON_DEFS_H__

namespace Ic3::Graphics::GCI
{

	ic3EnableCustomExceptionSupport();
	ic3EnableEnumTypeInfoSupport();

	template <typename TPClass>
	using TGPAHandle = TSharedHandle<TPClass>;

	using UniqueGPUObjectID = HFSIdentifier;
	using UniqueGPUObjectName = std::string;

	template <typename TPInput>
	inline UniqueGPUObjectID generateUniqueGPUObjectID( const TPInput & pInput )
	{
		return generateHFSIdentifier( pInput );
	}

#define ic3GpaDeclareClassHandle( pClassName ) ic3DeclareClassHandle( pClassName )
#define ic3GpaDeclareTypedefHandle( pAliasName, pTypeName ) ic3DeclareTypedefHandle( pAliasName, pTypeName )

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

		/// A special constant which can be used for object IDs to indicate that ID should be assigned automatically.
		/// In most cases it is safe to assume that object address will be used as the ID (unless stated otherwise).
		inline constexpr UniqueGPUObjectID GPU_OBJECT_ID_AUTO { Cppx::QLimits<uint64>::sMaxValue };

		/// An invalid object ID. Such IDs may refer to objects which are either uninitialised, marked for deletion,
		/// or do not yet exist in the object management system. This ID also means "not found" in case of queries.
		inline constexpr UniqueGPUObjectID GPU_OBJECT_ID_INVALID { Cppx::QLimits<uint64>::sMaxValue - 1 };

		///
		inline constexpr UniqueGPUObjectID GPU_OBJECT_ID_EMPTY { 0 };

		///
		inline constexpr bool isUniqueGPUObjectIDValid( UniqueGPUObjectID pUniqueID ) noexcept
		{
			return ( pUniqueID != GPU_OBJECT_ID_INVALID ) && ( pUniqueID != GPU_OBJECT_ID_EMPTY );
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
		eGPUDriverConfigMaskDefault                     = 0
	};

	/// @brief A set of index values for supported shader stages.
	///
	/// @see EPipelineStageIndex
	enum EShaderStageIndex : uint32
	{
		eShaderStageIndexGraphicsVertex,
		eShaderStageIndexGraphicsHull,
		eShaderStageIndexGraphicsDomain,
		eShaderStageIndexGraphicsGeometry,
		eShaderStageIndexGraphicsPixel,
		eShaderStageIndexCompute,

		/// Base stage index, i.e. index of the first supported stage. Values below this one are not valid stage indexes.
		/// To compute a zero-based index, subtract this from a valid stage index or use CxDef::getShaderStageAbsoluteIndex().
		eShaderStageIndexBase = eShaderStageIndexGraphicsVertex,

		/// Index of the last graphics stage. Used to verify if a specified index is a valid graphics stage index.
		eShaderStageIndexMaxGraphics = eShaderStageIndexGraphicsPixel,

		/// Index of the last stage (in the whole pipeline). Values above this one are not valid stage indexes.
		eShaderStageIndexMax = eShaderStageIndexCompute,

		/// An explicitly invalid stage index.
		eShaderStageIndexInvalid = 0xFFFFu
	};

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

		eShaderStageFlagGraphicsVertexBit   = 1u << eShaderStageIndexGraphicsVertex,
		eShaderStageFlagGraphicsHullBit     = 1u << eShaderStageIndexGraphicsHull,
		eShaderStageFlagGraphicsDomainBit   = 1u << eShaderStageIndexGraphicsDomain,
		eShaderStageFlagGraphicsGeometryBit = 1u << eShaderStageIndexGraphicsGeometry,
		eShaderStageFlagGraphicsPixelBit    = 1u << eShaderStageIndexGraphicsPixel,
		eShaderStageFlagComputeBit          = 1u << eShaderStageIndexCompute,

		/// Mask with all graphics stages bits set.
		eShaderStageMaskGraphicsAll = 0x007F,

		/// Mask with required stages for vertex-based shader setup (vertex shader and pixel shader).
		eShaderStageMaskGraphicsVsPs = eShaderStageFlagGraphicsVertexBit | eShaderStageFlagGraphicsPixelBit,

		/// Mask with all bits for all supported stages (for both graphics and compute) set.
		eShaderStageMaskAll = eShaderStageMaskGraphicsAll | eShaderStageFlagComputeBit,

		/// Mask with no valid bits set (integral value is 0).
		eShaderStageMaskNone = 0u,
	};

	static_assert( eShaderStageMaskAll < 0xFFFF );

	/// Constant Expressions
	namespace CxDef
	{

		/// @brief Returns an EShaderStageIndex matching the specified shader stage index value.
		/// @return Corresponding EShaderStageIndex for valid index values or E_SHADER_STAGE_INDEX_INVALID otherwise.
		IC3_ATTR_NO_DISCARD inline constexpr EShaderStageIndex getShaderStageIndexFromValue( native_uint pStageIndex )
		{
			return (pStageIndex <= eShaderStageIndexMax ) ? static_cast<EShaderStageIndex>( pStageIndex ) : eShaderStageIndexInvalid;
		}

		/// @brief Returns a 32-bit value which is a bit flag matching the shader stage specified using its index.
		/// @return One of E_SHADER_STAGE_FLAG_xxx values for a valid stage index or 0 otherwise, returned as uint32.
		IC3_ATTR_NO_DISCARD inline constexpr uint32 makeShaderStageBit( native_uint pStageIndex )
		{
			return ( 1 << static_cast<EShaderStageIndex>( pStageIndex ) ) & eShaderStageMaskAll;
		}

		/// @brief Returns a 32-bit value which is a bit flag matching the graphics shader stage specified using its index.
		/// @return One of E_SHADER_STAGE_FLAG_GRAPHICS_xxx values for a valid stage index or 0 otherwise, returned as uint32.
		IC3_ATTR_NO_DISCARD inline constexpr uint32 makeGraphicsShaderStageBit( native_uint pGraphicsStageIndex )
		{
			return ( 1 << static_cast<EShaderStageIndex>( pGraphicsStageIndex ) ) & eShaderStageMaskGraphicsAll;
		}

		/// @brief
		IC3_ATTR_NO_DISCARD inline constexpr EShaderStageFlags makeGraphicsShaderStageFlag( native_uint pGraphicsStageIndex )
		{
			return static_cast<EShaderStageFlags>( makeGraphicsShaderStageBit( static_cast<EShaderStageIndex>( pGraphicsStageIndex ) ) );
		}

	}

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
		ePipelineStageIndexShaderGraphicsVertex   = eShaderStageIndexGraphicsVertex,
		ePipelineStageIndexShaderGraphicsHull     = eShaderStageIndexGraphicsHull,
		ePipelineStageIndexShaderGraphicsDomain   = eShaderStageIndexGraphicsDomain,
		ePipelineStageIndexShaderGraphicsGeometry = eShaderStageIndexGraphicsGeometry,
		ePipelineStageIndexShaderGraphicsPixel    = eShaderStageIndexGraphicsPixel,
		ePipelineStageIndexShaderCompute          = eShaderStageIndexCompute,
		ePipelineStageIndexShaderMax              = eShaderStageIndexMax,

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
	enum ExPipelineStageFlags : uint32
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
