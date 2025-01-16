
#include "EGLOpenGL.h"
#include <Ic3/System/OpenGLDriver.h>

namespace Ic3::System
{

	namespace Platform
	{

		// Returns an array of EGLConfigs matching specified VisualConfig definition and API version (ES).
		std::vector<EGLConfig> _EAQueryCompatibleEGLConfigList(
				EGLDisplay pDisplay,
				const VisualConfig & pVisualConfig,
				const cppx::version & pTargetAPIVersion );

		// Computes a "compatibility rate", i.e. how much the specified FBConfig matches the visual.
		int _EAGetEGLConfigMatchRate(
				EGLDisplay pDisplay,
				EGLConfig pEGLConfig,
				const VisualConfig & pVisualConfig );

		// Translation: VisualConfig+APIVersion --> array of EGL_* attributes required by the system API.
		// Used for surface/context creation.
		void _EAGetAttribArrayForVisualConfig(
				const VisualConfig & pVisualConfig,
				const cppx::version & pTargetAPIVersion,
				int * pAttribArray );

		// Creates an EGL rendering context for the Core/Legacy API profile.
		// Uses full set of context creation features (debug and forward compatibility flags, detailed version, etc).
		EGLContext _EACreateCoreContextDefault(
				EGLRenderContextNativeData & pEGLContextNativeData,
				const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
				const OpenGLRenderContextCreateInfo & pCreateInfo,
				EGLContext pEGLShareContext );

		// Creates an EGL rendering context for the ES API profile.
		// Uses only the client API version number.
		EGLContext _EACreateCoreContextES(
				EGLRenderContextNativeData & pEGLContextNativeData,
				const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
				const cppx::version & pTargetAPIVersion,
				EGLContext pEGLShareContext );

		// Validates specified API version using selected target API profile (Core/ES).
		// 'cppx::version' parameter is an IN/OUT parameter and updated with a valid version number.
		void _EAValidateRequestedContextVersion( EOpenGLAPIClass pTargetAPIClass, cppx::version & pVersion );


		void Platform::EAInitializeGLDriver( EGLDriverNativeData & pEGLDriverNativeData )
		{
			EGLDisplay eglDisplay = ::eglGetDisplay( EGL_DEFAULT_DISPLAY );
			if( eglDisplay == EGL_NO_DISPLAY )
			{
				ic3EGLThrowLastError();
			}

			EGLint eglVersionMajor = 0;
			EGLint eglVersionMinor = 0;
			auto initResult = ::eglInitialize( eglDisplay, &eglVersionMajor, &eglVersionMinor );
			if( initResult == EGL_FALSE )
			{
				ic3EGLThrowLastError();
			}

			auto eglQueriedVersion = EGLCoreAPI::QueryRuntimeVersion();

			pEGLDriverNativeData.eglDisplay = eglDisplay;
			pEGLDriverNativeData.eglVersion.num_major = eglVersionMajor;
			pEGLDriverNativeData.eglVersion.mNumMinor = eglVersionMinor;
		}

		void Platform::EAReleaseGLDriver( EGLDriverNativeData & pEGLDriverNativeData )
		{
			auto eglResult = ::eglTerminate( pEGLDriverNativeData.eglDisplay );
			pEGLDriverNativeData.eglDisplay = EGL_NO_DISPLAY;
			pEGLDriverNativeData.eglVersion.num_major = 0;
			pEGLDriverNativeData.eglVersion.mNumMinor = 0;
		}

		EGLConfig Platform::EAChooseCoreFBConfig(
				EGLDisplay pDisplay,
				const VisualConfig & pVisualConfig,
				const cppx::version & pTargetAPIVersion )
		{
			auto eglConfigList = _EAQueryCompatibleEGLConfigList( pDisplay, pVisualConfig, pTargetAPIVersion );

			int bestMatchRate = 0;
			EGLConfig bestEGLConfig = nullptr;

			for( auto eglConfig : eglConfigList )
			{
				int matchRate = _EAGetEGLConfigMatchRate( pDisplay, eglConfig, pVisualConfig );
				if( matchRate > bestMatchRate )
				{
					bestMatchRate = matchRate;
					bestEGLConfig = eglConfig;
				}
			}

			// return bestEGLConfig;
			return eglConfigList[0];
		}

		EGLint Platform::EAQueryFBConfigAttribute( EGLDisplay pEGLDisplay, EGLConfig pEGLConfig, EGLenum pAttribute )
		{
			EGLint fbConfigAttribute = 0;

			auto eglResult = ::eglGetConfigAttrib( pEGLDisplay, pEGLConfig, pAttribute, &fbConfigAttribute );
			if( eglResult == EGL_FALSE )
			{
				ic3EGLThrowLastError();
			}

			return fbConfigAttribute;
		}

		void Platform::EACreateSurface(
				EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
				EGLDisplay pEGLDisplay,
				EGLNativeWindowType pWindow,
				EGLConfig pEGLConfig,
				const VisualConfig & pVisualConfig )
		{
			const EGLint defaultSurfaceAttributeList[] = { EGL_NONE };
			const EGLint sRGBSurfaceAttributeList[] = { EGL_GL_COLORSPACE, EGL_GL_COLORSPACE_SRGB, EGL_NONE };

			const EGLint * surfaceAttributeList = &( defaultSurfaceAttributeList[0] );
			if( pVisualConfig.colorDesc.colorSpace == EColorSpace::SRGB )
			{
				surfaceAttributeList = &( sRGBSurfaceAttributeList[0] );
			}

			auto surfaceHandle = ::eglCreateWindowSurface( pEGLDisplay, pEGLConfig, pWindow, surfaceAttributeList );
			if( surfaceHandle == EGL_NO_SURFACE )
			{
				ic3EGLThrowLastError();
			}

			pEGLSurfaceNativeData.eglDisplay = pEGLDisplay;
			pEGLSurfaceNativeData.eglNativeWindowHandle = pWindow;
			pEGLSurfaceNativeData.eglSurfaceHandle = surfaceHandle;
			pEGLSurfaceNativeData.eglFBConfig = pEGLConfig;
		}

		void Platform::EACreateSurfaceForCurrentThread( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData )
		{
			auto eDisplay = ::eglGetCurrentDisplay();
			auto eSurfaceHandle = ::eglGetCurrentSurface( EGL_DRAW );

			EGLint surfaceConfigID = 0;

			auto eglResult = ::eglQuerySurface( eDisplay, eSurfaceHandle, EGL_CONFIG_ID, &surfaceConfigID );
			if( eglResult == EGL_FALSE )
			{
				ic3EGLThrowLastError();
			}

			EGLConfig surfaceConfig;
			EGLint resultConfigsNum;
			EGLint scAttributeList[] = { EGL_CONFIG_ID, surfaceConfigID, EGL_NONE };

			eglResult = ::eglChooseConfig( eDisplay, &( scAttributeList[0] ), &surfaceConfig, 1, &resultConfigsNum );
			if( eglResult == EGL_FALSE )
			{
				ic3EGLThrowLastError();
			}

			pEGLSurfaceNativeData.eglDisplay = eDisplay;
			pEGLSurfaceNativeData.eglSurfaceHandle = eSurfaceHandle;
			pEGLSurfaceNativeData.eglFBConfig = surfaceConfig;
		}

		void Platform::EADestroySurface( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData )
		{
			auto eglResult = ::EADestroySurface( pEGLSurfaceNativeData.eglDisplay, pEGLSurfaceNativeData.eglSurfaceHandle );

			pEGLSurfaceNativeData.eglDisplay = nullptr;
			pEGLSurfaceNativeData.eglFBConfig = nullptr;
			pEGLSurfaceNativeData.eglSurfaceHandle = nullptr;
			pEGLSurfaceNativeData.eglNativeWindowHandle = nullptr;
		}

		void Platform::EACreateCoreContext(
				EGLRenderContextNativeData & pEGLContextNativeData,
				const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
				const OpenGLRenderContextCreateInfo & pCreateInfo )
		{
			auto createInfo = pCreateInfo;

			_EAValidateRequestedContextVersion(
					createInfo.runtimeVersionDesc.apiProfile,
					createInfo.runtimeVersionDesc.apiVersion );

			EGLContext shareContextHandle = EGL_NO_CONTEXT;
			EGLContext contextHandle = EGL_NO_CONTEXT;

			if( createInfo.flags.is_set( E_OPENGL_RENDER_CONTEXT_CREATE_FLAG_ENABLE_SHARING_BIT ) )
			{
				if( createInfo.shareContext )
				{
					shareContextHandle = createInfo.shareContext->mNativeData->eContextHandle;
				}
				else if( pCreateInfo.flags.is_set( E_OPENGL_RENDER_CONTEXT_CREATE_FLAG_SHARE_WITH_CURRENT_BIT ) )
				{
					if( auto * currentEGLContext = ::eglGetCurrentContext() )
					{
						shareContextHandle = currentEGLContext;
					}
				}
			}

			if( createInfo.runtimeVersionDesc.apiProfile != EOpenGLAPIClass::OpenGLES )
			{
				contextHandle = _EACreateCoreContextDefault(
						pEGLContextNativeData,
						pEGLSurfaceNativeData,
						createInfo,
						shareContextHandle );
			}

			if( ( createInfo.runtimeVersionDesc.apiProfile == EOpenGLAPIClass::OpenGLES ) || !contextHandle )
			{
				contextHandle = _EACreateCoreContextES(
						pEGLContextNativeData,
						pEGLSurfaceNativeData,
						createInfo.runtimeVersionDesc.apiVersion,
						shareContextHandle );
			}

			if( !contextHandle )
			{
				ic3EGLThrowError( "Failed to create EGL context" );
			}

			pEGLContextNativeData.eglDisplay = pEGLSurfaceNativeData.eglDisplay;
			pEGLContextNativeData.eContextHandle = contextHandle;
		}

		void Platform::EACreateCoreContextForCurrentThread( EGLRenderContextNativeData & pEGLContextNativeData )
		{
			auto eDisplay = ::eglGetCurrentDisplay();
			auto eContextHandle = ::eglGetCurrentContext();

			pEGLContextNativeData.eglDisplay = eDisplay;
			pEGLContextNativeData.eContextHandle = eContextHandle;
		}

		void Platform::EADestroyRenderContext( EGLRenderContextNativeData & pEGLContextNativeData )
		{
			if( pEGLContextNativeData.eContextHandle != nullptr )
			{
				auto eCurrentContext = ::eglGetCurrentContext();
				if( pEGLContextNativeData.eContextHandle == eCurrentContext )
				{
					::eglMakeCurrent( pEGLContextNativeData.eglDisplay, nullptr, nullptr, nullptr );
				}

				::eglDestroyContext( pEGLContextNativeData.eglDisplay, pEGLContextNativeData.eContextHandle );

				pEGLContextNativeData.eglDisplay = nullptr;
				pEGLContextNativeData.eContextHandle = nullptr;
			}
		}

		void Platform::EABindContextForCurrentThread(
				const EGLRenderContextNativeData & pEGLContextNativeData,
				const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData )
		{
			::eglMakeCurrent(
					pEGLContextNativeData.eglDisplay,
					pEGLSurfaceNativeData.eglSurfaceHandle,
					pEGLSurfaceNativeData.eglSurfaceHandle,
					pEGLContextNativeData.eContextHandle );
		}


		std::vector<EGLConfig> _EAQueryCompatibleEGLConfigList(
				EGLDisplay pDisplay,
				const VisualConfig & pVisualConfig,
				const cppx::version & pTargetAPIVersion )
		{
			std::vector<EGLConfig> result;

			int eglConfigAttribArray[cxEGLMaxEGLConfigAttributesNum];
			_EAGetAttribArrayForVisualConfig( pVisualConfig, pTargetAPIVersion, eglConfigAttribArray );

			// Output array where system will store IDs of enumerated pixel formats.
			EGLConfig eglConfigArray[cxEGLMaxEGLConfigsNum];
			// Number of pixel formats returned by the system.
			EGLint returnedEGLConfigsNum = 0U;

			// Enumerate framebuffer configs.
			EGLBoolean enumResult = ::eglChooseConfig(
					pDisplay,
					eglConfigAttribArray,
					eglConfigArray,
					cxEGLMaxEGLConfigsNum,
					&returnedEGLConfigsNum );

			if( enumResult == EGL_FALSE )
			{
				ic3EGLThrowLastError();
			}

			if( returnedEGLConfigsNum <= 0 )
			{
				ic3EGLThrowError( "No matching EGLConfigs found." );
			}

			result.reserve( static_cast<size_t>( returnedEGLConfigsNum ) );
			result.assign( &( eglConfigArray[0] ), &( eglConfigArray[returnedEGLConfigsNum] ) );

			return result;
		}

		int _EAGetEGLConfigMatchRate( EGLDisplay pDisplay, EGLConfig pEGLConfig, const VisualConfig & pVisualConfig )
		{
			int matchRate = 0;
			int EGLConfigAttribValue = 0;

			::eglGetConfigAttrib( pDisplay, pEGLConfig, EGL_DEPTH_SIZE, &EGLConfigAttribValue );
			ic3EGLHandleLastError();
			matchRate += ( EGLConfigAttribValue == pVisualConfig.depthStencilDesc.depthBufferSize );

			::eglGetConfigAttrib( pDisplay, pEGLConfig, EGL_STENCIL_SIZE, &EGLConfigAttribValue );
			ic3EGLHandleLastError();
			matchRate += ( EGLConfigAttribValue == pVisualConfig.depthStencilDesc.stencilBufferSize );

			::eglGetConfigAttrib( pDisplay, pEGLConfig, EGL_SAMPLES, &EGLConfigAttribValue );
			ic3EGLHandleLastError();
			matchRate += ( EGLConfigAttribValue == pVisualConfig.msaaDesc.quality );

			return matchRate;
		}

		void _EAGetAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, const cppx::version & pTargetAPIVersion, int * pAttribArray )
		{
			int attribIndex = 0;
			int renderableType = 0;

			if( pTargetAPIVersion.num_major == 1 )
			{
				renderableType = EGL_OPENGL_ES_BIT;
			}
			else if( pTargetAPIVersion.num_major == 2 )
			{
				renderableType = EGL_OPENGL_ES2_BIT;
			}
			else if( pTargetAPIVersion.num_major == 3 )
			{
				renderableType = EGL_OPENGL_ES3_BIT;
			}

			pAttribArray[attribIndex++] = EGL_RENDERABLE_TYPE;
			pAttribArray[attribIndex++] = renderableType;

			pAttribArray[attribIndex++] = EGL_SURFACE_TYPE;
			pAttribArray[attribIndex++] = EGL_WINDOW_BIT;

			pAttribArray[attribIndex++] = EGL_COLOR_BUFFER_TYPE;
			pAttribArray[attribIndex++] = EGL_RGB_BUFFER;

			if( ( pVisualConfig.msaaDesc.bufferCount != 0 ) && ( pVisualConfig.msaaDesc.quality != 0 ) )
			{
				pAttribArray[attribIndex++] = EGL_SAMPLE_BUFFERS;
				pAttribArray[attribIndex++] = pVisualConfig.msaaDesc.bufferCount;

				pAttribArray[attribIndex++] = EGL_SAMPLES;
				pAttribArray[attribIndex++] = pVisualConfig.msaaDesc.quality;
			}

			if( pVisualConfig.colorDesc.rgba.u32Code != 0 )
			{
				pAttribArray[attribIndex++] = EGL_RED_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.u8Red;

				pAttribArray[attribIndex++] = EGL_GREEN_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.u8Green;

				pAttribArray[attribIndex++] = EGL_BLUE_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.u8Blue;

				pAttribArray[attribIndex++] = EGL_ALPHA_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.u8Alpha;
			}

			if( pVisualConfig.depthStencilDesc.depthBufferSize != 0 )
			{
				pAttribArray[attribIndex++] = EGL_DEPTH_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.depthStencilDesc.depthBufferSize;
			}

			if( pVisualConfig.depthStencilDesc.stencilBufferSize != 0 )
			{
				pAttribArray[attribIndex++] = EGL_STENCIL_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.depthStencilDesc.stencilBufferSize;
			}

			pAttribArray[attribIndex++] = EGL_NONE;
		}

		EGLContext _EACreateCoreContextDefault(
				EGLRenderContextNativeData & pEGLContextNativeData,
				const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
				const OpenGLRenderContextCreateInfo & pCreateInfo,
				EGLContext pEGLShareContext )
		{
			auto currentAPI = ::eglQueryAPI();
			if( currentAPI == EGL_OPENGL_API )
			{
				auto eglBindResult = ::eglBindAPI( EGL_OPENGL_API );
				if( eglBindResult == EGL_FALSE )
				{
					ic3EGLThrowLastError();
				}
			}

			int contextAPIProfile = 0;
			if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Core )
			{
				contextAPIProfile = EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT;
			}
			else if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Legacy )
			{
				contextAPIProfile = EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT;
			}

			EGLint debugContextFlag = EGL_FALSE;
			if( pCreateInfo.flags.is_set( E_OPENGL_RENDER_CONTEXT_CREATE_FLAG_ENABLE_DEBUG_BIT ) )
			{
				debugContextFlag = EGL_TRUE;
			}

			EGLint forwardCompatibleContextFlag = EGL_FALSE;
			if( pCreateInfo.flags.is_set( E_OPENGL_RENDER_CONTEXT_CREATE_FLAG_FORWARD_COMPATIBLE_BIT ) )
			{
				forwardCompatibleContextFlag = EGL_TRUE;
			}

			const EGLint contextAttributesCore[] =
			{
				// Requested OpenGL API version: major part
				EGL_CONTEXT_MAJOR_VERSION, pCreateInfo.runtimeVersionDesc.apiVersion.num_major,
				// Requested OpenGL API version: minor part
				EGL_CONTEXT_MINOR_VERSION, pCreateInfo.runtimeVersionDesc.apiVersion.mNumMinor,
				//
				EGL_CONTEXT_OPENGL_PROFILE_MASK, contextAPIProfile,
				//
				EGL_CONTEXT_OPENGL_DEBUG, debugContextFlag,
				//
				EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, forwardCompatibleContextFlag,
				// Terminator
				EGL_NONE
			};

			EGLContext contextHandle = ::eglCreateContext(
					pEGLSurfaceNativeData.eglDisplay,
					pEGLSurfaceNativeData.eglFBConfig,
					pEGLShareContext,
					contextAttributesCore );

			if( contextHandle == EGL_NO_CONTEXT )
			{
				ic3EGLThrowLastError();
			}

			return contextHandle;
		}

		EGLContext _EACreateCoreContextES(
				EGLRenderContextNativeData & pEGLContextNativeData,
				const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
				const cppx::version & pTargetAPIVersion,
				EGLContext pEGLShareContext )
		{
			auto currentAPI = ::eglQueryAPI();
			if( currentAPI == EGL_OPENGL_ES_API )
			{
				auto eglBindResult = ::eglBindAPI( EGL_OPENGL_ES_API );
				if( eglBindResult == EGL_FALSE )
				{
					ic3EGLThrowLastError();
				}
			}

			const EGLint contextAttributesES[] =
			{
				//
				EGL_CONTEXT_CLIENT_VERSION, pTargetAPIVersion.num_major,
				// Terminator
				EGL_NONE
			};

			EGLContext contextHandle = ::eglCreateContext(
					pEGLSurfaceNativeData.eglDisplay,
					pEGLSurfaceNativeData.eglFBConfig,
					pEGLShareContext,
					contextAttributesES );

			if( contextHandle == EGL_NO_CONTEXT )
			{
				ic3EGLThrowLastError();
			}

			return contextHandle;
		}

		void _EAValidateRequestedContextVersion( EOpenGLAPIClass pTargetAPIClass, cppx::version & pVersion )
		{
			auto validatedVersion = cppx::cve::version_invalid;

			if( pTargetAPIClass == EOpenGLAPIClass::OpenGLES )
			{
				const cppx::version esVersionList[] =
				{
					{ 1, 0 },
					{ 1, 1 },
					{ 2, 0 },
					{ 3, 0 },
					{ 3, 1 },
					{ 3, 2 }
				};

				for( const auto & esVersion : esVersionList )
				{
					if( pVersion == esVersion )
					{
						validatedVersion = pVersion;
						break;
					}
				}
			}
			else
			{
				const cppx::version coreVersionList[] =
				{
					{ 1, 0 },
					{ 1, 1 },
					{ 1, 2 },
					{ 1, 3 },
					{ 1, 4 },
					{ 1, 5 },
					{ 2, 0 },
					{ 2, 1 },
					{ 3, 0 },
					{ 3, 1 },
					{ 3, 2 },
					{ 3, 3 },
					{ 4, 0 },
					{ 4, 1 },
					{ 4, 2 },
					{ 4, 3 },
					{ 4, 4 },
					{ 4, 5 },
					{ 4, 6 },
				};

				for( const auto & coreVersion : coreVersionList )
				{
					if( pVersion == coreVersion )
					{
						validatedVersion = pVersion;
						break;
					}
				}
			}

			if( validatedVersion == cppx::cve::version_invalid )
			{
				validatedVersion = { 1, 0 };
			}

			pVersion = validatedVersion;
		}

	}

} // namespace Ic3::System
