/*
 * Request.cpp
 *
 *  Created on: Jan 24, 2019
 *      Author: tim
 */

#include "../Requests/Request.hpp"

Request::Request()
{
	sender = nullptr;
	length = 0;
	bufferIndex = 0;
	checksum = 0;
	type = 0;
}

Request::~Request()
{
}
void Request::assignBuffer(unsigned int length, std::unique_ptr<unsigned char[]> dataBuffer)
{
	buffer = std::move(dataBuffer);
	this->length = length;
}

unsigned char* Request::getBuffer() const
{
	return buffer.get();
}

void Request::setType(unsigned int type)
{
	this->type = type;
}

unsigned int Request::getType() const
{
	return type;
}

unsigned int Request::getLength() const
{
	return length;
}

unsigned int Request::getChecksum() const
{
	return 0;
}

void Request::setSender(HostPort* sender)
{
	this->sender = sender;
}

HostPort* Request::getSender()
{
	return sender;
}
