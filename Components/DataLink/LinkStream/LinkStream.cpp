/*
 * LinkStream.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#include "LinkStream.hpp"
#include <string.h>

LinkStream::LinkStream()
{
	streamReadPosition = 0;
}

LinkStream::LinkStream(unsigned int initialSize)
{
	dataBuffer.reserve(initialSize);
	streamReadPosition = 0;
}

LinkStream::~LinkStream()
{
}

void LinkStream::appendValue(char value)
{
	dataBuffer.push_back(value);
}

void LinkStream::reserve(unsigned int size)
{
	dataBuffer.reserve(size);
}

void LinkStream::reset()
{
	dataBuffer.clear();
	streamReadPosition = 0;
}

void LinkStream::seekg(unsigned int position, unsigned int direction)
{
	auto currentPosition = streamReadPosition;
	if (direction == pos_beg)
	{
		currentPosition = position;
	}
	else if (direction == pos_end)
	{
		currentPosition = dataBuffer.size() - position;
	}
	else
	{
		currentPosition += position;
	}

	if (currentPosition > dataBuffer.size())
		currentPosition = dataBuffer.size();

	streamReadPosition = currentPosition;
}

unsigned int LinkStream::bytesAvailable()
{
	int result = (dataBuffer.size() - streamReadPosition);
	if (result < 0)
		result = 0;

	return result;
}

unsigned int LinkStream::streamSize()
{
	return dataBuffer.size();
}

char* LinkStream::begin()
{
	return &dataBuffer[0];
}

char* LinkStream::end()
{
	if (dataBuffer.size() == 0)
		return nullptr;
	return &dataBuffer[dataBuffer.size()];
}

char* LinkStream::iterator()
{
	if (streamReadPosition > dataBuffer.size())
		return end();
	return &dataBuffer[streamReadPosition];
}

LinkStream& operator<<(LinkStream& stream, const char* value)
{
	while (*value != '\0')
	{
		stream.appendValue(*value++);
	}
	stream.appendValue('\0');
	return stream;
}

LinkStream& operator<<(LinkStream& stream, const std::string& value)
{
	for (auto& character : value)
		stream.appendValue(character);
	stream.appendValue('\0');
	return stream;
}

LinkStream& operator<<(LinkStream& stream, LinkStream& value)
{
	for (auto& v : value)
	{
		stream << v;
	}

	return stream;
}

LinkStream& operator>>(LinkStream& stream, char& value)
{
	if (stream.bytesAvailable() < sizeof(char))
		throw "Failed to extract from CheckedStream. Not enough bytes available";

	value = stream.dataBuffer[stream.streamReadPosition++];
	return stream;
}

LinkStream& operator>>(LinkStream& stream, std::string& value)
{
	if (stream.bytesAvailable() < sizeof(char))
		throw "Failed to extract from CheckedStream. Not enough bytes available";

	value = stream.iterator();
	stream.streamReadPosition += value.size() + 1;

	return stream;
}
