
#ifndef __IC3_SYSTEM_EVENT_DEF_INPUT_MOUSE_H__
#define __IC3_SYSTEM_EVENT_DEF_INPUT_MOUSE_H__

#include "EventDefInput.h"

namespace Ic3::System
{

	using MouseCursorPos = Math::Vec2i32;

	// Represents invalid mouse position. Used as a default value for last position registered.
	inline constexpr MouseCursorPos CX_EVENT_MOUSE_POS_INVALID {Cppx::cxInt32Max, Cppx::cxInt32Max };

	enum EMouseButtonFlagBits : uint32
	{
		eMouseButtonFlagLeftBit = 0x0001,
		eMouseButtonFlagRightBit = 0x0002,
		eMouseButtonFlagMiddleBit = 0x0010,
		eMouseButtonFlagXB1Bit = 0x0020,
		eMouseButtonFlagXB2Bit = 0x0040,
		eMouseButtonMaskAll = 0x00FF
	};

	enum class EMouseButtonID : enum_default_value_t
	{
		Unknown,
		Left = static_cast<enum_default_value_t>( eMouseButtonFlagLeftBit ),
		Right = static_cast<enum_default_value_t>( eMouseButtonFlagRightBit ),
		Middle = static_cast<enum_default_value_t>( eMouseButtonFlagMiddleBit ),
		XB1 = static_cast<enum_default_value_t>( eMouseButtonFlagXB1Bit ),
		XB2 = static_cast<enum_default_value_t>( eMouseButtonFlagXB2Bit ),
	};

	enum class EMouseButtonActionType : enum_default_value_t
	{
		Click,
		DoubleClick,
		MultiClick,
		Release
	};

	enum class EMouseScrollDirection : enum_default_value_t
	{
		Normal = 1,
		Inverted = 2
	};

	struct EvtSharedInputMouseState
	{
		// Last cursor position registered by the event system.
		MouseCursorPos mLastCursorPos = CX_EVENT_MOUSE_POS_INVALID;

		// Last mouse button pressed. Used to detect multi-click sequences.
		EMouseButtonID mLastPressButtonID = EMouseButtonID::Unknown;

		// Timestamp of last registered mouse button press.
		perf_counter_value_t mLastPressTimestamp = 0u;

		// Current sequence length, i.e. number of clicks of the same button in a row.
		uint32 mCurrentMultiClickSequenceLength = 1;

        TBitmask<EMouseButtonFlagBits> mButtonStateMask = 0;
	};

	struct EvtInputMouse : public EvtInput
	{
		TBitmask<EMouseButtonFlagBits> mButtonStateMask;

		Math::Vec2i32 mCursorPos;

		Math::Vec2i32 mCursorPosWindowRelative;

		const EvtSharedInputMouseState * mInputMouseState;
	};

	struct EvtInputMouseButton : public EvtInputMouse
	{
		//
		EMouseButtonActionType mButtonAction;

		//
		EMouseButtonID mButtonID;
	};

	struct EvtInputMouseMove : public EvtInputMouse
	{
		//
		Math::Vec2i32 mMovementDelta;
	};

	struct EvtInputMouseScroll : public EvtInputMouse
	{
		//
		Math::Vec2d mScrollDelta;

		//
		EMouseScrollDirection mScrollDirection;
	};


	namespace CxDef
	{

		inline constexpr EMouseButtonFlagBits getMouseButtonFlagFromButtonID( EMouseButtonID pButtonID )
		{
			return static_cast<EMouseButtonFlagBits>( static_cast<uint32>( pButtonID ) & eMouseButtonMaskAll );
		}

	}

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_DEF_INPUT_MOUSE_H__
