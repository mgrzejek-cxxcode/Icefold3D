
#include "InputAssemblerCommon.h"

namespace Ic3::Graphics::GCI
{
	
	namespace SMU
	{

		cppx::bitmask<EIAVertexAttributeFlags> GetIAVertexInputActiveAttributesMask(
				const IAVertexAttributeInfoArray & pVertexAttributes ) noexcept
		{
			cppx::bitmask<EIAVertexAttributeFlags> activeAttributesMask = 0;
			for( uint32 attributeIndex = 0; attributeIndex < GCM::cxIAMaxVertexAttributesNum; ++attributeIndex )
			{
				if( pVertexAttributes[attributeIndex].IsActive() )
				{
					activeAttributesMask.set( CxDef::makeIAVertexAttributeFlag( attributeIndex ) );
				}
			}
			return activeAttributesMask;
		}

		uint32 GetIAVertexInputActiveAttributesNum( const IAVertexAttributeInfoArray & pVertexAttributes ) noexcept
		{
			const auto activeAttributesMask = GetIAVertexInputActiveAttributesMask( pVertexAttributes );
			return activeAttributesMask.count_bits();
		}

		IAVertexBufferRangeList GenerateActiveVertexBufferRanges( const IAVertexBufferReferenceArray & pVBReferences ) noexcept
		{
			// Single range is described by two uint16 values which is 4 bytes. More than 90% use cases fall
			// into 1-3 ranges per single binding state. We pre-allocate space for 4 ranges (which is 16 bytes)
			// to prevent from auto growing in typical cases (and few lost bytes per binding is negligible).
			constexpr auto optimalActiveRangesNumPreAllocSize = 4u;

			IAVertexBufferRangeList vertexBufferActiveRanges;
			vertexBufferActiveRanges.reserve( optimalActiveRangesNumPreAllocSize );

			IAVertexBufferRange currentVBRange{};
			currentVBRange.firstIndex = cxIAVertexStreamIndexUndefined;

			for( input_assembler_index_t streamIndex = 0; streamIndex < GCM::cxIAMaxVertexBufferBindingsNum; ++streamIndex )
			{
				const auto & currentVBReference = pVBReferences[streamIndex];

				if( !currentVBReference.IsEmpty() )
				{
					// If the range is not valid, "open" it.
					// Set the current stream as the first stream in the range and range size to 0.
					if( currentVBRange.firstIndex == cxIAVertexStreamIndexUndefined )
					{
						currentVBRange.firstIndex = streamIndex;
						currentVBRange.length = 0;
					}

					// Increase the length of a current range.
					++currentVBRange.length;
				}

				if( currentVBReference.IsEmpty() || ( streamIndex + 1 == GCM::cxIAMaxVertexBufferBindingsNum ) )
				{
					// If the range is not empty, add it to the list of active ranges.
					if( currentVBRange.length > 0 )
					{
						vertexBufferActiveRanges.push_back( currentVBRange );
					}

					// Reset the range by setting the first index to an invalid value and clear the length
					currentVBRange.firstIndex = cxIAVertexStreamIndexUndefined;
					currentVBRange.length = 0;
				}
			}

			// In case we have so many ranges we exceeded our typical pre-allocation space, shrink the vector to
			// ideally fit the space required. This will prevent from potentially more significant waste of memory.
			if( vertexBufferActiveRanges.size() > optimalActiveRangesNumPreAllocSize )
			{
				vertexBufferActiveRanges.shrink_to_fit();
			}

			return vertexBufferActiveRanges;
		}
		
	}
	
}
