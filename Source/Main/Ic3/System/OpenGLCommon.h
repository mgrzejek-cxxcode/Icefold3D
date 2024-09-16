
#ifndef __IC3_SYSTEM_GFX_OPENGL_COMMON_H__
#define __IC3_SYSTEM_GFX_OPENGL_COMMON_H__

#include "Visual.h"
#include "WindowCommon.h"

/// @brief Identifies a desktop OpenGL-based platform (like Win32, GLX or MacOS).
#define IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP 0x7001

/// @brief Identifies an ES-based platform (like Android or iOS).
#define IC3_SYSTEM_GL_PLATFORM_TYPE_ES 0x7002

/// @def IC3_SYSTEM_GL_PLATFORM_TYPE
/// @brief Defined as either IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP or IC3_SYSTEM_GL_PLATFORM_TYPE_ES, depending on the target platform.

#if( IC3_PCL_TARGET_OS & IC3_PCL_TARGET_FLAG_OS_DESKTOP )
#  define IC3_SYSTEM_GL_PLATFORM_TYPE IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP
#  include <GL/glew.h>
#else
#  define IC3_SYSTEM_GL_PLATFORM_TYPE IC3_SYSTEM_GL_PLATFORM_TYPE_ES
#  include <GLES3/gl3platform.h>
#  include <GLES3/gl3.h>
#  include <GLES3/gl31.h>
#endif

#define IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS 1

/// @def IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS
/// @brief Controls the GL error-checking macros. If TRUE, they are replaced with appropriate calls. Otherwise, all calls are no-ops.
/// @see ic3OpenGLCheckLastResult
/// @see ic3OpenGLCheckLastError
/// @see ic3OpenGLHandleLastError
/// @see ic3OpenGLResetErrorQueue
#if !defined( IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS )
#  if( IC3_RELEASE_OPT_MAX )
#    define IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS 0
#  else
#    define IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS 1
#  endif
#endif

namespace Ic3::System
{

	ic3SysDeclareHandle( OpenGLSystemDriver );
	ic3SysDeclareHandle( OpenGLDisplaySurface );
	ic3SysDeclareHandle( OpenGLRenderContext );

	struct OpenGLDisplaySurfaceCreateInfo;
	struct OpenGLRenderContextCreateInfo;

	/// @brief
	enum EOpenGLSurfaceCreateFlags : uint32
	{
		/// Surface is created as a fullscreen layer/window, adjusted to the selected display dimensions.
		eOpenGLDisplaySurfaceCreateFlagFullscreenBit = 0x1000,

		/// Enables adaptive sync for the surface: v-sync is performed only when the frame rate exceeds vertical frequency.
		eOpenGLDisplaySurfaceCreateFlagSyncAdaptiveBit = 0x2000,

		/// Disables vertical sync for the surface: buffers are swapped immediately upon request.
		eOpenGLDisplaySurfaceCreateFlagSyncDisabledBit = 0x4000,

		/// Enables vertical sync for the surface: swap is performed during a v-blank.
		eOpenGLDisplaySurfaceCreateFlagSyncVerticalBit = 0x8000,
	};

	/// @brief
	enum EOpenGLRenderContextCreateFlags : uint32
	{
		eOpenGLRenderContextCreateFlagEnableDebugBit = 0x1000,

		eOpenGLRenderContextCreateFlagForwardCompatibleBit = 0x2000,

		eOpenGLRenderContextCreateFlagEnableSharingBit = 0x0010,

		eOpenGLRenderContextCreateFlagShareWithCurrentBit = 0x0020
	};

	enum class EOpenGLAPIClass : enum_default_value_t
	{
		Desktop,
		GLES
	};

	/// @brief
	enum class EOpenGLAPIProfile : enum_default_value_t
	{
		Auto,
		Core,
		GLES,
		Legacy,
	};

	enum : exception_code_value_t
	{
		///
		eEXCSystemOpenGLAPIVersionNotSupported = Ic3::CxDef::declareExceptionCode( eExceptionCategorySystemOpenGL, 0x03 ),
		eEXCSystemOpenGLAPIProfileNotSupported = Ic3::CxDef::declareExceptionCode( eExceptionCategorySystemOpenGL, 0x04 ),
	};

	inline constexpr Version cxGLVersionBestSupported{Cppx::cxUint16Max, Cppx::cxUint16Max };

	inline constexpr Version cxGLVersionUnknown{0, 0 };

	inline constexpr Version cxGLVersionMaxDesktop{4, 6 };

	inline constexpr Version cxGLVersionMaxES{3, 2 };

	/// @brief
	struct OpenGLVersionSupportInfo
	{
		EOpenGLAPIClass mAPIClass;

		EOpenGLAPIProfile mAPIProfile;

		Version mAPIVersion;
	};

	/// @brief Represents combined info about the current OpenGL subsystem version.
	/// Basically, this struct stores the output from all version-related GL queries.
	struct OpenGLSystemVersionInfo
	{
	public:
		// Numeric version of the GL (GL_VERSION_MAJOR.GL_VERSION_MINOR)
		Version mAPIVersion;

		// Text version of the GL (GL_VERSION)
		std::string mAPIVersionStr;

		// Text version of the GLSL (GL_SHADING_LANGUAGE_VERSION)
		std::string mGLSLVersionStr;

		// Name of the renderer (GL_RENDERER_NAME)
		std::string mRendererName;

		// Name of the vendor (GL_VENDOR_NAME)
		std::string mVendorName;

	public:
		IC3_ATTR_NO_DISCARD	std::string toString() const;
	};

	struct OpenGLErrorInfo
	{
	public:
		// The error identifier. It will contain either a common OpenGL error
		// code (GLenum) or one of the subsystem-specific ones (AGL/EGL/GLX/WGL).
		uint32 mErrorCode;

		// Message describing the error, if provided. Some errors do not have
		// descriptions attached, so this can be an empty string in some cases.
		const char * mErrorString;

	public:
		constexpr OpenGLErrorInfo( bool pStatus )
		: mErrorCode( pStatus ? 0u : Cppx::QLimits<uint32>::sMaxValue )
		, mErrorString( Ic3::CxDef::STR_CHAR_EMPTY )
		{}

		template <typename TGLErrorCode>
		constexpr OpenGLErrorInfo( TGLErrorCode pErrorCode, const char * pErrorMessage = nullptr )
		: mErrorCode( numeric_cast<uint32>( pErrorCode ) )
		, mErrorString( pErrorMessage ? Ic3::CxDef::STR_CHAR_EMPTY : pErrorMessage )
		{}
	};

	class GLSystemException : public SystemException
	{
	public:
		OpenGLErrorInfo mOpenGLErrorInfo;

	public:
		explicit GLSystemException( ExceptionInfo pExceptionInfo )
		: SystemException( std::move( pExceptionInfo ) )
		, mOpenGLErrorInfo( true )
		{}

		GLSystemException( ExceptionInfo pExceptionInfo, OpenGLErrorInfo pOpenGLErrorInfo )
		: SystemException( std::move( pExceptionInfo ) )
		, mOpenGLErrorInfo( std::move( pOpenGLErrorInfo ) )
		{}
	};

	ic3SetExceptionCategoryType( eExceptionCategorySystemOpenGL, GLSystemException );

	class OpenGLCoreAPI
	{
	public:
		static Version queryRuntimeVersion();

		static bool checkLastResult();

		static bool checkLastError( GLenum pErrorCode );

		static void handleLastError();

		static void resetErrorQueue();

		static const char * translateErrorCode( GLenum pError );
	};

} // namespace Ic3::System

/// @def ic3OpenGLCheckLastResult
/// @brief A configuration-dependent macro which expands to either OpenGLCoreAPI::checkLastResult() or an empty statement.
/// @see IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS
/// @see Ic3::System::OpenGLCoreAPI::checkLastResult

/// @def ic3OpenGLCheckLastError
/// @brief A configuration-dependent macro which expands to either OpenGLCoreAPI::checkLastError() or an empty statement.
/// @see IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS
/// @see Ic3::System::OpenGLCoreAPI::checkLastError

/// @def ic3OpenGLHandleLastError
/// @brief A configuration-dependent macro which expands to either OpenGLCoreAPI::handleLastError() or an empty statement.
/// @see IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS
/// @see Ic3::System::OpenGLCoreAPI::handleLastError

/// @def ic3OpenGLResetErrorQueue
/// @brief A configuration-dependent macro which expands to either OpenGLCoreAPI::resetErrorQueue() or an empty statement.
/// @see IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS
/// @see Ic3::System::OpenGLCoreAPI::resetErrorQueue()

#if( IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS )
#  define ic3OpenGLCheckLastResult()            ::Ic3::System::OpenGLCoreAPI::checkLastResult()
#  define ic3OpenGLCheckLastError( pErrorCode ) ::Ic3::System::OpenGLCoreAPI::checkLastError( pErrorCode )
#  define ic3OpenGLHandleLastError()            ::Ic3::System::OpenGLCoreAPI::handleLastError()
#  define ic3OpenGLResetErrorQueue()            ::Ic3::System::OpenGLCoreAPI::resetErrorQueue()
#else
#  define ic3OpenGLCheckLastResult()
#  define ic3OpenGLCheckLastError( pErrorCode )
#  define ic3OpenGLHandleLastError()
#  define ic3OpenGLResetErrorQueue()
#endif

#endif // __IC3_SYSTEM_GFX_OPENGL_COMMON_H__
