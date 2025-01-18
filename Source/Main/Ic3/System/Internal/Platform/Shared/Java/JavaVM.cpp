
#include "JavaVM.h"
#include "JavaNativeInterface.h"
#include <map>

namespace Ic3::System
{

	struct JavaVMInstance::JavaVMInstancePrivateData
	{
		std::map<Platform::JNIThreadID, Platform::JNIThreadState *> jniThreadStateMap;
	};


	JavaVMInstance::JavaVMInstance( JavaVM * pJavaVM )
	: mJavaVM( pJavaVM )
	, _privateData( std::make_unique<JavaVMInstancePrivateData>() )
	{}

	JavaVMInstance::~JavaVMInstance() noexcept = default;

	JavaNativeInterfacePtr JavaVMInstance::AcquireJNI( Platform::JNIThreadID pJNIThreadID )
	{
		auto currentThreadID = std::this_thread::get_id();

		// Thread state can be auto created for the calling thread - and the calling thread only.
		// It involves a call to VM's AttachCurrentThread(), which works only for the current thread.
		bool autoCreateState = ( currentThreadID == pJNIThreadID );

		if( auto * jniObject = _AcquireJNI( pJNIThreadID, autoCreateState ) )
		{
			return JavaNativeInterfacePtr( jniObject, Platform::JavaNativeInterfaceDeleter{} );
		}

		return nullptr;
	}

	JavaNativeInterfacePtr JavaVMInstance::AcquireJNIForCurrentThread()
	{
		auto currentThreadID = std::this_thread::get_id();
		if( auto * jniObject = _AcquireJNI( currentThreadID, true ) )
		{
			return JavaNativeInterfacePtr( jniObject, Platform::JavaNativeInterfaceDeleter{} );
		}
		return nullptr;
	}

	void JavaVMInstance::InitializeCurrentThreadJNIState()
	{
		auto currentThreadID = std::this_thread::get_id();
		_GetJNIThreadState( currentThreadID, true );
	}

	void JavaVMInstance::SetCurrentThreadAutoReleaseState( bool pAutoRelease )
	{
		auto currentThreadID = std::this_thread::get_id();
		auto * jniThreadState = _GetJNIThreadState( currentThreadID, true );
		jniThreadState->flags.set( Platform::eJNIThreadStateAutoReleaseBit );
	}

	void JavaVMInstance::ReleaseCurrentThreadState()
	{
		auto currentThreadID = std::this_thread::get_id();

		// Call the destroy request with force=true (this in the explicit release function).
		// Note, that this call may still fail if the reference counter has not reached 0.
		_OnJNIThreadStateDestroyRequest( currentThreadID, true );
	}

	void JavaVMInstance::OnJavaNativeInterfacePtrDestroy( JavaNativeInterface * pJNI )
	{
		// This function is called within ~JavaNativeInterface().

		// Retrieve the thread state using JNI's thread id. This should never return nullptr,
		// because there is at least one remaining JNI instance (the one being currently destroyed).
		auto * jniThreadState = _GetJNIThreadState( pJNI->jniThreadID, false );
		Ic3DebugAssert( jniThreadState != nullptr );

		auto refCounter = jniThreadState->jniRefCounter.fetch_sub( 1, std::memory_order_acq_rel );
		if( refCounter == 1 )
		{
			// If this was the last active reference, we may want to release the whole state,
			// depending on the configuration. But this can be done only if the current thread
			// is the one that owns the state.
			auto currentThreadID = std::this_thread::get_id();
			if( pJNI->jniThreadID == currentThreadID )
			{
				// Call the destroy request, do not force deletion (it will be automatically
				// released if eJNIThreadStateAutoReleaseBit has been set for this thread).
				_OnJNIThreadStateDestroyRequest( pJNI->jniThreadID, false );
			}
		}
	}

	JavaNativeInterface * JavaVMInstance::_AcquireJNI( Platform::JNIThreadID pJNIThreadID, bool pAutoCreateState )
	{
		if( auto * jniThreadState = _GetJNIThreadState( pJNIThreadID, pAutoCreateState ) )
		{
			jniThreadState->jniRefCounter.fetch_add( 1, std::memory_order_acq_rel );
			return jniThreadState->jniObject;
		}
		return nullptr;
	}

	Platform::JNIThreadState * JavaVMInstance::_GetJNIThreadState( Platform::JNIThreadID pJNIThreadID, bool pAutoCreateState )
	{
		auto jniThreadStateIter = _privateData->jniThreadStateMap.find( pJNIThreadID );
		if( ( jniThreadStateIter == _privateData->jniThreadStateMap.end() ) && pAutoCreateState )
		{
			auto currentThreadID = std::this_thread::get_id();
			if( pJNIThreadID != currentThreadID )
			{
				Ic3ThrowDesc( eExcCodeDebugPlaceholder,
				              "AcquireJNIEnv(): JNIThreadState can be only initialized by the thread itself" );
			}

			auto jniThreadState = std::make_unique<Platform::JNIThreadState>();
			JNIEnv * jniEnv = nullptr;

			auto threadAttachResult = mJavaVM->AttachCurrentThread( &jniEnv, nullptr );
			if( threadAttachResult != JNI_OK )
			{
				Ic3ThrowDesc( eExcCodeDebugPlaceholder,
				              "JavaVM::AttachCurrentThread() has failed" );
			}

			jniThreadState->jniEnvPtr = jniEnv;
			jniThreadState->jniThreadID = pJNIThreadID;
			jniThreadState->jniObject = new JavaNativeInterface( *this, pJNIThreadID, jniEnv );

			auto insertResult = _privateData->jniThreadStateMap.insert( { pJNIThreadID, jniThreadState.release() } );
			jniThreadStateIter = insertResult.first;
		}

		Platform::JNIThreadState * jniThreadState = nullptr;

		if( jniThreadStateIter != _privateData->jniThreadStateMap.end() )
		{
			Ic3DebugAssert( jniThreadStateIter->second->jniObject != nullptr );
			jniThreadState = jniThreadStateIter->second;
		}

		return jniThreadState;
	}

	void JavaVMInstance::_OnJNIThreadStateDestroyRequest( Platform::JNIThreadID pJNIThreadID, bool pForceRelease )
	{
		auto jniThreadStateIter = _privateData->jniThreadStateMap.find( pJNIThreadID );
		if( jniThreadStateIter != _privateData->jniThreadStateMap.end() )
		{
			Ic3DebugAssert( pJNIThreadID == std::this_thread::get_id() );

			auto * jniThreadState = jniThreadStateIter->second;
			auto refCounter = jniThreadState->jniRefCounter.load( std::memory_order_acquire );

			if( refCounter > 0 )
			{
				Ic3ThrowDesc( eExcCodeDebugPlaceholder,
				              "_OnJNIThreadStateDestroyRequest(): refCounter > 0" );
			}

			if( jniThreadState->flags.is_set( Platform::eJNIThreadStateAutoReleaseBit ) || pForceRelease )
			{
				std::unique_ptr<Platform::JNIThreadState> jniThreadStatePtr{ jniThreadState };

				mJavaVM->DetachCurrentThread();

				_privateData->jniThreadStateMap.erase( jniThreadStateIter );

				delete jniThreadState->jniObject;
			}
		}
	}


	namespace Platform
	{

		void JavaNativeInterfaceDeleter::operator()( JavaNativeInterface * pJNI ) const
		{
			auto * jvmInstance = pJNI->mJVMInstance;
			jvmInstance->OnJavaNativeInterfacePtrDestroy( pJNI );
		}

	}

} // namespace Ic3::System
