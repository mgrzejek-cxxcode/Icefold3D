
#ifndef __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
#define __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__

#include "VertexAttributeKey.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>

namespace Ic3
{

	/**
	 *
	 */
	struct VertexAttributeShaderSemantics : public GCI::IAVertexAttributeSemantics
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
				uint32 pSemanticIndex = 0 )
		: IAVertexAttributeSemantics{
			GCIUtils::GetSemanticNameFromAttributeFlags( pSemanticFlags ),
			cppx::numeric_cast<uint8>( pSemanticIndex ) }
		, semanticFlags( pSemanticFlags )
		{}

		VertexAttributeShaderSemantics(
				cppx::immutable_string pSemanticName,
				uint32 pSemanticIndex = 0,
				cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags = 0 )
		: IAVertexAttributeSemantics{
				pSemanticName,
				cppx::numeric_cast<uint8>( pSemanticIndex ) }
		, semanticFlags( pSemanticFlags ? pSemanticFlags : GCIUtils::GetSemanticFlagsFromAttributeName( semanticName.str_view() ) )
		{}

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

	namespace GCIUtils
	{
		using namespace GCI::CXU;

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
		 *
		 * @param pAttributeKey
		 * @param pSemanticIndex
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD inline VertexAttributeShaderSemantics GetShaderSemanticsForAttributeKey(
				VertexAttributeKey pAttributeKey,
				uint32 pSemanticIndex = 0 )
		{
			return GCIUtils::GetSemanticsFromAttributeFlags( pAttributeKey.uAttributeKeyFlags, pSemanticIndex );
		}

	}

} // namespace Ic3

// namespace std
// {
//
// 	template <>
// 	struct hash<Ic3::VertexAttributeShaderSemantics>
// 	{
// 		size_t operator()( const Ic3::VertexAttributeShaderSemantics & pInput ) const noexcept
// 		{
// 			return hash<std::string>()( pInput.semanticName.str() );
// 		}
// 	};
//
// }

#endif // __IC3_NXMAIN_SHADER_INPUT_SEMANTICS_H__
