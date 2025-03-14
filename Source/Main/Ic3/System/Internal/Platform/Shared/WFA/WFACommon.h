
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_WFA_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_WFA_COMMON_H__

#include <Ic3/System/Prerequisites.h>

#include <comdef.h>
#include <wrl/client.h>

namespace Ic3::System
{

	template <typename TVal>
	using ComPtr = Microsoft::WRL::ComPtr<TVal>;

	namespace Platform
	{

		IC3_SYSTEM_API_NODISCARD std::string WFAQueryComErrorMessage( HRESULT pHResult );

		IC3_SYSTEM_API_NODISCARD std::string WFAQuerySystemErrorMessage( DWORD pErrorCode );

		IC3_SYSTEM_API void WFAGetLastSystemErrorAndPrintToDebugOutput( const char * pPrefix = nullptr );

		IC3_SYSTEM_API void WFAPrintSystemErrorToDebugOutput( DWORD pErrorCode, const char * pPrefix = nullptr );

	}

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_SHARED_WFA_COMMON_H__
