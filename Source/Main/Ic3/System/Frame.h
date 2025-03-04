
#ifndef __IC3_SYSTEM_FRAME_H__
#define __IC3_SYSTEM_FRAME_H__

#include "EventSource.h"
#include <Ic3/Math/Vector.h>

namespace Ic3::System
{

	using FramePos = Math::Pos2i;
	using FrameSize = Math::Size2u;

	/// @brief Predefined frame position: auto. When used, frame is spawned centered.
	inline constexpr FramePos cxFramePosAuto {-1, -1 };

	/// @brief Predefined frame position: origin. When used, frame is spawned at the origin point (0,0).
	inline constexpr FramePos cxFramePosOrigin {0, 0 };

	/// @brief Predefined frame size: auto. When used, frame size is selected by the implementation.
	inline constexpr FrameSize cxFrameSizeAuto {0, 0 };

	/// @brief Predefined frame size: max. When used, frame size is the maximum size allowed by the system.
	/// Max size usually means the current size of the screen, but selected frame style may affect this (Win32).
	inline constexpr FrameSize cxFrameSizeMax {cppx::cve::uint32_max, cppx::cve::uint32_max };

	/// @brief Window styles supported by the system.
	enum class EFrameStyle : enum_default_value_t
	{
		//
		Unspecified,
		// Title-bar only window, no system menu, non-resizeable, no close or minimize button.
		Caption,
		// Fixed window, with system menu, non-resizeable.
		Fixed,
		//
		Overlay,
		// "Normal" window with title bar, minimize/maximize buttons (if applicable), resizeable.
		Resizeable,
		// Default style is the one which is most convenient for rendering - fixed frame.
		Default = static_cast< enum_default_value_t >( Fixed ),
	};

	enum EFrameGeometryUpdateFlags : uint32
	{
		eFrameGeometryUpdateFlagPositionBit = 0x01,

		eFrameGeometryUpdateFlagSizeBit = 0x02,

		eFrameGeometryUpdateFlagStyleBit = 0x08,

		eFrameGeometryUpdateFlagSizeClientAreaBit = 0x10 | eFrameGeometryUpdateFlagSizeBit,

		eFrameGeometryUpdateFlagSizeOuterRectBit = 0x20 | eFrameGeometryUpdateFlagSizeBit,

		eFrameGeometryUpdateMaskAllDefault =
				eFrameGeometryUpdateFlagPositionBit |
				eFrameGeometryUpdateFlagSizeOuterRectBit |
				eFrameGeometryUpdateFlagStyleBit
	};

	/// @brief
	enum class EFrameSizeMode : enum_default_value_t
	{
		ClientArea,
		OuterRect
	};

	/// @brief
	struct FrameGeometry
	{
		FramePos position;
		FrameSize size;
		EFrameStyle style;
	};

	inline constexpr FrameGeometry CX_FRAME_GEOMETRY_DEFAULT{cxFramePosAuto, cxFrameSizeAuto, EFrameStyle::Default };

	class Frame : public EventSource
	{
	public:
		explicit Frame( SysContextHandle pSysContext );
		virtual ~Frame() noexcept;

		virtual void ResizeClientArea( const FrameSize & pSize ) = 0;

		virtual void ResizeFrame( const FrameSize & pSize ) = 0;

		virtual void SetFullscreenMode( bool pEnable ) = 0;

		virtual void SetTitle( const std::string & pTitleText ) = 0;

		virtual void UpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) = 0;

		IC3_SYSTEM_API_NODISCARD virtual FrameSize GetClientAreaSize() const = 0;

		IC3_SYSTEM_API_NODISCARD virtual FrameSize GetFrameSize() const = 0;

		IC3_SYSTEM_API_NODISCARD virtual bool IsFullscreen() const = 0;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_FRAME_H__
