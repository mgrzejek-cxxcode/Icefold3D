
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_WNF_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_WNF_COMMON_H__

#include <Ic3/System/Prerequisites.h>

#include <comdef.h>
#include <wrl/client.h>

namespace Ic3::System
{

	template <typename TVal>
	using ComPtr = Microsoft::WRL::ComPtr<TVal>;

	namespace Platform
	{

		std::string mseQueryCOMErrorMessage( HRESULT pHResult );

		std::string mseQuerySystemErrorMessage( DWORD pErrorCode );

	}

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_SHARED_WNF_COMMON_H__
