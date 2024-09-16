
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_NS_NS_KEYBOARD_SUPPORT_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_NS_NS_KEYBOARD_SUPPORT_H__

#include "NSICommon.h"
#include <Ic3/System/EventDefInputKeyboard.h>

namespace Ic3::System
{

	/// @brief This enum contains OSX-specific codes for modifier keys (control, shift, etc).
	/// These are really obfuscated in the Objective-C system API and some of them don't seem to  appear there at all.
	/// We use these values directly to examine flags present in the input events coming from the system.
	enum EOSXModifierKeyFlags : uint32
	{
		eOSXModifierKeyFlagCommandLeftBit  = 8,
		eOSXModifierKeyFlagCommandRightBit = 16,
		eOSXModifierKeyFlagControlLeftBit  = 1,
		eOSXModifierKeyFlagControlRightBit = 8192,
		eOSXModifierKeyFlagOptionLeftBit   = 32,
		eOSXModifierKeyFlagOptionRightBit  = 64,
		eOSXModifierKeyFlagShiftLeftBit    = 2,
		eOSXModifierKeyFlagShiftRightBit   = 4,
		eOSXModifierKeyMapMaskCommandAny = eOSXModifierKeyFlagCommandLeftBit | eOSXModifierKeyFlagCommandRightBit,
		eOSXModifierKeyMapMaskControlAny = eOSXModifierKeyFlagControlLeftBit | eOSXModifierKeyFlagControlRightBit,
		eOSXModifierKeyMapMaskOptionAny = eOSXModifierKeyFlagOptionLeftBit | eOSXModifierKeyFlagOptionRightBit,
		eOSXModifierKeyMapMaskShiftAny = eOSXModifierKeyFlagShiftLeftBit | eOSXModifierKeyFlagShiftRightBit,
	};

	TBitmask<EKeyModifierFlags> osxTranslateModifierKeyFlags( TBitmask<NSEventModifierFlags> pModifierFlags )
	{
		TBitmask<EKeyModifierFlags> resultMask = 0;

		const auto deviceIndependentFlags = makeBitmask<uint32>( pModifierFlags & NSEventModifierFlagDeviceIndependentFlagsMask );
		const auto deviceSpecificFlags = makeBitmask<uint32>( pModifierFlags & ( 0xFFFF ) );

		if( deviceIndependentFlags.isSet( NSEventModifierFlagCommand ) )
		{
			if( deviceSpecificFlags.isSetAnyOf( eOSXModifierKeyFlagCommandRightBit ) )
			{
				resultMask.set( eKeyModifierFlagAltRightBit );
			}
			else
			{
				resultMask.set( eKeyModifierFlagAltLeftBit );
			}
		}

		if( deviceIndependentFlags.isSet( NSEventModifierFlagControl ) )
		{
			if( deviceSpecificFlags.isSetAnyOf( eOSXModifierKeyFlagControlRightBit ) )
			{
				resultMask.set( eKeyModifierFlagControlRightBit );
			}
			else
			{
				resultMask.set( eKeyModifierFlagControlLeftBit );
			}
		}

		if( deviceIndependentFlags.isSet( NSEventModifierFlagShift ) )
		{
			if( deviceSpecificFlags.isSetAnyOf( eOSXModifierKeyFlagShiftRightBit ) )
			{
				resultMask.set( eKeyModifierFlagShiftRightBit );
			}
			else
			{
				resultMask.set( eKeyModifierFlagShiftLeftBit );
			}
		}

		if( deviceIndependentFlags.isSet( NSEventModifierFlagOption ) )
		{
			if( deviceSpecificFlags.isSetAnyOf( eOSXModifierKeyFlagOptionRightBit ) )
			{
				resultMask.set( eKeyModifierFlagGUIRightBit );
			}
			else
			{
				resultMask.set( eKeyModifierFlagGUILeftBit );
			}
		}

		return resultMask;
	}

}

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_NS_NS_KEYBOARD_SUPPORT_H__
