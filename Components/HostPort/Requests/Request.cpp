/*
 * Request.cpp
 *
 *  Created on: Jan 24, 2019
 *      Author: tim
 */

#include "../Requests/Request.hpp"

Request::Request()
{
	length = 0;
	bufferIndex = 0;
	buffer = nullptr;
	checksum = 0;
	type = 0;
}

Request::~Request()
{
	if(buffer != nullptr)
		delete [] buffer;
}
void Request::assignBuffer(unsigned int length, unsigned char* dataBuffer)
{
	buffer = dataBuffer;
	this->length = length;
}

unsigned char* Request::getBuffer()
{
	return buffer;
}

void Request::setType(unsigned int type)
{
	this->type = type;
}

unsigned int Request::getType()
{
	return type;
}

unsigned int Request::getLength()
{
	return length;
}

unsigned int Request::getChecksum()
{
	return 0;
}
