
#ifndef __IC3_SYSTEM_JAVA_NATIVE_INTERFACE_H__
#define __IC3_SYSTEM_JAVA_NATIVE_INTERFACE_H__

#include "JavaCommon.h"

namespace Ic3::System
{

	namespace Platform
	{

		enum EAndroidScreenOrientation : int
		{
			E_ANDROID_SCREEN_ORIENTATION_LANDSCAPE         = 0,
			E_ANDROID_SCREEN_ORIENTATION_LOCKED            = 0x0000000E,
			E_ANDROID_SCREEN_ORIENTATION_PORTRAIT          = 0x00000001,
			E_ANDROID_SCREEN_ORIENTATION_REVERSE_LANDSCAPE = 0x00000008,
			E_ANDROID_SCREEN_ORIENTATION_REVERSE_PORTRAIT  = 0x00000009,
			E_ANDROID_SCREEN_ORIENTATION_UNSPECIFIED       = static_cast<int>( 0xFFFFFFFF ),
		};

	}

	class JavaNativeInterface final
	{
	public:
		JavaVMInstance * const mJVMInstance;
		Platform::JNIThreadID const mJNIThreadID;
		Platform::JNIEnvPtr const mJNIEnv;

	public:
		JavaNativeInterface( JavaVMInstance & pJVMInstance,
		                     Platform::JNIThreadID pJNIThreadID,
		                     Platform::JNIEnvPtr pJNIEnvPtr );

		~JavaNativeInterface() noexcept;

		void nativeActivitySetRequestedOrientation( jobject pNativeActivity,
		                                            Platform::EAndroidScreenOrientation pOrientation );
	};

}

#endif // __IC3_SYSTEM_JAVA_NATIVE_INTERFACE_H__
