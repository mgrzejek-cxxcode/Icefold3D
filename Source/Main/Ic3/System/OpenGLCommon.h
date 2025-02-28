
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

#if( PCL_TARGET_OS & PCL_TARGET_FLAG_OS_DESKTOP )
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
/// @see Ic3OpenGLCheckLastResult
/// @see Ic3OpenGLCheckLastError
/// @see Ic3OpenGLHandleLastError
/// @see Ic3OpenGLResetErrorQueue
#if !defined( IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS )
#  if( IC3_RELEASE_OPT_MAX )
#    define IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS 0
#  else
#    define IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS 1
#  endif
#endif

namespace Ic3::System
{

	Ic3SysDeclareHandle( OpenGLSystemDriver );
	Ic3SysDeclareHandle( OpenGLDisplaySurface );
	Ic3SysDeclareHandle( OpenGLRenderContext );

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
		eEXCSystemOpenGLAPIVersionNotSupported = Ic3::CXU::DeclareExceptionCode( eExceptionCategorySystemOpenGL, 0x03 ),
		eEXCSystemOpenGLAPIProfileNotSupported = Ic3::CXU::DeclareExceptionCode( eExceptionCategorySystemOpenGL, 0x04 ),
	};

	inline constexpr cppx::version cxGLVersionBestSupported{cppx::cve::uint16_max, cppx::cve::uint16_max };

	inline constexpr cppx::version cxGLVersionUnknown{0, 0 };

	inline constexpr cppx::version cxGLVersionMaxDesktop{4, 6 };

	inline constexpr cppx::version cxGLVersionMaxES{3, 2 };

	/// @brief
	struct OpenGLVersionSupportInfo
	{
		EOpenGLAPIClass apiClass;

		EOpenGLAPIProfile apiProfile;

		cppx::version apiVersion;
	};

	/// @brief Represents combined info about the current OpenGL subsystem version.
	/// Basically, this struct stores the output from all version-related GL queries.
	struct OpenGLSystemVersionInfo
	{
	public:
		// Numeric version of the GL (GL_VERSION_MAJOR.GL_VERSION_MINOR)
		cppx::version apiVersion;

		// Text version of the GL (GL_VERSION)
		std::string apiVersionStr;

		// Text version of the GLSL (GL_SHADING_LANGUAGE_VERSION)
		std::string glslVersionStr;

		// Name of the renderer (GL_RENDERER_NAME)
		std::string rendererName;

		// Name of the vendor (GL_VENDOR_NAME)
		std::string vendorName;

	public:
		CPPX_ATTR_NO_DISCARD	std::string ToString() const;
	};

	struct OpenGLErrorInfo
	{
	public:
		// The error identifier. It will contain either a common OpenGL error
		// code (GLenum) or one of the subsystem-specific ones (AGL/EGL/GLX/WGL).
		uint32 errorCode;

		// Message describing the error, if provided. Some errors do not have
		// descriptions attached, so this can be an empty string in some cases.
		const char * errorString;

	public:
		constexpr OpenGLErrorInfo( bool pStatus )
		: errorCode( pStatus ? 0u : cppx::meta::limits<uint32>::max_value )
		, errorString( Ic3::kStrCharEmpty )
		{}

		template <typename TGLErrorCode>
		constexpr OpenGLErrorInfo( TGLErrorCode pErrorCode, const char * pErrorMessage = nullptr )
		: errorCode( cppx::numeric_cast<uint32>( pErrorCode ) )
		, errorString( pErrorMessage ? Ic3::kStrCharEmpty : pErrorMessage )
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

	Ic3SetExceptionCategoryType( eExceptionCategorySystemOpenGL, GLSystemException );

	class OpenGLCoreAPI
	{
	public:
		static cppx::version QueryRuntimeVersion();

		static bool CheckLastResult();

		static bool CheckLastError( GLenum pErrorCode );

		static void HandleLastError();

		static void ResetErrorQueue();

		static const char * translateErrorCode( GLenum pError );
	};

} // namespace Ic3::System

/// @def Ic3OpenGLCheckLastResult
/// @brief A configuration-dependent macro which expands to either OpenGLCoreAPI::CheckLastResult() or an empty statement.
/// @see IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS
/// @see Ic3::System::OpenGLCoreAPI::CheckLastResult

/// @def Ic3OpenGLCheckLastError
/// @brief A configuration-dependent macro which expands to either OpenGLCoreAPI::CheckLastError() or an empty statement.
/// @see IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS
/// @see Ic3::System::OpenGLCoreAPI::CheckLastError

/// @def Ic3OpenGLHandleLastError
/// @brief A configuration-dependent macro which expands to either OpenGLCoreAPI::HandleLastError() or an empty statement.
/// @see IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS
/// @see Ic3::System::OpenGLCoreAPI::HandleLastError

/// @def Ic3OpenGLResetErrorQueue
/// @brief A configuration-dependent macro which expands to either OpenGLCoreAPI::ResetErrorQueue() or an empty statement.
/// @see IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS
/// @see Ic3::System::OpenGLCoreAPI::ResetErrorQueue()

#if( IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS )
#  define Ic3OpenGLCheckLastResult()            ::Ic3::System::OpenGLCoreAPI::CheckLastResult()
#  define Ic3OpenGLCheckLastError( pErrorCode ) ::Ic3::System::OpenGLCoreAPI::CheckLastError( pErrorCode )
#  define Ic3OpenGLHandleLastError()            ::Ic3::System::OpenGLCoreAPI::HandleLastError()
#  define Ic3OpenGLResetErrorQueue()            ::Ic3::System::OpenGLCoreAPI::ResetErrorQueue()
#else
#  define Ic3OpenGLCheckLastResult()
#  define Ic3OpenGLCheckLastError( pErrorCode )
#  define Ic3OpenGLHandleLastError()
#  define Ic3OpenGLResetErrorQueue()
#endif

#endif // __IC3_SYSTEM_GFX_OPENGL_COMMON_H__
