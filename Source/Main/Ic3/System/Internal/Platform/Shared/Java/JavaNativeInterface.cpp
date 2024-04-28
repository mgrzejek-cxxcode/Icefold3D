
#include "JavaNativeInterface.h"

namespace Ic3::System
{

	JavaNativeInterface::JavaNativeInterface( JavaVMInstance & pJVMInstance,
	                                          Platform::JNIThreadID pJNIThreadID,
	                                          Platform::JNIEnvPtr pJNIEnvPtr )
	: mJVMInstance( &pJVMInstance )
	, mJNIThreadID( pJNIThreadID )
	, mJNIEnv( pJNIEnvPtr )
	{}

	JavaNativeInterface::~JavaNativeInterface() noexcept = default;

	void JavaNativeInterface::nativeActivitySetRequestedOrientation( jobject pNativeActivity,
	                                                                 Platform::EAndroidScreenOrientation pOrientation )
	{
		auto * jNativeActivityClass = mJNIEnv->GetObjectClass( pNativeActivity );
		auto * jMethod = mJNIEnv->GetMethodID( jNativeActivityClass, "setRequestedOrientation", "(I)V" );
		mJNIEnv->CallVoidMethod( pNativeActivity, jMethod, static_cast<int>( pOrientation ) );
	}

} // namespace Ic3::System
