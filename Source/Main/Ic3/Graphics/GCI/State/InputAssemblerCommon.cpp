
#include "InputAssemblerCommon.h"
#include "ShaderInputSignature.h"
#include "../Resources/Shader.h"

namespace Ic3::Graphics::GCI
{

	bool VertexAttributeLayoutDescriptorCreateInfo::Validate() const noexcept
	{
		if( PipelineStateDescriptorCreateInfoBase::Validate() )
		{
			if( auto * shaderInputSignature = vertexShaderWithBinary->GetShaderInputSignature() )
			{
				return GCU::ValidateVertexAttributeLayoutDefinitionWithShaderInputSignature( layoutDefinition, *shaderInputSignature );
			}
		}

		return true;
	}

	bool VertexSourceBindingDescriptorCreateInfo::Validate() const noexcept
	{
		return PipelineStateDescriptorCreateInfoBase::Validate();
	}

	namespace GCU
	{

		cppx::bitmask<EIAVertexAttributeFlags> IAGetActiveVertexAttributesMask(
				const IAVertexAttributeDescArray & pVertexAttributes ) noexcept
		{
			cppx::bitmask<EIAVertexAttributeFlags> activeAttributesMask = 0;
			for( uint32 attributeIndex = 0; attributeIndex < GCM::kIAMaxVertexAttributesNum; ++attributeIndex )
			{
				if( pVertexAttributes[attributeIndex].IsActive() )
				{
					activeAttributesMask.set( CXU::IAMakeVertexAttributeFlag( attributeIndex ) );
				}
			}
			return activeAttributesMask;
		}

		uint32 IAGetActiveVertexAttributesNum( const IAVertexAttributeDescArray & pVertexAttributes ) noexcept
		{
			const auto activeAttributesNum = std::count_if(
				pVertexAttributes.begin(),
				pVertexAttributes.end(),
				[]( const auto & pAttributeInfo ) -> bool {
					return pAttributeInfo.IsActive();
				});
			return static_cast<uint32>( activeAttributesNum );
		}

		uint32 IAGetActiveVertexBuffersNum( const IAVertexBufferReferenceArray & pVertexBufferReferences ) noexcept
		{
			const auto activeVertexBuffersNum = std::count_if(
				pVertexBufferReferences.begin(),
				pVertexBufferReferences.end(),
				[]( const auto & pBufferReference ) -> bool {
					return !pBufferReference.IsEmpty();
				});
			return static_cast<uint32>( activeVertexBuffersNum );
		}

		VertexSourceBindingIndexList IAGenerateActiveVertexBuffersIndexList(
				const IAVertexBufferReferenceArray & pVertexBufferReferences ) noexcept
		{
			const auto activeVertexBuffersNum = IAGetActiveVertexBuffersNum( pVertexBufferReferences );

			VertexSourceBindingIndexList vertexBufferBindingList{};

			if( activeVertexBuffersNum > 0 )
			{
				vertexBufferBindingList.reserve( activeVertexBuffersNum );

				for( input_assembler_index_t streamIndex = 0; streamIndex < GCM::kIAMaxDataStreamVertexBuffersNum; ++streamIndex )
				{
					const auto & currentBufferReference = pVertexBufferReferences[streamIndex];

					if( !currentBufferReference.IsEmpty() )
					{
						vertexBufferBindingList.push_back( streamIndex );
					}
				}
			}

			return vertexBufferBindingList;
		}

		VertexSourceBindingRageList IAGenerateActiveVertexBuffersRanges(
				const IAVertexBufferReferenceArray & pVertexBufferReferences ) noexcept
		{
			const auto activeVertexBuffersNum = IAGetActiveVertexBuffersNum( pVertexBufferReferences );

			VertexSourceBindingRageList vertexBufferActiveRanges{};

			if( activeVertexBuffersNum > 0 )
			{
				// Single range is described by two uint16 values which is 4 bytes. More than 90% use cases fall
				// into 1-3 ranges per single binding state. We pre-allocate space for 4 ranges (which is 16 bytes)
				// to prevent from multiple re-allocations in typical cases (and few lost bytes per binding is negligible).
				constexpr auto optimalActiveRangesNumPreAllocSize = 4u;

				vertexBufferActiveRanges.reserve( optimalActiveRangesNumPreAllocSize );

				IAVertexStreamArrayRange currentRange{};
				currentRange.firstIndex = kIAVertexStreamSlotUndefined;
				currentRange.length = 0;

				for( input_assembler_index_t streamIndex = 0; streamIndex < GCM::kIAMaxDataStreamVertexBuffersNum; ++streamIndex )
				{
					const auto & currentBufferReference = pVertexBufferReferences[streamIndex];

					if( !currentBufferReference.IsEmpty() )
					{
						// If the range is not valid, "open" it.
						// Set the current stream as the first stream in the range and range size to 0.
						if( currentRange.firstIndex == kIAVertexStreamSlotUndefined )
						{
							currentRange.firstIndex = streamIndex;
							currentRange.length = 0;
						}

						// Increase the length of a current range.
						++currentRange.length;
					}

					if( currentBufferReference.IsEmpty() || ( streamIndex + 1 == GCM::kIAMaxDataStreamVertexBuffersNum ) )
					{
						// If the range is not empty, add it to the list of active ranges.
						if( currentRange.length > 0 )
						{
							vertexBufferActiveRanges.push_back( currentRange );
						}

						// Reset the range by setting the first index to an invalid value and clear the length
						currentRange.firstIndex = kIAVertexStreamSlotUndefined;
						currentRange.length = 0;
					}
				}

				// In case we have so many ranges we exceeded our typical pre-allocation space, shrink the vector to
				// ideally fit the space required. This will prevent from potentially more significant waste of memory.
				if( vertexBufferActiveRanges.size() > optimalActiveRangesNumPreAllocSize )
				{
					vertexBufferActiveRanges.shrink_to_fit();
				}
			}

			return vertexBufferActiveRanges;
		}

	}
	
}
