
#include <Ic3/System/SysContextNative.h>
#include <Ic3/System/IO/MessagePipe.h>

#include <ctime>
#include <cstdlib>
#include <thread>

#include "TextMessage.h"

std::string GenerateRandomStr( size_t pLength = 32 )
{
	constexpr auto lettersNum = 'Z' - 'A' + 1;

	std::string result{};
	result.reserve( pLength );

	for( size_t i = 0; i < pLength; ++i )
	{
		result.push_back( static_cast<char>( 'A' + ( rand() % lettersNum ) ) );
	}

	return result;
}

using namespace Ic3;
using namespace Ic3::System;

int main()
{
	srand( time( nullptr ) );

	SysContextCreateInfo sysContextCreateInfo;
	auto sysContext = Platform::CreateSysContext( sysContextCreateInfo );
	auto pipeFactory = sysContext->CreatePipeFactory();

	IOTimeoutSettings ioInitTimeoutSettings{};
	ioInitTimeoutSettings.waitTimeout = cppx::milliseconds( 30000 );
	ioInitTimeoutSettings.yieldTimeBetweenRetries = cppx::milliseconds( 500 );

	auto msgPipe = CreateMessageWritePipe<TextPipeMessage>( *pipeFactory, "ic3-log", ioInitTimeoutSettings );

	std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

	while( true )
	{
		TextPipeMessage message{};
		message.text = GenerateRandomStr();

		printf( "Sending: %s\n", message.text.data() );
		msgPipe.WriteMessage( message );

		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}

	return 0;
}
