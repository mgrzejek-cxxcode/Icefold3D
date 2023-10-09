
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_ERROR_UTILS_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_ERROR_UTILS_H__

#include <Ic3/CoreLib/Exception.h>

namespace Ic3::Graphics::GCI
{

	class HresultErrorException : public UserExternalException
	{
	public:
		using BaseException = UserExternalException;

		explicit HresultErrorException( Ic3::ExceptionInfo pInfo )
		: BaseException( std::move( pInfo ) )
		{}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_ERROR_UTILS_H__
