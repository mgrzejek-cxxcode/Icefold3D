
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_JAVA_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_JAVA_COMMON_H__

#include <Ic3/System/Prerequisites.h>
#include <jni.h>
#include <thread>

namespace Ic3::System
{

	class JavaNativeInterface;
	class JavaVMInstance;

	namespace Platform
	{

		using JNIEnvPtr = JNIEnv *;
		using JNIThreadID = std::thread::id;

	}

	using JavaNativeInterfacePtr = std::shared_ptr<JavaNativeInterface>;

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_SHARED_JAVA_COMMON_H__
