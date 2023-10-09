
#pragma once

#ifndef __IC3_CORELIB_SIGNAL_PROXY_H__
#define __IC3_CORELIB_SIGNAL_PROXY_H__

#include "SignalCommon.h"

namespace Ic3
{

	class SignalProxy
	{
	public:
		SignalProxy()
		{}

		virtual ~SignalProxy()
		{}

		virtual void onConnect( signal_code_value_t pSignalCode, void * pReceiver ) = 0;

		virtual void onDisconnect( signal_code_value_t pSignalCode, void * pReceiver, bool pResult ) = 0;

		virtual void onEmit( signal_code_value_t pSignalCode ) = 0;

		virtual void onHandler( signal_code_value_t pSignalCode, void * pReceiver ) = 0;
	};

}

#endif // __IC3_CORELIB_SIGNAL_PROXY_H__
