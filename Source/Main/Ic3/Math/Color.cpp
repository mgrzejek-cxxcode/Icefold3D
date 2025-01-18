
#include "Color.h"
#include <ctime>
#include <cstdlib>

namespace Ic3::Math
{

	RGBAColorU8 generateRandomColor()
	{
		return RGBAColorU8{
			static_cast<uint8>( rand() % 256 ),
			static_cast<uint8>( rand() % 256 ),
			static_cast<uint8>( rand() % 256 )
		};
	}

} // namespace Ic3::Math
