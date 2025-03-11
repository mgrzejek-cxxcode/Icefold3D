
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_JAVA_VM_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_JAVA_VM_H__

#include "JavaCommon.h"
#include <memory>

namespace Ic3::System
{

	namespace Platform
	{

		/// @brief Internal flags used for ThreadState.
		enum EJNIThreadStateFlags : enum_default_value_t
		{
			// If set, the thread's JNI state is released automatically when the internal
			// ref counter drops to 0. See JavaVMInstance::SetCurrentThreadAutoReleaseState().
			eJNIThreadStateAutoReleaseBit = 0x8000
		};

		/// @brief Thread-specific JNI-related data. Created for each thread attached to the VM.
		struct JNIThreadState
		{
			// ID of the thread this state belongs to.
			JNIThreadID jniThreadID;

			// Internal reference counter. Incremented for every new JNIPtr created.
			std::atomic<uint32> jniRefCounter;

			// Pointer to the JNIEnv struct.
			JNIEnvPtr jniEnvPtr = nullptr;

			// JavaNativeInterface object.
			JavaNativeInterface * jniObject = nullptr;

			// Internal flags.
			cppx::bitmask<EJNIThreadStateFlags> flags = 0;
		};

		struct JavaNativeInterfaceDeleter
		{
			IC3_SYSTEM_API void operator()( JavaNativeInterface * pJNI ) const;
		};

	}

	/// @brief Represents an instance of the Java Virtual Machine. Associated with a JavaVM pointer.
	/// @details
	/// The core interface of the Java support layer. It is created for an existing JavaVM pointer,
	/// provides abstraction on top of it and adds a thread-safe JNI management API.
	class IC3_SYSTEM_CLASS JavaVMInstance final
	{
		friend struct Platform::JavaNativeInterfaceDeleter;

	public:
		// The JavaVM pointer associated with the instance.
		JavaVM * const mJavaVM;

	public:
		explicit JavaVMInstance( JavaVM * pJavaVM );
		~JavaVMInstance() noexcept;

		/// @brief Acquires an active reference to the JNI object of the specified thread.
		/// @details
		/// This function provides a way to obtain any thread's JNI object using its thread ID.
		/// If requested thread's state has not yet been created, this function will automatically
		/// do this *IF* (and only if) the current calling thread is the one whose JNI is being
		/// requested. In other words, a thread must first initialize its state before anyone else
		/// can retrieve it. This can be done with InitializeCurrentThreadJNIState().
		JavaNativeInterfacePtr AcquireJNI( Platform::JNIThreadID pJNIThreadID );

		/// @brief Acquires an active reference to the JNI object of the current thread.
		/// @see JavaVMInstance::AcquireJNI
		JavaNativeInterfacePtr AcquireJNIForCurrentThread();

		/// @brief Initializes the JNI state for the current thread.
		void InitializeCurrentThreadJNIState();

		/// @brief Sets the auto-release state for the current thread.
		/// @details
		/// By default, when the last instance of JavaNativeInterfacePtr is destroyed, it only causes
		/// the internal reference counter to be decremented - the JNI object itself is kept alive,
		/// so it can be again quickly retrieved. In this case, ReleaseCurrentThreadState() needs
		/// to be called before thread finishes execution. To make this an automated process, call
		/// this function with 'true', which will cause the JNI state to be release when ref counter
		/// drops to 0. A drawback is that the JNI state needs to re-created every time it's acquired.
		void SetCurrentThreadAutoReleaseState( bool pAutoRelease );

		/// @brief Releases all JNI resources created for the current thread.
		void ReleaseCurrentThreadState();

	friendapi( private ):
		/// @brief Used by JavaNativeInterfaceDeleter, called when JNI objects is to be destroyed.
		void OnJavaNativeInterfacePtrDestroy( JavaNativeInterface * pJNI );

	private:
		// Returns the JNI object for a specified thread. If not present, auto-creates it if requested.
		JavaNativeInterface * _AcquireJNI( Platform::JNIThreadID pJNIThreadID, bool pAutoCreateState );

		// Returns the whole state object for a specified thread. If not present, auto-creates it if requested.
		Platform::JNIThreadState * _GetJNIThreadState( Platform::JNIThreadID pJNIThreadID, bool pAutoCreateState );

		// Handles destruction of the thread state for a specified thread.
		void _OnJNIThreadStateDestroyRequest( Platform::JNIThreadID pJNIThreadID, bool pForceRelease );

	private:
		struct JavaVMInstancePrivateData;
		std::unique_ptr<JavaVMInstancePrivateData> _privateData;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_SHARED_JAVA_VM_H__
