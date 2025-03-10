
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_COMMON_H__

#include <Ic3/System/Prerequisites.h>
#include <Ic3/System/Internal/Platform/Shared/WFA/WFACommon.h>
#include <utility>
#include <comdef.h>
#include <windows.h>

namespace Ic3::System
{

	namespace Platform
	{

		enum EWin32SystemWindowFlags : uint32
		{
			eWin32SystemWindowFlagWMStateFullscreen = 0x8000
		};

		inline constexpr UINT CX_WIN32_MESSAGE_ID_FULLSCREEN_STATE_CHANGE = 0x0477;

	}

	template <typename TBaseType, typename TNativeData>
	class Win32NativeObject : public NativeObject<TBaseType, TNativeData>
	{
	public:
		template <typename... TBaseTypeArgs>
		Win32NativeObject( TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{}

		virtual ~Win32NativeObject() = default;
	};

}

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_COMMON_H__
