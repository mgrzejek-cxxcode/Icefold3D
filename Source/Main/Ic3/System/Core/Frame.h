
#ifndef __IC3_SYSTEM_FRAME_H__
#define __IC3_SYSTEM_FRAME_H__

#include "WindowCommon.h"
#include "../Events/EventSource.h"

namespace Ic3::System
{

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

	class IC3_SYSTEM_CLASS Frame : public EventSource
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

		CPPX_ATTR_NO_DISCARD virtual FrameSize GetClientAreaSize() const = 0;

		CPPX_ATTR_NO_DISCARD virtual FrameSize GetFrameSize() const = 0;

		CPPX_ATTR_NO_DISCARD virtual bool IsFullscreen() const = 0;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_FRAME_H__
