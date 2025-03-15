
#ifndef __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
#define __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__

#include "CommonGCIDefs.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>

namespace Ic3
{

	/**
	 * @brief
	 */
	enum EVertexAttributeSemanticFlags : uint16
	{
		eVertexAttributeSemanticFlagNone = 0,

		eVertexAttributeSemanticFlagPositionBit = 0x0001,
		eVertexAttributeSemanticFlagNormalBit = 0x0002,
		eVertexAttributeSemanticFlagTangentBit = 0x0004,
		eVertexAttributeSemanticFlagBiTangentBit = 0x0008,
		eVertexAttributeSemanticMaskGeometryDataAll = 0x000F,

		eVertexAttributeSemanticFlagTexCoord0Bit = 0x0010,
		eVertexAttributeSemanticFlagTexCoord1Bit = 0x0020,
		eVertexAttributeSemanticFlagTexCoord2Bit = 0x0040,
		eVertexAttributeSemanticFlagTexCoord3Bit = 0x0080,
		eVertexAttributeSemanticFlagTexCoord4Bit = 0x0100,
		eVertexAttributeSemanticFlagTexCoord5Bit = 0x0200,
		eVertexAttributeSemanticMaskTexCoordAll = 0x03F0,

		eVertexAttributeSemanticFlagBlendIndicesBit = 0x0400,
		eVertexAttributeSemanticFlagBlendWeightsBit = 0x0800,
		eVertexAttributeSemanticMaskSkinDataAll = 0x0C00,

		eVertexAttributeSemanticFlagFixedColorBit = 0x1000,

		eVertexAttributeSemanticFlagInstanceMatrixBit = 0x2000,
		eVertexAttributeSemanticFlagInstanceUserDataBit = 0x4000,
		eVertexAttributeSemanticMaskInstanceDataAll = 0x6000,

		eVertexAttributeSemanticFlagCustomAttributeBit = 0x8000,

		eVertexAttributeSemanticMaskStandardAttribsAll = 0x7FFF,
		eVertexAttributeSemanticMaskAll = 0xFFFF,

		eVertexAttributeSemanticMaskTexCoordP01 =
		eVertexAttributeSemanticFlagTexCoord0Bit | eVertexAttributeSemanticFlagTexCoord1Bit,

		eVertexAttributeSemanticMaskTexCoordP23 =
		eVertexAttributeSemanticFlagTexCoord2Bit | eVertexAttributeSemanticFlagTexCoord3Bit,
	};

	static_assert( sizeof( EVertexAttributeSemanticFlags ) == sizeof( uint16 ) );

	enum class EVertexAttributeSemanticID : uint32
	{
		Unknown = 0,
		SIDPosition = eVertexAttributeSemanticFlagPositionBit,
		SIDNormal = eVertexAttributeSemanticFlagNormalBit,
		SIDTangent = eVertexAttributeSemanticFlagTangentBit,
		SIDBiTangent = eVertexAttributeSemanticFlagBiTangentBit,
		SIDTexCoord0 = eVertexAttributeSemanticFlagTexCoord0Bit,
		SIDTexCoord1 = eVertexAttributeSemanticFlagTexCoord1Bit,
		SIDTexCoord2 = eVertexAttributeSemanticFlagTexCoord2Bit,
		SIDTexCoord3 = eVertexAttributeSemanticFlagTexCoord3Bit,
		SIDTexCoord4 = eVertexAttributeSemanticFlagTexCoord4Bit,
		SIDTexCoord5 = eVertexAttributeSemanticFlagTexCoord5Bit,
		SIDTexCoordP01 = eVertexAttributeSemanticMaskTexCoordP01,
		SIDTexCoordP23 = eVertexAttributeSemanticMaskTexCoordP23,
		SIDBlendIndices = eVertexAttributeSemanticFlagBlendIndicesBit,
		SIDBlendWeights = eVertexAttributeSemanticFlagBlendWeightsBit,
		SIDFixedColor = eVertexAttributeSemanticFlagFixedColorBit,
		SIDInstanceMatrix = eVertexAttributeSemanticFlagInstanceMatrixBit,
		SIDInstanceUserData = eVertexAttributeSemanticFlagInstanceUserDataBit,
		SIDCustomAttribute = eVertexAttributeSemanticFlagCustomAttributeBit,
	};

	/**
	 *
	 */
	struct IC3_NXMAIN_CLASS VertexAttributeShaderSemantics : public GCI::IAVertexAttributeSemantics
	{
		/**
		 * Bitmask with semantic flags, describing the usage of this attribute. These flags are strictly related to the
		 * semantic name and for standard (system) attributes, one can be fetched from another via corresponding GCIUtils
		 * functions: GetSemanticNameFromAttributeID() and GetSystemSemantic{ID|Flags}ForAttributeName().
		 */
		cppx::bitmask<EVertexAttributeSemanticFlags> semanticFlags = 0;

		VertexAttributeShaderSemantics() = default;

		VertexAttributeShaderSemantics(
				cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags,
				uint32 pSemanticIndex = 0 );

		VertexAttributeShaderSemantics(
				cppx::immutable_string pSemanticName,
				uint32 pSemanticIndex = 0,
				cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags = 0 );

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool operator==( const VertexAttributeShaderSemantics & pRhs ) const noexcept
		{
			return ( semanticName == pRhs.semanticName ) && (semanticFlags == pRhs.semanticFlags );
		}

		CPPX_ATTR_NO_DISCARD bool operator<( const VertexAttributeShaderSemantics & pRhs ) const noexcept
		{
			return ( semanticName < pRhs.semanticName ) || ( ( semanticName == pRhs.semanticName ) && ( semanticFlags < pRhs.semanticFlags ) );
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return semanticName.empty();
		}

		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept
		{
			return !IsEmpty();
		}

		void Clear()
		{
			semanticName.clear();
			semanticIndex = 0;
			semanticFlags = 0;
		}
	};

	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysPosition = "POSITION";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysNormal = "NORMAL";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysTangent = "TANGENT";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysBiTangent = "BITANGENT";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysFixedColor = "COLOR";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysTexCoord0 = "TEXCOORD0";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysTexCoord1 = "TEXCOORD1";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysTexCoord2 = "TEXCOORD2";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysTexCoord3 = "TEXCOORD3";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysTexCoord4 = "TEXCOORD4";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysTexCoord5 = "TEXCOORD5";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysTexCoordP01 = "TEXCOORDP01";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysTexCoordP23 = "TEXCOORDP23";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysBlendIndices = "BLEND_INDICES";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysBlendWeights = "BLEND_WEIGHTS";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysInstanceMatrix = "INSTANCE_MATRIX";
	inline constexpr cppx::string_view kVertexAttributeSemanticNameSysInstanceUserData = "INSTANCE_UDATA";

	namespace VertexFormat
	{

		/**
		 * @brief 
		 * @param pAttributeSemanticID 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD cppx::string_view GetSemanticNameFromAttributeID(
				EVertexAttributeSemanticID pAttributeSemanticID );

		/**
		 * @brief 
		 * @param pAttributeSemanticFlags 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD cppx::string_view GetSemanticNameFromAttributeFlags(
				cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags );

		/**
		 * @brief 
		 * @param pAttributeSemanticName 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD EVertexAttributeSemanticID GetSemanticIDFromAttributeName(
				const cppx::string_view & pAttributeSemanticName );

		/**
		 * @brief 
		 * @param pAttributeSemanticName 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD cppx::bitmask<EVertexAttributeSemanticFlags> GetSemanticFlagsFromAttributeName(
				const cppx::string_view & pAttributeSemanticName );

		/**
		 * 
		 * @param pAttributeSemanticID 
		 * @param pSemanticIndex 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD VertexAttributeShaderSemantics GetSemanticsFromAttributeID(
				EVertexAttributeSemanticID pAttributeSemanticID,
				uint32 pSemanticIndex = 0 );

		/**
		 * 
		 * @param pAttributeSemanticFlags
		 * @param pSemanticIndex 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD VertexAttributeShaderSemantics GetSemanticsFromAttributeFlags(
				cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags,
				uint32 pSemanticIndex = 0 );

		/**
		 * @brief 
		 * @param pSemanticName 
		 * @param pSemanticIndex 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD GCI::IAVertexAttributeSemantics ProcessIndexedAttributeSemantics(
				const cppx::immutable_string & pSemanticName,
				native_uint pSemanticIndex );

		/**
		 * @brief 
		 * @param pInputSemantics 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD GCI::IAVertexAttributeSemantics ProcessIndexedAttributeSemantics(
				const GCI::IAVertexAttributeSemantics & pInputSemantics );

		/**
		 *
		 * @param pSemanticFlags
		 * @return
		 */
		inline constexpr bool IsStandardSystemAttribute( cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags )
		{
			return // Standard attributes, by default are single-slot attributes with unique semantics.
			       ( pSemanticFlags.is_set_any_of( eVertexAttributeSemanticMaskStandardAttribsAll ) && ( pSemanticFlags.count_bits() == 1 ) ) ||
			       // Exception: packed texture coordinates, where a single attribute holds two set of texture coords.
			       ( ( pSemanticFlags & eVertexAttributeSemanticMaskTexCoordAll ).count_bits() == 2 );
		}

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
