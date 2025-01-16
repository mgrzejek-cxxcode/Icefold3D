
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_JAVA_NATIVE_INTERFACE_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_JAVA_NATIVE_INTERFACE_H__

#include "JavaCommon.h"

namespace Ic3::System
{

	namespace Platform
	{

		enum EAndroidScreenOrientation : int
		{
			eAndroidScreenOrientationLandscape        = 0,
			eAndroidScreenOrientationLocked           = 0x0000000E,
			eAndroidScreenOrientationPortrait         = 0x00000001,
			eAndroidScreenOrientationReverseLandscape = 0x00000008,
			eAndroidScreenOrientationReversePortrait  = 0x00000009,
			eAndroidScreenOrientationUnspecified      = static_cast<int>( 0xFFFFFFFF ),
		};

	}

	class JavaNativeInterface final
	{
	public:
		JavaVMInstance * const mJVMInstance;
		Platform::JNIThreadID const mJNIThreadID;
		Platform::JNIEnvPtr const mJNIEnv;

	public:
		JavaNativeInterface(
				JavaVMInstance & pJVMInstance,
				Platform::JNIThreadID pJNIThreadID,
				Platform::JNIEnvPtr pJNIEnvPtr );

		~JavaNativeInterface() noexcept;

		/// [NativeActivity API]
		void NASetRequestedOrientation(
				jobject pNativeActivity,
				Platform::EAndroidScreenOrientation pOrientation );
	};

}

#endif // __IC3_SYSTEM_PLATFORM_SHARED_JAVA_NATIVE_INTERFACE_H__
