
#include "color.h"
#include <cstdlib>

namespace cxm
{

	rgba_color_u8 generate_random_color()
	{
		return rgba_color_u8{
			static_cast<uint8>( rand() % 256 ),
			static_cast<uint8>( rand() % 256 ),
			static_cast<uint8>( rand() % 256 ),
			255u
		};
	}

} // namespace cxm
