
#ifndef __IC3_SYSTEM_WINDOW_COMMON_H__
#define __IC3_SYSTEM_WINDOW_COMMON_H__

#include "Visual.h"

#include <cxm/vector.h>

namespace Ic3::System
{

    Ic3SysDeclareHandle( Window );
	Ic3SysDeclareHandle( WindowManager );

	using FramePos = cxm::pos2i;
	using FrameSize = cxm::size2u;

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

	/// @brief
	struct FrameGeometry
	{
		FramePos position;
		FrameSize size;
		EFrameStyle style;
	};

	inline constexpr FrameGeometry CX_FRAME_GEOMETRY_DEFAULT{ cxFramePosAuto, cxFrameSizeAuto, EFrameStyle::Default };

	struct WindowCreateInfo
	{
		/// Geometry of the window's frame (position, size, style, etc).
		FrameGeometry frameGeometry = CX_FRAME_GEOMETRY_DEFAULT;

		/// Window title (text visible on the title bar - if present).
		std::string title;

		/// Visual config for the window. Describes primary rendering properties of the surface.
		VisualConfig visualConfig;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_WINDOW_COMMON_H__
