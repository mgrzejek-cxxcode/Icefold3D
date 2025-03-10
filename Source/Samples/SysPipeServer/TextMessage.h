//
// Created by Ferrclaw on 09/03/2025.
//

#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include <string>

struct TextPipeMessage
{
	std::string text;

	void * GetData() noexcept
	{
		return text.data();
	}

	const void * GetData() const noexcept
	{
		return text.data();
	}

	size_t GetSize() const noexcept
	{
		return text.size();
	}

	void Resize( size_t pBytesNum ) noexcept
	{
		text.resize( pBytesNum );
	}
};

#endif //TEXTMESSAGE_H
