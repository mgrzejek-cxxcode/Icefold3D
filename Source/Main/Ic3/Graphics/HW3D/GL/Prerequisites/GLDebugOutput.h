
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_DEBUG_OUTPUT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_DEBUG_OUTPUT_H__

#include <unordered_set>

#if( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_ES )
#  define GLAPIENTRY
#endif

namespace Ic3::Graphics::GCI
{

	enum class GLDebugOutputVersion : enum_default_value_t
	{
		Unknown,
		AMDExt,
		ARBExt,
		KHRCore,
	};

	class GLDebugOutput
	{
	public:
		GLDebugOutput( GLDebugOutputVersion pVersion );
		virtual ~GLDebugOutput();

		virtual void EnableDebugOutput( bool pEnable );
		virtual void EnableSync( bool pEnable );

		void EnableBreakOnEvent( bool pEnable );
		void EnableEventFilter( bool pEnable );

		void SetEventFilter( GLuint pEventID, bool pIgnored );
		void ResetEventFilters();

		const char * GetExtensionName() const;
		uint64 GetEventsCounter() const;
		GLDebugOutputVersion GetVersion() const;

		bool IsDebugOutputActive() const;
		bool IsEventFilterEnabled() const;
		bool IsEventIgnored( GLuint pEventID ) const;

		static std::unique_ptr<GLDebugOutput> CreateInterface( GLDebugOutputVersion pHint = GLDebugOutputVersion::Unknown );

	protected:
		void ProcessEvent( GLenum pEventSeverity, const char * pEventInfo );

	private:
		virtual void SetCallbackActive( bool pEnable ) = 0;

		static bool CheckAPISupport( GLDebugOutputVersion pVersion );
		static bool ValidateVersion( GLDebugOutputVersion pVersion );

	protected:
		enum StateFlags : uint32
		{
			eStateFlagDebugCallbackActive = 0x0001,
			eStateFlagEnableBreakOnEvent = 0x0010,
			eStateFlagEnableEventFilter = 0x0020
		};
		using IgnoredEventsMap = std::unordered_set<GLuint>;
		GLDebugOutputVersion _apiVersion;
		IgnoredEventsMap _ignoredEventSet;
		uint64 _processedEventsNum;
		cppx::bitmask<StateFlags> _stateFlags;
	};


#if( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )

	class GLDebugOutputAMDExt final : public GLDebugOutput
	{
	public:
		GLDebugOutputAMDExt();
		virtual ~GLDebugOutputAMDExt();

	private:
		virtual void SetCallbackActive( bool pEnable ) override;

		void HandleEvent( GLuint pEventID, GLenum pEventCategory, GLenum pEventSeverity, const GLchar * pMessage );

		static void GLAPIENTRY EventCallback(
				GLuint pEventID,
				GLenum pEventCategory,
				GLenum pEventSeverity,
				GLsizei pLength,
				const GLchar * pMessage,
				GLvoid * pUserParam );
	};

	class GLDebugOutputARBExt final : public GLDebugOutput
	{
	public:
		GLDebugOutputARBExt();
		virtual ~GLDebugOutputARBExt();

		virtual void EnableSync( bool pEnable ) override;

	private:
		virtual void SetCallbackActive( bool pEnable ) override;

		void HandleEvent( GLuint pEventID, GLenum pEventCategory, GLenum pEventType, GLenum pEventSeverity, const GLchar * pMessage );

		static void GLAPIENTRY EventCallback(
				GLuint pEventID,
				GLenum pEventSource,
				GLenum pEventType,
				GLenum pEventSeverity,
				GLsizei pLength,
				const GLchar * pMessage,
				const GLvoid * pUserParam );
	};

	class GLDebugOutputKHRCore final : public GLDebugOutput
	{
	public:
		GLDebugOutputKHRCore();
		virtual ~GLDebugOutputKHRCore();

		virtual void EnableSync( bool pEnable ) override;

	private:
		virtual void SetCallbackActive( bool pEnable ) override;

		void HandleEvent( GLuint pEventID, GLenum pEventCategory, GLenum pEventType, GLenum pEventSeverity, const GLchar * pMessage );

		static void GLAPIENTRY EventCallback(
				GLuint pEventID,
				GLenum pEventSource,
				GLenum pEventType,
				GLenum pEventSeverity,
				GLsizei pLength,
				const GLchar * pMessage,
				const GLvoid * pUserParam );
	};

#endif

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_DEBUG_OUTPUT_H__
