
#ifndef __IC3_SYSTEM_OPENGL_SYSTEM_DRIVER_H__
#define __IC3_SYSTEM_OPENGL_SYSTEM_DRIVER_H__

#include "OpenGLCommon.h"

namespace Ic3::System
{

	Ic3SysDeclareHandle( DisplayManager );
	Ic3SysDeclareHandle( OpenGLDisplaySurface );
	Ic3SysDeclareHandle( OpenGLRenderContext );

	/// @brief Contains parameters used to specify how a GL display surface should be created.
	struct OpenGLDisplaySurfaceCreateInfo : public WindowCreateInfo
	{
		/// Minimum version of the OpenGL API the surface should support. This value is Desktop-/ES-specific.
		cppx::version minimumAPIVersion;

		///
		EOpenGLAPIClass targetAPIClass;

		/// Creation flags, describing additional surface properties.
		cppx::bitmask<EOpenGLSurfaceCreateFlags> flags = 0;

	};

	/// @brief Contains parameters used to specify how a GL render context should be created.
	struct OpenGLRenderContextCreateInfo
	{
		/// Handle to an existing GLRenderContext used to share resources. If set to null, no sharing is done.
		OpenGLRenderContextHandle shareContext = nullptr;

		/// Target version of the API the context should support. Supported version will at least match the requested.
		cppx::version requestedAPIVersion = cxGLVersionBestSupported;

		/// Selected API profile the context should *at least* support.
		EOpenGLAPIProfile contextAPIProfile = EOpenGLAPIProfile::Auto;

		/// Creation flags, describing additional context properties.
		cppx::bitmask<EOpenGLRenderContextCreateFlags> flags = 0;
	};

	/// @brief
	class IC3_SYSTEM_CLASS OpenGLSystemDriver : public SysObject
	{
		friend class OpenGLDisplaySurface;
		friend class OpenGLRenderContext;

	public:
		DisplayManagerHandle const mDisplayManager;

	public:
        explicit OpenGLSystemDriver( DisplayManagerHandle pDisplayManager );
        virtual ~OpenGLSystemDriver() noexcept;

		/// @brief Initializes core OpenGL state and system-level interfaces.
		/// This method also creates any additionally required
		OpenGLVersionSupportInfo InitializePlatform();

		const OpenGLVersionSupportInfo & GetVersionSupportInfo() const;

		/// @brief Releases temporary init state created automatically by the library. Context must be a valid GL context.
		/// Call this method after you have created actual display surface and context. It's not mandatory, (everything
		/// will be released at shutdown anyway), but on some platforms this could free some extra memory and release
		/// couple system-level interfaces.
		void ReleaseInitState( OpenGLRenderContext & pRenderContext );

		/// @brief Creates a system OpenGL surface (usually - a window) with a configuration matching specified requirements.
		/// @param pCreateInfo CreateInfo struct with a surface specification (VisualConfig, window geometry, etc.)
		CPPX_ATTR_NO_DISCARD OpenGLDisplaySurfaceHandle CreateDisplaySurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo );

		/// @brief Creates a OpenGLDisplaySurface object, that wraps currently bound surface for the current thread.
		///
		/// This function creates a logical surface object without creating an actual (system-level) surface. It enables
		/// usage of the library API in environments where low-level GL stuff is created externally (a good example is
		/// the editor, where wxWidgets manages underlying GL surface/context). Important note: on some environments there
		/// might be no way to obtain all data required to Create a wrapper around surface. If that is the case, this
		/// method will throw an exception with EXC_NOT_SUPPORTED code.
		///
		/// @return A handle to an object representing currently bound OpenGL surface. If no surface is bound, a null is returned.
		///
		/// @see OpenGLDisplaySurface
		CPPX_ATTR_NO_DISCARD OpenGLDisplaySurfaceHandle CreateDisplaySurfaceForCurrentThread();

		/// @brief Creates a system OpenGL render context with a configuration matching specified requirements.
		/// @param pSurface Surface to be used for context creation. Context can be bound to any surface compatible with this one.
		/// @param pCreateInfo CreateInfo struct with a context specification (OpenGL API version, profile, etc.)
		///
		/// @see OpenGLDisplaySurface
		/// @see OpenGLRenderContextCreateInfo
		CPPX_ATTR_NO_DISCARD OpenGLRenderContextHandle CreateRenderContext(
				OpenGLDisplaySurface & pSurface,
				const OpenGLRenderContextCreateInfo & pCreateInfo );

		/// @brief Creates a OpenGLRenderContext object, that wraps currently bound context for the current thread.
		///
		/// This functions works similar to CreateDisplaySurfaceForCurrentThread(), but does that for the currently
		/// bound OpenGL render context. See description of CreateDisplaySurfaceForCurrentThread() for details.
		///
		/// @return A handle to an object representing currently bound OpenGL context. If no context is bound, a null is returned.
		CPPX_ATTR_NO_DISCARD OpenGLRenderContextHandle CreateRenderContextForCurrentThread();

		/// @brief
		void ResetContextBinding();

		/// @brief
		CPPX_ATTR_NO_DISCARD std::vector<EDepthStencilFormat> QuerySupportedDepthStencilFormats( EColorFormat pColorFormat ) const;

		/// @brief
		CPPX_ATTR_NO_DISCARD std::vector<EMSAAMode> QuerySupportedMSAAModes(
				EColorFormat pColorFormat,
				EDepthStencilFormat pDepthStencilFormat ) const;

		/// @brief
		CPPX_ATTR_NO_DISCARD bool IsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const;

		/// @brief
		CPPX_ATTR_NO_DISCARD bool IsRenderContextBound() const;

	friendapi:
		/// Used by the OpenGLDisplaySurface class. Releases OS-level state of the specified surface.
		virtual void ReleaseSystemDisplaySurface( OpenGLDisplaySurface & pDisplaySurface ) noexcept;

		/// Used by the OpenGLRenderContext class. Releases OS-level state of the specified context.
		virtual void ReleaseSystemRenderContext( OpenGLRenderContext & pRenderContext ) noexcept;

	private:
		virtual OpenGLVersionSupportInfo _NativeQueryVersionSupportInfo() const noexcept;

		virtual void _NativeInitializePlatform() = 0;

		virtual void _NativeReleaseInitState() noexcept = 0;

		virtual OpenGLDisplaySurfaceHandle _NativeCreateDisplaySurface(
				const OpenGLDisplaySurfaceCreateInfo & pCreateInfo ) = 0;

		virtual OpenGLDisplaySurfaceHandle _NativeCreateDisplaySurfaceForCurrentThread() = 0;

		virtual void _NativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface ) = 0;

		virtual OpenGLRenderContextHandle _NativeCreateRenderContext(
				OpenGLDisplaySurface & pSurface,
				const OpenGLRenderContextCreateInfo & pCreateInfo ) = 0;

		virtual OpenGLRenderContextHandle _NativeCreateRenderContextForCurrentThread() = 0;

		virtual void _NativeDestroyRenderContext( OpenGLRenderContext & pRenderContext ) = 0;

		virtual void _NativeResetContextBinding() = 0;

		virtual std::vector<EDepthStencilFormat> _NativeQuerySupportedDepthStencilFormats(
				EColorFormat pColorFormat ) const = 0;

		virtual std::vector<EMSAAMode> _NativeQuerySupportedMSAAModes(
				EColorFormat pColorFormat,
				EDepthStencilFormat pDepthStencilFormat ) const = 0;

		virtual bool _NativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const = 0;

		virtual bool _NativeIsRenderContextBound() const = 0;

	protected:
		OpenGLVersionSupportInfo _versionSupportInfo;
	};

	/// @brief
	class IC3_SYSTEM_CLASS OpenGLDisplaySurface : public Frame
	{
		friend class OpenGLSystemDriver;

	public:
		OpenGLSystemDriverHandle const mGLSystemDriver;

	public:
        explicit OpenGLDisplaySurface( OpenGLSystemDriverHandle pGLSystemDriver, void * pNativeData );
        virtual ~OpenGLDisplaySurface() noexcept;

		/// @brief
		void SwapBuffers();

		///
		CPPX_ATTR_NO_DISCARD EOpenGLAPIClass QuerySupportedAPIClass() const;

		///
		CPPX_ATTR_NO_DISCARD VisualConfig QueryVisualConfig() const;

		/// @brief
		CPPX_ATTR_NO_DISCARD FrameSize QueryRenderAreaSize() const;

		/// @brief
		CPPX_ATTR_NO_DISCARD bool SysValidate() const;

		/// @copybrief Frame::ResizeClientArea
		virtual void ResizeClientArea( const FrameSize & pSize ) override final;

        /// @copybrief Frame::ResizeFrame
        virtual void ResizeFrame( const FrameSize & pSize ) override final;

		/// @copybrief Frame::SetFullscreenMode
		virtual void SetFullscreenMode( bool pEnable ) override final;

		/// @copybrief Frame::SetTitle
		virtual void SetTitle( const std::string & pTitleText ) override final;

		/// @copybrief Frame::UpdateGeometry
		virtual void UpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

		/// @copybrief Frame::GetClientAreaSize
		CPPX_ATTR_NO_DISCARD virtual FrameSize GetClientAreaSize() const override final;

		/// @copybrief Frame::GetSize
		CPPX_ATTR_NO_DISCARD virtual FrameSize GetFrameSize() const override final;

		/// @copybrief Frame::IsFullscreen
		CPPX_ATTR_NO_DISCARD virtual bool IsFullscreen() const override final;

	protected:
		virtual void OnDestroySystemObjectRequested() override;

		void SetInternalOwnershipFlag( bool pOwnershipFlag );

		bool HasInternalOwnershipFlag() const;

	private:
		virtual void _NativeSwapBuffers() = 0;

		virtual EOpenGLAPIClass _NativeQuerySupportedAPIClass() const noexcept = 0;

		virtual VisualConfig _NativeQueryVisualConfig() const = 0;

		virtual FrameSize _NativeQueryRenderAreaSize() const = 0;

		virtual bool _NativeSysValidate() const = 0;

		virtual void _NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) = 0;

		virtual void _NativeSetFullscreenMode( bool pEnable ) = 0;

		virtual void _NativeSetTitle( const std::string & pTitle ) = 0;

		virtual void _NativeUpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) = 0;

		virtual FrameSize _NativeGetSize( EFrameSizeMode pSizeMode ) const = 0;

		virtual bool _NativeIsFullscreen() const = 0;

	private:
		bool _internalOwnershipFlag = true;
	};

	/// @brief
	class IC3_SYSTEM_CLASS OpenGLRenderContext : public SysObject
	{
		friend class OpenGLSystemDriver;

	public:
		OpenGLSystemDriverHandle const mGLSystemDriver;

	public:
        explicit OpenGLRenderContext( OpenGLSystemDriverHandle pGLSystemDriver );
        virtual ~OpenGLRenderContext() noexcept;

		/// @brief
		void BindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface );

		/// @brief
		CPPX_ATTR_NO_DISCARD bool SysCheckIsCurrent() const;

		/// @brief
		CPPX_ATTR_NO_DISCARD bool SysValidate() const;

		/// @brief
		CPPX_ATTR_NO_DISCARD OpenGLSystemVersionInfo QuerySystemVersionInfo() const;

	protected:
		virtual void OnDestroySystemObjectRequested() override;

		void SetInternalOwnershipFlag( bool pOwnershipFlag );

		bool HasInternalOwnershipFlag() const;

	private:
		virtual void _NativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface ) = 0;

		virtual bool _NativeSysCheckIsCurrent() const = 0;

		virtual bool _NativeSysValidate() const = 0;

	private:
		bool _internalOwnershipFlag = true;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_OPENGL_SYSTEM_DRIVER_H__
