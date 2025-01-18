
#include "../GLPrerequisites.h"
#include "../GLApiTranslationLayer.h"

namespace Ic3::Graphics::GCI
{

#if( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )

	static const size_t sEventInfoBufferSize = 1024;

	GLDebugOutput::GLDebugOutput( GLDebugOutputVersion pVersion )
	: _apiVersion( pVersion )
	, _processedEventsNum( 0 )
	, _stateFlags( 0 )
	{}

	GLDebugOutput::~GLDebugOutput()
	{}

	void GLDebugOutput::EnableDebugOutput( bool pEnable )
	{
		if( pEnable )
		{
			glEnable( GL_DEBUG_OUTPUT );
			Ic3OpenGLHandleLastError();
		}
		else
		{
			glDisable( GL_DEBUG_OUTPUT );
			Ic3OpenGLHandleLastError();
		}

		SetCallbackActive( pEnable );
		Ic3DebugOutputFmt( "Debug output has been %s.", pEnable ? "enabled" : "disabled" );
	}

	void GLDebugOutput::EnableSync( bool pEnable )
	{
		// By default - nothing. AMD extension, for example, does not have this concept.
	}

	void GLDebugOutput::EnableBreakOnEvent( bool pEnable )
	{
		_stateFlags.set_or_unset( eStateFlagEnableBreakOnEvent, pEnable );
	}

	void GLDebugOutput::EnableEventFilter( bool pEnable )
	{
		_stateFlags.set_or_unset( eStateFlagEnableEventFilter, pEnable );
	}

	void GLDebugOutput::SetEventFilter( GLuint pEventID, bool pIgnored )
	{
		if( pIgnored )
		{
			_ignoredEventSet.insert( pEventID );
		}
		else
		{
			_ignoredEventSet.erase( pEventID );
		}
	}

	void GLDebugOutput::ResetEventFilters()
	{
		_ignoredEventSet.clear();
	}

	const char * GLDebugOutput::GetExtensionName() const
	{
		return ATL::TranslateGLDebugOutputExtensionName( _apiVersion );
	}

	uint64 GLDebugOutput::GetEventsCounter() const
	{
		return _processedEventsNum;
	}

	GLDebugOutputVersion GLDebugOutput::GetVersion() const
	{
		return _apiVersion;
	}

	bool GLDebugOutput::IsDebugOutputActive() const
	{
		return _stateFlags.is_set( eStateFlagDebugCallbackActive );
	}

	bool GLDebugOutput::IsEventIgnored( GLuint pEventID ) const
	{
		if( !_stateFlags.is_set( eStateFlagEnableEventFilter ) || _ignoredEventSet.empty() )
		{
			return false;
		}
		auto eventIter = _ignoredEventSet.find( pEventID );
		return eventIter != _ignoredEventSet.end();
	}

	std::unique_ptr<GLDebugOutput> GLDebugOutput::CreateInterface( GLDebugOutputVersion pHint )
	{
		GLDebugOutputVersion selectedVersion = GLDebugOutputVersion::Unknown;

		if( CheckAPISupport( GLDebugOutputVersion::KHRCore ) )
		{
			selectedVersion = GLDebugOutputVersion::KHRCore;
		}
		else if( CheckAPISupport( GLDebugOutputVersion::ARBExt ) )
		{
			selectedVersion = GLDebugOutputVersion::ARBExt;
		}
		else if( CheckAPISupport( GLDebugOutputVersion::AMDExt ) )
		{
			selectedVersion = GLDebugOutputVersion::AMDExt;
		}

		// If selected version remains Unknown, neither version of
		// the DebugOutput API is supported by the current runtime.
		if( selectedVersion == GLDebugOutputVersion::Unknown )
		{
			return nullptr;
		}

		// Preferred API version overwrites the selected one, as long
		// as it is a valid version and is supported by the runtime.
		if( ValidateVersion( pHint ) && CheckAPISupport( pHint ) )
		{
			selectedVersion = pHint;
		}

		std::unique_ptr<GLDebugOutput> debugOutputInterface;
		switch( selectedVersion )
		{
			case GLDebugOutputVersion::AMDExt:
				debugOutputInterface = std::make_unique<GLDebugOutputAMDExt>();
				break;

			case GLDebugOutputVersion::ARBExt:
				debugOutputInterface = std::make_unique<GLDebugOutputARBExt>();
				break;

			case GLDebugOutputVersion::KHRCore:
				debugOutputInterface = std::make_unique<GLDebugOutputKHRCore>();
				break;
		}

		if( debugOutputInterface )
		{
			auto apiVersion = debugOutputInterface->GetVersion();
			const char * extensionName = debugOutputInterface->GetExtensionName();

			Ic3DebugOutputFmt( "GLDebugOutput object has been initialized (version %u: %s)", apiVersion, extensionName );

		}

		return debugOutputInterface;
	}

	void GLDebugOutput::ProcessEvent( GLenum pEventSeverity, const char * pEventInfo )
	{
		++_processedEventsNum;

		Ic3DebugOutput( pEventInfo );

		if( ( pEventSeverity == GL_DEBUG_SEVERITY_HIGH_ARB ) && _stateFlags.is_set( eStateFlagEnableBreakOnEvent ) )
		{
			Ic3DebugInterrupt();
		}
	}

	bool GLDebugOutput::CheckAPISupport( GLDebugOutputVersion pVersion )
	{
		return true;
	}

	bool GLDebugOutput::ValidateVersion( GLDebugOutputVersion pVersion )
	{
		return ( pVersion == GLDebugOutputVersion::AMDExt ) ||
		       ( pVersion == GLDebugOutputVersion::ARBExt ) ||
		       ( pVersion == GLDebugOutputVersion::KHRCore );
	}


	GLDebugOutputAMDExt::GLDebugOutputAMDExt()
	: GLDebugOutput( GLDebugOutputVersion::AMDExt )
	{}

	GLDebugOutputAMDExt::~GLDebugOutputAMDExt() = default;

	void GLDebugOutputAMDExt::SetCallbackActive( bool pEnable )
	{
		if( pEnable )
		{
			glDebugMessageCallbackAMD( EventCallback, this );
			_stateFlags.set( eStateFlagDebugCallbackActive );
		}
		else
		{
			glDebugMessageCallbackAMD( nullptr, nullptr );
			_stateFlags.unset( eStateFlagDebugCallbackActive );
		}
	}

	void GLDebugOutputAMDExt::HandleEvent( GLuint pEventID, GLenum pEventCategory, GLenum pEventSeverity, const GLchar * pMessage )
	{
		if( IsEventIgnored( pEventID ) )
		{
			return;
		}

		char eventInfoBuffer[sEventInfoBufferSize];
		snprintf( eventInfoBuffer,
		          sEventInfoBufferSize,
		          "\n----------------------------------------------------------------------\n"\
					"[GL Debug Event (via AMD_debug_output)]\n\n"\
					"- ID: %u\n"\
					"- Category: %s\n"\
					"- Severity: %s\n\n"\
					"- Description: %s\n"\
					"----------------------------------------------------------------------\n",
		          pEventID,
		          ATL::TranslateGLDebugEventCategoryStrAMD( pEventCategory ),
		          ATL::TranslateGLDebugEventSeverityStr( pEventSeverity ),
		          pMessage );

		ProcessEvent( pEventSeverity, eventInfoBuffer );
	}

	void GLDebugOutputAMDExt::EventCallback( GLuint pEventID,
	                                         GLenum pEventCategory,
	                                         GLenum pEventSeverity,
	                                         GLsizei pLength,
	                                         const GLchar * pMessage,
	                                         GLvoid * pUserParam )
	{
		auto * debugOutputInterface = reinterpret_cast<GLDebugOutputAMDExt *>( pUserParam );
		debugOutputInterface->HandleEvent( pEventID, pEventCategory, pEventSeverity, pMessage );
	}


	GLDebugOutputARBExt::GLDebugOutputARBExt()
	: GLDebugOutput( GLDebugOutputVersion::ARBExt )
	{}

	GLDebugOutputARBExt::~GLDebugOutputARBExt() = default;

	void GLDebugOutputARBExt::EnableSync( bool pEnable )
	{
		if( pEnable )
		{
			glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB );
			Ic3OpenGLHandleLastError();
		}
		else
		{
			glDisable( GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB );
			Ic3OpenGLHandleLastError();
		}
	}

	void GLDebugOutputARBExt::SetCallbackActive( bool pEnable )
	{
		if( pEnable )
		{
			glDebugMessageCallbackARB( EventCallback, this );
			_stateFlags.set( eStateFlagDebugCallbackActive );
		}
		else
		{
			glDebugMessageCallbackARB( nullptr, nullptr );
			_stateFlags.unset( eStateFlagDebugCallbackActive );
		}
	}

	void GLDebugOutputARBExt::HandleEvent( GLuint pEventID, GLenum pEventSource, GLenum pEventType, GLenum pEventSeverity, const GLchar * pMessage )
	{
		if( IsEventIgnored( pEventID ) )
		{
			return;
		}

		char eventInfoBuffer[sEventInfoBufferSize];
		snprintf( eventInfoBuffer,
		          sEventInfoBufferSize,
		          "\n----------------------------------------------------------------------\n"\
					"[GL Debug Event (via ARB_debug_output)]\n\n"\
					"- ID: %u\n"\
					"- Source: %s\n"\
					"- Type: %s\n"\
					"- Severity: %s\n\n"\
					"- Description: %s\n"\
					"----------------------------------------------------------------------\n",
		          pEventID,
		          ATL::TranslateGLDebugEventSourceStr( pEventSource ),
		          ATL::TranslateGLDebugEventTypeStr( pEventType ),
		          ATL::TranslateGLDebugEventSeverityStr( pEventSeverity ),
		          pMessage );

		ProcessEvent( pEventSeverity, eventInfoBuffer );
	}

	void GLDebugOutputARBExt::EventCallback( GLuint pEventID,
	                                         GLenum pEventSource,
	                                         GLenum pEventType,
	                                         GLenum pEventSeverity,
	                                         GLsizei pLength,
	                                         const GLchar * pMessage,
	                                         const GLvoid * pUserParam )
	{
		auto * debugOutputInterface = reinterpret_cast<GLDebugOutputARBExt *>( const_cast<void *>( pUserParam ) );
		debugOutputInterface->HandleEvent( pEventID, pEventSource, pEventType, pEventSeverity, pMessage );
	}


	GLDebugOutputKHRCore::GLDebugOutputKHRCore()
	: GLDebugOutput( GLDebugOutputVersion::KHRCore )
	{}

	GLDebugOutputKHRCore::~GLDebugOutputKHRCore() = default;

	void GLDebugOutputKHRCore::EnableSync( bool pEnable )
	{
		if( pEnable )
		{
			glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			Ic3OpenGLHandleLastError();
		}
		else
		{
			glDisable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			Ic3OpenGLHandleLastError();
		}
	}

	void GLDebugOutputKHRCore::SetCallbackActive( bool pEnable )
	{
		if( pEnable )
		{
			glDebugMessageCallback( EventCallback, this );
			_stateFlags.set( eStateFlagDebugCallbackActive );
		}
		else
		{
			glDebugMessageCallback( nullptr, nullptr );
			_stateFlags.unset( eStateFlagDebugCallbackActive );
		}
	}

	void GLDebugOutputKHRCore::HandleEvent( GLuint pEventID, GLenum pEventSource, GLenum pEventType, GLenum pEventSeverity, const GLchar * pMessage )
	{
		if( IsEventIgnored( pEventID ) )
		{
			return;
		}

		char eventInfoBuffer[sEventInfoBufferSize];
		snprintf( eventInfoBuffer,
		          sEventInfoBufferSize,
		          "\n----------------------------------------------------------------------\n"\
		           "[GL Debug Event (via KHR_debug)]\n\n"\
		           "- ID: %u\n"\
		           "- Source: %s\n"\
		           "- Type: %s\n"\
		           "- Severity: %s\n\n"\
		           "- Description: %s\n"\
		           "----------------------------------------------------------------------\n",
		          pEventID,
		          ATL::TranslateGLDebugEventSourceStr( pEventSource ),
		          ATL::TranslateGLDebugEventTypeStr( pEventType ),
		          ATL::TranslateGLDebugEventSeverityStr( pEventSeverity ),
		          pMessage );

		ProcessEvent( pEventSeverity, eventInfoBuffer );
	}

	void GLDebugOutputKHRCore::EventCallback( GLuint pEventID,
	                                          GLenum pEventSource,
	                                          GLenum pEventType,
	                                          GLenum pEventSeverity,
	                                          GLsizei pLength,
	                                          const GLchar * pMessage,
	                                          const GLvoid * pUserParam )
	{
		auto * debugOutputInterface = reinterpret_cast<GLDebugOutputKHRCore *>( const_cast<void *>( pUserParam ) );
		debugOutputInterface->HandleEvent( pEventID, pEventSource, pEventType, pEventSeverity, pMessage );
	}

#elif( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_ES )

	GLDebugOutput::GLDebugOutput( GLDebugOutputVersion /* pVersion */ )
	: _apiVersion( GLDebugOutputVersion::Unknown )
	, _processedEventsNum( 0 )
	, _stateFlags( 0 )
	{}

	GLDebugOutput::~GLDebugOutput()
	{}

	void GLDebugOutput::EnableDebugOutput( bool pEnable )
	{}

	void GLDebugOutput::EnableSync( bool pEnable )
	{}

	void GLDebugOutput::EnableBreakOnEvent( bool pEnable )
	{}

	void GLDebugOutput::EnableEventFilter( bool pEnable )
	{}

	void GLDebugOutput::SetEventFilter( GLuint pEventID, bool pIgnored )
	{}

	void GLDebugOutput::ResetEventFilters()
	{}

	const char * GLDebugOutput::GetExtensionName() const
	{
		return ATL::TranslateGLDebugOutputExtensionName( GLDebugOutputVersion::Unknown );
	}

	uint64 GLDebugOutput::GetEventsCounter() const
	{
		return 0;
	}

	GLDebugOutputVersion GLDebugOutput::GetVersion() const
	{
		return GLDebugOutputVersion::Unknown;
	}

	bool GLDebugOutput::IsDebugOutputActive() const
	{
		return false;
	}

	bool GLDebugOutput::IsEventIgnored( GLuint pEventID ) const
	{
		return true;
	}

	std::unique_ptr<GLDebugOutput> GLDebugOutput::CreateInterface( GLDebugOutputVersion pHint )
	{
		return nullptr;
	}

	void GLDebugOutput::ProcessEvent( GLenum pEventSeverity, const char * pEventInfo )
	{}

	bool GLDebugOutput::CheckAPISupport( GLDebugOutputVersion pVersion )
	{
		return pVersion == GLDebugOutputVersion::Unknown;
	}

	bool GLDebugOutput::ValidateVersion( GLDebugOutputVersion pVersion )
	{
		return pVersion == GLDebugOutputVersion::Unknown;
	}

#endif

} // namespace Ic3::Graphics::GCI
