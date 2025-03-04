
#include "ShaderInputSignature.h"
#include <sstream>

namespace Ic3::Graphics::GCI
{

	namespace GCU
	{

		bool ValidateVertexAttributeLayoutDefinitionWithShaderInputSignature(
				const IAVertexAttributeLayoutDefinition & pLayoutDefinition,
				const ShaderInputSignature & pShaderInputSignature )
		{
			if( pLayoutDefinition.activeAttributesNum != pShaderInputSignature.activeAttributesMask.count_bits() )
			{
				Ic3DebugOutputFmt(
						"[IAVertexAttributeLayout/ShaderInputSignature] Layout definition has activeAttributesNum=%u, in signature: %u (mask).",
						static_cast<uint32>( pLayoutDefinition.activeAttributesNum ),
						static_cast<uint32>( pShaderInputSignature.activeAttributesMask.count_bits() ) );

				return false;
			}

			if( pLayoutDefinition.activeAttributesMask != pShaderInputSignature.activeAttributesMask )
			{
				Ic3DebugOutputFmt(
						"[IAVertexAttributeLayout/ShaderInputSignature] Layout definition has activeAttributesMask=%u, in signature: %u.",
						static_cast<uint32>( pLayoutDefinition.activeAttributesMask ),
						static_cast<uint32>( pShaderInputSignature.activeAttributesMask ) );

				return false;
			}

			for( uint32 iAttributeIndex = 0; CXU::IAIsVertexAttributeSlotValid( iAttributeIndex ); ++iAttributeIndex )
			{
				const auto & attributeDesc = pLayoutDefinition.attributeArray[iAttributeIndex];
				const auto & attributeSignature = pShaderInputSignature.attributeSignatures[iAttributeIndex];

				if( attributeDesc.IsActive() != attributeSignature.IsActive() )
				{
					Ic3DebugOutputFmt(
							"[IAVertexAttributeLayout/ShaderInputSignature] Attribute desc at index %u: %s, in signature: %s.",
							iAttributeIndex,
							attributeDesc.IsActive() ? "active" : "inactive",
							attributeSignature.IsActive() ? "active" : "inactive" );

					return false;
				}

				if( attributeDesc.IsActive() )
				{
					if( attributeDesc.attribInfo.attributeSlot != iAttributeIndex )
					{
						Ic3DebugOutputFmt(
								"[IAVertexAttributeLayout/ShaderInputSignature] Attribute desc at index %u has attributeSlot=%u.",
								iAttributeIndex,
								static_cast<uint32>( iAttributeIndex ) );

						return false;
					}

					if( attributeDesc.attribInfo.dataFormat != attributeSignature.format )
					{
						const auto layoutDescAttrBaseValuesNum = CXU::GetVertexAttribFormatComponentsNum( attributeDesc.attribInfo.dataFormat );
						const auto shaderAttrBaseValuesNum = CXU::GetVertexAttribFormatComponentsNum( attributeSignature.format );
						const auto layoutDescAttrBaseType = CXU::GetVertexAttribFormatBaseDataType( attributeDesc.attribInfo.dataFormat );
						const auto shaderAttrBaseType = CXU::GetVertexAttribFormatBaseDataType( attributeSignature.format );
						const auto layoutDescAttrIsNormalized = CXU::IsVertexAttribFormatNormalized( attributeDesc.attribInfo.dataFormat );

						if( ( layoutDescAttrBaseValuesNum == shaderAttrBaseValuesNum ) &&
						    CXU::IsVertexAttribFormatNormalized( attributeDesc.attribInfo.dataFormat ) &&
						    CXU::IsBaseDataTypeIntegral( layoutDescAttrBaseType ) &&
						    CXU::IsBaseDataTypeFloatingPoint( shaderAttrBaseType ) )
						{
							Ic3DebugOutputFmt(
									"[IAVertexAttributeLayout/ShaderInputSignature] Attribute desc at index %u: has format=%x (integral, normalized), in signature: %x (floating-point).",
									iAttributeIndex,
									static_cast<uint32>( attributeDesc.attribInfo.dataFormat ),
									static_cast<uint32>( attributeSignature.format ) );
						}
						else
						{
							Ic3DebugOutputFmt(
									"[IAVertexAttributeLayout/ShaderInputSignature] Attribute desc at index %u: format=%x, in signature: %x.",
									iAttributeIndex,
									static_cast<uint32>( attributeDesc.attribInfo.dataFormat ),
									static_cast<uint32>( attributeSignature.format ) );

							return false;
						}
					}

					if( attributeDesc.semantics.semanticName != attributeSignature.semanticName )
					{
						Ic3DebugOutputFmt(
								"[IAVertexAttributeLayout/ShaderInputSignature] Attribute desc at index %u: semanticName=%x, in signature: %x.",
								iAttributeIndex,
								attributeDesc.semantics.semanticName.data(),
								attributeSignature.semanticName.data() );

						return false;
					}

					if( attributeDesc.semantics.semanticIndex != attributeSignature.semanticIndex )
					{
						Ic3DebugOutputFmt(
								"[IAVertexAttributeLayout/ShaderInputSignature] Attribute desc at index %u: semanticIndex=%u, in signature: %u.",
								iAttributeIndex,
								attributeDesc.semantics.semanticIndex,
								attributeSignature.semanticIndex );

						return false;
					}
				}
			}

			return true;
		}

		cppx::immutable_string GenerateShaderInputSignatureSerialString(
				const ShaderInputSignature & pShaderInputSignature )
		{
			std::stringstream strStream{};
			strStream << std::hex << pShaderInputSignature.activeAttributesMask;

			std::string serialString{};
			serialString += "ShInSig<0x";
			serialString += strStream.str();

			for( auto iAttribute = 0; iAttribute < pShaderInputSignature.attributeSignatures.size(); ++iAttribute )
			{
			}

			serialString += ">";

			return serialString;
		}


	} // namespace GCU

} // namespace Ic3::Graphics::GCI
