/*
 * CheckedLinkStream.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#include "CheckedLinkStream.hpp"

CheckedLinkStream::CheckedLinkStream()
{
	streamSum = 0;
}

CheckedLinkStream::CheckedLinkStream(unsigned int initialSize) :
		LinkStream(initialSize)
{
	streamSum = 0;
}

CheckedLinkStream::~CheckedLinkStream()
{

}

void CheckedLinkStream::appendValue(char value)
{
	streamSum += value;
	LinkStream::appendValue(value);
}

int8_t CheckedLinkStream::getChecksum()
{
	return -streamSum;
}

bool CheckedLinkStream::verifyStream(int8_t checksum)
{
	return ((streamSum + checksum) == 0);
}

char* CheckedLinkStream::getBuffer()
{
	return dataBuffer.data();
}
