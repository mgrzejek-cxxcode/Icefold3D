
#include <Ic3/System/SysContextNative.h>
#include <Ic3/System/IO/MessagePipe.h>

#include <ctime>
#include <cstdlib>
#include <thread>

#include "../SysPipeServer/TextMessage.h"

using namespace Ic3;
using namespace Ic3::System;

int main()
{
	SysContextCreateInfo sysContextCreateInfo;
	auto sysContext = Platform::CreateSysContext( sysContextCreateInfo );
	auto pipeFactory = sysContext->CreatePipeFactory();

	IOTimeoutSettings ioInitTimeoutSettings{};
	ioInitTimeoutSettings.waitTimeout = cppx::milliseconds( 30000 );
	ioInitTimeoutSettings.yieldTimeBetweenRetries = cppx::milliseconds( 500 );

	auto msgPipe = CreateMessageReadPipe<TextPipeMessage>( *pipeFactory, "ic3-log", ioInitTimeoutSettings );

	while( true )
	{
		if( msgPipe.IsDataAvailable() )
		{
			TextPipeMessage message{};
			msgPipe.ReadMessage( message );
			std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
		}
		else
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
		}

		if( !msgPipe.IsValid() )
		{
			Ic3DebugOutput( "PIPE INVALID" );
			msgPipe = CreateMessageReadPipe<TextPipeMessage>( *pipeFactory, "ic3-log", ioInitTimeoutSettings );
		}
	}

	return 0;
}
