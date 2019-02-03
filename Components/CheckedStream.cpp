/*
 * CheckedStream.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#include "CheckedStream.hpp"

CheckedStream::CheckedStream(unsigned int size)
{
	dataBuffer.reserve(size);
	streamSum = 0;
}

CheckedStream::~CheckedStream()
{
	// TODO Auto-generated destructor stub
}

void CheckedStream::appendChar(char value)
{
	streamSum += static_cast<unsigned char>(value);
	dataBuffer.push_back(value);
}

int CheckedStream::getChecksum()
{
	return -streamSum;
}

char* CheckedStream::getBuffer()
{
	return dataBuffer.data();
}

bool CheckedStream::verifyStream(int checksum)
{
	// Adds the sum of the stream to the supplied checksum
	// When they match the result will be 0
	int result = this->streamSum + checksum;
	return (result == 0);
}

CheckedStream& operator<<(CheckedStream& stream, const char* value)
{
	while(*value != '\0')
	{
		stream.appendChar(*value++);
	}
	return stream;
}

CheckedStream& operator<<(CheckedStream& stream, char value)
{
	stream.appendChar(value);
	return stream;
}

