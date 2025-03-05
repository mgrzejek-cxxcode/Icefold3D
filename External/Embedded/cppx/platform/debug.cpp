
#include "../prerequisites.h"

namespace cppx
{

	namespace impl
	{

		void debug_output_impl( const char * pFormat, ... )
		{
			const size_t localBufferSize = 4096;
			char localBuffer[localBufferSize];

			size_t printCount = 0;
			char * bufferPtr = &( localBuffer[0] );

			{
				va_list varArgsList;
				va_start( varArgsList, pFormat );

				// This returns the number that *would be written* if buffer was large enough.
				// For a valid case, it should always smaller than our buffer size. If not,
				// we need to make the buffer capacity a bit bigger.
				printCount = vsnprintf( bufferPtr, localBufferSize, pFormat, varArgsList );

				if( printCount > localBufferSize )
				{
					// Just signal via debug break. We don't need any runtime
					// error handling - this is used only in debug mode anyway.
					PCL_DEBUG_BREAK();
				}

				va_end( varArgsList );
			}

			printf(localBuffer);
			// PCL_DEBUG_OUTPUT( localBuffer );
		}

		void debug_default_print_handler( const char * pText, size_t pLength )
		{
			printf(pText);
			//PCL_DEBUG_OUTPUT( pText );
		}

	}

} // namespace cppx
